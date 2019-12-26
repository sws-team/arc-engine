#include "enemy.h"
#include "globalvariables.h"
#include "map.h"
#include "lifebar.h"
#include "settings.h"
#include "Engine/engine.h"
#include "Game/Level/level.h"
#include "Game/Level/tower.h"
#include "camera.h"
#include "Game/Level/projectile.h"
#include "Game/Collisions/collisions.h"

Enemy::Enemy(const RESOURCES::TEXTURE_TYPE &texture_id,
			 const Vector2f &startPos,
			 const EnemyStats& stats,
			 const Vector2i &cellSize)
	: GameObject(texture_id, startPos,
				 Vector2i(GlobalVariables::MAP_CELL_SIZE * cellSize.x,
						  GlobalVariables::MAP_CELL_SIZE * cellSize.y),
				 4)
	,m_stats(stats)
	,moveCounter(0)
	,spriteDirection(DEFAULT_DOWN)
	,isFreezed(false)
	,startFreeze(false)
	,ability(nullptr)
	,m_stopped(false)
	,m_visible(true)
{

	m_size.x = GlobalVariables::MAP_CELL_SIZE * cellSize.x;
	m_size.y = GlobalVariables::MAP_CELL_SIZE * cellSize.y;

	m_spritePos = Vector2f(0, 0);

	const float road = 4 * GlobalVariables::Instance().mapTileSize().x - GlobalVariables::Instance().mapTileSize().x * cellSize.x;
	if (road != 0.f)
	{
		m_spritePos.x = rand() % static_cast<int>(road);
		m_spritePos.y = rand() % static_cast<int>(road);
	}
	m_pos = startPos;

	m_data = m_stats;
	lifeBar = new LifeBar();
	lifeBar->init(Vector2i(getSize().x, LifeBar::LIFE_BAR_HEIGHT * Settings::Instance().getScaleFactor().y), Color::Red);
}

Enemy::~Enemy()
{
	delete lifeBar;
}

void Enemy::setAbility(EnemyAbility *ability)
{
	this->ability = ability;
}

EnemyStats Enemy::getData() const
{
	return m_data;
}

EnemyStats Enemy::getPureStats() const
{
	return m_stats;
}

bool Enemy::moveStep()
{
	moveCounter++;
	moveEnemy(currentStep);
	if (moveCounter >= m_data.speed)
	{
		setPos(targetPos);
		return true;
	}
	return false;
}

void Enemy::moveNext(int direction)
{
	if (isFreezed && !startFreeze)
	{
		startFreeze = true;
		m_data.speed += freezeK;
		freezeTimer.clock.restart();
	}
	else if (!isFreezed && startFreeze)
	{
		startFreeze = false;
		m_data.speed -= freezeK;
	}

	targetPos = m_pos;

	moveCounter = 0;
	currentDirection = direction;

	currentStep.x = 0;
	currentStep.y = 0;

	SPRITE_DIRECTION newDirection = DEFAULT_DOWN;
	switch (currentDirection)
	{
	case Map::STAY:
		break;
	case Map::UP:
		targetPos.y -= GlobalVariables::Instance().mapTileSize().y;
		currentStep.y = -GlobalVariables::Instance().mapTileSize().y / m_data.speed;
		newDirection = SPRITE_DIRECTION::SPRITE_UP;
		break;
	case Map::DOWN:
		targetPos.y += GlobalVariables::Instance().mapTileSize().y;
		currentStep.y = GlobalVariables::Instance().mapTileSize().y / m_data.speed;
		newDirection = SPRITE_DIRECTION::DEFAULT_DOWN;
		break;
	case Map::LEFT:
		targetPos.x -= GlobalVariables::Instance().mapTileSize().x;
		currentStep.x = -GlobalVariables::Instance().mapTileSize().x / m_data.speed;
		newDirection = SPRITE_DIRECTION::SPRITE_LEFT;
		break;
	case Map::RIGHT:
		targetPos.x += GlobalVariables::Instance().mapTileSize().x;
		currentStep.x = GlobalVariables::Instance().mapTileSize().x / m_data.speed;
		newDirection = SPRITE_DIRECTION::SPRITE_RIGHT;
		break;
	case Map::NO_MOVE:
		return;
	}

	if (newDirection != spriteDirection)
	{
		float angle = 0;
		Vector2f origin;
		origin.x = 0;
		origin.y = 0;
		switch (newDirection)
		{
		case DEFAULT_DOWN:
			if (spriteDirection == SPRITE_LEFT)
				angle = -RIGHT_ANGLE;
			else if (spriteDirection == SPRITE_RIGHT)
				angle = RIGHT_ANGLE;
			break;
		case SPRITE_LEFT:
			if (spriteDirection == DEFAULT_DOWN)
			{
				angle = RIGHT_ANGLE;
				origin.y = m_size.y;
			}
			else if (spriteDirection == SPRITE_UP)
			{
				angle = -RIGHT_ANGLE;
				origin.y = m_size.y;
			}
			break;
		case SPRITE_UP:
			if (spriteDirection == SPRITE_LEFT)
			{
				angle = RIGHT_ANGLE;
				origin.x = m_size.x;
				origin.y = m_size.y;
			}
			else if (spriteDirection == SPRITE_RIGHT)
			{
				angle = -RIGHT_ANGLE;
				origin.x = m_size.x;
				origin.y = m_size.y;
			}
			break;
		case SPRITE_RIGHT:
			if (spriteDirection == DEFAULT_DOWN)
			{
				angle = -RIGHT_ANGLE;
				origin.x = m_size.x;
			}
			else if (spriteDirection == SPRITE_UP)
			{
				angle = RIGHT_ANGLE;
				origin.x = m_size.x;
			}
			break;
		}
		spriteDirection = newDirection;
		sprite.setOrigin(origin);
		sprite.rotate(angle);
		update();
	}
}

