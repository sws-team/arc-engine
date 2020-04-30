#include "enemy.h"
#include "map.h"
#include "lifebar.h"
#include "level.h"
#include "tower.h"
#include "camera.h"
#include "projectile.h"
#include "gamepanel.h"
#include "gameoptions.h"
#include "engine.h"
#include "gamemanagers.h"
#include "managers.h"
#include "enginedef.h"
#include "collisions.h"

Enemy::Enemy(const TextureType &texture_id,
			 const sf::Vector2f &startPos,
			 const EnemyStats& stats,
			 const sf::Vector2i &cellSize,
			 const float frameCount,
			 const float animationSpeed)
	: GameObject(texture_id, startPos,
				 sf::Vector2i(GameOptions::MAP_CELL_SIZE * cellSize.x * ENEMY_SCALE,
						  GameOptions::MAP_CELL_SIZE * cellSize.y * ENEMY_SCALE),
				 4)
	,m_type(UNKNOWN)
	,m_stats(stats)
	,spriteDirection(DEFAULT_DOWN)
	,isFreezed(false)
	,ability(nullptr)
	,m_stopped(false)
	,m_visible(true)
	,m_burned(false)
	,m_lastCell(sf::Vector2i(0, 0))
	,lastUp(false)
	,m_ignoreMoveTimer(false)
	,m_speedModifier(1.0)
{
	moveStep = sf::Vector2f(Engine::Instance().settingsManager()->getScaleFactor().x * 1.f,
						Engine::Instance().settingsManager()->getScaleFactor().y * 1.f);

	m_size.x = GameOptions::MAP_CELL_SIZE * cellSize.x * ENEMY_SCALE;
	m_size.y = GameOptions::MAP_CELL_SIZE * cellSize.y * ENEMY_SCALE;

	sprite.scale(1.f/ENEMY_SCALE, 1.f/ENEMY_SCALE);

	m_spritePos = sf::Vector2f(0, 0);

	const float road = 4 * Engine::Instance().options<GameOptions>()->mapTileSize().x -
			Engine::Instance().options<GameOptions>()->mapTileSize().x * cellSize.x;
	if (road != 0.f)
	{
		m_spritePos.x = rand() % static_cast<int>(road);
		m_spritePos.y = rand() % static_cast<int>(road);
	}
	m_pos = startPos;

	m_data = m_stats;
	lifeBar = new LifeBar();
	lifeBar->init(sf::Vector2i(getEnemySize().x,
						   LifeBar::LIFE_BAR_HEIGHT * Engine::Instance().settingsManager()->getScaleFactor().y),
				  sf::Color::Red);

	burnAnimation = new GameObject(GAME_TEXTURE::BURN, startPos, sf::Vector2i(32, 32), 4);
	this->frameCount = frameCount;
	this->animationSpeed = animationSpeed;
	setPos(m_pos + m_spritePos);
}

