#ifndef TOWER_H
#define TOWER_H

#include "Game/gameobject.h"

struct TowerStats
{
	TowerStats(float damage,
			   float attackSpeed,
			   float damageOffset,
			   float radius,
			   float projectileSpeed)
		: damage(damage)
		,attackSpeed(attackSpeed)
		,damageOffset(damageOffset)
		,radius(radius)
		,projectileSpeed(projectileSpeed)
	{

	}
	float damage;
	float attackSpeed;
	float damageOffset;
	float radius;
	float projectileSpeed;
};

class Enemy;
class Projectile;

class Tower : public GameObject
{
public:
	Tower(const RESOURCES::TEXTURE_TYPE &texture_id, const Vector2f &pos, const TowerStats& stats);
	void update() override;

	void draw(RenderTarget *const target) override;
	void shoot(const vector<Enemy*>& enemies);
	virtual void action(Enemy *enemy);

	void select();
	void deselect();

	vector<Projectile *> projectiles() const;

	void removeProjectile(Projectile *projectile);

private:
	TowerStats m_stats;
	CircleShape radius;

	static bool isIntersects(const FloatRect& rect, const Vector2f& center, float radius);
	bool m_selected;
	Timer shootTimer;

	vector<Projectile*> m_projectiles;
	Timer moveTimer;
};

class TowersFactory
{
public:
	enum TOWER_TYPES
	{
		BASE,
		POWER,
		ROCKET,
		FREEZE,
		SPLASH,
		IMPROVED,
	};
	static Tower *createTower(TOWER_TYPES type, const Vector2f& pos);
	static TowerStats getTowerStats(TOWER_TYPES type);
};

class BaseTower : public Tower
{
public:
	BaseTower(const Vector2f &pos);
	const static TowerStats STATS;

private:
};

class PowerTower : public Tower
{
public:
	PowerTower(const Vector2f &pos);
	const static TowerStats STATS;
};

class RocketTower : public Tower
{
public:
	RocketTower(const Vector2f &pos);
	const static TowerStats STATS;

private:
};

class FreezeTower : public Tower
{
public:
	FreezeTower(const Vector2f &pos);
	const static TowerStats STATS;

	void action(Enemy *enemy) override;
private:
};

class SplashTower : public Tower
{
public:
	SplashTower(const Vector2f &pos);
	const static TowerStats STATS;

private:
};

class ImprovedTower : public Tower
{
public:
	ImprovedTower(const Vector2f &pos);
	const static TowerStats STATS;

private:
};


#endif // TOWER_H
