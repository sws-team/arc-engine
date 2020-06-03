#include "tower.h"
#include "enemy.h"
#include "projectile.h"
#include "engine.h"
#include "level.h"
#include "mapeffects.h"
#include "lifebar.h"
#include "gameoptions.h"
#include "managers.h"
#include "gamemanagers.h"
#include "collisions.h"
#include "gameplatform.h"
#include "achievements.h"

const float Tower::TOWER_SCAlE = 1.f/3.f;

Tower::Tower(const TextureType &texture_id,
			 const sf::Vector2f &pos,
			 const TowerStats &stats)
	: GameObject(texture_id, pos,
				 sf::Vector2i(GameOptions::CELL_SIZE/TOWER_SCAlE,
							  GameOptions::CELL_SIZE/TOWER_SCAlE), 1)
	,m_stats(stats)
	,m_level(1)
	,m_kills(0)
	,m_isActive(true)
	,m_downgraded(false)
	,m_blinded(false)
	,m_regressed(false)
	,m_invulnerable(false)
	,inactiveIntermediate(false)
{
	sprite.scale(TOWER_SCAlE, TOWER_SCAlE);
	const float lowEnergyScale = 0.5f;
	const sf::Vector2f lowEnergyOffset = sf::Vector2f(Engine::Instance().options<GameOptions>()->mapTileSize().x/2,
													  Engine::Instance().options<GameOptions>()->mapTileSize().y/2);
	lowEnergyObject = new GameObject(GAME_TEXTURE::DOWN_EFFECT, pos + lowEnergyOffset,
									 sf::Vector2i(GameOptions::CELL_SIZE,
												  GameOptions::CELL_SIZE), 4);
	lowEnergyObject->animationSpeed = 150;
	lowEnergyObject->sprite.scale(lowEnergyScale, lowEnergyScale);

	activeObject = new GameObject(GAME_TEXTURE::WEB_EFFECT, pos, size, ACTIVE_FRAME_COUNT);
	activeObject->animationSpeed = INACTIVE_ANIMATION_SPEED;
	activeObject->sprite.scale(TOWER_SCAlE, TOWER_SCAlE);
}

Tower::~Tower()
{

}

void Tower::update()
{
	GameObject::update();
	if (m_downgraded)
		lowEnergyObject->update();
	if (!m_isActive || inactiveIntermediate)
	{
		if (inactiveIntermediate &&
				inactiveTimer.check(ACTIVE_FRAME_COUNT * INACTIVE_ANIMATION_SPEED - 50))
		{
			inactiveIntermediate = false;
			activeObject->row = 1;
			activeObject->currentFrame = 0;
			activeObject->frameCount = INACTIVE_FRAME_COUNT;
			activeObject->updateTextureRect();
		}
		activeObject->update();
	}
}

void Tower::draw(sf::RenderTarget * const target)
{
	GameObject::draw(target);
	if (m_downgraded)
		lowEnergyObject->draw(target);
	if (!m_isActive || inactiveIntermediate)
		activeObject->draw(target);
}

void Tower::action()
{
	if (!isActive())
	{
		shoot(nullptr);
		return;
	}

	if (!actionTimer.check(actualAttackSpeed()))
		return;

	//finding target
	Enemy *target = findNearestEnemy(std::vector<Enemy*>());
	shoot(target);
}

void Tower::shoot(Enemy *target)
{
	Engine::Instance().soundManager()->playOnce(m_shotSound);
}

void Tower::collide(const std::vector<Enemy *> &enemies)
{

}

void Tower::upgrade()
{
	m_level++;

	currentFrame = 0;
	row++;

	const float k = 1 + Balance::Instance().getTowerUpgradeGain();
	m_stats.cost *= 1 + Balance::Instance().getTowerUpgradeGain() * Tower::UPGRADE_PRICE_MODIFIER;
	m_stats.damage *= k;
	m_stats.radius *= k;
	m_stats.attackSpeed *= 1 - Balance::Instance().getTowerUpgradeGain();

	Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::UPGRADE, this->pos(),
											 sf::Vector2i(GameOptions::CELL_SIZE, GameOptions::CELL_SIZE),
											 150, 5, 0);
	Engine::Instance().soundManager()->playOnce(GAME_SOUND::UPGRADE);
	updateTextureRect();
}

TOWER_TYPES Tower::type() const
{
	return m_type;
}

void Tower::setType(const TOWER_TYPES &type)
{
	m_type = type;
}