void Enemy::update()
{
	if (startFreeze)
	{
		if (freezeTimer.check(freezeDuration))
			isFreezed = false;
	}
	setPos(m_pos + m_spritePos);
	GameObject::update();
}

void Enemy::draw(RenderTarget * const target)
{
	if (!m_visible)
		return;

	GameObject::draw(target);
	drawLifeBar(target);

	if (ability != nullptr)
		ability->draw(target);
}

void Enemy::hit(float damage)
{
	m_data.health -= damage * (1 - m_data.reflection);
}

bool Enemy::isAlive() const
{
	return m_data.health > 0.f;
}

void Enemy::useAbility()
{
	if (ability != nullptr)
		ability->update();
}

void Enemy::freeze(float k, int duration)
{
	if (isFreezed || startFreeze)
	{
		freezeDuration = max(freezeDuration, duration);
//		m_data.speed += k;
//		freezeK += k;
		return;
	}

	startFreeze = false;
	freezeDuration = duration;
	freezeK = k;
	isFreezed = true;
}

void Enemy::heal(float health)
{
	m_data.health += health;
	if (m_data.health > m_stats.health)
		m_data.health = m_stats.health;
	Engine::Instance().level()->addAnimation(RESOURCES::HEAL_EFFECT,
											 this->getCenter() - Vector2f(GlobalVariables::MAP_CELL_SIZE/2,
																		  GlobalVariables::MAP_CELL_SIZE/2),
											 Vector2i(GlobalVariables::MAP_CELL_SIZE,
													  GlobalVariables::MAP_CELL_SIZE),
											 50, 8, 0);
}

void Enemy::protect(float shell, bool show)
{
	m_data.reflection += shell;
	if(m_data.reflection >= 0.9f)
		m_data.reflection = 0.9f;
	if (show)
		Engine::Instance().level()->addAnimation(RESOURCES::SHELL_EFFECT, this->pos(),
											 Vector2i(GlobalVariables::MAP_CELL_SIZE, GlobalVariables::MAP_CELL_SIZE),
											 50, 4, 0);
}

void Enemy::drawLifeBar(RenderTarget *target)
{
	if (m_data.health <= 0)
		return;

	const float healthRate = m_data.health / m_stats.health;
	lifeBar->setPos(Vector2f(sprite.getGlobalBounds().left,
							 sprite.getGlobalBounds().top) - Vector2f(0, (LIFEBAR_OFFSET + LifeBar::LIFE_BAR_HEIGHT) * Settings::Instance().getScaleFactor().y));
	lifeBar->setValue(healthRate);
	lifeBar->draw(target);
}

