#include "enemy.h"
#include "globalvariables.h"
#include "map.h"

Enemy::Enemy(const RESOURCES::TEXTURE_TYPE &texture_id,
			 const Vector2f &startPos,
			 const EnemyStats& stats)
	: GameObject(texture_id, startPos,
				 Vector2i(GlobalVariables::CELL_SIZE, GlobalVariables::CELL_SIZE),
				 4)
	,m_stats(stats)
	,moveCounter(0)
{

}

EnemyStats Enemy::getStats() const
{
	return m_stats;
}

bool Enemy::moveStep()
{
	moveCounter++;
	move(currentStep);
	if (moveCounter >= m_stats.speed)
	{
		setPos(targetPos);
		return true;
	}
	return false;
}

void Enemy::moveNext(int direction)
{
	targetPos = pos();
	moveCounter = 0;
	currentDirection = direction;

	currentStep.x = 0;
	currentStep.y = 0;
	switch (currentDirection)
	{
	case Map::STAY:
		break;
	case Map::UP:
		targetPos.y -= GlobalVariables::Instance().tileSize().y;
		currentStep.y = -GlobalVariables::Instance().tileSize().y / m_stats.speed;
		break;
	case Map::DOWN:
		targetPos.y += GlobalVariables::Instance().tileSize().y;
		currentStep.y = GlobalVariables::Instance().tileSize().y / m_stats.speed;
		break;
	case Map::LEFT:
		targetPos.x -= GlobalVariables::Instance().tileSize().x;
		currentStep.x = -GlobalVariables::Instance().tileSize().x / m_stats.speed;
		break;
	case Map::RIGHT:
		targetPos.x += GlobalVariables::Instance().tileSize().x;
		currentStep.x = GlobalVariables::Instance().tileSize().x / m_stats.speed;
		break;
	}
}

void Enemy::update()
{

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
		stats.speed = 10;
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