void Tower::increaseAttackSpeed(float value)
{
	m_stats.attackSpeed /= value;
}

void Tower::decreaseAttackSpeed(float value)
{
	m_stats.attackSpeed *= value;
}

void Tower::increaseDamage(float value)
{
	m_stats.damage *= value;
}

void Tower::decreaseDamage(float value)
{
	m_stats.damage /= value;
}

int Tower::level() const
{
	return m_level;
}

void Tower::checkKill(Enemy *enemy)
{
	if (!enemy->isAlive())
	{
		m_kills++;
		//FIXME
//		GamePlatform::Instance().incrementValue(STAT_TOWER_MAX_KILLS);
	}
}

Enemy *Tower::findNearestEnemy(const std::vector<Enemy *> &exclude)
{
	Enemy *target = nullptr;
	float min = INT_MAX;
	const sf::Vector2f aPos = getCenter();
	const std::vector <Enemy*> enemies = Engine::Instance().options<GameOptions>()->level()->getAllEnemies();
	for(Enemy *enemy : enemies)
	{
		if (!enemy->isVisible())
			continue;
		if (find(exclude.begin(), exclude.end(), enemy) != exclude.end())
			continue;
		if (TowersFactory::isIntersects(enemy->enemyRect(), aPos,
										actualRadius() * Engine::Instance().options<GameOptions>()->mapTileSize().x))
		{
			const float x = fabs(enemy->enemyCenter().x - this->getCenter().x);
			const float y = fabs(enemy->enemyCenter().y - this->getCenter().y);
			if (x + y < min)
			{
				min = x + y;
				target = enemy;
			}
		}
	}
	return target;
}

bool Tower::isInvulnerable() const
{
	return m_invulnerable;
}

void Tower::levelDown()
{
	if (m_level == 1)
		return;

	m_level--;

	currentFrame = 0;
	row--;

	const float k = 1 + Balance::Instance().getTowerUpgradeGain();
	m_stats.cost /= 1 + Balance::Instance().getTowerUpgradeGain() * Tower::UPGRADE_PRICE_MODIFIER;
	m_stats.damage /= k;
	m_stats.radius /= k;
	m_stats.attackSpeed /= 1 - Balance::Instance().getTowerUpgradeGain();

	Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::DOWNGRADE, this->pos(),
											 sf::Vector2i(GameOptions::CELL_SIZE, GameOptions::CELL_SIZE),
											 150, 5, 0);
}

void Tower::setInvulnerable(bool invulnerable)
{
	m_invulnerable = invulnerable;
}

bool Tower::isRegressed() const
{
	return m_regressed;
}

void Tower::setRegressed(bool regressed)
{
    m_regressed = regressed;
}

bool Tower::isBlinded() const
{
    return m_blinded;
}

void Tower::setBlinded(bool blinded)
{
	m_blinded = blinded;
}

void Tower::setActive(bool isActive)
{
	if (m_isActive == isActive)
		return;
	m_isActive = isActive;
	if (isActive)
		activeObject->row = 2;
	else
		activeObject->row = 0;

	activeObject->frameCount = ACTIVE_FRAME_COUNT;
	activeObject->currentFrame = 0;
	activeObject->updateTextureRect();

	inactiveIntermediate = true;
	inactiveTimer.reset();
}

bool Tower::isActive() const
{
	return m_isActive;
}

void Tower::setDowngrade(bool isDowngrade)
{
	if (m_downgraded == isDowngrade)
		return;

	m_downgraded = isDowngrade;
}

bool Tower::isDowngraded() const
{
	return m_downgraded;
}

float Tower::actualRadius() const
{
	if (m_blinded)
		return m_stats.radius * Balance::Instance().getBlindValue();
	return m_stats.radius;
}

float Tower::actualDamage(const ARMOR_TYPE armorType) const
{
	float damage = m_stats.damage + m_stats.armorDamage.at(armorType);
	if (m_downgraded)
		damage *= Balance::Instance().getDowngradeValue();
	if (m_regressed)
		damage *= Balance::Instance().getRegressValue();
	const int offsetMin = static_cast<int>(damage * m_stats.damageOffset);
	const int offsetMax = offsetMin * 2;
	const int offset = offsetMax == 0 ? 0 : rand() % offsetMax;
	const float realDamage = damage - offsetMin + offset;
	return realDamage;
}

