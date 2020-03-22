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
	if (!moveTimer.check(m_data.speed))
		return;

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

bool Enemy::getLastUp() const
{
	return lastUp;
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

	float animationSpeed = 200;
	float frameCount = 4;
	sf::Vector2i size;
	size.x = 1;
	size.y = 1;
	TextureType texture_id = 0;
	switch (type)
	{
	//small
	case INFANTRY:
		texture_id = GAME_TEXTURE::ENEMY_INFANTRY;
		size.x = 1;
		size.y = 1;
		break;
	case CAR:
		texture_id = GAME_TEXTURE::ENEMY_CAR;
		size.x = 1;
		size.y = 1;
		animationSpeed = 50;
		abilityType = EnemyInfo::STRONG;
		break;
	case TRICYCLE:
		texture_id = GAME_TEXTURE::ENEMY_TRICYCLE;
		size.x = 1;
		size.y = 1;
		break;
	case SMALL_NEXT:
		texture_id = GAME_TEXTURE::ENEMY_ANT;
		size.x = 1;
		size.y = 1;
		break;
	case SELFHEAL_ENEMY:
		texture_id = GAME_TEXTURE::ENEMY_SELF_HEAL;
		size.x = 1;
		size.y = 1;
		abilityType = EnemyInfo::SELF_HEAL;
		break;
	case TRACTOR:
		texture_id = GAME_TEXTURE::ENEMY_TRACTOR;
		abilityType = EnemyInfo::DOWN_TOWER;
		animationSpeed = 100;
		frameCount = 5;
		size.x = 1;
		size.y = 1;
		break;
	case ANOTHER_ENEMY:
		texture_id = GAME_TEXTURE::ENEMY_PLANE;
		size.x = 1;
		size.y = 1;
		break;
		//mid
	case TANK:
		texture_id = GAME_TEXTURE::ENEMY_TANK;
		size.x = 2;
		size.y = 2;
		abilityType = EnemyInfo::STRONG;
		break;
	case SPIDER:
		texture_id = GAME_TEXTURE::ENEMY_SPIDER;
		size.x = 2;
		size.y = 2;
		animationSpeed = 100;
		frameCount = 5;
		abilityType = EnemyInfo::SHUTDOWN_TOWER;
		break;
	case MID_FAST:
		texture_id = GAME_TEXTURE::ENEMY_HELICOPTER;
		size.x = 2;
		size.y = 2;
		break;
	case REPAIR_ENEMY:
		texture_id = GAME_TEXTURE::ENEMY_REPAIR;
		abilityType = EnemyInfo::HEAL_NEAR;
		size.x = 2;
		size.y = 2;
		break;
	case SHIELD_ENEMY:
		texture_id = GAME_TEXTURE::ENEMY_SHIELD;
		size.x = 2;
		size.y = 2;
		animationSpeed = 300;
		frameCount = 9;
		abilityType = EnemyInfo::SHELL_NEAR;
		break;
	case TELEPORT_ENEMY:
		texture_id = GAME_TEXTURE::ENEMY_TELEPORT;
		size.x = 2;
		size.y = 2;
		abilityType = EnemyInfo::TELEPORT;
		break;
		//big
	case BIG_SLOW:
		texture_id = GAME_TEXTURE::ENEMY_AIRCARRIER;
		size.x = 4;
		size.y = 4;
		break;
	case BIG_TANK:
		texture_id = GAME_TEXTURE::ENEMY_BIG_TANK;
		size.x = 4;
		size.y = 4;
		abilityType = EnemyInfo::RAGE;
		break;
	case SPAWN_ENEMY:
		texture_id = GAME_TEXTURE::ENEMY_COW;
		size.x = 4;
		size.y = 4;
		abilityType = EnemyInfo::SPAWN;
		break;
	default:
		break;
	}
	EnemyInfo info;
	info.texture_id = texture_id;
	info.size = size;
	info.stats = stats;
	info.abilityType = abilityType;
	info.animationSpeed = animationSpeed;
	info.frameCount = frameCount;
	return info;
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
	: AreaAbility(HEAL_INTERVAL)
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
			enemy->heal(enemy->getPureStats().health * 0.1f);
	}
}

ShellNearAbility::ShellNearAbility()
	: AreaAbility(SHELL_INTERVAL)
{

}

