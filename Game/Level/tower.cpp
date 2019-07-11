#include "tower.h"
#include "globalvariables.h"
#include "enemy.h"
#include "projectile.h"

Tower::Tower(const RESOURCES::TEXTURE_TYPE &texture_id, const Vector2f &pos, const TowerStats &stats)
	: GameObject(texture_id, pos,
				 Vector2i(GlobalVariables::CELL_SIZE, GlobalVariables::CELL_SIZE),
				 4)
	,m_stats(stats)
	,m_selected(false)
{
	radius.setRadius(m_stats.radius * GlobalVariables::Instance().tileSize().x);
	radius.setFillColor(Color(34, 255, 56, 120));
	radius.setPosition(pos);
	radius.setOrigin(radius.getRadius() - GlobalVariables::Instance().tileSize().x/2,
					 radius.getRadius() - GlobalVariables::Instance().tileSize().y/2);
}

void Tower::update()
{
	if (moveTimer.check(Projectile::PROJECTILE_GAME_SPEED))
	{
		for(Projectile *projectile : m_projectiles)
		{
			const float x1 = pos().x;
			const float y1 = pos().y;

			const float y2 = y1 + m_stats.projectileSpeed * sin(projectile->angle * M_PI/180);
			const float x2 = x1 + m_stats.projectileSpeed * cos(projectile->angle * M_PI/180);

			projectile->move(x2-x1, y2-y1);
		}
	}
	for(Projectile *projectile : m_projectiles)
		projectile->update();
}

void Tower::draw(RenderTarget * const target)
{
	if (m_selected)
		target->draw(radius);
	GameObject::draw(target);
	for(Projectile *projectile : m_projectiles)
		projectile->draw(target);
}

void Tower::shoot(const vector<Enemy *> &enemies)
{
	if (!shootTimer.check(m_stats.attackSpeed))
		return;

	Enemy *target = nullptr;
	float min = INT_MAX;
	const Vector2f aPos = getCenter();
	for(Enemy *enemy : enemies)
	{
		if (isIntersects(enemy->gameRect(), aPos, m_stats.radius * GlobalVariables::Instance().tileSize().x))
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
	if (target != nullptr)
	{
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
		Projectile *projectile = new Projectile(RESOURCES::BASE_PROJECTILE, aPos, frameSize, 3, angle);
		m_projectiles.push_back(projectile);
	}
}

void Tower::action(Enemy *enemy)
{
	enemy->hit(m_stats.damage);
}

void Tower::select()
{
	m_selected = true;
}

void Tower::deselect()
{
	m_selected = false;
}

vector<Projectile *> Tower::projectiles() const
{
	return m_projectiles;
}

void Tower::removeProjectile(Projectile *projectile)
{
	m_projectiles.erase( std::remove( m_projectiles.begin(), m_projectiles.end(), projectile ), m_projectiles.end() );
	delete projectile;
	projectile = nullptr;
}

bool Tower::isIntersects(const FloatRect &rect, const Vector2f &center, float radius)
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
	case SPLASH:
		tower = new SplashTower(pos);
		break;
	case IMPROVED:
		tower = new ImprovedTower(pos);
		break;
	}
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
	case SPLASH:
		return SplashTower::STATS;
	case IMPROVED:
		return ImprovedTower::STATS;
	}
	return BaseTower::STATS;
}

const TowerStats BaseTower::STATS = TowerStats(10, 300, 10, 2, 10);
const TowerStats PowerTower::STATS = TowerStats(10, 300, 10, 4, 10);
const TowerStats RocketTower::STATS = TowerStats(10, 300, 10, 4, 10);
const TowerStats FreezeTower::STATS = TowerStats(1, 300, 0, 3, 15);
const TowerStats SplashTower::STATS = TowerStats(10, 300, 10, 4, 10);
const TowerStats ImprovedTower::STATS = TowerStats(10, 100, 10, 6, 50);

BaseTower::BaseTower(const Vector2f &pos)
	: Tower(RESOURCES::TOWER_BASE, pos, STATS)
{

}

PowerTower::PowerTower(const Vector2f &pos)
	: Tower(RESOURCES::TOWER_POWER, pos, STATS)
{

}

RocketTower::RocketTower(const Vector2f &pos)
	: Tower(RESOURCES::TOWER_ROCKET, pos, STATS)
{

}

FreezeTower::FreezeTower(const Vector2f &pos)
	: Tower(RESOURCES::TOWER_FREEZE, pos, STATS)
{

}

void FreezeTower::action(Enemy *enemy)
{
	Tower::action(enemy);
	enemy->freeze(45.f, 3000);
}

SplashTower::SplashTower(const Vector2f &pos)
	: Tower(RESOURCES::TOWER_FREEZE, pos, STATS)
{

}

ImprovedTower::ImprovedTower(const Vector2f &pos)
	: Tower(RESOURCES::TOWER_FREEZE, pos, STATS)
{

}
