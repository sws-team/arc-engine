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
#include "Game/gamepanel.h"

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

void Enemy::setReflection(const float reflection)
{
	m_data.reflection = reflection;
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
	//small
	case SCORPION:
		texture_id = RESOURCES::ENEMY_SCORPION;
		stats.health = 125.f;
		stats.speed = 60.f;
		stats.damage = 10.f;
		size.x = 1;
		size.y = 1;
		break;
	case SMALL_MEDIUM:
		texture_id = RESOURCES::ENEMY_CAR;
		stats.health = 175.f;
		stats.speed = 70.f;
		stats.damage = 10.f;
		size.x = 1;
		size.y = 1;
		ability = new StrongAbility();
		break;
	case SMALL_FAST:
		texture_id = RESOURCES::ENEMY_TRICYCLE;
		stats.health = 100.f;
		stats.speed = 90.f;
		stats.damage = 5.f;
		size.x = 1;
		size.y = 1;
		break;
	case SMALL_NEXT:
		texture_id = RESOURCES::ENEMY_ANT;
		stats.health = 50.f;
		stats.speed = 55.f;
		stats.damage = 5.f;
		size.x = 1;
		size.y = 1;
		break;
	case SELFHEAL_ENEMY:
		texture_id = RESOURCES::ENEMY_CAR;
		stats.health = 225.f;
		stats.speed = 65.f;
		stats.damage = 15.f;
		size.x = 1;
		size.y = 1;
		ability = new SelfHealAbility();
		break;
	case DOWN_TOWER_ENEMY:
		texture_id = RESOURCES::ENEMY_DOWN_TOWER;
		stats.health = 250.f;
		stats.speed = 55.f;
		stats.damage = 15.f;
		size.x = 1;
		size.y = 1;
		ability = new DownTowerAbility();
		break;
	case ANOTHER_ENEMY:
		texture_id = RESOURCES::ENEMY_PLANE;
		stats.health = 200.f;
		stats.speed = 75.f;
		stats.damage = 15.f;
		size.x = 1;
		size.y = 1;
		break;
		//mid
	case MID_SLOW:
		texture_id = RESOURCES::ENEMY_TANK;
		stats.health = 500.f;
		stats.speed = 45.f;
		stats.damage = 35.f;
		size.x = 2;
		size.y = 2;
		ability = new StrongAbility();
		break;
	case SPIDER:
		texture_id = RESOURCES::ENEMY_SPIDER;
		stats.health = 350.f;
		stats.speed = 40.f;
		stats.damage = 30.f;
		size.x = 2;
		size.y = 2;
		ability = new ShutdownTowerAbility();
		break;
	case MID_FAST:
		texture_id = RESOURCES::ENEMY_HELICOPTER;
		stats.health = 300.f;
		stats.speed = 70.f;
		stats.damage = 20.f;
		size.x = 2;
		size.y = 2;
		break;
	case REPAIR_ENEMY:
		texture_id = RESOURCES::ENEMY_REPAIR;
		stats.health = 400.f;
		stats.speed = 55.f;
		stats.damage = 20.f;
		size.x = 2;
		size.y = 2;
		ability = new HealNearAbility();
		break;
	case SHELL_ENEMY:
		texture_id = RESOURCES::ENEMY_SHELL;
		stats.health = 250.f;
		stats.speed = 50.f;
		stats.damage = 20.f;
		size.x = 2;
		size.y = 2;
		ability = new ShellNearAbility();
		break;
	case TELEPORT_ENEMY:
		texture_id = RESOURCES::ENEMY_TELEPORT;
		stats.health = 300.f;
		stats.speed = 75.0f;
		stats.damage = 20.f;
		size.x = 2;
		size.y = 2;
		ability = new TeleportAbility();
		break;
		//big
	case BIG_SLOW:
		texture_id = RESOURCES::ENEMY_AIRCARRIER;
		stats.health = 2000.f;
		stats.speed = 30.f;
		stats.damage = 75.f;
		stats.reflection = 0.5f;
		size.x = 4;
		size.y = 4;
		break;
	case BIG_MEDIUM:
		texture_id = RESOURCES::ENEMY_BIG_TANK;
		stats.health = 900.f;
		stats.speed = 35.f;
		stats.damage = 60.f;
		size.x = 4;
		size.y = 4;
		ability = new RageAbility();
		break;
	case SPAWN_ENEMY:
		texture_id = RESOURCES::ENEMY_COW;
		stats.health = 750.f;
		stats.speed = 40.f;
		stats.damage = 40.f;
		size.x = 4;
		size.y = 4;
		ability = new SpawnEnemy();
		break;
	default:
		break;
	}
	stats.speed = 100.f - stats.speed;
	Enemy *enemy = new Enemy(texture_id, startPos, stats, size);
	enemy->setAbility(ability);
	if (ability != nullptr)
		ability->setOwner(enemy);
	return enemy;
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
												 Vector2i(GlobalVariables::CELL_SIZE,
														  GlobalVariables::CELL_SIZE),
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
		float weight = 0;
		for(Tower* tower : towers)
		{
			if (tower->type() == POWER)
				continue;
			if (!tower->isActive())
				continue;
			if (tower->isDowngraded())
				continue;
			const float currentWeight = Engine::Instance().panel()->getTowerSellCost(tower);
			if (currentWeight > weight)
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

SpawnEnemy::SpawnEnemy()
	: EnemyAbility(BEGIN_SPAWN_INTERVAL)
	,m_state(READY)
{

}

void SpawnEnemy::use()
{
	switch (m_state)
	{
	case READY:
	{
		owner->setStopped(true);
		Engine::Instance().level()->addAnimation(RESOURCES::ENEMY_COW,
												 owner->pos(),
												 Vector2i(GlobalVariables::CELL_SIZE * 2,
														  GlobalVariables::CELL_SIZE * 2),
												 200, 4, ACTIVATE_SPAWN_ROW);
		m_interval = 800;
		m_state = SPAWN;
		currentSpawnCount = 0;
		spawnCount = SPAWN_COUNT + rand() % SPAWN_COUNT_OFFSET;
	}
		break;
	case SPAWN:
	{
		const Vector2i cell = Engine::Instance().camera()->posToCellMap(owner->enemyPos());
		const int direction = Engine::Instance().level()->getTileDirectionByCell(cell);

		Engine::Instance().level()->spawn(owner->pos(), SMALL_NEXT, 0.1f, direction);

		m_interval = SPAWN_INTERVAL;
		m_state = WAIT_SPAWN;
		currentSpawnCount++;
	}
		break;
	case WAIT_SPAWN:
	{
		if(currentSpawnCount >= spawnCount)
		{
			Engine::Instance().level()->addAnimation(RESOURCES::ENEMY_COW,
													 owner->pos(),
													 Vector2i(GlobalVariables::CELL_SIZE * 2,
															  GlobalVariables::CELL_SIZE * 2),
													 200, 4, DEACTIVATE_SPAWN_ROW);
			m_interval = 800;
			m_state = FINISHED;
		}
		else
			m_state = SPAWN;
	}
		break;

	case FINISHED:
	{
		owner->setStopped(false);
		m_interval = BEGIN_SPAWN_INTERVAL;
		m_state = READY;
	}
		break;
	default:
		break;
	}
}

StrongAbility::StrongAbility()
	: EnemyAbility(STRONG_INTERVAL)
{

}

void StrongAbility::use()
{
	owner->protect(0.05f);
}

RageAbility::RageAbility()
	: EnemyAbility(RAGE_INTERVAL)
{

}

void RageAbility::use()
{
	const float k = owner->getData().health / owner->getPureStats().health;
	const float protect = 1 - k;
	owner->setReflection(protect);
}