void Enemy::moveEnemy(const Vector2f &d)
{
	m_pos += d;
	update();
}

bool Enemy::isVisible() const
{
	return m_visible;
}

void Enemy::setVisible(bool visible)
{
	m_visible = visible;
}

bool Enemy::isStopped() const
{
	return m_stopped;
}

void Enemy::setStopped(bool stopped)
{
	m_stopped = stopped;
}

Vector2f Enemy::enemyPos() const
{
	return m_pos + Vector2f(1, 1);
}

Vector2f Enemy::enemyCenter() const
{
	return Vector2f(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width/2,
					sprite.getGlobalBounds().top + sprite.getGlobalBounds().height/2);
}

Enemy *EnemiesFactory::createEnemy(ENEMY_TYPES type, const Vector2f &startPos)
{
	Vector2i size;
	size.x = 1;
	size.y = 1;
	EnemyStats stats;
	stats.speed = 0.f;
	stats.health = 0.f;
	stats.damage = 0.f;
	stats.reflection = 0.f;
	EnemyAbility *ability = nullptr;
	RESOURCES::TEXTURE_TYPE texture_id;
	switch (type)
	{
	case SMALL_NEXT:
		texture_id = RESOURCES::ENEMY_SCORPION;
		stats.health = 175.f;
		stats.speed = 12.f;
		stats.damage = 15.f;
		size.x = 1;
		size.y = 1;
		break;
	case SMALL_SLOW:
		texture_id = RESOURCES::ENEMY_SCORPION;
		stats.health = 135.f;
		stats.speed = 15.f;
		stats.damage = 15.f;
		size.x = 1;
		size.y = 1;
		break;
	case SMALL_MEDIUM:
		texture_id = RESOURCES::ENEMY_CAR;
		stats.health = 115.f;
		stats.speed = 7.4f;
		stats.damage = 10.f;
		size.x = 1;
		size.y = 1;
		break;
	case SMALL_FAST:
		texture_id = RESOURCES::ENEMY_TRICYCLE;
		stats.health = 95.f;
		stats.speed = 5.f;
		stats.damage = 5.f;
		size.x = 1;
		size.y = 1;
		break;
	case MID_SLOW:
		texture_id = RESOURCES::ENEMY_TANK;
		stats.health = 400.f;
		stats.speed = 20.f;
		stats.damage = 40.f;
		size.x = 2;
		size.y = 2;
		break;
	case MID_MEDIUM:
		texture_id = RESOURCES::ENEMY_SPIDER;
		stats.health = 300.f;
		stats.speed = 15.f;
		stats.damage = 30.f;
		size.x = 2;
		size.y = 2;
		ability = new ShutdownTowerAbility();
		break;
	case MID_FAST:
		texture_id = RESOURCES::ENEMY_HELICOPTER;
		stats.health = 200.f;
		stats.speed = 10.f;
		stats.damage = 20.f;
		size.x = 2;
		size.y = 2;
		break;
	case BIG_SLOW:
		texture_id = RESOURCES::ENEMY_AIRCARRIER;
		stats.health = 1000.f;
		stats.speed = 40.f;
		stats.damage = 80.f;
		size.x = 4;
		size.y = 4;
		break;
	case BIG_MEDIUM:
		texture_id = RESOURCES::ENEMY_BIG_TANK;
		stats.health = 750.f;
		stats.speed = 30.f;
		stats.damage = 60.f;
		size.x = 4;
		size.y = 4;
		break;
	case BIG_FAST:
		texture_id = RESOURCES::ENEMY_PLANE;
		stats.health = 500.f;
		stats.speed = 20.f;
		stats.damage = 40.f;
		size.x = 4;
		size.y = 4;
		break;
	case REPAIR_ENEMY:
		texture_id = RESOURCES::ENEMY_REPAIR;
		stats.health = 200.f;
		stats.speed = 13.f;
		stats.damage = 15.f;
		size.x = 2;
		size.y = 2;
		ability = new HealNearAbility();
		break;
	case SHELL_ENEMY:
		texture_id = RESOURCES::ENEMY_SHELL;
		stats.health = 200.f;
		stats.speed = 8.f;
		stats.damage = 15.f;
		size.x = 2;
		size.y = 2;
		ability = new ShellNearAbility();
		break;
	case TELEPORT_ENEMY:
		texture_id = RESOURCES::ENEMY_TELEPORT;
		stats.health = 200.f;
		stats.speed = 12.5f;
		stats.damage = 15.f;
		size.x = 2;
		size.y = 2;
		ability = new TeleportAbility();
		break;
	case SELFHEAL_ENEMY:
		texture_id = RESOURCES::ENEMY_CAR;
		stats.health = 250.f;
		stats.speed = 9.f;
		stats.damage = 10.f;
		size.x = 1;
		size.y = 1;
		ability = new SelfHealAbility();
		break;
	case DOWN_TOWER_ENEMY:
		texture_id = RESOURCES::ENEMY_DOWN_TOWER;
		stats.health = 250.f;
		stats.speed = 10.f;
		stats.damage = 10.f;
		size.x = 1;
		size.y = 1;
		ability = new DownTowerAbility();
		break;
	default:
		break;
	}
	stats.health *= 1.2f;
	stats.speed *= 3.f;
	Enemy *enemy = new Enemy(texture_id, startPos, stats, size);
	enemy->setAbility(ability);
	if (ability != nullptr)
		ability->setOwner(enemy);
	return enemy;
}

