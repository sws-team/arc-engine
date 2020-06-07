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
	,healing(false)
	,healingValue(0.f)
	,healCount(0)
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

	this->frameCount = frameCount;
	this->animationSpeed = animationSpeed;
	setPos(m_pos + m_spritePos);

	TextureType burnTexture;
	TextureType healTextureId;
	if (cellSize.x == 2)
	{
		healTextureId = GAME_TEXTURE::HEAL_MID_EFFECT;
		burnTexture = GAME_TEXTURE::BURN_MID;
	}
	else if(cellSize.x == 4)
	{
		healTextureId = GAME_TEXTURE::HEAL_BIG_EFFECT;
		burnTexture = GAME_TEXTURE::BURN_BIG;
	}
	else
	{
		healTextureId = GAME_TEXTURE::HEAL_SMALL_EFFECT;
		burnTexture = GAME_TEXTURE::BURN_SMALL;
	}

	burnAnimation = new GameObject(burnTexture, startPos,
								   sf::Vector2i(GameOptions::MAP_CELL_SIZE * cellSize.x,
												GameOptions::MAP_CELL_SIZE * cellSize.y), 9);
	burnAnimation->animationSpeed = 100;

	healAnimation = new GameObject(healTextureId, startPos, this->size, HEAL_FRAME_COUNT);
	healAnimation->animationSpeed = HEAL_ANIMATION_SPEED;
	healAnimation->sprite.scale(1/ENEMY_SCALE, 1/ENEMY_SCALE);
}

Enemy::~Enemy()
{
	delete healAnimation;
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
	if (ability != nullptr)
		ability->moved();
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
		burnAnimation->setPos(this->pos());
		if (burnAttack.check(Balance::Instance().getBurnAttackSpeed()))
			hit(Balance::Instance().getBurnDamage());
		burnAnimation->update();
	}
	if (healing)
	{
		if (healTimer.check(HEAL_FRAME_COUNT * HEAL_ANIMATION_SPEED))
		{
			upHealth(healingValue);
			healCount++;
		}
		healAnimation->setPos(this->pos());
		healAnimation->update();
		if (healCount == HEAL_ANIMATION_K)
			healing = false;
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
	if (healing)
		healAnimation->draw(target);
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
	healing = true;
	healingValue = health/(HEAL_FRAME_COUNT * HEAL_ANIMATION_K);
	healCount = 0;
	healTimer.reset();
}