Enemy::~Enemy()
{
	delete burnAnimation;
	delete lifeBar;
	if (ability != nullptr)
		delete ability;
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

void Enemy::moveEnemy()
{
	if (!m_ignoreMoveTimer)
	{
		if (!moveTimer.check(actualSpeed()))
			return;
	}

	sf::Vector2f offset;
	offset.x = 0;
	offset.y = 0;

	switch (currentDirection)
	{
	case Map::STAY:
		break;
	case Map::UP:
		offset.y -= actualMoveStep().y;
		break;
	case Map::DOWN:
		offset.y += actualMoveStep().y;
		break;
	case Map::LEFT:
		offset.x -= actualMoveStep().x;
		break;
	case Map::RIGHT:
		offset.x += actualMoveStep().x;
		break;
	case Map::NO_MOVE:
		return;
	}
	m_pos += offset;
	setPos(m_pos + m_spritePos);
	offset.x *= 1.f/Engine::Instance().options<GameOptions>()->gameSpeed();
	offset.y *= 1.f/Engine::Instance().options<GameOptions>()->gameSpeed();
	update();
}

void Enemy::moveNext(int direction)
{
	if (direction == 0)
		return;

	lastUp = false;
	currentDirection = direction;
	SPRITE_DIRECTION newDirection = DEFAULT_DOWN;
	switch (currentDirection)
	{
	case Map::STAY:
		break;
	case Map::UP:
		lastUp = true;
		newDirection = SPRITE_DIRECTION::SPRITE_UP;
		break;
	case Map::DOWN:
		newDirection = SPRITE_DIRECTION::DEFAULT_DOWN;
		break;
	case Map::LEFT:
		newDirection = SPRITE_DIRECTION::SPRITE_LEFT;
		break;
	case Map::RIGHT:
		newDirection = SPRITE_DIRECTION::SPRITE_RIGHT;
		break;
	case Map::NO_MOVE:
		return;
	}
	if (newDirection != spriteDirection)
	{
		float angle = 0;
		sf::Vector2f origin;
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
	if (isFreezed)
	{
		if (freezeTimer.check(freezeDuration))
			isFreezed = false;
	}
	if (m_burned)
	{
		if (burnTimer.check(Balance::Instance().getBurnDuration()))
			m_burned = false;
	}
	if (m_burned)
	{
		const int x = this->size.x/ ENEMY_SCALE /GameOptions::MAP_CELL_SIZE - 1;
		const int y = this->size.y/ ENEMY_SCALE /GameOptions::MAP_CELL_SIZE - 1;
		const sf::Vector2f posOffset = sf::Vector2f(Engine::Instance().options<GameOptions>()->mapTileSize().x/2 * x,
											Engine::Instance().options<GameOptions>()->mapTileSize().y/2 * y);
		burnAnimation->setPos(this->pos() + posOffset);
		if (burnAttack.check(Balance::Instance().getBurnAttackSpeed()))
			hit(Balance::Instance().getBurnDamage());
		burnAnimation->update();
	}
	GameObject::update();
}

void Enemy::draw(sf::RenderTarget * const target)
{
	if (!m_visible)
		return;

	GameObject::draw(target);
	drawLifeBar(target);

	if (ability != nullptr)
		ability->draw(target);
	if (m_burned)
		burnAnimation->draw(target);
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
	if (isFreezed)
	{
		freezeK = fmax(freezeK, k);
		freezeDuration = fmax(freezeDuration, duration);
		return;
	}
	freezeTimer.reset();
	freezeDuration = duration;
	freezeK = k;
	isFreezed = true;
}

void Enemy::heal(float health)
{
	m_data.health += health;
	if (m_data.health > m_stats.health)
		m_data.health = m_stats.health;
	Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::HEAL_EFFECT,
											 this->getCenter() - sf::Vector2f(GameOptions::MAP_CELL_SIZE/2,
																		  GameOptions::MAP_CELL_SIZE/2),
											 sf::Vector2i(GameOptions::MAP_CELL_SIZE,
													  GameOptions::MAP_CELL_SIZE),
											 50, 8, 0);
}

void Enemy::protect(float shell, bool show)
{
	m_data.reflection += shell;
	if(m_data.reflection >= 0.9f)
		m_data.reflection = 0.9f;
	if (m_data.reflection < 0.f)
		m_data.reflection = 0.f;
	if (show)
		Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::SHELL_EFFECT, this->pos(),
											 sf::Vector2i(GameOptions::MAP_CELL_SIZE, GameOptions::MAP_CELL_SIZE),
												 50, 4, 0);
}

void Enemy::setReflection(const float reflection)
{
	m_data.reflection = reflection;
}

void Enemy::drawLifeBar(sf::RenderTarget *target)
{
	if (m_data.health <= 0)
		return;

	const float healthRate = m_data.health / m_stats.health;
	lifeBar->setPos(sf::Vector2f(sprite.getGlobalBounds().left,
							 sprite.getGlobalBounds().top) - sf::Vector2f(0, (LIFEBAR_OFFSET + LifeBar::LIFE_BAR_HEIGHT) * Engine::Instance().settingsManager()->getScaleFactor().y));
	lifeBar->setValue(healthRate);
	lifeBar->draw(target);
}

sf::Vector2f Enemy::actualMoveStep() const
{
	if (isFreezed)
		return sf::Vector2f(moveStep.x * freezeK,
							moveStep.y * freezeK);
	return moveStep;
}

void Enemy::setIgnoreMoveTimer(bool ignoreMoveTimer)
{
	m_ignoreMoveTimer = ignoreMoveTimer;
}

float Enemy::actualSpeed() const
{
	return m_data.speed * m_speedModifier;
}

bool Enemy::getLastUp() const
{
	return lastUp;
}

ARMOR_TYPE Enemy::getArmorType() const
{
	return m_stats.armorType;
}

void Enemy::setFaster(const float modifier)
{
	m_speedModifier = modifier;
}

EnemyAbility *Enemy::getAbility()
{
	return ability;
}

sf::Vector2i Enemy::getLastCell() const
{
	return m_lastCell;
}

void Enemy::setLastCell(const sf::Vector2i &lastCell)
{
	m_lastCell = lastCell;
}

void Enemy::startBurn()
{
	m_burned = true;
	burnTimer.reset();
}