vector<ENEMY_TYPES> EnemiesFactory::generateEnemies(unsigned int n)
{
	vector<ENEMY_TYPES> spawnEnemies;
	switch (n)
	{
	case 0:
	{
		addEnemiesByType(SMALL_SLOW, 200, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 50, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 10, &spawnEnemies);
	}
		break;
	case 1:
	{
		addEnemiesByType(SMALL_SLOW, 300, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 100, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 50, &spawnEnemies);
		addEnemiesByType(REPAIR_ENEMY, 5, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 10, &spawnEnemies);
		addEnemiesByType(MID_FAST, 1, &spawnEnemies);
		addEnemiesByType(SMALL_NEXT, 100, &spawnEnemies);
	}
		break;
	case 2:
	{
		addEnemiesByType(SMALL_SLOW, 400, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 150, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 75, &spawnEnemies);
		addEnemiesByType(SELFHEAL_ENEMY, 2, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 50, &spawnEnemies);
		addEnemiesByType(DOWN_TOWER_ENEMY, 5, &spawnEnemies);
		addEnemiesByType(MID_FAST, 5, &spawnEnemies);
		addEnemiesByType(SMALL_NEXT, 150, &spawnEnemies);
	}
		break;
	case 3:
	{
		addEnemiesByType(SMALL_SLOW, 500, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 250, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 100, &spawnEnemies);
		addEnemiesByType(SELFHEAL_ENEMY, 5, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 75, &spawnEnemies);
		addEnemiesByType(DOWN_TOWER_ENEMY, 15, &spawnEnemies);
		addEnemiesByType(MID_FAST, 10, &spawnEnemies);
		addEnemiesByType(SMALL_NEXT, 200, &spawnEnemies);
	}
		break;
	case 4:
	{
		addEnemiesByType(SMALL_SLOW, 500, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 250, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 100, &spawnEnemies);
		addEnemiesByType(REPAIR_ENEMY, 5, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 75, &spawnEnemies);
		addEnemiesByType(DOWN_TOWER_ENEMY, 15, &spawnEnemies);
		addEnemiesByType(MID_FAST, 10, &spawnEnemies);
		addEnemiesByType(SELFHEAL_ENEMY, 5, &spawnEnemies);
		addEnemiesByType(SMALL_NEXT, 250, &spawnEnemies);
	}
		break;
	case 5:
	{
		addEnemiesByType(SMALL_SLOW, 500, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 300, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 100, &spawnEnemies);
		addEnemiesByType(REPAIR_ENEMY, 10, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 75, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 25, &spawnEnemies);
		addEnemiesByType(DOWN_TOWER_ENEMY, 15, &spawnEnemies);
		addEnemiesByType(MID_FAST, 10, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 1, &spawnEnemies);
		addEnemiesByType(SELFHEAL_ENEMY, 10, &spawnEnemies);
		addEnemiesByType(SMALL_NEXT, 300, &spawnEnemies);
	}
		break;
	case 6:
	{
		addEnemiesByType(SMALL_SLOW, 500, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 300, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 200, &spawnEnemies);
		addEnemiesByType(REPAIR_ENEMY, 10, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 75, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 25, &spawnEnemies);
		addEnemiesByType(SHELL_ENEMY, 15, &spawnEnemies);
		addEnemiesByType(DOWN_TOWER_ENEMY, 15, &spawnEnemies);
		addEnemiesByType(MID_FAST, 20, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 2, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 2, &spawnEnemies);
		addEnemiesByType(SELFHEAL_ENEMY, 20, &spawnEnemies);
		addEnemiesByType(SMALL_NEXT, 350, &spawnEnemies);
	}
		break;
	case 7:
	{
		addEnemiesByType(SMALL_SLOW, 600, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 500, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 200, &spawnEnemies);
		addEnemiesByType(REPAIR_ENEMY, 20, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 100, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 45, &spawnEnemies);
		addEnemiesByType(SHELL_ENEMY, 25, &spawnEnemies);
		addEnemiesByType(DOWN_TOWER_ENEMY, 35, &spawnEnemies);
		addEnemiesByType(MID_FAST, 25, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 3, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 2, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 2, &spawnEnemies);
		addEnemiesByType(SELFHEAL_ENEMY, 20, &spawnEnemies);
		addEnemiesByType(TELEPORT_ENEMY, 20, &spawnEnemies);
		addEnemiesByType(SMALL_NEXT, 400, &spawnEnemies);
	}
		break;
	case 8:
	{
		addEnemiesByType(SMALL_SLOW, 800, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 500, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 250, &spawnEnemies);
		addEnemiesByType(REPAIR_ENEMY, 40, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 150, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 50, &spawnEnemies);
		addEnemiesByType(SHELL_ENEMY, 25, &spawnEnemies);
		addEnemiesByType(DOWN_TOWER_ENEMY, 35, &spawnEnemies);
		addEnemiesByType(MID_FAST, 25, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 3, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 2, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 2, &spawnEnemies);
		addEnemiesByType(SELFHEAL_ENEMY, 40, &spawnEnemies);
		addEnemiesByType(TELEPORT_ENEMY, 30, &spawnEnemies);
		addEnemiesByType(SMALL_NEXT, 500, &spawnEnemies);
	}
		break;
	case 9:
	{
		addEnemiesByType(SMALL_SLOW, 900, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 600, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 300, &spawnEnemies);
		addEnemiesByType(REPAIR_ENEMY, 50, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 100, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 150, &spawnEnemies);
		addEnemiesByType(SHELL_ENEMY, 50, &spawnEnemies);
		addEnemiesByType(DOWN_TOWER_ENEMY, 40, &spawnEnemies);
		addEnemiesByType(MID_FAST, 30, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 5, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 3, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 3, &spawnEnemies);
		addEnemiesByType(SELFHEAL_ENEMY, 50, &spawnEnemies);
		addEnemiesByType(TELEPORT_ENEMY, 40, &spawnEnemies);
		addEnemiesByType(SMALL_NEXT, 600, &spawnEnemies);
	}
		break;
	case 10:
	{
		addEnemiesByType(SMALL_SLOW, 1000, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 600, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 300, &spawnEnemies);
		addEnemiesByType(REPAIR_ENEMY, 75, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 150, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 200, &spawnEnemies);
		addEnemiesByType(SHELL_ENEMY, 50, &spawnEnemies);
		addEnemiesByType(DOWN_TOWER_ENEMY, 50, &spawnEnemies);
		addEnemiesByType(MID_FAST, 45, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 7, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 5, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 5, &spawnEnemies);
		addEnemiesByType(SELFHEAL_ENEMY, 75, &spawnEnemies);
		addEnemiesByType(TELEPORT_ENEMY, 60, &spawnEnemies);
		addEnemiesByType(SMALL_NEXT, 700, &spawnEnemies);
	}
		break;
	case 11:
	{
		addEnemiesByType(SMALL_SLOW, 1200, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 700, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 300, &spawnEnemies);
		addEnemiesByType(REPAIR_ENEMY, 100, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 200, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 250, &spawnEnemies);
		addEnemiesByType(SHELL_ENEMY, 75, &spawnEnemies);
		addEnemiesByType(DOWN_TOWER_ENEMY, 70, &spawnEnemies);
		addEnemiesByType(MID_FAST, 50, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 9, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 7, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 7, &spawnEnemies);
		addEnemiesByType(SELFHEAL_ENEMY, 90, &spawnEnemies);
		addEnemiesByType(TELEPORT_ENEMY, 75, &spawnEnemies);
		addEnemiesByType(SMALL_NEXT, 800, &spawnEnemies);
	}
		break;
	case 12:
	{
		addEnemiesByType(SMALL_SLOW, 1400, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 800, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 400, &spawnEnemies);
		addEnemiesByType(REPAIR_ENEMY, 130, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 260, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 300, &spawnEnemies);
		addEnemiesByType(SHELL_ENEMY, 80, &spawnEnemies);
		addEnemiesByType(DOWN_TOWER_ENEMY, 90, &spawnEnemies);
		addEnemiesByType(MID_FAST, 75, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 10, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 8, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 8, &spawnEnemies);
		addEnemiesByType(SELFHEAL_ENEMY, 100, &spawnEnemies);
		addEnemiesByType(TELEPORT_ENEMY, 100, &spawnEnemies);
		addEnemiesByType(SMALL_NEXT, 900, &spawnEnemies);
	}
		break;
	case 13:
	{
		addEnemiesByType(SMALL_SLOW, 1450, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 900, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 450, &spawnEnemies);
		addEnemiesByType(REPAIR_ENEMY, 150, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 280, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 350, &spawnEnemies);
		addEnemiesByType(SHELL_ENEMY, 100, &spawnEnemies);
		addEnemiesByType(DOWN_TOWER_ENEMY, 100, &spawnEnemies);
		addEnemiesByType(MID_FAST, 100, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 15, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 10, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 10, &spawnEnemies);
		addEnemiesByType(SELFHEAL_ENEMY, 150, &spawnEnemies);
		addEnemiesByType(TELEPORT_ENEMY, 150, &spawnEnemies);
		addEnemiesByType(SMALL_NEXT, 1000, &spawnEnemies);
	}
		break;
	case 14:
	{
		addEnemiesByType(SMALL_SLOW, 1500, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 1000, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 500, &spawnEnemies);
		addEnemiesByType(REPAIR_ENEMY, 200, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 300, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 400, &spawnEnemies);
		addEnemiesByType(SHELL_ENEMY, 150, &spawnEnemies);
		addEnemiesByType(DOWN_TOWER_ENEMY, 150, &spawnEnemies);
		addEnemiesByType(MID_FAST, 150, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 50, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 25, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 25, &spawnEnemies);
		addEnemiesByType(SELFHEAL_ENEMY, 200, &spawnEnemies);
		addEnemiesByType(TELEPORT_ENEMY, 200, &spawnEnemies);
		addEnemiesByType(SMALL_NEXT, 1000, &spawnEnemies);
	}
		break;
	case 128:
	{
		addEnemiesByType(SMALL_SLOW, 50000, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 10000, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 10000, &spawnEnemies);
		addEnemiesByType(REPAIR_ENEMY, 2000, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 2000, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 2000, &spawnEnemies);
		addEnemiesByType(SHELL_ENEMY, 2000, &spawnEnemies);
		addEnemiesByType(DOWN_TOWER_ENEMY, 2000, &spawnEnemies);
		addEnemiesByType(MID_FAST, 2000, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 2000, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 2000, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 2000, &spawnEnemies);
		addEnemiesByType(SELFHEAL_ENEMY, 2000, &spawnEnemies);
		addEnemiesByType(TELEPORT_ENEMY, 2000, &spawnEnemies);
		addEnemiesByType(SMALL_NEXT, 30000, &spawnEnemies);
	}
		break;
	default:
		break;
	}
	return spawnEnemies;
}