float Tower::actualAttackSpeed() const
{
	float attackSpeed = m_stats.attackSpeed;
	if (m_downgraded)
		attackSpeed /= Balance::Instance().getDowngradeValue();
	if (m_regressed)
		attackSpeed /= Balance::Instance().getRegressValue();
	return attackSpeed;
}


int Tower::kills() const
{
    return m_kills;
}

TowerStats Tower::data() const
{
	return m_stats;
}

Tower *TowersFactory::createTower(TOWER_TYPES type, const sf::Vector2f &pos)
{
	Tower *tower = nullptr;
	switch (type)
	{
	case BASE:
		tower = new BaseTower(pos);
		break;
	case POWER:
		tower = new PowerTower(pos);
		break;
	case ROCKET:
		tower = new RocketTower(pos);
		break;
	case FREEZE:
		tower = new FreezeTower(pos);
		break;
	case LASER:
		tower = new LaserTower(pos);
		break;
	case IMPROVED:
		tower = new ImprovedTower(pos);
		break;
	}
	tower->setType(type);
	tower->frameCount = TowersFactory::getFrameCount(type);
	tower->animationSpeed = TowersFactory::getAnimationSpeed(type);
	return tower;
}

float TowersFactory::getAnimationSpeed(TOWER_TYPES type)
{
	switch (type)
	{
	case BASE:
		return 100;
	case POWER:
		return 150;
	case ROCKET:
		return 120;
	case FREEZE:
		return 100;
	case LASER:
		return 80;
	case IMPROVED:
		return 100;
	}
	return 200;
}

int TowersFactory::getFrameCount(TOWER_TYPES type)
{
	switch (type)
	{
	case BASE:
		return 6;
	case POWER:
		return 7;
	case ROCKET:
		return 6;
	case FREEZE:
		return 6;
	case LASER:
		return 6;
	case IMPROVED:
		return 6;
	}
	return 1;
}

bool TowersFactory::isIntersects(const sf::FloatRect &rect, const sf::Vector2f &center, float radius)
{
	float x = fabs(rect.left - center.x);
	float y = fabs(rect.top - center.y);
	float r = sqrtf(powf(x, 2) + powf(y, 2));

	if (r < radius)
		return true;

	x = fabs(rect.left + rect.width - center.x);
	y = fabs(rect.top - center.y);

	r = sqrtf(powf(x, 2) + powf(y, 2));
	if (r < radius)
		return true;

	x = fabs(rect.left + rect.width - center.x);
	y = fabs(rect.top + rect.height - center.y);

	r = sqrtf(powf(x, 2) + powf(y, 2));

	if (r < radius)
		return true;

	x = fabs(rect.left - center.x);
	y = fabs(rect.top + rect.height - center.y);

	r = sqrtf(powf(x, 2) + powf(y, 2));

	if (r < radius)
		return true;

	return false;
}

BaseTower::BaseTower(const sf::Vector2f &pos)
	: ProjectilesTower(GAME_TEXTURE::TOWER_BASE, pos, Balance::Instance().getTowerStats(BASE))
{
	projectileInfo.size = sf::Vector2i(16, 8);
	projectileInfo.frameCount = 1;
	projectileInfo.texture_id = GAME_TEXTURE::BASE_PROJECTILE;
	projectileInfo.explosion_texture_id = GAME_TEXTURE::BASE_EXPLOSION_EFFECT;
	projectileInfo.explosionSize = sf::Vector2i(64, 64);
	projectileInfo.explosionFrameCount = 8;
	m_shotSound = GAME_SOUND::BASE_SHOT;

	TowersCounter::Instance().baseTowerCount++;
}

BaseTower::~BaseTower()
{
	TowersCounter::Instance().baseTowerCount--;
}

void BaseTower::projectileAction(Enemy *enemy)
{
	ProjectilesTower::projectileAction(enemy);
	const float penetration = Balance::Instance().getbaseTowerPenetration() * powf(10, level());
	enemy->protect(-penetration);
}

void BaseTower::upgrade()
{
	ProjectilesTower::upgrade();
	if (level() == ABILITY_LEVEL)
		this->setInvulnerable(true);
}

void BaseTower::levelDown()
{
	ProjectilesTower::levelDown();
	this->setInvulnerable(false);
}

const sf::Color PowerTower::POWER_TOWER_AREA_COLOR = sf::Color(23, 200, 124, 64);
const sf::Vector2i PowerTower::BLAST_SIZE = sf::Vector2i(256, 256);