sf::Vector2f Enemy::getEnemySize() const
{
	const sf::Vector2f dsize = getSize();
	return sf::Vector2f(dsize.x / ENEMY_SCALE,
					dsize.y / ENEMY_SCALE);
}

sf::FloatRect Enemy::enemyRect() const
{
	return sf::FloatRect(pos(), getEnemySize());
}

ENEMY_TYPES Enemy::type() const
{
	return m_type;
}

void Enemy::setType(const ENEMY_TYPES &type)
{
	m_type = type;
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

sf::Vector2f Enemy::enemyPos() const
{
	return m_pos + sf::Vector2f(1, 1);
}

sf::Vector2f Enemy::enemyCenter() const
{
	return sf::Vector2f(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width/2,
					sprite.getGlobalBounds().top + sprite.getGlobalBounds().height/2);
}

EnemiesFactory::EnemyInfo EnemiesFactory::getEnemyInfo(ENEMY_TYPES type)
{
	EnemyInfo::ABILITY_TYPE abilityType = EnemyInfo::NONE;

	const EnemyStats stats = Balance::Instance().getEnemyStats(type);

	sf::Vector2i size;
	size.x = 1;
	size.y = 1;
	TextureType texture_id = 0;
	switch (type)
	{
	//small
	case INFANTRY:
	{
		texture_id = GAME_TEXTURE::ENEMY_INFANTRY;
		size.x = 1;
		size.y = 1;
	}
		break;
	case CAR:
	{
		texture_id = GAME_TEXTURE::ENEMY_CAR;
		size.x = 1;
		size.y = 1;
		abilityType = EnemyInfo::STRONG;
	}
		break;
	case TRICYCLE:
	{
		texture_id = GAME_TEXTURE::ENEMY_TRICYCLE;
		size.x = 1;
		size.y = 1;
	}
		break;
	case WORM:
	{
		texture_id = GAME_TEXTURE::ENEMY_WORM;
		size.x = 1;
		size.y = 1;
	}
		break;
	case SELFHEAL_ENEMY:
	{
		texture_id = GAME_TEXTURE::ENEMY_SELF_HEAL;
		size.x = 1;
		size.y = 1;
		abilityType = EnemyInfo::SELF_HEAL;
	}
		break;
	case TRACTOR:
	{
		texture_id = GAME_TEXTURE::ENEMY_TRACTOR;
		abilityType = EnemyInfo::DOWN_TOWER;
		size.x = 1;
		size.y = 1;
	}
		break;
		//mid
	case ANOTHER_ENEMY:
	{
		texture_id = GAME_TEXTURE::ENEMY_PLANE;
		size.x = 2;
		size.y = 2;
		abilityType = EnemyInfo::FASTER;
	}
		break;
	case TANK:
	{
		texture_id = GAME_TEXTURE::ENEMY_TANK;
		size.x = 2;
		size.y = 2;
		abilityType = EnemyInfo::DOWNGRADE_TOWER;
	}
		break;
	case SPIDER:
	{
		texture_id = GAME_TEXTURE::ENEMY_SPIDER;
		size.x = 2;
		size.y = 2;
		abilityType = EnemyInfo::SHUTDOWN_TOWER;
	}
		break;
	case MID_FAST:
	{
		texture_id = GAME_TEXTURE::ENEMY_HELICOPTER;
		size.x = 2;
		size.y = 2;
		abilityType = EnemyInfo::STRONG;
	}
		break;
	case REPAIR_ENEMY:
	{
		texture_id = GAME_TEXTURE::ENEMY_REPAIR;
		size.x = 2;
		size.y = 2;
		abilityType = EnemyInfo::HEAL_NEAR;
	}
		break;
	case SHIELD_ENEMY:
	{
		texture_id = GAME_TEXTURE::ENEMY_SHIELD;
		size.x = 2;
		size.y = 2;
		abilityType = EnemyInfo::SHELL_NEAR;
	}
		break;
	case TELEPORT_ENEMY:
	{
		texture_id = GAME_TEXTURE::ENEMY_TELEPORT;
		size.x = 2;
		size.y = 2;
		abilityType = EnemyInfo::TELEPORT;
	}
		break;
		//big
	case BIG_SLOW:
	{
		texture_id = GAME_TEXTURE::ENEMY_AIRCARRIER;
		size.x = 4;
		size.y = 4;
		abilityType = EnemyInfo::RAGE;
	}
		break;
	case BIG_TANK:
	{
		texture_id = GAME_TEXTURE::ENEMY_BIG_TANK;
		size.x = 4;
		size.y = 4;
		abilityType = EnemyInfo::KILL_TOWER;
	}
		break;
	case SPAWN_ENEMY:
	{
		texture_id = GAME_TEXTURE::ENEMY_COW;
		size.x = 4;
		size.y = 4;
		abilityType = EnemyInfo::SPAWN;
	}
		break;
	case SPAWN_WORM:
	{
		const EnemyInfo wormInfo = getEnemyInfo(WORM);
		texture_id = GAME_TEXTURE::ENEMY_SPAWN_WORM;
		size = wormInfo.size;
		abilityType = wormInfo.abilityType;
	}
		break;
	default:
		break;
	}
	EnemyInfo info;
	info.texture_id = texture_id;
	info.size = size;
	info.stats = stats;
	info.abilityType = abilityType;
	info.animationSpeed = getAnimationSpeed(type);
	info.frameCount = getFrameCount(type);
	return info;
}

float EnemiesFactory::getAnimationSpeed(ENEMY_TYPES type)
{
	switch (type)
	{
	case INFANTRY:
		break;
	case CAR:
		return 50;
	case TRICYCLE:
		break;
	case WORM:
		return 200;
		break;
	case SELFHEAL_ENEMY:
		break;
	case TRACTOR:
		return 100;
	case ANOTHER_ENEMY:
		return FasterAbility::FASTER_ANIMATION_SPEED;
	case TANK:
		break;
	case SPIDER:
		return 100;
	case MID_FAST:
		return 100;
	case REPAIR_ENEMY:
		break;
	case SHIELD_ENEMY:
		return 300;
	case TELEPORT_ENEMY:
		return TeleportAbility::TELEPORT_DEFAULT_ANIMATION_SPEED;
	case BIG_SLOW:
		break;
	case BIG_TANK:
		break;
	case SPAWN_ENEMY:
		return SpawnEnemy::DEFAULT_ANIMATION_SPEED;
	case SPAWN_WORM:
		return getAnimationSpeed(WORM);
	default:
		break;
	}
	return 200;
}

float EnemiesFactory::getFrameCount(ENEMY_TYPES type)
{
	switch (type)
	{
	case INFANTRY:
		break;
	case CAR:
		break;
	case TRICYCLE:
		break;
	case WORM:
		return 6;
	case SELFHEAL_ENEMY:
		break;
	case TRACTOR:
		return 5;
	case ANOTHER_ENEMY:
		break;
	case TANK:
		break;
	case SPIDER:
		return 5;
	case MID_FAST:
		break;
	case REPAIR_ENEMY:
		break;
	case SHIELD_ENEMY:
		return 9;
	case TELEPORT_ENEMY:
		return TeleportAbility::TELEPORT_FRAME_COUNT;
	case BIG_SLOW:
		break;
	case BIG_TANK:
		break;
	case SPAWN_ENEMY:
		return SpawnEnemy::DEFAULT_SPAWN_FRAME_COUNT;
	case SPAWN_WORM:
		return getFrameCount(WORM);
	default:
		break;
	}
	return 4;
}

Enemy *EnemiesFactory::createEnemy(ENEMY_TYPES type, const sf::Vector2f &startPos)
{
	const EnemyInfo info = getEnemyInfo(type);

	EnemyAbility *ability = nullptr;
	switch (info.abilityType)
	{
	case EnemyInfo::RAGE:
		ability = new RageAbility();
		break;
	case EnemyInfo::SPAWN:
		ability = new SpawnEnemy();
		break;
	case EnemyInfo::TELEPORT:
		ability = new TeleportAbility();
		break;
	case EnemyInfo::SHELL_NEAR:
		ability = new ShellNearAbility();
		break;
	case EnemyInfo::HEAL_NEAR:
		ability = new HealNearAbility();
		break;
	case EnemyInfo::SHUTDOWN_TOWER:
		ability = new ShutdownTowerAbility();
		break;
	case EnemyInfo::DOWN_TOWER:
		ability = new DownTowerAbility();
		break;
	case EnemyInfo::STRONG:
		ability = new StrongAbility();
		break;
	case EnemyInfo::SELF_HEAL:
		ability = new SelfHealAbility();
		break;
	case EnemyInfo::FASTER:
		ability = new FasterAbility();
		break;
	case EnemyInfo::KILL_TOWER:
		ability = new KillTowerAbility();
		break;
	case EnemyInfo::DOWNGRADE_TOWER:
		ability = new DowngradeTowerAbility();
		break;
	default:
		break;
	}

	const EnemyStats stats = info.stats;
	Enemy *enemy = new Enemy(info.texture_id,
							 startPos,
							 stats, info.size,
							 info.frameCount,
							 info.animationSpeed);
	enemy->setAbility(ability);
	enemy->setType(type);
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

void EnemyAbility::draw(sf::RenderTarget * const target)
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
	area = cells * Engine::Instance().options<GameOptions>()->mapTileSize().x;
	m_interval = m_abilityInterval;
}

bool AreaAbility::inArea(Enemy *enemy) const
{
	const sf::Vector2f center = owner->enemyCenter();
	const sf::Vector2f enemyCenter = enemy->enemyCenter();
	if (center.x < enemyCenter.x  + area &&
			center.x > enemyCenter.x - area &&
			center.y < enemyCenter.y  + area &&
			center.y > enemyCenter.y - area)
		return true;
	return false;
}

HealNearAbility::HealNearAbility()
	: AreaAbility(Balance::Instance().getHealInterval())
{

}

void HealNearAbility::use()
{
	const std::vector<Enemy*> enemies = Engine::Instance().options<GameOptions>()->level()->getAllEnemies();
	for(Enemy *enemy : enemies)
	{
		if (enemy == owner)
			continue;
		if (inArea(enemy))
			enemy->heal(enemy->getPureStats().health * Balance::Instance().getHealValue());
	}
}

ShellNearAbility::ShellNearAbility()
	: AreaAbility(Balance::Instance().getShieldInterval())
{

}

void ShellNearAbility::use()
{
	const std::vector<Enemy*> enemies = Engine::Instance().options<GameOptions>()->level()->getAllEnemies();
	for(Enemy *enemy : enemies)
	{
		if (inArea(enemy))
			enemy->protect(Balance::Instance().getShieldValue());
	}
}

SelfHealAbility::SelfHealAbility()
	: EnemyAbility(Balance::Instance().getSelfhealInterval())
{

}

void SelfHealAbility::use()
{
	owner->heal(owner->getPureStats().health * Balance::Instance().getSelfhealValue());
}

TeleportAbility::TeleportAbility()
	: EnemyAbility(Balance::Instance().getTeleportInterval())
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
		owner->row = TELEPORT_DISAPPEAR_ROW;
		owner->animationSpeed = TELEPORT_ANIMATION_SPEED;
		owner->currentFrame = 0;
		m_state = DISAPPEAR;
		m_interval = TELEPORT_ANIMATION_SPEED * TELEPORT_FRAME_COUNT - 50;
	}
		break;
	case DISAPPEAR:
	{
		owner->setVisible(false);
		owner->setIgnoreMoveTimer(true);
		const int steps = 200 + rand() % 500;
		for (int i = 0; i <  steps; ++i)
			Engine::Instance().options<GameOptions>()->level()->enemyMove(owner);
		owner->setIgnoreMoveTimer(false);
		m_state = APPEAR;
		m_interval = 1000;
	}
		break;
	case APPEAR:
	{		
		owner->setVisible(true);
		owner->row = TELEPORT_APPEAR_ROW;
		owner->animationSpeed = TELEPORT_ANIMATION_SPEED;
		owner->currentFrame = 0;
		m_state = FINISH;
		m_interval = TELEPORT_ANIMATION_SPEED * TELEPORT_FRAME_COUNT;
	}
		break;
	case FINISH:
	{
		owner->row = 0;
		owner->currentFrame = 0;
		owner->animationSpeed = TELEPORT_DEFAULT_ANIMATION_SPEED;
		owner->setStopped(false);
		m_state = READY;
		m_interval = m_abilityInterval;
	}
		break;
	default:
		break;
	}
}



