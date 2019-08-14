#ifndef TOWER_H
#define TOWER_H

#include "Game/gameobject.h"
#include "Game/leveldef.h"

struct TowerStats
{
	TowerStats(float damage,
			   float attackSpeed,
			   int radius,
			   float projectileSpeed,
			   float cost)
		: damage(damage)
		,attackSpeed(attackSpeed)
		,radius(radius)
		,projectileSpeed(projectileSpeed)
		,cost(cost)
	{

	}
	float damage;
	float attackSpeed;
	int radius;
	float projectileSpeed;
	float cost;
};

class Enemy;
class Projectile;
class Tower;

class TowersFactory
{
public:
	static Tower *createTower(TOWER_TYPES type, const Vector2f& pos);
	static TowerStats getTowerStats(TOWER_TYPES type);
	static bool isIntersects(const FloatRect& rect, const Vector2f& center, float radius);
};

class Tower : public GameObject
{
public:
	Tower(const RESOURCES::TEXTURE_TYPE &texture_id,
		  const Vector2f &pos,
		  const TowerStats& stats);


	//GameObject
	virtual void update() override;
	virtual void draw(RenderTarget *const target) override;

	//Tower
	void action(const vector<Enemy*>& enemies);
	virtual void shoot(Enemy* target);
	virtual void collide(const vector<Enemy*>& enemies);
	virtual void upgrade();
	TowerStats data() const;

	void hitEnemy(Enemy* enemy);
	void select();
	void deselect();

	TOWER_TYPES type() const;
	void setType(const TOWER_TYPES &type);

	static const float LEVEL_GAIN;
	static const float TOWER_SCAlE;

protected:
	TowerStats m_stats;
	Timer actionTimer;
private:
	CircleShape radius;
	bool m_selected;

	TOWER_TYPES m_type;

	int m_level;
};

class ProjectilesTower : public Tower
{
public:
	ProjectilesTower(const RESOURCES::TEXTURE_TYPE &texture_id,
					 const RESOURCES::TEXTURE_TYPE &projectile_id,
					 const Vector2f &pos,
					 const TowerStats& stats);

	//GameObject
	void draw(RenderTarget *const target) final;
	void update() final;

	//Tower
	void shoot(Enemy *target) final;
	void collide(const vector<Enemy*>& enemies) final;

	//ProjectilesTower
	vector<Projectile*> projectiles() const;
	void removeProjectile(Projectile *projectile);

	void checkEnemy(Enemy *enemy);

protected:
	virtual void projectileAction(Enemy *enemy);
	virtual void moveProjectile(Projectile *projectile);

private:
	vector<Projectile*> m_projectiles;
	Timer moveTimer;
};

class BaseTower : public ProjectilesTower
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

	void draw(RenderTarget *const target) final;

	bool hasEnergy();
	void setHighlighted(bool isHighlighted);

	float gain() const;

	void upgrade() override;
private:
	static const int ENERGY_GAIN;
	bool m_isHighlighted;
	int m_gain;

	CircleShape powerRadius;
};

class RocketTower : public ProjectilesTower
{
public:
	RocketTower(const Vector2f &pos);
	const static TowerStats STATS;
	void moveProjectile(Projectile *projectile) override;
	void projectileAction(Enemy *enemy) override;
private:
	const static int ZERO_GROUND;
	float m_zeroGround;
};

class FreezeTower : public ProjectilesTower
{
public:
	FreezeTower(const Vector2f &pos);
	const static TowerStats STATS;

	void projectileAction(Enemy *enemy) override;
private:
};

class LaserTower : public Tower
{
public:
	LaserTower(const Vector2f &pos);
	const static TowerStats STATS;

	void draw(RenderTarget *const target) final;
	void update() final;

	void shoot(Enemy *target) final;

private:	
	Vertex lineTower;
	Vertex lineTarget;
	Sprite laser;

	Enemy *currentTarget;
	Timer damageTimer;
};

class ImprovedTower : public ProjectilesTower
{
public:
	ImprovedTower(const Vector2f &pos);
	const static TowerStats STATS;
};

#endif // TOWER_H