PowerTower::PowerTower(const sf::Vector2f &pos)
	: Tower(GAME_TEXTURE::TOWER_POWER, pos, Balance::Instance().getTowerStats(POWER))
	,gainCount(false)
	,abilityProgress(nullptr)
{
	zeroGround = Engine::Instance().options<GameOptions>()->mapTileSize().x * Balance::Instance().getBlastCells();
	powerRect.setSize(sf::Vector2f(m_stats.radius * Engine::Instance().options<GameOptions>()->mapTileSize().x,
							   m_stats.radius * Engine::Instance().options<GameOptions>()->mapTileSize().y));
	powerRect.setFillColor(POWER_TOWER_AREA_COLOR);
	powerRect.setPosition(pos - Engine::Instance().options<GameOptions>()->tileSize());
	TowersCounter::Instance().powerTowerCount++;
}

PowerTower::~PowerTower()
{
	if (abilityProgress != nullptr)
		delete abilityProgress;
	TowersCounter::Instance().powerTowerCount--;
}

void PowerTower::draw(sf::RenderTarget * const target)
{
	Tower::draw(target);
	if (abilityProgress != nullptr)
		abilityProgress->draw(target);
}

bool PowerTower::hasEnergy()
{
	return actionTimer.check(actualAttackSpeed());
}

void PowerTower::updateGain()
{
	gainCount++;
	if (gainCount == Balance::Instance().getBlastCount())
	{
		gainCount = 0;
		if (level() == ABILITY_LEVEL)
			activateBlast();
	}
	if (abilityProgress != nullptr)
	{
		const float rate = static_cast<float>(gainCount) / Balance::Instance().getBlastCount();
		abilityProgress->setValue(rate);
	}
}

void PowerTower::upgrade()
{
	Tower::upgrade();
	m_stats.attackSpeed = getAttackSpeed(level());
	m_stats.radius = getRadius(level());
	upgradePowerRect();
	if (level() == ABILITY_LEVEL)
	{
		abilityProgress = new LifeBar();
		abilityProgress->init(sf::Vector2i(this->sprite.getGlobalBounds().width,
							  LifeBar::LIFE_BAR_HEIGHT * Engine::Instance().settingsManager()->getScaleFactor().y),
							  sf::Color::Cyan);
		abilityProgress->setPos(sf::Vector2f(this->sprite.getGlobalBounds().left,
										 this->sprite.getGlobalBounds().top +
										 this->sprite.getGlobalBounds().height -
										 LifeBar::LIFE_BAR_HEIGHT * Engine::Instance().settingsManager()->getScaleFactor().y));
		abilityProgress->setValue(0);
	}
}

void PowerTower::levelDown()
{
	if (abilityProgress != nullptr)
	{
		delete abilityProgress;
		abilityProgress = nullptr;
	}
	Tower::levelDown();
	m_stats.radius = getRadius(level());
	m_stats.attackSpeed = getAttackSpeed(level());
	upgradePowerRect();
}

float PowerTower::getRadius(int level)
{
	switch (level)
	{
	case 2:
		return 10;
	case 3:
		return 14;
	case 4:
		return 18;
	default:
		return Balance::Instance().getTowerStats(POWER).radius;
		break;
	}
}

float PowerTower::getAttackSpeed(int level)
{
	const float tower_as = Balance::Instance().getTowerStats(POWER).attackSpeed;
	float as = tower_as;
	as *= Balance::Instance().getTowerUpgradeGain();
	as *= ENERGY_MODIFIER;
	const float k = (level - 1) * ENERGY_MODIFIER;
	as *= k;
	const float result = tower_as - as;
	return result;
}

sf::FloatRect PowerTower::getValidArea() const
{
	return powerRect.getGlobalBounds();
}

void PowerTower::upgradePowerRect()
{
	powerRect.setSize(sf::Vector2f(m_stats.radius * Engine::Instance().options<GameOptions>()->mapTileSize().x,
								   m_stats.radius * Engine::Instance().options<GameOptions>()->mapTileSize().y));

	powerRect.setPosition(this->pos() -
						  sf::Vector2f(Engine::Instance().options<GameOptions>()->tileSize().x * level(),
								   Engine::Instance().options<GameOptions>()->tileSize().y * level()));
}

