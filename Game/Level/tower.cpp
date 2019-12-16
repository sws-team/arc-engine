#include "tower.h"
#include "globalvariables.h"
#include "enemy.h"
#include "projectile.h"
#include "Game/Collisions/collisions.h"
#include "ResourcesManager/resourcesmanager.h"
#include "Engine/engine.h"
#include "Game/Level/level.h"
#include "Game/Audio/soundcontroller.h"

const float Tower::LEVEL_GAIN = 0.25f;
const float Tower::TOWER_SCAlE = 1.f/3.f;

Tower::Tower(const RESOURCES::TEXTURE_TYPE &texture_id, const Vector2f &pos, const TowerStats &stats)
	: GameObject(texture_id, pos,
				 Vector2i(GlobalVariables::CELL_SIZE/TOWER_SCAlE, GlobalVariables::CELL_SIZE/TOWER_SCAlE),
				 4)
	,m_stats(stats)
	,m_level(1)
{
	sprite.scale(TOWER_SCAlE, TOWER_SCAlE);
	abilityDamage.isActive = false;
	abilityAttackSpeed.isActive = false;
}

void Tower::update()
{
	if (abilityDamage.isActive)
	{
		if (abilityDamage.timer.check(abilityDamage.duration))
		{
			abilityDamage.isActive = false;
			m_stats.damage /= abilityDamage.value;
		}
	}
	if (abilityAttackSpeed.isActive)
	{
		if (abilityAttackSpeed.timer.check(abilityAttackSpeed.duration))
		{
			abilityAttackSpeed.isActive = false;
			m_stats.attackSpeed *= abilityAttackSpeed.value;
		}
	}
}

void Tower::draw(RenderTarget * const target)
{
	GameObject::draw(target);
}