void Enemy::protect(float shell)
{
	m_data.reflection += shell;
	if(m_data.reflection >= 0.9f)
		m_data.reflection = 0.9f;
	if (m_data.reflection < 0.f)
		m_data.reflection = 0.f;
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

void Enemy::upHealth(const float value)
{
	m_data.health += value;
	if (m_data.health > m_stats.health)
		m_data.health = m_stats.health;
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
	case WAR_VEHICLE:
	{
		texture_id = GAME_TEXTURE::ENEMY_WAR_VEHICLE;
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
	case SLUGGY:
	{
		texture_id = GAME_TEXTURE::ENEMY_SLUGGY;
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
	case MECHSPIDER:
	{
		texture_id = GAME_TEXTURE::ENEMY_MECHSPIDER;
		size.x = 2;
		size.y = 2;
		abilityType = EnemyInfo::SHUTDOWN_TOWER;
	}
		break;
	case TUATARA:
	{
		texture_id = GAME_TEXTURE::ENEMY_TUATARA;
		size.x = 2;
		size.y = 2;
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
	case WALKER:
	{
		texture_id = GAME_TEXTURE::ENEMY_WALKER;
		size.x = 2;
		size.y = 2;
		abilityType = EnemyInfo::TELEPORT;
	}
		break;
		//big
	case GIANT_SLUG:
	{
		texture_id = GAME_TEXTURE::ENEMY_GIANT_SLUG;
		size.x = 4;
		size.y = 4;
		abilityType = EnemyInfo::RAGE;
	}
		break;
	case HEAVY_TANK:
	{
		texture_id = GAME_TEXTURE::ENEMY_HEAVY_TANK;
		size.x = 4;
		size.y = 4;
		abilityType = EnemyInfo::KILL_TOWER;
	}
		break;
	case BUGSAURUS:
	{
		texture_id = GAME_TEXTURE::ENEMY_BUGSAURUS;
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
	case JUMPER:
	{
		texture_id = GAME_TEXTURE::ENEMY_JUMPER;
		size.x = 2;
		size.y = 2;
		abilityType = EnemyInfo::JUMP;
	}
		break;
	case ROLLER:
	{
		texture_id = GAME_TEXTURE::ENEMY_ROLLING;
		size.x = 1;
		size.y = 1;
		abilityType = EnemyInfo::ROLLING;
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
	case WAR_VEHICLE:
		return 50;
	case TRICYCLE:
		break;
	case WORM:
		return 200;
		break;
	case TRACTOR:
		return 100;
	case SLUGGY:
		return FasterAbility::FASTER_ANIMATION_SPEED;
	case TANK:
		return 200;
		break;
	case MECHSPIDER:
		return 100;
	case TUATARA:
		return 100;
	case REPAIR_ENEMY:
		break;
	case SHIELD_ENEMY:
		return 300;
	case WALKER:
		return TeleportAbility::TELEPORT_DEFAULT_ANIMATION_SPEED;
	case GIANT_SLUG:
		break;
	case HEAVY_TANK:
		break;
	case BUGSAURUS:
		return SpawnEnemy::DEFAULT_ANIMATION_SPEED;
	case SPAWN_WORM:
		return getAnimationSpeed(WORM);
	case JUMPER:
		return JumpAbility::JUMPER_ANIMATION_SPEED;
		break;
	case ROLLER:
		return RollingAbility::ROLLING_ANIMATION_SPEED;
		break;
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
		return 6;
		break;
	case WAR_VEHICLE:
		break;
	case TRICYCLE:
		break;
	case WORM:
		return 6;
	case SELFHEAL_ENEMY:
		break;
	case TRACTOR:
		return 5;
	case SLUGGY:
		break;
	case TANK:
		return 5;
		break;
	case MECHSPIDER:
		return 5;
	case TUATARA:
		break;
	case REPAIR_ENEMY:
		break;
	case SHIELD_ENEMY:
		return 9;
	case WALKER:
		return TeleportAbility::TELEPORT_FRAME_COUNT;
	case GIANT_SLUG:
		break;
	case HEAVY_TANK:
		break;
	case BUGSAURUS:
		return SpawnEnemy::DEFAULT_SPAWN_FRAME_COUNT;
	case SPAWN_WORM:
		return getFrameCount(WORM);
	case JUMPER:
		return JumpAbility::JUMPER_STAY_FRAME_COUNT;
	case ROLLER:
		return 2;
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
	case EnemyInfo::JUMP:
		ability = new JumpAbility();
		break;
	case EnemyInfo::ROLLING:
		ability = new RollingAbility();
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
	if (type == JUMPER)
		enemy->setStopped(true);

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

void EnemyAbility::moved()
{

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
	Engine::Instance().soundManager()->playOnce(GAME_SOUND::REPAIR);
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
	Engine::Instance().soundManager()->playOnce(GAME_SOUND::HEAL);
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
		Engine::Instance().soundManager()->playOnce(GAME_SOUND::TELEPORT);
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
		Engine::Instance().soundManager()->playOnce(GAME_SOUND::TELEPORT);
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

void TowerEffectAbility::targetRemoved(Tower *tower)
{
	if (tower == targetTower)
		targetTower = nullptr;
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

		Tower *target = findTarget(owner->enemyCenter(), info.cells);
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
		if (targetTower == nullptr || Collision::PixelPerfectTest(targetTower->getSprite(), projectile->getSprite()))
		{
			delete projectile;
			projectile = nullptr;
			Engine::Instance().soundManager()->playOnce(info.catchSound);
			getBack();
			if (targetTower != nullptr && !targetTower->isDowngraded() && targetTower->isActive())
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
#define RANDOM_TARGET
Tower *TowerEffectAbility::findTarget(const sf::Vector2f& center, const float cells)
{
	Tower *target = nullptr;
	const std::vector<Tower*> towers = Engine::Instance().options<GameOptions>()->level()->getAllTowers();
#ifdef RANDOM_TARGET
	std::vector<Tower*> possibleTargets;
#else
	float weight = 0;
#endif
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
		if (r > Engine::Instance().options<GameOptions>()->tileSize().x * cells)
			continue;

#ifdef RANDOM_TARGET
		possibleTargets.push_back(tower);
#else
		const float currentWeight = Engine::Instance().options<GameOptions>()->panel()->getTowerSellCost(tower);
		if (currentWeight > weight)
		{
			target = tower;
			weight = currentWeight;
		}
#endif
	}
#ifdef RANDOM_TARGET
	if (!possibleTargets.empty())
	{
		const int n = rand() % possibleTargets.size();
		return possibleTargets.at(n);
	}
#endif
	return target;
}

ShutdownTowerAbility::ShutdownTowerAbility()
	: TowerEffectAbility(Balance::Instance().getShutdownInterval())
{
	info.enemyTextureId = GAME_TEXTURE::ENEMY_MECHSPIDER;
	info.pojectileTextureId = GAME_TEXTURE::WEB;
	info.projectileSize = sf::Vector2i(GameOptions::MAP_CELL_SIZE, GameOptions::MAP_CELL_SIZE);
	info.duration = Balance::Instance().getShutdownDuration();
	info.catchSound = GAME_SOUND::CATCH;
	info.cells = Balance::Instance().getShutdownCells();
	info.projectileSpeed = 10;
	info.projectileAnimationSpeed = 150;
	info.profectileFrameCount = 4;

	web = new GameObject(GAME_TEXTURE::WEB, sf::Vector2f(0,0),
						 sf::Vector2i(GameOptions::CELL_SIZE,
									  GameOptions::CELL_SIZE), 4);
}

ShutdownTowerAbility::~ShutdownTowerAbility()
{
	if (targetTower != nullptr && !targetTower->isActive())
		effect(false);
}

void ShutdownTowerAbility::draw(sf::RenderTarget * const target)
{
	TowerEffectAbility::draw(target);
	if (m_state == WAIT)
		web->draw(target);
}

void ShutdownTowerAbility::update()
{
	TowerEffectAbility::update();
	if (m_state == WAIT)
	{
		web->update();
		if (web->currentFrame == web->frameCount && web->row == 0)
		{
			web->currentFrame = 0;
			web->row = 1;
		}
	}
}

void ShutdownTowerAbility::effect(bool isActive)
{
	if (targetTower == nullptr)
		return;
	if (targetTower->isActive() == !isActive)
		return;
	targetTower->setActive(!isActive);
	if (isActive)
	{
		web->currentFrame = 0;
		web->row = 0;
		web->updateTextureRect();
		web->setPos(targetTower->pos());
	}
	else
	{
		web->currentFrame = 2;
		web->row = 0;
		web->updateTextureRect();
	}
}

DownTowerAbility::DownTowerAbility()
	: TowerEffectAbility(Balance::Instance().getDowngradeInterval())
{
	info.enemyTextureId = GAME_TEXTURE::ENEMY_TRACTOR;
	info.pojectileTextureId = GAME_TEXTURE::DOWNGRADE_PROJECTILE;
	info.projectileSize = sf::Vector2i(32, 32);
	info.duration = Balance::Instance().getDowngradeDuration();
	info.catchSound = GAME_SOUND::ELECTRIC_SHOCK;
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

void DownTowerAbility::update()
{
	if (m_state == FINISHED &&
			targetTower != nullptr &&
			!targetTower->isDowngraded())
		use();
	TowerEffectAbility::update();
}

void DownTowerAbility::effect(bool isActive)
{
	if (targetTower != nullptr)
		targetTower->setDowngrade(isActive);
}

KillTowerAbility::KillTowerAbility()
	: TowerEffectAbility(Balance::Instance().getKillTowerInterval())
{
	info.enemyTextureId = GAME_TEXTURE::ENEMY_HEAVY_TANK;
	info.pojectileTextureId = GAME_TEXTURE::ENEMY_ROCKET;
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
	Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::EXPLOSION,
																	 targetTower->pos() - Engine::Instance().options<GameOptions>()->tileSize(),
																	 sf::Vector2i(1/Tower::TOWER_SCAlE * GameOptions::CELL_SIZE,
																				  1/Tower::TOWER_SCAlE * GameOptions::CELL_SIZE),
																	 80, 8, 0);
	Engine::Instance().options<GameOptions>()->level()->deleteTower(targetTower);
	targetTower = nullptr;
}

DowngradeTowerAbility::DowngradeTowerAbility()
	: TowerEffectAbility(Balance::Instance().getDowngradeTowerInterval())
{
	info.enemyTextureId = GAME_TEXTURE::ENEMY_TANK;
	info.pojectileTextureId = GAME_TEXTURE::ENEMY_BULLET;
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
		Engine::Instance().soundManager()->playOnce(GAME_SOUND::SPAWN_ROAR);
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
	,isShow(false)
{
	strongAnimation = new GameObject(GAME_TEXTURE::SHELL_EFFECT, sf::Vector2f(0,0),
									 sf::Vector2i(GameOptions::MAP_CELL_SIZE, GameOptions::MAP_CELL_SIZE),
									 STRONG_FRAMES_COUNT);
	strongAnimation->animationSpeed = STRONG_ANIMATION_SPEED;
}

StrongAbility::~StrongAbility()
{
	delete strongAnimation;
}

void StrongAbility::draw(sf::RenderTarget * const target)
{
	if (isShow)
		strongAnimation->draw(target);
}

void StrongAbility::update()
{
	EnemyAbility::update();
	if (strongAbilityTimer.check(STRONG_ANIMATION_SPEED * STRONG_FRAMES_COUNT))
	{
		isShow = false;
		return;
	}
	strongAnimation->update();
}

void StrongAbility::moved()
{
	if (!isShow)
		return;

	strongAnimation->setPos(owner->pos());
}

void StrongAbility::use()
{
	isShow = true;
	owner->protect(-Balance::Instance().getStrongValue());
	moved();
	strongAnimation->currentFrame = 0;
	strongAbilityTimer.reset();
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

JumpAbility::JumpAbility()
	: EnemyAbility(Balance::Instance().getJumpInterval())
	,isJump(false)
{

}

void JumpAbility::use()
{
	isJump = !isJump;
	if (isJump)
	{
		owner->setStopped(false);
		owner->row = 1;
		owner->currentFrame = 0;
		owner->frameCount = JUMPER_FRAME_COUNT;
		owner->updateTextureRect();
		m_interval = JUMPER_FRAME_COUNT * JUMPER_ANIMATION_SPEED - 50;
	}
	else
	{
		owner->setStopped(true);
		owner->row = 0;
		owner->currentFrame = 0;
		owner->frameCount = JUMPER_STAY_FRAME_COUNT;
		owner->updateTextureRect();
		m_interval = Balance::Instance().getJumpInterval();
	}
}

RollingAbility::RollingAbility()
	: EnemyAbility(Balance::Instance().getRollInterval())
	,m_state(MOVING)
{

}

void RollingAbility::use()
{
	switch (m_state)
	{
	case MOVING:
	{
		owner->row = 0;
		owner->currentFrame = 0;
		owner->updateTextureRect();
		owner->animationSpeed = ROLLING_ANIMATION_SPEED;

		m_interval = Balance::Instance().getRollInterval();
		m_state = MOVE_TO_ROLL;
	}
		break;
	case MOVE_TO_ROLL:
	{
		owner->row = 1;
		owner->currentFrame = 0;
		owner->updateTextureRect();

		m_interval = ROLLING_ANIMATION_SPEED * 2;
		m_state = ROLLING;
	}
		break;
	case ROLLING:
	{
		owner->row = 2;
		owner->currentFrame = 0;
		owner->updateTextureRect();
		owner->animationSpeed = ROLLING_ANIMATION_SPEED;
		owner->setFaster(ROLLING_VALUE);
		owner->protect(0.6f);

		m_interval = Balance::Instance().getRollInterval() * 1.5f;
		m_state = ROLL_TO_MOVE;
	}
		break;
	case ROLL_TO_MOVE:
	{
		owner->row = 3;
		owner->currentFrame = 0;
		owner->updateTextureRect();
		owner->animationSpeed = ROLLING_ANIMATION_SPEED;
		owner->setFaster(1.0);
		owner->protect(-0.5);

		m_interval = ROLLING_ANIMATION_SPEED * 2;
		m_state = MOVING;
	}
		break;
	default:
		break;
	}
}
