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
		for(Projectile *projectile : projectiles)
		{
			const float x1 = pos().x;
			const float y1 = pos().y;

			const float y2 = y1 + m_stats.projectileSpeed * sin(projectile->angle * M_PI/180);
			const float x2 = x1 + m_stats.projectileSpeed * cos(projectile->angle * M_PI/180);

			projectile->move(x2-x1, y2-y1);
		}
	}
	for(Projectile *projectile : projectiles)
		projectile->update();
}

void Tower::draw(RenderTarget * const target)
{
	if (m_selected)
		target->draw(radius);
	GameObject::draw(target);
	for(Projectile *projectile : projectiles)
		projectile->draw(target);
}
//#include "Engine/engine.h"
//#include "camera.h"
void Tower::shoot(const vector<Enemy *> &enemies)
{
//	const Vector2i pixelPos = Mouse::getPosition(*Engine::Instance().window());
//	const Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos, *Engine::Instance().camera()->getView());
//	cout << "POS "<< pos.x << " " << pos.y << endl;
//	cout << "center "<< getCenter().x << " " << getCenter().y << endl;
//	const Vector2f R = radius.getPosition() - radius.getOrigin();
//	cout << "center "<< R.x << " " << R.y << endl << endl;

//	return;

//	for(Enemy *enemy : enemies)
//	{
//		cout << "INTERSECTs " << isIntersects(enemy->gameRect(), getCenter(), m_stats.radius) << endl;
//	}
//	cout << "RADIUS " << m_stats.radius << endl << endl;
//	return;


	if (!shootTimer.check(m_stats.attackSpeed))
		return;

//	const Vector2i pixelPos = Mouse::getPosition(*Engine::Instance().window());
//	const Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos, *Engine::Instance().camera()->getView());

//	const float a = this->getCenter().x - pos.x;
//	const float b = this->getCenter().y - pos.y;
//	const float tg = ( b / a );
//	float angle = atanf(tg) * 180 / M_PI;

//	if (a < 0)
//		angle += 180;
//	else if (b < 0)
//		angle += 360;
//	return;

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
		projectiles.push_back(projectile);
	}
}

void Tower::select()
{
	m_selected = true;
}

void Tower::deselect()
{
	m_selected = false;
}

void Tower::removeProjectile(Projectile *projectile)
{
	projectiles.erase( std::remove( projectiles.begin(), projectiles.end(), projectile ), projectiles.end() );
	delete projectile;
	projectile = nullptr;
}

bool Tower::isIntersects(const FloatRect &rect, const Vector2f &center, float radius)
{
	float x = fabs(rect.left - center.x);
	float y = fabs(rect.top - center.y);
	float r = sqrtf(powf(x, 2) + powf(y, 2));
//	cout << "top left " << r << endl;
	if (r < radius)
		return true;

	x = fabs(rect.left + rect.width - center.x);
	y = fabs(rect.top - center.y);

	r = sqrtf(powf(x, 2) + powf(y, 2));
//	cout << "top right " << r << endl;
	if (r < radius)
		return true;

	x = fabs(rect.left + rect.width - center.x);
	y = fabs(rect.top + rect.height - center.y);

	r = sqrtf(powf(x, 2) + powf(y, 2));
//	cout << "bottom right " << r << endl;

//	cout << "targetx " << rect.left + rect.width << " targety "<< rect.top + rect.height << endl;
//	cout << "towerx " << center.x<< " towery "<< center.y << endl;
//	cout << "x " << x << " y "<< y << endl;

	if (r < radius)
		return true;

	x = fabs(rect.left - center.x);
	y = fabs(rect.top + rect.height - center.y);

	r = sqrtf(powf(x, 2) + powf(y, 2));
//	cout << "bottom left " << r << endl;

	if (r < radius)
		return true;

	return false;
}

const TowerStats BaseTower::STATS = TowerStats(10, 300, 10, 4, 10);

BaseTower::BaseTower(const Vector2f &pos)
	: Tower(RESOURCES::TOWER_BASE, pos, STATS)
{

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

		break;
	case ROCKET:

		break;
	case FREEZE:

		break;
	case SPLASH:

		break;
	case IMPROVED:

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

		break;
	case ROCKET:

		break;
	case FREEZE:

		break;
	case SPLASH:

		break;
	case IMPROVED:

		break;
	}
}