void ShellNearAbility::use()
{
	const std::vector<Enemy*> enemies = Engine::Instance().options<GameOptions>()->level()->getAllEnemies();
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
		Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::ENEMY_TELEPORT,
												 owner->pos(),
												 sf::Vector2i(GameOptions::CELL_SIZE,
														  GameOptions::CELL_SIZE),
												 200, 4, TELEPORT_DISAPPEAR_ROW);
		m_state = DISAPPEAR;
		m_interval = 250;
	}
		break;
	case DISAPPEAR:
	{
		for (int i = 0; i < 6; ++i)
		{
			if (Engine::Instance().options<GameOptions>()->level()->getEndRect().contains(owner->enemyPos()))
				continue;
			const sf::Vector2i cell = Engine::Instance().options<GameOptions>()->camera()->posToCellMap(owner->enemyPos());
			const int direction = Engine::Instance().options<GameOptions>()->level()->getTileDirectionByCell(cell);
			owner->moveNext(direction);
		}
		m_state = APPEAR;
		m_interval = 500;
	}
		break;
	case APPEAR:
	{
		Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::ENEMY_TELEPORT,
												 owner->pos(),
												 sf::Vector2i(GameOptions::CELL_SIZE,
														  GameOptions::CELL_SIZE),
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
		m_interval = owner->animationSpeed * owner->frameCount;
		m_state = SHOOT;
	}
		break;
	case SHOOT:
	{
		owner->row = 3;
		owner->frameCount = 1;

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
			if (r > Engine::Instance().options<GameOptions>()->tileSize().x * TOWER_EFFECT_CELLS)
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
{
	info.enemyTextureId = GAME_TEXTURE::ENEMY_SPIDER;
	info.animationSize = sf::Vector2i(GameOptions::CELL_SIZE * Enemy::ENEMY_SCALE,
								  GameOptions::CELL_SIZE * Enemy::ENEMY_SCALE);
	info.pojectileTextureId = GAME_TEXTURE::WEB;
	info.projectileSize = sf::Vector2i(GameOptions::CELL_SIZE, GameOptions::CELL_SIZE);
	info.duration = TOWER_DISABLED_DURATION;
	info.catchSound = GAME_SOUND::CATCH;
}

ShutdownTowerAbility::~ShutdownTowerAbility()
{
	if (targetTower != nullptr && !targetTower->isActive())
		targetTower->setActive(true);
}

void ShutdownTowerAbility::effect(bool isActive)
{
	if (targetTower != nullptr)
		targetTower->setActive(!isActive);
}

DownTowerAbility::DownTowerAbility()
{
	info.enemyTextureId = GAME_TEXTURE::ENEMY_TRACTOR;
	info.animationSize = sf::Vector2i(GameOptions::MAP_CELL_SIZE * Enemy::ENEMY_SCALE,
								  GameOptions::MAP_CELL_SIZE * Enemy::ENEMY_SCALE);
	info.pojectileTextureId = GAME_TEXTURE::DOWNGRADE_PROJECTILE;
	info.projectileSize = sf::Vector2i(200, 16);
	info.duration = TOWER_DOWNGRADED_DURATION;
	info.catchSound = GAME_SOUND::CATCH;
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
		Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::ENEMY_COW,
												 owner->pos(),
												 sf::Vector2i(GameOptions::CELL_SIZE * 2,
														  GameOptions::CELL_SIZE * 2),
												 200, 4, ACTIVATE_SPAWN_ROW);
		m_interval = 800;
		m_state = SPAWN;
		currentSpawnCount = 0;
		spawnCount = SPAWN_COUNT + rand() % SPAWN_COUNT_OFFSET;
	}
		break;
	case SPAWN:
	{
		const sf::Vector2i cell = Engine::Instance().options<GameOptions>()->camera()->posToCellMap(owner->enemyPos());
		const int direction = Engine::Instance().options<GameOptions>()->level()->getTileDirectionByCell(cell);

		Engine::Instance().options<GameOptions>()->level()->spawn(owner->pos(), SMALL_NEXT, 0.1f, direction);

		m_interval = SPAWN_INTERVAL;
		m_state = WAIT_SPAWN;
		currentSpawnCount++;
	}
		break;
	case WAIT_SPAWN:
	{
		if(currentSpawnCount >= spawnCount)
		{
			Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::ENEMY_COW,
													 owner->pos(),
													 sf::Vector2i(GameOptions::CELL_SIZE * 2,
															  GameOptions::CELL_SIZE * 2),
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