TowerEffectAbility::TowerEffectAbility(float msec)
	: EnemyAbility(msec)
	,targetTower(nullptr)
	,m_state(READY)
	,projectile(nullptr)
{

}

void TowerEffectAbility::draw(sf::RenderTarget * const target)
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

Tower *TowerEffectAbility::getTarget()
{
	return targetTower;
}

void TowerEffectAbility::use()
{
	switch (m_state)
	{
	case READY:
	{
		reflection = owner->getData().reflection;
		owner->setReflection(reflection + REFLECTION_MODIFIER);
		owner->setStopped(true);

		owner->row = 1;
		owner->frameCount = 5;
		owner->currentFrame = 0;
		m_interval = owner->animationSpeed * owner->frameCount;
		m_state = SHOOT;
	}
		break;
	case SHOOT:
	{
		owner->row = 3;
		owner->frameCount = 1;
		owner->currentFrame = 0;

		Tower *target = nullptr;
		const sf::Vector2f center = owner->enemyCenter();
		const std::vector<Tower*> towers = Engine::Instance().options<GameOptions>()->level()->getAllTowers();
		float weight = 0;
		for(Tower* tower : towers)
		{
			if (tower->type() == POWER)
				continue;
			if (tower->isInvulnerable())
				continue;
			if (!tower->isActive())
				continue;
			if (tower->isDowngraded())
				continue;
			const sf::Vector2f towerCenter = tower->getCenter();
			const float a = fabs(center.x - towerCenter.x);
			const float b = fabs(center.y - towerCenter.y);
			const float r = powf(powf(a, 2) + powf(b, 2), 0.5f);
			if (r > Engine::Instance().options<GameOptions>()->tileSize().x * info.cells)
				continue;

			const float currentWeight = Engine::Instance().options<GameOptions>()->panel()->getTowerSellCost(tower);
			if (currentWeight > weight)
			{
				target = tower;
				weight = currentWeight;
			}
		}
		if (target == nullptr)
		{
			getBack();
			return;
		}
		const sf::Vector2f aPos = owner->enemyCenter();
		const float a = aPos.x - target->getCenter().x;
		const float b = aPos.y - target->getCenter().y;
		const float tg = ( b / a );
		float angle = atanf(tg) * 180 / M_PI;
		if (a < 0)
			angle += 180;
		else if (b < 0)
			angle += 360;
		angle -= 180;

		projectile = new GameObject(info.pojectileTextureId, aPos,
									info.projectileSize, info.profectileFrameCount);		
		projectile->animationSpeed = info.projectileAnimationSpeed;
		projectile->sprite.setRotation(angle);
		m_angle = angle;
		targetTower = target;

		m_interval = 20;
		m_state = MOVE;
	}
		break;
	case MOVE:
	{
		if (moveTimer.check(Projectile::PROJECTILE_GAME_SPEED))
		{
			const float x1 = owner->pos().x;
			const float y1 = owner->pos().y;

			const float y2 = y1 + info.projectileSpeed * sinf(m_angle * M_PI/180);
			const float x2 = x1 + info.projectileSpeed * cosf(m_angle * M_PI/180);

			projectile->move(x2-x1, y2-y1);
		}
		if (Collision::PixelPerfectTest(targetTower->getSprite(), projectile->getSprite()))
		{
			delete projectile;
			projectile = nullptr;
			Engine::Instance().soundManager()->playOnce(info.catchSound);
			getBack();
			if (!targetTower->isDowngraded() && targetTower->isActive())
				effect(true);
			else
				use();
		}
	}
		break;
	case WAIT:
	{
		owner->setStopped(false);
		owner->setReflection(reflection - REFLECTION_MODIFIER);
		m_state = FINISHED;
		m_interval = info.duration;

		owner->row = 0;
		owner->frameCount = 5;
		owner->currentFrame = 0;
		owner->update();
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
	owner->row = 2;
	owner->frameCount = 5;
	owner->update();
	m_interval = owner->animationSpeed * owner->frameCount;
	m_state = WAIT;
}

ShutdownTowerAbility::ShutdownTowerAbility()
	: TowerEffectAbility(Balance::Instance().getShutdownInterval())
{
	info.enemyTextureId = GAME_TEXTURE::ENEMY_SPIDER;
	info.animationSize = sf::Vector2i(GameOptions::CELL_SIZE * Enemy::ENEMY_SCALE,
									  GameOptions::CELL_SIZE * Enemy::ENEMY_SCALE);
	info.pojectileTextureId = GAME_TEXTURE::WEB;
	info.projectileSize = sf::Vector2i(GameOptions::CELL_SIZE, GameOptions::CELL_SIZE);
	info.duration = Balance::Instance().getShutdownDuration();
	info.catchSound = GAME_SOUND::CATCH;
	info.cells = Balance::Instance().getShutdownCells();
	info.projectileSpeed = 10;
	info.projectileAnimationSpeed = 150;
	info.profectileFrameCount = 1;
}

ShutdownTowerAbility::~ShutdownTowerAbility()
{
	if (targetTower != nullptr && !targetTower->isActive())
		effect(false);
}

void ShutdownTowerAbility::stop()
{
	m_state = FINISHED;
	if (m_state == FINISHED)
		use();
}

void ShutdownTowerAbility::effect(bool isActive)
{
	if (targetTower == nullptr)
		return;
	if (targetTower->isActive() == !isActive)
		return;
	targetTower->setActive(!isActive);
	if (isActive)
		Engine::Instance().options<GameOptions>()->level()->addAnimation(
					GAME_TEXTURE::WEB, targetTower->pos(),
					sf::Vector2i(GameOptions::CELL_SIZE,
								 GameOptions::CELL_SIZE),
					Balance::Instance().getShutdownDuration()/4, 4, 0);
}

DownTowerAbility::DownTowerAbility()
	: TowerEffectAbility(Balance::Instance().getDowngradeInterval())
{
	info.enemyTextureId = GAME_TEXTURE::ENEMY_TRACTOR;
	info.animationSize = sf::Vector2i(GameOptions::MAP_CELL_SIZE * Enemy::ENEMY_SCALE,
									  GameOptions::MAP_CELL_SIZE * Enemy::ENEMY_SCALE);
	info.pojectileTextureId = GAME_TEXTURE::DOWNGRADE_PROJECTILE;
	info.projectileSize = sf::Vector2i(32, 32);
	info.duration = Balance::Instance().getDowngradeDuration();
	info.catchSound = GAME_SOUND::CATCH;
	info.cells = Balance::Instance().getDowngradeCells();
	info.projectileSpeed = 5;
	info.projectileAnimationSpeed = 50;
	info.profectileFrameCount = 5;
}

DownTowerAbility::~DownTowerAbility()
{
	if (targetTower != nullptr && targetTower->isDowngraded())
		targetTower->setDowngrade(false);
}

void DownTowerAbility::effect(bool isActive)
{
	if (targetTower != nullptr)
		targetTower->setDowngrade(isActive);
}

KillTowerAbility::KillTowerAbility()
	: TowerEffectAbility(Balance::Instance().getKillTowerInterval())
{
	info.enemyTextureId = GAME_TEXTURE::ENEMY_BIG_TANK;
	info.animationSize = sf::Vector2i(GameOptions::CELL_SIZE * Enemy::ENEMY_SCALE,
									  GameOptions::CELL_SIZE * Enemy::ENEMY_SCALE);
	info.pojectileTextureId = GAME_TEXTURE::ENEMY_BULLET;
	info.projectileSize = sf::Vector2i(56, 24);
	info.duration = 0;
	info.catchSound = GAME_SOUND::TANK_SHOOT;
	info.cells = Balance::Instance().getKillTowerCells();
	info.projectileSpeed = 15;
	info.projectileAnimationSpeed = 150;
	info.profectileFrameCount = 3;
}

void KillTowerAbility::effect(bool isActive)
{
	if (!isActive)
		return;
	if(targetTower == nullptr)
		return;
	Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::BOMB_EXPLOSION, targetTower->pos(),
																	 sf::Vector2i(GameOptions::MAP_CELL_SIZE, GameOptions::MAP_CELL_SIZE),
																	 200, 12, 0);
	Engine::Instance().options<GameOptions>()->level()->deleteTower(targetTower);
	targetTower = nullptr;
}

DowngradeTowerAbility::DowngradeTowerAbility()
	: TowerEffectAbility(Balance::Instance().getDowngradeTowerInterval())
{
	info.enemyTextureId = GAME_TEXTURE::ENEMY_TANK;
	info.animationSize = sf::Vector2i(GameOptions::CELL_SIZE * Enemy::ENEMY_SCALE,
									  GameOptions::CELL_SIZE * Enemy::ENEMY_SCALE);
	info.pojectileTextureId = GAME_TEXTURE::ENEMY_ROCKET;
	info.projectileSize = sf::Vector2i(32, 16);
	info.duration = 0;
	info.catchSound = GAME_SOUND::DOWN_SHOOT;
	info.cells = Balance::Instance().getDowngradeTowerCells();
	info.projectileSpeed = 15;
	info.projectileAnimationSpeed = 150;
	info.profectileFrameCount = 3;
}

void DowngradeTowerAbility::effect(bool isActive)
{
	if (!isActive)
		return;
	if(targetTower == nullptr)
		return;

	targetTower->levelDown();
}

#ifdef KILL_AREA
KillAreaTowersAbility::KillAreaTowersAbility()
	: TowerEffectAbility(Balance::Instance().getKillAreaTowersInterval())
{
	info.enemyTextureId = GAME_TEXTURE::ENEMY_BIG_TANK;
	info.animationSize = sf::Vector2i(GameOptions::CELL_SIZE * 2 * Enemy::ENEMY_SCALE,
									  GameOptions::CELL_SIZE * 2 * Enemy::ENEMY_SCALE);
	info.pojectileTextureId = GAME_TEXTURE::ENEMY_BULLET;
	info.projectileSize = sf::Vector2i(56, 24);
	info.duration = 0;
	info.catchSound = GAME_SOUND::CATCH;
	info.cells = Balance::Instance().getKillAreaTowersCells();
	info.projectileSpeed = 15;
	info.profectileFrameCount = 1;

	zeroGround = Engine::Instance().options<GameOptions>()->tileSize().x * Balance::Instance().getFreezeTowerCells();
}

void KillAreaTowersAbility::effect(bool isActive)
{
	if (!isActive)
		return;
	if(targetTower == nullptr)
		return;
	Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::BOMB_EXPLOSION, targetTower->pos(),
																	 sf::Vector2i(GameOptions::MAP_CELL_SIZE, GameOptions::MAP_CELL_SIZE),
																	 200, 12, 0);
	const std::vector<Tower *> towers = findAreaTowers(targetTower);
	for(Tower* tower : towers)
		Engine::Instance().options<GameOptions>()->level()->deleteTower(tower);
	targetTower = nullptr;
}