void PowerTower::activateBlast()
{
	const sf::Vector2f pos = sf::Vector2f(BLAST_SIZE.x/2 * Engine::Instance().settingsManager()->getScaleFactor().x - Engine::Instance().options<GameOptions>()->mapTileSize().x,
								  BLAST_SIZE.y/2 * Engine::Instance().settingsManager()->getScaleFactor().y - Engine::Instance().options<GameOptions>()->mapTileSize().y);
	Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::BLAST, this->pos() - pos, BLAST_SIZE, 25, 4, 0);
	const sf::Vector2f center = this->getCenter();
	const std::vector <Enemy*> enemies = Engine::Instance().options<GameOptions>()->level()->getAllEnemies();
	for(Enemy* enemy : enemies)
	{
		const float a = fabs(center.x - enemy->enemyCenter().x);
		const float b = fabs(center.y - enemy->enemyCenter().y);
		const float r = powf(powf(a, 2) + powf(b, 2), 0.5f);
		if (r <= zeroGround)
		{
			float modifier = 0.1f;
			switch (enemy->size.x)
			{
			case 96:
				modifier *= 3;
				break;
			case 192:
				modifier *= 1.5f;
				break;
			default:
				break;
			}
			const float actualDamage = enemy->getPureStats().health * modifier;
			enemy->hit(actualDamage);
			checkKill(enemy);
		}
	}
}

RocketTower::RocketTower(const sf::Vector2f &pos)
	: ProjectilesTower(GAME_TEXTURE::TOWER_ROCKET, pos, Balance::Instance().getTowerStats(ROCKET))
{
	zeroGround = Engine::Instance().options<GameOptions>()->tileSize().x * Balance::Instance().getRocketTowerCells();
	m_shotSound = GAME_SOUND::ROCKET_SHOT;

	projectileInfo.size = sf::Vector2i(32, 16);
	projectileInfo.frameCount = 4;
	projectileInfo.texture_id = GAME_TEXTURE::ROCKET_PROJECTILE;
	projectileInfo.explosion_texture_id = GAME_TEXTURE::ROCKET_EXPLOSION_EFFECT;
	projectileInfo.explosionSize = sf::Vector2i(64, 64);
	projectileInfo.explosionFrameCount = 12;
	TowersCounter::Instance().rocketTowerCount++;
}

RocketTower::~RocketTower()
{
	TowersCounter::Instance().rocketTowerCount--;
}

void RocketTower::moveProjectile(Projectile *projectile)
{
	if (projectile->target != nullptr)
	{
		const sf::Vector2f aPos = projectile->getCenter();
		const float a = aPos.x - projectile->target->getCenter().x;
		const float b = aPos.y - projectile->target->getCenter().y;
		const float tg = ( b / a );
		float angle = atanf(tg) * 180 / M_PI;
		if (a < 0)
			angle += 180;
		else if (b < 0)
			angle += 360;

		angle -= 180;

		projectile->setAngle(angle);
	}
	ProjectilesTower::moveProjectile(projectile);
}

void RocketTower::projectileAction(Enemy *enemy)
{
	if(level() == ABILITY_LEVEL && !isDowngraded())
		enemy->startBurn();
	const sf::Vector2f epicenter = enemy->enemyCenter();
	const std::vector <Enemy*> enemies = Engine::Instance().options<GameOptions>()->level()->getAllEnemies();
	for(Enemy* levelEnemy : enemies)
	{
		const float a = fabs(epicenter.x - levelEnemy->enemyCenter().x);
		const float b = fabs(epicenter.y - levelEnemy->enemyCenter().y);
		const float r = powf(powf(a, 2) + powf(b, 2), 0.5f);
		if (r <= zeroGround)
		{
			const float actualDamage = (1 - r/zeroGround) * m_stats.damage;
			levelEnemy->hit(actualDamage);
			checkKill(levelEnemy);
			if(level() == ABILITY_LEVEL)
				levelEnemy->startBurn();
		}
	}
	ProjectilesTower::projectileAction(enemy);
}

FreezeTower::FreezeTower(const sf::Vector2f &pos)
	: ProjectilesTower(GAME_TEXTURE::TOWER_FREEZE, pos, Balance::Instance().getTowerStats(FREEZE))
{
	m_shotSound = GAME_SOUND::FREEZE_SHOT;
	projectileInfo.size = sf::Vector2i(20, 10);
	projectileInfo.frameCount = 3;
	projectileInfo.texture_id = GAME_TEXTURE::FREEZE_PROJECTILE;
	projectileInfo.explosion_texture_id = GAME_TEXTURE::FREEZE_EXPLOSION_EFFECT;
	projectileInfo.explosionSize = sf::Vector2i(64, 64);
	projectileInfo.explosionFrameCount = 8;
	TowersCounter::Instance().freezeTowerCount++;
	zeroGround = Engine::Instance().options<GameOptions>()->tileSize().x * Balance::Instance().getFreezeTowerCells();
}