void EnemiesFactory::addEnemiesByType(const ENEMY_TYPES type, int count, vector<ENEMY_TYPES> *spawnEnemies)
{
	for (int i = 0; i < count; ++i)
		spawnEnemies->push_back(type);
}

EnemyAbility::EnemyAbility(float msec)
	: GameDrawable()
	,m_interval(msec)
{
	const int offset = static_cast<int>(m_interval * 0.2f);
	m_abilityInterval = m_interval + rand() % offset - offset/2 ;
}

void EnemyAbility::draw(RenderTarget * const target)
{

}

void EnemyAbility::update()
{
	if (abilityTimer.check(m_interval))
		use();
}

void EnemyAbility::setOwner(Enemy *owner)
{
	this->owner = owner;
}

AreaAbility::AreaAbility(float msec)
	: EnemyAbility(msec)
{
	const int cells = 3 + rand() % 5;
	area = cells * GlobalVariables::Instance().mapTileSize().x;
	m_interval = m_abilityInterval;
}

bool AreaAbility::inArea(Enemy *enemy) const
{
	const Vector2f center = owner->enemyCenter();
	const Vector2f enemyCenter = enemy->enemyCenter();
	if (center.x < enemyCenter.x  + area &&
			center.x > enemyCenter.x - area &&
			center.y < enemyCenter.y  + area &&
			center.y > enemyCenter.y - area)
		return true;
	return false;
}