std::vector<Tower *> KillAreaTowersAbility::findAreaTowers(Tower *tower)
{
	std::vector<Tower *> results;
	const sf::Vector2f epicenter = tower->getCenter();
	const std::vector <Tower*> towers = Engine::Instance().options<GameOptions>()->level()->getAllTowers();
	for(Tower* target : towers)
	{
		if (target == tower)
			continue;

		const float a = fabs(epicenter.x - target->getCenter().x);
		const float b = fabs(epicenter.y - target->getCenter().y);
		const float r = powf(powf(a, 2) + powf(b, 2), 0.5f);
		if (r <= zeroGround)
			results.push_back(target);
	}
	results.push_back(tower);
	return results;
}
#endif

SpawnEnemy::SpawnEnemy()
	: EnemyAbility(Balance::Instance().getSpawnInterval())
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
		owner->row = ACTIVATE_SPAWN_ROW;
		owner->frameCount = SPAWN_FRAME_COUNT;
		owner->animationSpeed = SPAWN_ANIMATION_SPEED * owner->frameCount;
		owner->currentFrame = 0;
		m_interval = owner->animationSpeed;
		m_state = SPAWN;
		currentSpawnCount = 0;
		spawnCount = Balance::Instance().getSpawnCount() + rand() % Balance::Instance().getSpawnCountOffset();
	}
		break;
	case SPAWN:
	{
		owner->row = PROCESS_SPAWN_ROW;
		owner->frameCount = PRECESS_FRAME_COUNT;
		owner->animationSpeed = 10 * owner->frameCount;
		owner->currentFrame = 0;

		const sf::Vector2i cell = Engine::Instance().options<GameOptions>()->camera()->posToCellMap(owner->enemyPos());
		const int direction = Engine::Instance().options<GameOptions>()->level()->getTileDirectionByCell(cell);

		Enemy *enemy = Engine::Instance().options<GameOptions>()->level()->spawn(
					owner->pos(), SPAWN_WORM, 0.1f, direction);

		for (int i = 0; i < 155; ++i)
			Engine::Instance().options<GameOptions>()->level()->enemyMove(enemy);

		m_interval = owner->animationSpeed;
		m_state = WAIT_SPAWN;
		currentSpawnCount++;
	}
		break;
	case WAIT_SPAWN:
	{
		if(currentSpawnCount >= spawnCount)
		{
			owner->row = DEACTIVATE_SPAWN_ROW;
			owner->frameCount = SPAWN_FRAME_COUNT;
			owner->animationSpeed = SPAWN_ANIMATION_SPEED * owner->frameCount;
			owner->currentFrame = 0;
			m_interval = owner->animationSpeed;
			m_state = FINISHED;
		}
		else
			m_state = SPAWN;
	}
		break;

	case FINISHED:
	{
		owner->row = 0;
		owner->frameCount = DEFAULT_SPAWN_FRAME_COUNT;
		owner->animationSpeed = DEFAULT_ANIMATION_SPEED;
		owner->currentFrame = 0;

		owner->setStopped(false);
		m_interval = Balance::Instance().getSpawnInterval();
		m_state = READY;
	}
		break;
	default:
		break;
	}
}

StrongAbility::StrongAbility()
	: EnemyAbility(Balance::Instance().getStrongInterval())
{

}

void StrongAbility::use()
{
	owner->protect(Balance::Instance().getStrongValue());
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

FasterAbility::FasterAbility()
	: EnemyAbility(FASTER_INTERVAL)
{

}

void FasterAbility::use()
{
	const float k = owner->getData().health / owner->getPureStats().health;
	owner->animationSpeed = FASTER_ANIMATION_SPEED + 4 * FASTER_ANIMATION_SPEED * k;
	owner->setFaster(k);
}
