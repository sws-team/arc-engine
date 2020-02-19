#include "tower.h"
#include "globalvariables.h"
#include "enemy.h"
#include "projectile.h"
#include "Game/Collisions/collisions.h"
#include "ResourcesManager/resourcesmanager.h"
#include "Engine/engine.h"
#include "Game/Level/level.h"
#include "Game/Audio/soundcontroller.h"
#include "mapeffects.h"
#include "settings.h"
#include "lifebar.h"

const float Tower::LEVEL_GAIN = 0.25f;
const float Tower::TOWER_SCAlE = 1.f/3.f;

Tower::Tower(const RESOURCES::TEXTURE_TYPE &texture_id, const Vector2f &pos, const TowerStats &stats)
	: GameObject(texture_id, pos,
				 Vector2i(GlobalVariables::CELL_SIZE/TOWER_SCAlE,
						  GlobalVariables::CELL_SIZE/TOWER_SCAlE),
				 1)
	,m_stats(stats)
	,m_level(1)
	,m_kills(0)
	,m_isActive(true)
	,m_downgraded(false)
	,m_blinded(false)
	,m_regressed(false)
	,m_invulnerable(false)
{
	sprite.scale(TOWER_SCAlE, TOWER_SCAlE);
}

void Tower::update()
{
	GameObject::update();
}

void Tower::draw(RenderTarget * const target)
{
	GameObject::draw(target);
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
	Enemy *target = findNearestEnemy(vector<Enemy*>());
	shoot(target);
}

void Tower::shoot(Enemy *target)
{
	SoundController::Instance().playOnce(m_shotSound);
}

void Tower::collide(const vector<Enemy *> &enemies)
{

}

void Tower::upgrade()
{
	m_level++;

	m_stats.cost *= 1 + Tower::LEVEL_GAIN;
	m_stats.damage *= 1 + Tower::LEVEL_GAIN;
	m_stats.radius *= 1 + Tower::LEVEL_GAIN;
	m_stats.attackSpeed *= 1 - Tower::LEVEL_GAIN;

	Engine::Instance().level()->addAnimation(RESOURCES::UPGRADE, this->pos(),
											 Vector2i(GlobalVariables::CELL_SIZE, GlobalVariables::CELL_SIZE),
											 250, 4, 0);
}

void Tower::hitEnemy(Enemy *enemy)
{

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
		m_kills++;
}