HealNearAbility::HealNearAbility()
	: AreaAbility(HEAL_INTERVAL)
{

}

void HealNearAbility::use()
{
	const vector<Enemy*> enemies = Engine::Instance().level()->getAllEnemies();
	for(Enemy *enemy : enemies)
	{
		if (enemy == owner)
			continue;
		if (inArea(enemy))
			enemy->heal(enemy->getPureStats().health * 0.1f);
	}
}

ShellNearAbility::ShellNearAbility()
	: AreaAbility(SHELL_INTERVAL)
{

}

void ShellNearAbility::use()
{
	const vector<Enemy*> enemies = Engine::Instance().level()->getAllEnemies();
	for(Enemy *enemy : enemies)
	{
		if (inArea(enemy))
			enemy->protect(0.1f);
	}
}

SelfHealAbility::SelfHealAbility()
	: EnemyAbility(SELF_HEAL_INTERVAL)
{

}

void SelfHealAbility::use()
{
	owner->heal(owner->getPureStats().health * 0.1f);
}

TeleportAbility::TeleportAbility()
	: EnemyAbility(TELEPORT_INTERVAL)
	,m_state(READY)
{

}

void TeleportAbility::use()
{
	switch (m_state)
	{
	case READY:
	{
		owner->setStopped(true);
		owner->setVisible(false);
		Engine::Instance().level()->addAnimation(RESOURCES::ENEMY_TELEPORT,
												 owner->pos(),
												 Vector2i(GlobalVariables::CELL_SIZE,
														  GlobalVariables::CELL_SIZE),
												 200, 4, TELEPORT_DISAPPEAR_ROW);
		m_state = DISAPPEAR;
		m_interval = 250;
	}
		break;
	case DISAPPEAR:
	{
		for (int i = 0; i < 6; ++i)
		{
			if (Engine::Instance().level()->getEndRect().contains(owner->enemyPos()))
				continue;
			while(!owner->moveStep())
			{

			}
			const Vector2i cell = Engine::Instance().camera()->posToCellMap(owner->enemyPos());
			const int direction = Engine::Instance().level()->getTileDirectionByCell(cell);
			owner->moveNext(direction);
		}
		m_state = APPEAR;
		m_interval = 500;
	}
		break;
	case APPEAR:
	{
		Engine::Instance().level()->addAnimation(RESOURCES::ENEMY_TELEPORT,
												 owner->pos(),
												 Vector2i(GlobalVariables::CELL_SIZE, GlobalVariables::CELL_SIZE),
												 200, 4, TELEPORT_APPEAR_ROW);
		m_state = FINISH;
		m_interval = 200 * 4;
	}
		break;
	case FINISH:
	{
		owner->setVisible(true);
		owner->setStopped(false);
		m_state = READY;
		m_interval = m_abilityInterval;
	}
		break;
	default:
		break;
	}
}



