#include "enemy.h"
#include "globalvariables.h"
#include "map.h"
#include "lifebar.h"
#include "settings.h"

Enemy::Enemy(const RESOURCES::TEXTURE_TYPE &texture_id,
			 const Vector2f &startPos,
			 const EnemyStats& stats)
	: GameObject(texture_id, startPos,
				 Vector2i(GlobalVariables::CELL_SIZE, GlobalVariables::CELL_SIZE),
				 4)
	,m_stats(stats)
	,moveCounter(0)
	,spriteDirection(DEFAULT_DOWN)
	,isFreezed(false)
	,startFreeze(false)
{
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
	move(currentStep);
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

	targetPos = pos();
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
		targetPos.y -= GlobalVariables::Instance().tileSize().y;
		currentStep.y = -GlobalVariables::Instance().tileSize().y / m_data.speed;
		newDirection = SPRITE_DIRECTION::SPRITE_UP;
		break;
	case Map::DOWN:
		targetPos.y += GlobalVariables::Instance().tileSize().y;
		currentStep.y = GlobalVariables::Instance().tileSize().y / m_data.speed;
		newDirection = SPRITE_DIRECTION::DEFAULT_DOWN;
		break;
	case Map::LEFT:
		targetPos.x -= GlobalVariables::Instance().tileSize().x;
		currentStep.x = -GlobalVariables::Instance().tileSize().x / m_data.speed;
		newDirection = SPRITE_DIRECTION::SPRITE_LEFT;
		break;
	case Map::RIGHT:
		targetPos.x += GlobalVariables::Instance().tileSize().x;
		currentStep.x = GlobalVariables::Instance().tileSize().x / m_data.speed;
		newDirection = SPRITE_DIRECTION::SPRITE_RIGHT;
		break;
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
				origin.y = GlobalVariables::CELL_SIZE;
			}
			else if (spriteDirection == SPRITE_UP)
			{
				angle = -RIGHT_ANGLE;
				origin.y = GlobalVariables::CELL_SIZE;
			}
			break;
		case SPRITE_UP:
			if (spriteDirection == SPRITE_LEFT)
			{
				angle = RIGHT_ANGLE;
				origin.x = GlobalVariables::CELL_SIZE;
				origin.y = GlobalVariables::CELL_SIZE;
			}
			else if (spriteDirection == SPRITE_RIGHT)
			{
				angle = -RIGHT_ANGLE;
				origin.x = GlobalVariables::CELL_SIZE;
				origin.y = GlobalVariables::CELL_SIZE;
			}
			break;
		case SPRITE_RIGHT:
			if (spriteDirection == DEFAULT_DOWN)
			{
				angle = -RIGHT_ANGLE;
				origin.x = GlobalVariables::CELL_SIZE;
			}
			else if (spriteDirection == SPRITE_UP)
			{
				angle = RIGHT_ANGLE;
				origin.x = GlobalVariables::CELL_SIZE;
			}
			break;
		}
		spriteDirection = newDirection;

		sprite.setOrigin(origin);
		sprite.rotate(angle);
	}
}

void Enemy::update()
{
	if (startFreeze)
	{
		if (freezeTimer.check(freezeDuration))		
			isFreezed = false;
	}
	GameObject::update();
}

void Enemy::draw(RenderTarget * const target)
{
	GameObject::draw(target);
	drawLifeBar(target);
}

void Enemy::hit(float damage)
{
	m_data.health -= damage;
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
//	if (spaceShip->getData().health <= 0)
//		return;

	const float healthRate = m_data.health / m_stats.health;
	lifeBar->setPos(pos() - Vector2f(0, LifeBar::LIFE_BAR_HEIGHT * Settings::Instance().getScaleFactor().y));
	lifeBar->setValue(healthRate);

	lifeBar->draw(target);
}

Enemy *EnemiesFactory::createEnemy(EnemiesFactory::TYPES type, const Vector2f &startPos)
{
	EnemyStats stats;
	stats.speed = 0.f;
	stats.health = 0.f;
	stats.damage = 0.f;
	RESOURCES::TEXTURE_TYPE texture_id;
	switch (type)
	{
	case SMALL_SLOW:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 45.f;
		stats.speed = 15;
		stats.damage = 1.f;
		break;
	case SMALL_MEDIUM:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 45.f;
		stats.speed = 0.2f;
		break;
	case SMALL_FAST:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 45.f;
		stats.speed = 0.2f;
		break;
	case MID_SLOW:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 45.f;
		stats.speed = 0.2f;
		break;
	case MID_MEDIUM:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 45.f;
		stats.speed = 0.2f;
		break;
	case MID_FAST:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 45.f;
		stats.speed = 0.2f;
		break;
	case BIG_SLOW:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 45.f;
		stats.speed = 0.2f;
		break;
	case BIG_MEDIUM:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 45.f;
		stats.speed = 0.2f;
		break;
	case BIG_FAST:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 45.f;
		stats.speed = 0.2f;
		break;
	case VERY_FAST:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 45.f;
		stats.speed = 0.2f;
		break;
	case VERY_BIG:
		texture_id = RESOURCES::ENEMY_TEXTURE;
		stats.health = 45.f;
		stats.speed = 0.2f;
		break;
	default:
		break;
	}
	Enemy *enemy = new Enemy(texture_id, startPos, stats);
	return enemy;
}