FreezeTower::~FreezeTower()
{
	TowersCounter::Instance().freezeTowerCount--;
}

void FreezeTower::projectileAction(Enemy *enemy)
{
	ProjectilesTower::projectileAction(enemy);
	const float freezeValue = Balance::Instance().getFreezeValue();
	const float freezeDuration = Balance::Instance().getFreezeTowerDuration() + level() * EngineDefs::MSEC;
	enemy->freeze(freezeValue, freezeDuration);
	if (level() == ABILITY_LEVEL && !isDowngraded())
	{
		const sf::Vector2f epicenter = enemy->enemyCenter();
		const std::vector <Enemy*> enemies = Engine::Instance().options<GameOptions>()->level()->getAllEnemies();
		for(Enemy* levelEnemy : enemies)
		{
			const float a = fabs(epicenter.x - levelEnemy->enemyCenter().x);
			const float b = fabs(epicenter.y - levelEnemy->enemyCenter().y);
			const float r = powf(powf(a, 2) + powf(b, 2), 0.5f);
			if (r <= zeroGround)
				levelEnemy->freeze(freezeValue, freezeDuration);
		}
	}
}

const sf::Vector2i LaserTower::LASER_SIZE = sf::Vector2i(16, 16);

LaserTower::LaserTower(const sf::Vector2f &pos)
	: Tower(GAME_TEXTURE::TOWER_LASER, pos, Balance::Instance().getTowerStats(LASER))
{
	lineTower.position = this->pos() + sf::Vector2f(97 * Engine::Instance().settingsManager()->getScaleFactor().x * TOWER_SCAlE,
												24 * Engine::Instance().settingsManager()->getScaleFactor().y * TOWER_SCAlE);
	lineTower.color = sf::Color::Cyan;
	mainTarget.lineTarget.color = sf::Color::Cyan;
	mainTarget.currentTarget = nullptr;
	mainTarget.laser = new GameObject(GAME_TEXTURE::LASER_PROJECTILE, sf::Vector2f(0,0), LASER_SIZE, 3);
//	mainTarget.laser.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::LASER_PROJECTILE));
	m_shotSound = GAME_SOUND::LASER_SHOT;
	TowersCounter::Instance().laserTowerCount++;
}

LaserTower::~LaserTower()
{
	clearLasers();
	TowersCounter::Instance().laserTowerCount--;
}

void LaserTower::shoot(Enemy *target)
{
	if (level() == ABILITY_LEVEL && target != nullptr)
	{
		clearLasers();

		std::vector<Enemy*> exclude;
		exclude.push_back(target);
		while(true)
		{
			Enemy *enemy = findNearestEnemy(exclude);
			if (enemy == nullptr)
				break;
			if (targets.size() == Balance::Instance().getLaserTowerMaxExtraTargets())
				break;

			exclude.push_back(enemy);

			LaserTarget *laserTarget = new LaserTarget;
			laserTarget->lineTarget.color = sf::Color::Cyan;
			laserTarget->currentTarget = enemy;
			laserTarget->laser = new GameObject(GAME_TEXTURE::LASER_PROJECTILE, sf::Vector2f(0,0), LASER_SIZE, 3);
			targets.push_back(laserTarget);
		}
	}

	mainTarget.currentTarget = target;
	if (target != nullptr)
		Tower::shoot(target);
}

LaserTower::LaserTarget::~LaserTarget()
{
	delete laser;
}

void LaserTower::updateLaserTarget(LaserTower::LaserTarget *laserTarget, float damageModifier)
{
	if (laserTarget->currentTarget == nullptr)
		return;

	laserTarget->lineTarget = laserTarget->currentTarget->getCenter();
	laserTarget->laser->setPos(laserTarget->lineTarget.position -
								  sf::Vector2f(LASER_SIZE.x/2 * Engine::Instance().settingsManager()->getScaleFactor().x,
										   LASER_SIZE.y/2 * Engine::Instance().settingsManager()->getScaleFactor().y));

	if(laserTarget->damageTimer.check(actualAttackSpeed()))
	{
		laserTarget->currentTarget->hit(actualDamage(laserTarget->currentTarget->getArmorType()) * damageModifier);
		checkKill(laserTarget->currentTarget);
		if (!laserTarget->currentTarget->isAlive())
			laserTarget->currentTarget = nullptr;
	}
	const int alpha = rand() % 255;
	laserTarget->lineTarget.color.a = alpha;
	laserTarget->laser->update();
}