TowerEffectAbility::TowerEffectAbility()
	: EnemyAbility(SHUTDOWN_INTERVAL)
	,targetTower(nullptr)
	,m_state(READY)
	,projectile(nullptr)
{

}

void TowerEffectAbility::draw(RenderTarget * const target)
{
	if (projectile != nullptr)
		projectile->draw(target);
}

void TowerEffectAbility::update()
{
	EnemyAbility::update();
	if (projectile != nullptr)
		projectile->update();
}

void TowerEffectAbility::use()
{
	switch (m_state)
	{
	case READY:
	{
		owner->setStopped(true);
		Engine::Instance().level()->addAnimation(info.enemyTextureId, owner->pos(),
												 info.animationSize,
												 200, 4, 1);
		m_interval = 800;
		m_state = SHOOT;
	}
		break;
	case SHOOT:
	{
		owner->row = 3;
		owner->frameCount = 1;

		Tower *target = nullptr;
		const vector<Tower*> towers = Engine::Instance().level()->getAllTowers();
		float weightX = INT_MAX;
		float weightY = INT_MAX;
		for(Tower* tower : towers)
		{
			if (tower->type() == POWER)
				continue;
			if (!tower->isActive())
				continue;
			if (tower->isDowngraded())
				continue;

			const float currentWeightX = fabs(tower->getCenter().x - owner->getCenter().x);
			const float currentWeightY = fabs(tower->getCenter().y - owner->getCenter().y);
			if (currentWeightX < weightX && currentWeightY < weightY)
				target = tower;
		}
		if (target == nullptr)
		{
			getBack();
			return;
		}
		const Vector2f aPos = owner->enemyCenter();
		const float a = aPos.x - target->getCenter().x;
		const float b = aPos.y - target->getCenter().y;
		const float tg = ( b / a );
		float angle = atanf(tg) * 180 / M_PI;
		if (a < 0)
			angle += 180;
		else if (b < 0)
			angle += 360;
		angle -= 180;

		projectile = new GameObject(info.pojectileTextureId, aPos, info.projectileSize, 1);
		projectile->sprite.setRotation(angle);
		m_angle = angle;
		targetTower = target;

		m_interval = 20;
		m_state = MOVE;
	}
		break;
	case MOVE:
	{
		const float projectleSpeed = 5;
		if (moveTimer.check(Projectile::PROJECTILE_GAME_SPEED))
		{
			const float x1 = owner->pos().x;
			const float y1 = owner->pos().y;

			const float y2 = y1 + projectleSpeed * sinf(m_angle * M_PI/180);
			const float x2 = x1 + projectleSpeed * cosf(m_angle * M_PI/180);

			projectile->move(x2-x1, y2-y1);
		}
		if (Collision::PixelPerfectTest(targetTower->getSprite(), projectile->getSprite()))
		{
			delete projectile;
			projectile = nullptr;
			getBack();
			effect(true);
		}
	}
		break;
	case WAIT:
	{
		owner->setStopped(false);
		m_state = FINISHED;
		m_interval = info.duration;
	}
		break;
	case FINISHED:
	{
		effect(false);
		m_state = READY;
		m_interval = m_abilityInterval;
	}
		break;
	default:
		break;
	}
}

