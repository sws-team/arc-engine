#include "enemy.h"
#include "globalvariables.h"
#include "map.h"
#include "lifebar.h"
#include "settings.h"

#define TEST_BUILD

Enemy::Enemy(const RESOURCES::TEXTURE_TYPE &texture_id,
			 const Vector2f &startPos,
			 const EnemyStats& stats,
			 const Vector2i &cellSize)
	: GameObject(texture_id, startPos,
				 Vector2i(GlobalVariables::MAP_CELL_SIZE * cellSize.x, GlobalVariables::MAP_CELL_SIZE * cellSize.y),
				 4)
	,m_stats(stats)
	,moveCounter(0)
	,spriteDirection(DEFAULT_DOWN)
	,isFreezed(false)
	,startFreeze(false)
{

	m_size.x = GlobalVariables::Instance().mapTileSize().x * cellSize.x;
	m_size.y = GlobalVariables::Instance().mapTileSize().y * cellSize.y;

	m_spritePos = Vector2f(0, 0);

	const float road = 4 * GlobalVariables::MAP_CELL_SIZE - m_size.x;
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

EnemyStats Enemy::getData() const
{
	return m_data;
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
	GameObject::draw(target);

	drawLifeBar(target);
}

void Enemy::hit(float damage)
{
	m_data.health -= damage * (1 - m_data.reflection);
}

bool Enemy::isAlive() const
{
	return m_data.health > 0.f;
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

void Enemy::drawLifeBar(RenderTarget *target)
{
	if (m_data.health <= 0)
		return;

	const float healthRate = m_data.health / m_stats.health;
	lifeBar->setPos(pos() - Vector2f(0, LifeBar::LIFE_BAR_HEIGHT * Settings::Instance().getScaleFactor().y));
	lifeBar->setValue(healthRate);

	lifeBar->draw(target);
}

void Enemy::moveEnemy(const Vector2f &d)
{
	m_pos += d;
	update();
}

Vector2f Enemy::enemyPos() const
{
	return m_pos + Vector2f(1, 1);
}

Vector2f Enemy::enemyCenter() const
{
	return m_pos + m_spritePos + Vector2f(m_size.x/2, m_size.y/2);
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
	RESOURCES::TEXTURE_TYPE texture_id;
	switch (type)
	{
	case SMALL_SLOW:
		texture_id = RESOURCES::ENEMY_BIG_MED;
		stats.health = 50.f;
		stats.speed = 10.f;
		stats.damage = 20.f;
		size.x = 1;
		size.y = 1;
		break;
	case SMALL_MEDIUM:
		texture_id = RESOURCES::ENEMY_BIG_TEXTURE;
		stats.health = 40.f;
		stats.speed = 7.5f;
		stats.damage = 10.f;
		size.x = 1;
		size.y = 1;
		break;
	case SMALL_FAST:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 30.f;
		stats.speed = 5.f;
		stats.damage = 5.f;
		size.x = 2;
		size.y = 2;
		break;
	case MID_SLOW:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 100.f;
		stats.speed = 20.f;
		stats.damage = 40.f;
		size.x = 2;
		size.y = 2;
		break;
	case MID_MEDIUM:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 80.f;
		stats.speed = 15.f;
		stats.damage = 20.f;
		size.x = 2;
		size.y = 2;
		break;
	case MID_FAST:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 60.f;
		stats.speed = 10.f;
		stats.damage = 10.f;
		break;
	case BIG_SLOW:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 200.f;
		stats.speed = 40.f;
		stats.damage = 80.f;
		size.x = 4;
		size.y = 4;
		break;
	case BIG_MEDIUM:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 160.f;
		stats.speed = 30.f;
		stats.damage = 40.f;
		size.x = 4;
		size.y = 4;
		break;
	case BIG_FAST:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 120.f;
		stats.speed = 20.f;
		stats.damage = 20.f;
		size.x = 4;
		size.y = 4;
		break;
	case VERY_FAST:
		texture_id = RESOURCES::ENEMY_TEXTURE;
#ifdef TEST_BUILD
		stats.health = 5.f;
#else
		stats.health = 35.f;
#endif
		stats.speed = 2.5f;
		stats.damage = 15.f;
		break;
	case VERY_BIG:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 400.f;
		stats.speed = 45.f;
		stats.damage = 90.f;
		size.x = 4;
		size.y = 4;
		break;
	default:
		break;
	}
	stats.speed *= 5;
	Enemy *enemy = new Enemy(texture_id, startPos, stats, size);
	return enemy;
}

vector<ENEMY_TYPES> EnemiesFactory::generateEnemies(unsigned int n)
{
	vector<ENEMY_TYPES> spawnEnemies;
	switch (n)
	{
	case 0:
	{
#ifdef TEST_BUILD
		addEnemiesByType(VERY_FAST, 3, &spawnEnemies);
#else
		addEnemiesByType(SMALL_SLOW, 100, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 40, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 10, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 5, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 2, &spawnEnemies);
		addEnemiesByType(MID_FAST, 1, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 0, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 0, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 0, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 0, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 0, &spawnEnemies);
#endif
	}
		break;
	case 1:
	{
		addEnemiesByType(SMALL_SLOW, 150, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 80, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 20, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 10, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 5, &spawnEnemies);
		addEnemiesByType(MID_FAST, 2, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 0, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 1, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 0, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 0, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 0, &spawnEnemies);
	}
		break;
	case 2:
	{
		addEnemiesByType(SMALL_SLOW, 200, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 100, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 50, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 20, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 15, &spawnEnemies);
		addEnemiesByType(MID_FAST, 5, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 2, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 3, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 1, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 0, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 0, &spawnEnemies);
	}
		break;
	case 3:
	{
		addEnemiesByType(SMALL_SLOW, 250, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 150, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 50, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 50, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 25, &spawnEnemies);
		addEnemiesByType(MID_FAST, 15, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 10, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 5, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 2, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 0, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 0, &spawnEnemies);
	}
		break;
	case 4:
	{
		addEnemiesByType(SMALL_SLOW, 150, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 100, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 100, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 75, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 30, &spawnEnemies);
		addEnemiesByType(MID_FAST, 20, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 15, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 10, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 5, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 0, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 0, &spawnEnemies);
	}
		break;
	case 5:
	{
		addEnemiesByType(SMALL_SLOW, 100, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 75, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 125, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 100, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 50, &spawnEnemies);
		addEnemiesByType(MID_FAST, 25, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 20, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 25, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 10, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 0, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 1, &spawnEnemies);
	}
		break;
	case 6:
	{
		addEnemiesByType(SMALL_SLOW, 75, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 50, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 150, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 150, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 75, &spawnEnemies);
		addEnemiesByType(MID_FAST, 30, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 30, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 25, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 10, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 0, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 1, &spawnEnemies);
	}
		break;
	case 7:
	{
		addEnemiesByType(SMALL_SLOW, 50, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 25, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 50, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 200, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 100, &spawnEnemies);
		addEnemiesByType(MID_FAST, 35, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 30, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 25, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 10, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 1, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 1, &spawnEnemies);
	}
		break;
	case 8:
	{
		addEnemiesByType(SMALL_SLOW, 25, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 10, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 25, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 250, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 150, &spawnEnemies);
		addEnemiesByType(MID_FAST, 50, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 45, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 35, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 20, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 1, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 2, &spawnEnemies);
	}
		break;
	case 9:
	{
		addEnemiesByType(SMALL_SLOW, 10, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 5, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 5, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 200, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 200, &spawnEnemies);
		addEnemiesByType(MID_FAST, 75, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 75, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 50, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 35, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 2, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 2, &spawnEnemies);
	}
		break;
	case 10:
	{
		addEnemiesByType(SMALL_SLOW, 0, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 0, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 0, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 150, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 100, &spawnEnemies);
		addEnemiesByType(MID_FAST, 50, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 100, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 75, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 50, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 2, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 3, &spawnEnemies);
	}
		break;
	case 11:
	{
		addEnemiesByType(SMALL_SLOW, 0, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 0, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 0, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 100, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 75, &spawnEnemies);
		addEnemiesByType(MID_FAST, 25, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 150, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 100, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 75, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 5, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 5, &spawnEnemies);
	}
		break;
	case 12:
	{
		addEnemiesByType(SMALL_SLOW, 0, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 0, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 0, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 75, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 50, &spawnEnemies);
		addEnemiesByType(MID_FAST, 10, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 200, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 150, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 100, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 10, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 10, &spawnEnemies);
	}
		break;
	case 13:
	{
		addEnemiesByType(SMALL_SLOW, 0, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 0, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 0, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 50, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 25, &spawnEnemies);
		addEnemiesByType(MID_FAST, 5, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 300, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 200, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 100, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 20, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 20, &spawnEnemies);
	}
		break;
	case 14:
	{
		addEnemiesByType(SMALL_SLOW, 0, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 0, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 0, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 25, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 10, &spawnEnemies);
		addEnemiesByType(MID_FAST, 0, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 150, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 300, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 200, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 30, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 30, &spawnEnemies);
	}
		break;
	case 128:
	{
		addEnemiesByType(SMALL_SLOW, 100, &spawnEnemies);
		addEnemiesByType(SMALL_MEDIUM, 40, &spawnEnemies);
		addEnemiesByType(SMALL_FAST, 10, &spawnEnemies);
		addEnemiesByType(MID_SLOW, 5, &spawnEnemies);
		addEnemiesByType(MID_MEDIUM, 2, &spawnEnemies);
		addEnemiesByType(MID_FAST, 1, &spawnEnemies);
		addEnemiesByType(BIG_SLOW, 0, &spawnEnemies);
		addEnemiesByType(BIG_MEDIUM, 0, &spawnEnemies);
		addEnemiesByType(BIG_FAST, 0, &spawnEnemies);
		addEnemiesByType(VERY_FAST, 0, &spawnEnemies);
		addEnemiesByType(VERY_BIG, 0, &spawnEnemies);
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
