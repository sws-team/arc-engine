#ifndef TOWER_H
#define TOWER_H

#include "gameobject.h"
#include "Game/leveldef.h"
#include "enginedef.h"

struct TowerStats
{
	TowerStats()
	{

	}
	TowerStats(float damage,
			   float attackSpeed,
			   float radius,
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
	float radius;
	float projectileSpeed;
	float cost;
};

class Enemy;
class Projectile;
class Tower;

class TowersFactory
{
public:
	static Tower *createTower(TOWER_TYPES type, const sf::Vector2f& pos);
	static TowerStats getTowerStats(TOWER_TYPES type);
	static bool isIntersects(const sf::FloatRect& rect, const sf::Vector2f& center, float radius);
	constexpr static float UPGRADE_TO_2_COST_MODIFIER = 0.5f;
	constexpr static float UPGRADE_TO_3_COST_MODIFIER = 0.75f;
};

class Tower : public GameObject
{
public:
	Tower(const TextureType &texture_id,
		  const sf::Vector2f &pos,
		  const TowerStats& stats);

	//GameObject
	virtual void update() override;
	virtual void draw(sf::RenderTarget *const target) override;

	//Tower
	void action();
	virtual void shoot(Enemy* target);
	virtual void collide(const std::vector<Enemy*>& enemies);
	virtual void upgrade();
	TowerStats data() const;

	TOWER_TYPES type() const;
	void setType(const TOWER_TYPES &type);

	static const float LEVEL_GAIN;
	static const float TOWER_SCAlE;

	void increaseAttackSpeed(float value);
	void decreaseAttackSpeed(float value);
	void increaseDamage(float value);
	void decreaseDamage(float value);

	int level() const;
	int kills() const;

	void setActive(bool isActive);
	bool isActive() const;

	void setDowngrade(bool isDowngrade);
	bool isDowngraded() const;

	bool isBlinded() const;
	void setBlinded(bool blinded);

	void setRegressed(bool regressed);
	bool isRegressed() const;

	float actualRadius() const;
	float actualDamage() const;
	float actualAttackSpeed() const;

	static constexpr int ABILITY_LEVEL = 4;
	void setInvulnerable(bool invulnerable);

	bool isInvulnerable() const;

protected:
	TowerStats m_stats;
	Timer actionTimer;
	SoundType m_shotSound;
	void checkKill(Enemy *enemy);
	Enemy *findNearestEnemy(const std::vector<Enemy*>& exclude);

private:
	TOWER_TYPES m_type;
	int m_level;
	int m_kills;
	bool m_isActive;
	bool m_downgraded;
	bool m_blinded;
	bool m_regressed;
	bool m_invulnerable;
};

class ProjectilesTower : public Tower
{
public:
	ProjectilesTower(const TextureType &texture_id,
					 const sf::Vector2f &pos,
					 const TowerStats& stats);

	//GameObject
	void draw(sf::RenderTarget *const target) final;
	void update() final;

	//Tower
	void shoot(Enemy *target) final;
	void collide(const std::vector<Enemy*>& enemies) final;

	//ProjectilesTower
	std::vector<Projectile*> projectiles() const;
	void removeProjectile(Projectile *projectile);

	void checkEnemy(Enemy *enemy);

protected:
	virtual void projectileAction(Enemy *enemy);
	virtual void moveProjectile(Projectile *projectile);
	virtual sf::Vector2f shootPos() const;
	virtual void createdProjectile(Projectile *projectile);

	struct ProjectileInfo
	{
		TextureType texture_id;
		sf::Vector2i size;
		int frameCount;
		TextureType explosion_texture_id;
		sf::Vector2i explosionSize;
		int explosionFrameCount;
	};
	ProjectileInfo projectileInfo;

	std::vector<Projectile*> m_projectiles;
private:
	Timer moveTimer;
};

class BaseTower : public ProjectilesTower
{
public:
	BaseTower(const sf::Vector2f &pos);

	const static TowerStats STATS;
	void projectileAction(Enemy *enemy) override;
	void upgrade() override;
private:
};

class PowerTower : public Tower
{
public:
	PowerTower(const sf::Vector2f &pos);
	~PowerTower() override;
	const static TowerStats STATS;

	void draw(sf::RenderTarget *const target) final;

	bool hasEnergy();
	void setHighlighted(bool isHighlighted);

	float gain() const;
	void updateGain();

	void upgrade() override;

	sf::FloatRect getValidArea() const;

	static const int COST_OFFSET;
	static const sf::Color POWER_TOWER_AREA_COLOR;
private:
	static const float ENERGY_GAIN;
	bool m_isHighlighted;
	float m_gain;

	sf::RectangleShape powerRect;
	void upgradePowerRect();

	int gainCount;
	constexpr static int MAX_GAIN = 5;
	void activateBlast();
	float zeroGround;
	constexpr static int ZERO_GROUND = 6;
	class LifeBar* abilityProgress;
	static const sf::Vector2i BLAST_SIZE;
};

class RocketTower : public ProjectilesTower
{
public:
	RocketTower(const sf::Vector2f &pos);

	const static TowerStats STATS;
	void moveProjectile(Projectile *projectile) override;
	void projectileAction(Enemy *enemy) override;
private:
	const static int ZERO_GROUND;
	float zeroGround;
};

class FreezeTower : public ProjectilesTower
{
public:
	FreezeTower(const sf::Vector2f &pos);
	const static TowerStats STATS;

	void projectileAction(Enemy *enemy) override;
private:
	static constexpr int ABILITY_FREEZE_CELLS = 2;
	static constexpr float BASE_FREEZE_VALUE = 0.5f;
};

class LaserTower : public Tower
{
public:
	LaserTower(const sf::Vector2f &pos);
	~LaserTower() override;

	const static TowerStats STATS;

	void draw(sf::RenderTarget *const target) final;
	void update() final;

	void shoot(Enemy *target) final;

private:	
	sf::Vertex lineTower;

	struct LaserTarget
	{
		sf::Vertex lineTarget;
		GameObject *laser;
		Enemy *currentTarget;
		Timer damageTimer;
	};
	std::vector<LaserTarget> targets;
	LaserTarget mainTarget;
	static constexpr int MAX_EXTRA_TARGETS = 4;

	void updateLaserTarget(LaserTarget *laserTarget, float damageModifier);
	void drawLaserTarget(sf::RenderTarget * const target,
						 LaserTarget *laserTarget);
	static const sf::Vector2i LASER_SIZE;
	void clearLasers();
};

class ImprovedTower : public ProjectilesTower
{
public:
	ImprovedTower(const sf::Vector2f &pos);
	const static TowerStats STATS;
protected:
	void createdProjectile(Projectile *projectile) override;
};

#endif // TOWER_H