void Tower::action(const vector<Enemy *> &enemies)
{
	if (!actionTimer.check(m_stats.attackSpeed))
		return;

	Enemy *target = nullptr;
	float min = INT_MAX;
	const Vector2f aPos = getCenter();
	for(Enemy *enemy : enemies)
	{
		if (TowersFactory::isIntersects(enemy->gameRect(), aPos, m_stats.radius * GlobalVariables::Instance().mapTileSize().x))
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
	m_stats.radius += 2 * (m_level - 1);
	m_stats.attackSpeed *= 1 - Tower::LEVEL_GAIN;
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

void Tower::increaseAttackSpeed(int duration, int value)
{
	abilityAttackSpeed.isActive = true;
	abilityAttackSpeed.duration = duration;
	m_stats.attackSpeed /= abilityAttackSpeed.value;
	abilityAttackSpeed.timer.clock.restart();
}

void Tower::increaseDamage(int duration, int value)
{
	abilityDamage.isActive = true;
	abilityDamage.duration = duration;
	m_stats.damage *= abilityDamage.value;
	abilityDamage.timer.clock.restart();
}

int Tower::level() const
{
	return m_level;
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
const TowerStats PowerTower::STATS = TowerStats		(0,		5000,	5,		0,		60);
const TowerStats BaseTower::STATS = TowerStats		(5,		450,	4,		20,		50);
const TowerStats FreezeTower::STATS = TowerStats	(5,		350,	6,		10,		80);
const TowerStats RocketTower::STATS = TowerStats	(100,	3500,	12,		20,		150);
const TowerStats LaserTower::STATS = TowerStats		(15,	200,	6,		0,		180);
const TowerStats ImprovedTower::STATS = TowerStats	(20,	150,	7,		50,		250);

BaseTower::BaseTower(const Vector2f &pos)
	: ProjectilesTower(RESOURCES::TOWER_BASE, RESOURCES::BASE_PROJECTILE, pos, STATS)
{
	m_shotSound = "sounds/towers/base_shot.ogg";
}

PowerTower::PowerTower(const Vector2f &pos)
	: Tower(RESOURCES::TOWER_POWER, pos, STATS)
	,m_isHighlighted(false)
{
	powerRadius.setRadius(STATS.radius * GlobalVariables::Instance().mapTileSize().x);
	powerRadius.setFillColor(Color::Transparent);
	powerRadius.setOutlineColor(Color(23, 200, 124));
	powerRadius.setOutlineThickness(3);
	powerRadius.setPosition(pos);	
	powerRadius.setOrigin(powerRadius.getRadius() - GlobalVariables::Instance().mapTileSize().x,
						  powerRadius.getRadius() - GlobalVariables::Instance().mapTileSize().y);
	m_gain = ENERGY_GAIN;
}

void PowerTower::draw(RenderTarget * const target)
{
	if (m_isHighlighted)
		target->draw(powerRadius);
	Tower::draw(target);
}

bool PowerTower::hasEnergy()
{
	return actionTimer.check(m_stats.attackSpeed);
}

void PowerTower::setHighlighted(bool isHighlighted)
{
	m_isHighlighted = isHighlighted;
}

float PowerTower::gain() const
{
	return m_gain;
}

void PowerTower::upgrade()
{
	m_gain *= LEVEL_GAIN;
	Tower::upgrade();
	upgradePowerRadius();
}

void PowerTower::upgradePowerRadius()
{
	powerRadius.setRadius(m_stats.radius * GlobalVariables::Instance().mapTileSize().x);
	powerRadius.setOrigin(powerRadius.getRadius() - GlobalVariables::Instance().mapTileSize().x,
						  powerRadius.getRadius() - GlobalVariables::Instance().mapTileSize().y);
}


const int RocketTower::ZERO_GROUND = 3;

RocketTower::RocketTower(const Vector2f &pos)
	: ProjectilesTower(RESOURCES::TOWER_ROCKET, RESOURCES::ROCKET_PROJECTILE, pos, STATS)
{
	m_zeroGround = GlobalVariables::Instance().tileSize().x * ZERO_GROUND;
	m_shotSound = "sounds/towers/rocket_shot.ogg";
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
	const Vector2f epicenter = enemy->enemyCenter();
	const vector <Enemy*> enemies = Engine::Instance().level()->getAllEnemies();
	for(Enemy* levelEnemy : enemies)
	{
		const float a = fabs(epicenter.x - levelEnemy->enemyCenter().x);
		const float b = fabs(epicenter.y - levelEnemy->enemyCenter().y);
		const float r = powf(powf(a, 2) + powf(b, 2), 0.5f);
		if (r <= m_zeroGround)
		{
			const float actualDamage = r/m_zeroGround * m_stats.damage;
			levelEnemy->hit(actualDamage);
		}
	}
	ProjectilesTower::projectileAction(enemy);
}

FreezeTower::FreezeTower(const Vector2f &pos)
	: ProjectilesTower(RESOURCES::TOWER_FREEZE, RESOURCES::FREEZE_PROJECTILE, pos, STATS)
{
	m_shotSound = "sounds/towers/freeze_shot.ogg";
}

void FreezeTower::projectileAction(Enemy *enemy)
{
	ProjectilesTower::projectileAction(enemy);
	enemy->freeze(45.f, 3000);
}

LaserTower::LaserTower(const Vector2f &pos)
	: Tower(RESOURCES::TOWER_LASER, pos, STATS)
{
	currentTarget = nullptr;
	lineTower.position = getCenter();
	lineTower.color = Color::Red;
	lineTarget.color = Color::Blue;
	laser.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::LASER_PROJECTILE));
	m_shotSound = "sounds/towers/laser_shot.ogg";
}

void LaserTower::shoot(Enemy *target)
{
	currentTarget = target;
	if (target != nullptr)
		Tower::shoot(target);
}

void LaserTower::update()
{
	Tower::update();
	if (currentTarget == nullptr)
		return;

	lineTarget = currentTarget->getCenter();
	laser.setPosition(lineTarget.position - Vector2f(laser.getGlobalBounds().width/2, laser.getGlobalBounds().height/2));

	if(damageTimer.check(m_stats.attackSpeed))
		currentTarget->hit(m_stats.damage);
}

void LaserTower::draw(RenderTarget * const target)
{
	Tower::draw(target);
	if (currentTarget == nullptr)
		return;
	Vertex line[] = { lineTower, lineTarget };
	target->draw(line, 2, Lines);
	target->draw(laser);
}

ImprovedTower::ImprovedTower(const Vector2f &pos)
	: ProjectilesTower(RESOURCES::TOWER_IMPROVED, RESOURCES::IMPROVED_PROJECTILE, pos, STATS)
{
	m_shotSound = "sounds/towers/improved_shot.ogg";
}

ProjectilesTower::ProjectilesTower(const RESOURCES::TEXTURE_TYPE &texture_id, const RESOURCES::TEXTURE_TYPE &projectile_id,
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

	projectile->move(x2-x1, y2-y1);
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

	const Vector2f aPos = getCenter();
	const float a = aPos.x - target->enemyCenter().x;
	const float b = aPos.y - target->enemyCenter().y;
	const float tg = ( b / a );
	float angle = atanf(tg) * 180 / M_PI;
	if (a < 0)
		angle += 180;
	else if (b < 0)
		angle += 360;

	angle -= 180;

	const Vector2i frameSize = Vector2i(10, 5);
	Projectile *projectile = new Projectile(RESOURCES::BASE_PROJECTILE, aPos, frameSize, 1);
	projectile->setAngle(angle);
	projectile->target = target;
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
	enemy->hit(m_stats.damage);
	Engine::Instance().level()->addAnimation(RESOURCES::EXPLOSION_EFFECT, enemy->enemyCenter() - Vector2f(32, 32), Vector2i(64, 64), 100, 16, 0);
}