void LaserTower::drawLaserTarget(sf::RenderTarget * const target,
								 LaserTower::LaserTarget *laserTarget)
{
	if (laserTarget->currentTarget == nullptr)
		return;
	sf::Vertex line[] = { lineTower, laserTarget->lineTarget };
	target->draw(line, 2, sf::Lines);
	laserTarget->laser->draw(target);
}

void LaserTower::clearLasers()
{
	for(LaserTarget* laserTarget : targets)
		delete laserTarget;
	targets.clear();
}

void LaserTower::update()
{
	Tower::update();

	if (!isActive())
	{
		if (mainTarget.currentTarget != nullptr)
			mainTarget.currentTarget = nullptr;
		if (!targets.empty())
			clearLasers();
		return;
	}

	updateLaserTarget(&mainTarget, 1.f);
	for(LaserTarget* laserTarget : targets)
		updateLaserTarget(laserTarget, 0.5);

	for (auto it = targets.begin(); it != targets.end();)
	{
		LaserTarget* laserTarget = *it;
		if (laserTarget->currentTarget == nullptr)
		{
			delete laserTarget;
			it = targets.erase(it);
		}
		else
			++it;
	}
}

void LaserTower::draw(sf::RenderTarget * const target)
{
	Tower::draw(target);

	drawLaserTarget(target, &mainTarget);

	sf::Vertex lastVertex = mainTarget.lineTarget;
	for(LaserTarget* laserTarget : targets)
	{
		drawLaserTarget(target, laserTarget);
		lastVertex = laserTarget->lineTarget;
	}
}

ImprovedTower::ImprovedTower(const sf::Vector2f &pos)
	: ProjectilesTower(GAME_TEXTURE::TOWER_IMPROVED, pos, Balance::Instance().getTowerStats(IMPROVED))
{
	m_shotSound = GAME_SOUND::IMPROVED_SHOT;

	projectileInfo.size = sf::Vector2i(24, 12);
	projectileInfo.frameCount = 1;
	projectileInfo.texture_id = GAME_TEXTURE::IMPROVED_PROJECTILE;
	projectileInfo.explosion_texture_id = GAME_TEXTURE::IMPROVED_EXPLOSION_EFFECT;
	projectileInfo.explosionSize = sf::Vector2i(96, 96);
	projectileInfo.explosionFrameCount = 8;
	TowersCounter::Instance().improvedTowerCount++;
}

ImprovedTower::~ImprovedTower()
{
	TowersCounter::Instance().improvedTowerCount--;
}

void ImprovedTower::createdProjectile(Projectile *projectile)
{
	if (level() == ABILITY_LEVEL && !isDowngraded())
	{
		float angle = projectile->angle();
		for (int i = 0; i < 7; ++i)
		{
			angle += 45;
			Projectile *extraProjectile = new Projectile(projectileInfo.texture_id,
													projectile->pos(),
													projectileInfo.size,
													projectileInfo.frameCount);
			extraProjectile->setAngle(angle);
			extraProjectile->target = projectile->target;
			m_projectiles.push_back(extraProjectile);
		}
	}
}

ProjectilesTower::ProjectilesTower(const TextureType &texture_id,
								   const sf::Vector2f &pos,
								   const TowerStats &stats)
	: Tower(texture_id, pos, stats)
{

}

void ProjectilesTower::removeProjectile(Projectile *projectile)
{
	m_projectiles.erase( remove( m_projectiles.begin(), m_projectiles.end(), projectile ), m_projectiles.end() );
	delete projectile;
	projectile = nullptr;
}

void ProjectilesTower::checkEnemy(Enemy *enemy)
{
	for(Projectile *projectile : m_projectiles)
	{
		if (projectile->target == enemy)
			projectile->target = nullptr;
	}
}

void ProjectilesTower::moveProjectile(Projectile *projectile)
{
	const float x1 = pos().x;
	const float y1 = pos().y;

	const float y2 = y1 + m_stats.projectileSpeed * sinf(projectile->angle() * M_PI/180);
	const float x2 = x1 + m_stats.projectileSpeed * cosf(projectile->angle() * M_PI/180);

	const sf::Vector2f offset = sf::Vector2f(x2-x1, y2-y1);
	projectile->move(offset);
}

sf::Vector2f ProjectilesTower::shootPos() const
{
	return getCenter();
}