Enemy *Tower::findNearestEnemy(const vector<Enemy *> &exclude)
{
	Enemy *target = nullptr;
	float min = INT_MAX;
	const Vector2f aPos = getCenter();
	const vector <Enemy*> enemies = Engine::Instance().level()->getAllEnemies();
	for(Enemy *enemy : enemies)
	{
		if (find(exclude.begin(), exclude.end(), enemy) != exclude.end())
			continue;
		if (TowersFactory::isIntersects(enemy->enemyRect(), aPos,
										actualRadius() * GlobalVariables::Instance().mapTileSize().x))
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

bool Tower::isActive() const
{
	return m_isActive;
}

void Tower::setDowngrade(bool isDowngrade)
{
	m_downgraded = isDowngrade;
	if (isDowngrade)
		Engine::Instance().level()->addAnimation(RESOURCES::DOWN_EFFECT, this->pos(),
											 Vector2i(GlobalVariables::CELL_SIZE,
													  GlobalVariables::CELL_SIZE),
											 DownTowerAbility::TOWER_DOWNGRADED_DURATION/4, 4, 0);
}

bool Tower::isDowngraded() const
{
	return m_downgraded;
}

float Tower::actualRadius() const
{
	if (m_blinded)
		return m_stats.radius/3;
	return m_stats.radius;
}

float Tower::actualDamage() const
{
	float damage = m_downgraded ? m_stats.damage * DownTowerAbility::DOWNGRADE_VALUE : m_stats.damage;
	damage = m_regressed ? damage * TowersRegress::REGRESS_VALUE : damage;
	return damage;
}

float Tower::actualAttackSpeed() const
{
	const float attackSpeed = m_regressed ? m_stats.attackSpeed / TowersRegress::REGRESS_VALUE : m_stats.attackSpeed;
	return attackSpeed;
}

void Tower::setActive(bool isActive)
{
	m_isActive = isActive;
	if (!isActive)
		Engine::Instance().level()->addAnimation(RESOURCES::WEB, this->pos(),
											 Vector2i(GlobalVariables::CELL_SIZE,
													  GlobalVariables::CELL_SIZE),
											 ShutdownTowerAbility::TOWER_DISABLED_DURATION/4, 4, 0);
}

int Tower::kills() const
{
    return m_kills;
}

TowerStats Tower::data() const
{
	return m_stats;
}

Tower *TowersFactory::createTower(TOWER_TYPES type, const Vector2f &pos)
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
	return tower;
}

TowerStats TowersFactory::getTowerStats(TOWER_TYPES type)
{
	switch (type)
	{
	case BASE:
		return BaseTower::STATS;
	case POWER:
		return PowerTower::STATS;
	case ROCKET:
		return RocketTower::STATS;
	case FREEZE:
		return FreezeTower::STATS;
	case LASER:
		return LaserTower::STATS;
	case IMPROVED:
		return ImprovedTower::STATS;
	}
	return BaseTower::STATS;
}

bool TowersFactory::isIntersects(const FloatRect &rect, const Vector2f &center, float radius)
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
const float PowerTower::ENERGY_GAIN = 10;
const int PowerTower::COST_OFFSET = 10;
											//		dmg	atk_speed	r	pr_speed	cost
const TowerStats PowerTower::STATS = TowerStats		(0,		5000,	6,		0,		60);
const TowerStats BaseTower::STATS = TowerStats		(8,		450,	4,		20,		50);
const TowerStats FreezeTower::STATS = TowerStats	(6,		350,	5,		10,		100);
const TowerStats RocketTower::STATS = TowerStats	(40,	5000,	12,		5,		150);
const TowerStats LaserTower::STATS = TowerStats		(5,		135,	6,		0,		200);
const TowerStats ImprovedTower::STATS = TowerStats	(11,	250,	7,		40,		250);

BaseTower::BaseTower(const Vector2f &pos)
	: ProjectilesTower(RESOURCES::TOWER_BASE, pos, STATS)
{
	projectileInfo.size = Vector2i(16, 8);
	projectileInfo.frameCount = 1;
	projectileInfo.texture_id = RESOURCES::BASE_PROJECTILE;
	projectileInfo.explosion_texture_id = RESOURCES::BASE_EXPLOSION_EFFECT;
	projectileInfo.explosionSize = Vector2i(12, 12);
	projectileInfo.explosionFrameCount = 16;
	m_shotSound = "sounds/towers/base_shot.ogg";
}

void BaseTower::projectileAction(Enemy *enemy)
{
	ProjectilesTower::projectileAction(enemy);
	const float penetration = 0.00001f * powf(10, level());
	enemy->protect(penetration, false);
}

void BaseTower::upgrade()
{
	ProjectilesTower::upgrade();
	if (level() == ABILITY_LEVEL)
		this->setInvulnerable(true);
}

const Color PowerTower::POWER_TOWER_AREA_COLOR = Color(23, 200, 124, 100);
const Vector2i PowerTower::BLAST_SIZE = Vector2i(256, 256);

PowerTower::PowerTower(const Vector2f &pos)
	: Tower(RESOURCES::TOWER_POWER, pos, STATS)
	,m_isHighlighted(false)
	,gainCount(false)
	,abilityProgress(nullptr)
{
	zeroGround = GlobalVariables::Instance().mapTileSize().x * ZERO_GROUND;
	powerRect.setSize(Vector2f(STATS.radius * GlobalVariables::Instance().mapTileSize().x,
							   STATS.radius * GlobalVariables::Instance().mapTileSize().y));
	powerRect.setFillColor(POWER_TOWER_AREA_COLOR);
	powerRect.setPosition(pos - GlobalVariables::Instance().tileSize());
	m_gain = ENERGY_GAIN;
}

PowerTower::~PowerTower()
{
	if (abilityProgress != nullptr)
		delete abilityProgress;
}

void PowerTower::draw(RenderTarget * const target)
{
	if (m_isHighlighted)
		target->draw(powerRect);
	Tower::draw(target);
	if (abilityProgress != nullptr)
		abilityProgress->draw(target);
}

bool PowerTower::hasEnergy()
{
	return actionTimer.check(actualAttackSpeed());
}

void PowerTower::setHighlighted(bool isHighlighted)
{
	m_isHighlighted = isHighlighted;
}

float PowerTower::gain() const
{
	return m_gain;
}

void PowerTower::updateGain()
{
	gainCount++;
	if (gainCount == MAX_GAIN)
	{
		gainCount = 0;
		if (level() == ABILITY_LEVEL)
			activateBlast();
	}
	if (abilityProgress != nullptr)
	{
		const float rate = static_cast<float>(gainCount) / MAX_GAIN;
		abilityProgress->setValue(rate);
	}
}

void PowerTower::upgrade()
{
	m_gain *= 1 + LEVEL_GAIN;
	Tower::upgrade();
	switch (level())
	{
	case 1:
		m_stats.radius = STATS.radius;
		break;
	case 2:
		m_stats.radius = 10;
		break;
	case 3:
		m_stats.radius = 14;
		break;
	default:
		break;
	}
	upgradePowerRect();
	if (level() == ABILITY_LEVEL)
	{
		abilityProgress = new LifeBar();
		abilityProgress->init(Vector2i(this->sprite.getGlobalBounds().width,
							  LifeBar::LIFE_BAR_HEIGHT * Settings::Instance().getScaleFactor().y),
							  Color::Cyan);
		abilityProgress->setPos(Vector2f(this->sprite.getGlobalBounds().left,
										 this->sprite.getGlobalBounds().top +
										 this->sprite.getGlobalBounds().height -
										 LifeBar::LIFE_BAR_HEIGHT * Settings::Instance().getScaleFactor().y));
		abilityProgress->setValue(0);
	}
}

FloatRect PowerTower::getValidArea() const
{
	return powerRect.getGlobalBounds();
}

void PowerTower::upgradePowerRect()
{
	powerRect.setSize(Vector2f(m_stats.radius * GlobalVariables::Instance().mapTileSize().x,
							   m_stats.radius * GlobalVariables::Instance().mapTileSize().y));

	powerRect.setPosition(this->pos() -
						  Vector2f(GlobalVariables::Instance().tileSize().x * level(),
								   GlobalVariables::Instance().tileSize().y * level()));
}

void PowerTower::activateBlast()
{
	const Vector2f pos = Vector2f(BLAST_SIZE.x/2 * Settings::Instance().getScaleFactor().x - GlobalVariables::Instance().mapTileSize().x,
								  BLAST_SIZE.y/2 * Settings::Instance().getScaleFactor().y - GlobalVariables::Instance().mapTileSize().y);
	Engine::Instance().level()->addAnimation(RESOURCES::BLAST, this->pos() - pos, BLAST_SIZE, 25, 4, 0);
	const Vector2f center = this->getCenter();
	const vector <Enemy*> enemies = Engine::Instance().level()->getAllEnemies();
	for(Enemy* enemy : enemies)
	{
		const float a = fabs(center.x - enemy->enemyCenter().x);
		const float b = fabs(center.y - enemy->enemyCenter().y);
		const float r = powf(powf(a, 2) + powf(b, 2), 0.5f);
		if (r <= zeroGround)
		{
			const float actualDamage = r/zeroGround * BLAST_DAMAGE;
			enemy->hit(actualDamage);
			checkKill(enemy);
		}
	}
}


const int RocketTower::ZERO_GROUND = 3;

RocketTower::RocketTower(const Vector2f &pos)
	: ProjectilesTower(RESOURCES::TOWER_ROCKET, pos, STATS)
{
	zeroGround = GlobalVariables::Instance().tileSize().x * ZERO_GROUND;
	m_shotSound = "sounds/towers/rocket_shot.ogg";

	projectileInfo.size = Vector2i(32, 16);
	projectileInfo.frameCount = 4;
	projectileInfo.texture_id = RESOURCES::ROCKET_PROJECTILE;
	projectileInfo.explosion_texture_id = RESOURCES::ROCKET_EXPLOSION_EFFECT;
	projectileInfo.explosionSize = Vector2i(96, 96);
	projectileInfo.explosionFrameCount = 16;
}

void RocketTower::moveProjectile(Projectile *projectile)
{
	if (projectile->target != nullptr)
	{
		const Vector2f aPos = projectile->getCenter();
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
	if(level() == ABILITY_LEVEL)
		enemy->startBurn();
	const Vector2f epicenter = enemy->enemyCenter();
	const vector <Enemy*> enemies = Engine::Instance().level()->getAllEnemies();
	for(Enemy* levelEnemy : enemies)
	{
		const float a = fabs(epicenter.x - levelEnemy->enemyCenter().x);
		const float b = fabs(epicenter.y - levelEnemy->enemyCenter().y);
		const float r = powf(powf(a, 2) + powf(b, 2), 0.5f);
		if (r <= zeroGround)
		{
			const float actualDamage = r/zeroGround * m_stats.damage;
			levelEnemy->hit(actualDamage);
			checkKill(levelEnemy);
			if(level() == ABILITY_LEVEL)
				levelEnemy->startBurn();
		}
	}
	ProjectilesTower::projectileAction(enemy);
}

FreezeTower::FreezeTower(const Vector2f &pos)
	: ProjectilesTower(RESOURCES::TOWER_FREEZE, pos, STATS)
{
	m_shotSound = "sounds/towers/freeze_shot.ogg";
	projectileInfo.size = Vector2i(20, 10);
	projectileInfo.frameCount = 1;
	projectileInfo.texture_id = RESOURCES::FREEZE_PROJECTILE;
	projectileInfo.explosion_texture_id = RESOURCES::FREEZE_EXPLOSION_EFFECT;
	projectileInfo.explosionSize = Vector2i(12, 12);
	projectileInfo.explosionFrameCount = 16;
}

void FreezeTower::projectileAction(Enemy *enemy)
{
	ProjectilesTower::projectileAction(enemy);
	const float durationOffset = level() * 1000;
	const float freezeValue = BASE_FREEZE_VALUE;
	const float freezeDuration = 2000 + durationOffset;
	enemy->freeze(freezeValue, freezeDuration);

	if (level() == ABILITY_LEVEL)
	{
		const Vector2f epicenter = enemy->enemyCenter();
		const vector <Enemy*> enemies = Engine::Instance().level()->getAllEnemies();
		for(Enemy* levelEnemy : enemies)
		{
			const float a = fabs(epicenter.x - levelEnemy->enemyCenter().x);
			const float b = fabs(epicenter.y - levelEnemy->enemyCenter().y);
			const float r = powf(powf(a, 2) + powf(b, 2), 0.5f);
			if (r <= GlobalVariables::Instance().tileSize().x * ABILITY_FREEZE_CELLS)
				levelEnemy->freeze(BASE_FREEZE_VALUE, freezeDuration);
		}
	}
}

const Vector2i LaserTower::LASER_SIZE = Vector2i(16, 16);

LaserTower::LaserTower(const Vector2f &pos)
	: Tower(RESOURCES::TOWER_LASER, pos, STATS)
{
	lineTower.position = this->pos() + Vector2f(97 * Settings::Instance().getScaleFactor().x * TOWER_SCAlE,
												24 * Settings::Instance().getScaleFactor().y * TOWER_SCAlE);
	lineTower.color = Color::Cyan;
	mainTarget.lineTarget.color = Color::Cyan;
	mainTarget.currentTarget = nullptr;
	mainTarget.laser = new GameObject(RESOURCES::LASER_PROJECTILE, Vector2f(0,0), LASER_SIZE, 3);
//	mainTarget.laser.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::LASER_PROJECTILE));
	m_shotSound = "sounds/towers/laser_shot.ogg";
}

LaserTower::~LaserTower()
{
	clearLasers();
}

void LaserTower::shoot(Enemy *target)
{
	if (level() == ABILITY_LEVEL && target != nullptr)
	{
		clearLasers();

		vector<Enemy*> exclude;
		exclude.push_back(target);
		while(true)
		{
			Enemy *enemy = findNearestEnemy(exclude);
			if (enemy == nullptr)
				break;
			if (targets.size() == MAX_EXTRA_TARGETS)
				break;

			exclude.push_back(enemy);

			LaserTarget laserTarget;
			laserTarget.lineTarget.color = Color::Cyan;
			laserTarget.currentTarget = enemy;
			laserTarget.laser = new GameObject(RESOURCES::LASER_PROJECTILE, Vector2f(0,0), Vector2i(16, 16), 3);
			targets.push_back(laserTarget);
		}
	}

	mainTarget.currentTarget = target;
	if (target != nullptr)
		Tower::shoot(target);
}

void LaserTower::updateLaserTarget(LaserTower::LaserTarget *laserTarget, float damageModifier)
{
	if (laserTarget->currentTarget == nullptr)
		return;

	laserTarget->lineTarget = laserTarget->currentTarget->getCenter();
	laserTarget->laser->setPos(laserTarget->lineTarget.position -
								  Vector2f(LASER_SIZE.x/2 * Settings::Instance().getScaleFactor().x,
										   LASER_SIZE.y/2 * Settings::Instance().getScaleFactor().y));

	if(laserTarget->damageTimer.check(actualAttackSpeed()))
	{
		laserTarget->currentTarget->hit(actualDamage() * damageModifier);
		checkKill(laserTarget->currentTarget);
		if (!laserTarget->currentTarget->isAlive())
			laserTarget->currentTarget = nullptr;
	}
	const int alpha = rand() % 255;
	laserTarget->lineTarget.color.a = alpha;
	laserTarget->laser->update();
}

void LaserTower::drawLaserTarget(RenderTarget * const target,
								 LaserTower::LaserTarget *laserTarget)
{
	if (laserTarget->currentTarget == nullptr)
		return;
	Vertex line[] = { lineTower, laserTarget->lineTarget };
	target->draw(line, 2, Lines);
	laserTarget->laser->draw(target);
}

void LaserTower::clearLasers()
{
	for(const LaserTarget& laserTarget : targets)
		delete laserTarget.laser;
	targets.clear();
}

void LaserTower::update()
{
	Tower::update();
	updateLaserTarget(&mainTarget, 1.f);
	for(LaserTarget& laserTarget : targets)	
		updateLaserTarget(&laserTarget, 0.5);

	for (auto it = targets.begin(); it != targets.end();)
	{
		LaserTarget& laserTarget = *it;
		if (laserTarget.currentTarget == nullptr)
			it = targets.erase(it);
		else
			++it;
	}
}

void LaserTower::draw(RenderTarget * const target)
{
	Tower::draw(target);

	drawLaserTarget(target, &mainTarget);

	Vertex lastVertex = mainTarget.lineTarget;
	for(LaserTarget& laserTarget : targets)
	{
		drawLaserTarget(target, &laserTarget);
		lastVertex = laserTarget.lineTarget;
	}
}

ImprovedTower::ImprovedTower(const Vector2f &pos)
	: ProjectilesTower(RESOURCES::TOWER_IMPROVED, pos, STATS)
{
	m_shotSound = "sounds/towers/improved_shot.ogg";

	projectileInfo.size = Vector2i(40, 16);
	projectileInfo.frameCount = 5;
	projectileInfo.texture_id = RESOURCES::IMPROVED_PROJECTILE;
	projectileInfo.explosion_texture_id = RESOURCES::IMPROVED_EXPLOSION_EFFECT;
	projectileInfo.explosionSize = Vector2i(32, 32);
	projectileInfo.explosionFrameCount = 16;
}

void ImprovedTower::createdProjectile(Projectile *projectile)
{
	if (level() == ABILITY_LEVEL)
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

ProjectilesTower::ProjectilesTower(const RESOURCES::TEXTURE_TYPE &texture_id,
								   const Vector2f &pos,
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

	const Vector2f offset = Vector2f(x2-x1, y2-y1);
	projectile->move(offset);
}

Vector2f ProjectilesTower::shootPos() const
{
	return getCenter();
}

void ProjectilesTower::createdProjectile(Projectile *projectile)
{

}

void ProjectilesTower::draw(RenderTarget * const target)
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

	const Vector2f aPos = shootPos();
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

void ProjectilesTower::collide(const vector<Enemy *> &enemies)
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

vector<Projectile *> ProjectilesTower::projectiles() const
{
	return m_projectiles;
}

void ProjectilesTower::projectileAction(Enemy *enemy)
{
	enemy->hit(actualDamage());
	checkKill(enemy);
	const Vector2f size = enemy->getEnemySize();

	int maxX = size.x/2;
	if (size.x > projectileInfo.explosionSize.x)
		maxX = static_cast<int>(size.x/2 - projectileInfo.explosionSize.x/2);

	int maxY = size.y/2;
	if (size.y > projectileInfo.explosionSize.y)
		maxY = static_cast<int>(size.y/2 - projectileInfo.explosionSize.y/2);

	const Vector2f offset = Vector2f(rand() % maxX * 2 - maxX, rand() % maxY * 2 - maxY);

	Engine::Instance().level()->addAnimation(projectileInfo.explosion_texture_id,
											 enemy->enemyCenter() + offset -
											 Vector2f(projectileInfo.explosionSize.x/2,
													  projectileInfo.explosionSize.y/2),
											 projectileInfo.explosionSize,
											 50, projectileInfo.explosionFrameCount, 0);
}