void TowerEffectAbility::getBack()
{
	owner->row = 0;
	owner->frameCount = 4;
	owner->update();
	Engine::Instance().level()->addAnimation(info.enemyTextureId, owner->pos(),
											 info.animationSize,
											 200, 4, 2);
	m_interval = 200 * 4;
	m_state = WAIT;
}


ShutdownTowerAbility::ShutdownTowerAbility()
{
	info.enemyTextureId = RESOURCES::ENEMY_SPIDER;
	info.animationSize = Vector2i(GlobalVariables::CELL_SIZE, GlobalVariables::CELL_SIZE);
	info.pojectileTextureId = RESOURCES::WEB;
	info.projectileSize = Vector2i(GlobalVariables::CELL_SIZE, GlobalVariables::CELL_SIZE);
	info.duration = TOWER_DISABLED_DURATION;
}

void ShutdownTowerAbility::effect(bool isActive)
{
	if (targetTower != nullptr)
		targetTower->setActive(!isActive);
}

DownTowerAbility::DownTowerAbility()
{
	info.enemyTextureId = RESOURCES::ENEMY_DOWN_TOWER;
	info.animationSize = Vector2i(GlobalVariables::MAP_CELL_SIZE, GlobalVariables::MAP_CELL_SIZE);
	info.pojectileTextureId = RESOURCES::DOWNGRADE_PROJECTILE;
	info.projectileSize = Vector2i(200, 16);
	info.duration = TOWER_DOWNGRADED_DURATION;
}

void DownTowerAbility::effect(bool isActive)
{
	if (targetTower != nullptr)
		targetTower->setDowngrade(isActive);
}