void ProjectilesTower::createdProjectile(Projectile *projectile)
{

}

void ProjectilesTower::draw(sf::RenderTarget * const target)
{
	Tower::draw(target);
	for(Projectile *projectile : m_projectiles)
		projectile->draw(target);
}

void ProjectilesTower::update()
{
	Tower::update();
	if (moveTimer.check(Projectile::PROJECTILE_GAME_SPEED))
	{
		for(Projectile *projectile : m_projectiles)
			moveProjectile(projectile);
	}

	for(Projectile *projectile : m_projectiles)
		projectile->update();
}

void ProjectilesTower::shoot(Enemy *target)
{
	if (target == nullptr)
		return;

	const sf::Vector2f aPos = shootPos();
	const float a = aPos.x - target->enemyCenter().x;
	const float b = aPos.y - target->enemyCenter().y;
	const float tg = ( b / a );
	float angle = atanf(tg) * 180 / M_PI;
	if (a < 0)
		angle += 180;
	else if (b < 0)
		angle += 360;

	angle -= 180;

	Projectile *projectile = new Projectile(projectileInfo.texture_id,
											aPos,
											projectileInfo.size,
											projectileInfo.frameCount);
	projectile->setAngle(angle);
	projectile->target = target;
	createdProjectile(projectile);
	m_projectiles.push_back(projectile);
	Tower::shoot(target);
}

void ProjectilesTower::collide(const std::vector<Enemy *> &enemies)
{
	for(Enemy* enemy : enemies)
	{
		for(Projectile *projectile : m_projectiles)
		{
			if (Collision::PixelPerfectTest(enemy->getSprite(), projectile->getSprite()))
			{
				projectileAction(enemy);
				removeProjectile(projectile);
			}
		}
	}
}

std::vector<Projectile *> ProjectilesTower::projectiles() const
{
	return m_projectiles;
}

void ProjectilesTower::projectileAction(Enemy *enemy)
{
	const sf::Vector2f scaleFactor = Engine::Instance().settingsManager()->getScaleFactor();
	enemy->hit(actualDamage(enemy->getArmorType()));
	checkKill(enemy);
	const sf::Vector2f size = enemy->getEnemySize();

	int maxX = size.x/2;
	if (size.x > projectileInfo.explosionSize.x)
		maxX = static_cast<int>(size.x/2 - projectileInfo.explosionSize.x/2);

	int maxY = size.y/2;
	if (size.y > projectileInfo.explosionSize.y)
		maxY = static_cast<int>(size.y/2 - projectileInfo.explosionSize.y/2);

	const sf::Vector2f offset = sf::Vector2f((rand() % maxX * 2 - maxX) * scaleFactor.x,
											 (rand() % maxY * 2 - maxY)) * scaleFactor.y;

	const sf::Vector2f centerOffset = sf::Vector2f(projectileInfo.explosionSize.x/2 * scaleFactor.x,
												   projectileInfo.explosionSize.y/2 * scaleFactor.y);

	Engine::Instance().options<GameOptions>()->level()->addAnimation(projectileInfo.explosion_texture_id,
											 enemy->enemyCenter() - centerOffset + offset,
											 projectileInfo.explosionSize,
											 50, projectileInfo.explosionFrameCount, 0);
}


TowersCounter &TowersCounter::Instance()
{
	static TowersCounter towersCounter;
	return towersCounter;
}

void TowersCounter::reset()
{
	baseTowerCount = 0;
	laserTowerCount = 0;
	powerTowerCount = 0;
	freezeTowerCount = 0;
	rocketTowerCount = 0;
	improvedTowerCount = 0;
}

bool TowersCounter::canBuildBaseTower() const
{
	return baseTowerCount < Balance::Instance().getTowerLimit(BASE);
}

bool TowersCounter::canBuildFreezeTower() const
{
	return freezeTowerCount < Balance::Instance().getTowerLimit(FREEZE);
}

bool TowersCounter::canBuildRocketTower() const
{
	return rocketTowerCount < Balance::Instance().getTowerLimit(ROCKET);
}

bool TowersCounter::canBuildPowerTower() const
{
	return powerTowerCount < Balance::Instance().getTowerLimit(POWER);
}

bool TowersCounter::canBuildLaserTower() const
{
	return laserTowerCount < Balance::Instance().getTowerLimit(LASER);
}

bool TowersCounter::canBuildImprovedTower() const
{
	return improvedTowerCount < Balance::Instance().getTowerLimit(IMPROVED);
}
