#include "tower.h"
#include "globalvariables.h"
#include "enemy.h"
#include "projectile.h"
#include "Game/Collisions/collisions.h"
#include "ResourcesManager/resourcesmanager.h"

Tower::Tower(const RESOURCES::TEXTURE_TYPE &texture_id, const Vector2f &pos, const TowerStats &stats)
	: GameObject(texture_id, pos,
				 Vector2i(GlobalVariables::CELL_SIZE * 2, GlobalVariables::CELL_SIZE * 2),
				 4)
	,m_stats(stats)
	,m_selected(false)
{
	radius.setRadius(m_stats.radius * GlobalVariables::Instance().tileSize().x);
	radius.setFillColor(Color(34, 255, 56, 120));
	radius.setPosition(pos);
	radius.setOrigin(radius.getRadius() - GlobalVariables::Instance().tileSize().x/2,
					 radius.getRadius() - GlobalVariables::Instance().tileSize().y/2);
	sprite.scale(0.5f, 0.5f);
}

void Tower::update()
{

}

void Tower::draw(RenderTarget * const target)
{
	if (m_selected)
		target->draw(radius);
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
		if (TowersFactory::isIntersects(enemy->gameRect(), aPos, m_stats.radius * GlobalVariables::Instance().tileSize().x))
		{
			const float x = fabs(enemy->getCenter().x - this->getCenter().x);
			const float y = fabs(enemy->getCenter().y - this->getCenter().y);
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

}

void Tower::collide(const vector<Enemy *> &enemies)
{

}

void Tower::upgrade()
{
	level++;

	const float k = 0.3f;
	m_stats.cost = m_stats.cost * k;
	m_stats.damage = m_stats.damage * k;
	m_stats.radius = m_stats.radius * k;
	m_stats.attackSpeed = m_stats.attackSpeed * k;
}

void Tower::hitEnemy(Enemy *enemy)
{

}

void Tower::select()
{
	m_selected = true;
}

void Tower::deselect()
{
	m_selected = false;
}

TowersFactory::TOWER_TYPES Tower::type() const
{
	return m_type;
}

void Tower::setType(const TowersFactory::TOWER_TYPES &type)
{
	m_type = type;
}

TowerStats Tower::data() const
{
	return m_stats;
}

Tower *TowersFactory::createTower(TowersFactory::TOWER_TYPES type, const Vector2f &pos)
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

TowerStats TowersFactory::getTowerStats(TowersFactory::TOWER_TYPES type)
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

const TowerStats BaseTower::STATS = TowerStats(10, 300, 10, 2, 10, 20);
const TowerStats PowerTower::STATS = TowerStats(10, 300, 10, 4, 10, 25);
const TowerStats RocketTower::STATS = TowerStats(1, 3000, 10, 10, 3, 50);
const TowerStats FreezeTower::STATS = TowerStats(1, 300, 0, 3, 15, 40);
const TowerStats LaserTower::STATS = TowerStats(10, 300, 10, 4, 10, 80);
const TowerStats ImprovedTower::STATS = TowerStats(10, 100, 10, 6, 50, 100);
const TowerStats ElectricTower::STATS = TowerStats(10, 300, 10, 4, 10, 20);

BaseTower::BaseTower(const Vector2f &pos)
	: ProjectilesTower(RESOURCES::TOWER_BASE, RESOURCES::BASE_PROJECTILE, pos, STATS)
{

}

PowerTower::PowerTower(const Vector2f &pos)
	: Tower(RESOURCES::TOWER_POWER, pos, STATS)
	,m_isHighlighted(false)
{
	powerRadius.setRadius(STATS.radius * GlobalVariables::Instance().tileSize().x);
	powerRadius.setFillColor(Color::Transparent);
	powerRadius.setOutlineColor(Color(23, 200, 124));
	powerRadius.setOutlineThickness(3);
	powerRadius.setPosition(pos);
	powerRadius.setOrigin(powerRadius.getRadius() - GlobalVariables::Instance().tileSize().x/2,
						  powerRadius.getRadius() - GlobalVariables::Instance().tileSize().y/2);
}

void PowerTower::draw(RenderTarget * const target)
{
	if (m_isHighlighted)
		target->draw(powerRadius);
	Tower::draw(target);
}

bool PowerTower::hasEnergy()
{
	return actionTimer.check(4000);
}

void PowerTower::setHighlighted(bool isHighlighted)
{
	m_isHighlighted = isHighlighted;
}


RocketTower::RocketTower(const Vector2f &pos)
	: ProjectilesTower(RESOURCES::TOWER_ROCKET, RESOURCES::ROCKET_PROJECTILE, pos, STATS)
{

}

void RocketTower::moveProjectile(Projectile *projectile)
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

	ProjectilesTower::moveProjectile(projectile);
}

FreezeTower::FreezeTower(const Vector2f &pos)
	: ProjectilesTower(RESOURCES::TOWER_FREEZE, RESOURCES::FREEZE_PROJECTILE, pos, STATS)
{

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
}

void LaserTower::shoot(Enemy *target)
{
	currentTarget = target;
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

}

void ProjectilesTower::removeProjectile(Projectile *projectile)
{
	m_projectiles.erase( remove( m_projectiles.begin(), m_projectiles.end(), projectile ), m_projectiles.end() );
	delete projectile;
	projectile = nullptr;
}

void ProjectilesTower::moveProjectile(Projectile *projectile)
{
	const float x1 = pos().x;
	const float y1 = pos().y;

	const float y2 = y1 + m_stats.projectileSpeed * sin(projectile->angle() * M_PI/180);
	const float x2 = x1 + m_stats.projectileSpeed * cos(projectile->angle() * M_PI/180);

	projectile->move(x2-x1, y2-y1);
}

ProjectilesTower::ProjectilesTower(const RESOURCES::TEXTURE_TYPE &texture_id, const RESOURCES::TEXTURE_TYPE &projectile_id,
								   const Vector2f &pos,
								   const TowerStats &stats)
	: Tower(texture_id, pos, stats)
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
	const float a = aPos.x - target->getCenter().x;
	const float b = aPos.y - target->getCenter().y;
	const float tg = ( b / a );
	float angle = atanf(tg) * 180 / M_PI;
	if (a < 0)
		angle += 180;
	else if (b < 0)
		angle += 360;

	angle -= 180;

	const Vector2i frameSize = Vector2i(10, 6);
	Projectile *projectile = new Projectile(RESOURCES::BASE_PROJECTILE, aPos, frameSize, 3);
	projectile->setAngle(angle);
	projectile->target = target;
	m_projectiles.push_back(projectile);
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
}

ElectricTower::ElectricTower(const Vector2f &pos)
	: Tower(RESOURCES::TOWER_ELECTRIC, pos, STATS)
{

}

void ElectricTower::draw(RenderTarget * const target)
{
	Tower::draw(target);
//	if (currentTarget == nullptr)
//		return;
	Vertex line[] = { lineTower, lineTarget };
	target->draw(line, 2, Lines);
}

void ElectricTower::shoot(Enemy *target)
{

}
