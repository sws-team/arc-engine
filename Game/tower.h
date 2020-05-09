#ifndef TOWER_H
#define TOWER_H

#include "gameobject.h"
#include "enginedef.h"
#include "Balance/balance.h"

struct TowersCounter
{
	static TowersCounter &Instance();

	int baseTowerCount;
	int freezeTowerCount;
	int rocketTowerCount;
	int laserTowerCount;
	int improvedTowerCount;
	int powerTowerCount;

	void reset();

	bool canBuildBaseTower() const;
	bool canBuildFreezeTower() const;
	bool canBuildRocketTower() const;
	bool canBuildPowerTower() const;
	bool canBuildLaserTower() const;
	bool canBuildImprovedTower() const;
};

class Enemy;
class Projectile;
class Tower;

class TowersFactory
{
public:
	static Tower *createTower(TOWER_TYPES type, const sf::Vector2f& pos);
	static bool isIntersects(const sf::FloatRect& rect, const sf::Vector2f& center, float radius);
	constexpr static float UPGRADE_TO_2_COST_MODIFIER = 0.35f;
	constexpr static float UPGRADE_TO_3_COST_MODIFIER = 0.5f;
	static float getAnimationSpeed(TOWER_TYPES type);
	static int getFrameCount(TOWER_TYPES type);
};

class Tower : public GameObject
{
public:
	Tower(const TextureType &texture_id,
		  const sf::Vector2f &pos,
		  const TowerStats& stats);
	virtual ~Tower();

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
	float actualDamage(const ARMOR_TYPE armorType) const;
	float actualAttackSpeed() const;

	static constexpr int ABILITY_LEVEL = 4;
	void setInvulnerable(bool invulnerable);

	bool isInvulnerable() const;

	virtual void levelDown();

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
	~BaseTower() override;

	void projectileAction(Enemy *enemy) override;
	void upgrade() override;
	void levelDown() override;
private:
};

class PowerTower : public Tower
{
public:
	PowerTower(const sf::Vector2f &pos);
	~PowerTower() override;

	void draw(sf::RenderTarget *const target) final;

	bool hasEnergy();
	void setHighlighted(bool isHighlighted);

	void updateGain();

	void upgrade() override;
	void levelDown() override;

	static float getRadius(int level);
	static float getAttackSpeed(int level);

	sf::FloatRect getValidArea() const;

	static const sf::Color POWER_TOWER_AREA_COLOR;
private:
	bool m_isHighlighted;
	sf::RectangleShape powerRect;
	void upgradePowerRect();

	int gainCount;
	void activateBlast();
	float zeroGround;
	class LifeBar* abilityProgress;
	static const sf::Vector2i BLAST_SIZE;
};

class RocketTower : public ProjectilesTower
{
public:
	RocketTower(const sf::Vector2f &pos);
	~RocketTower() override;

	void moveProjectile(Projectile *projectile) override;
	void projectileAction(Enemy *enemy) override;
private:
	float zeroGround;
};

class FreezeTower : public ProjectilesTower
{
public:
	FreezeTower(const sf::Vector2f &pos);
	~FreezeTower() override;

	void projectileAction(Enemy *enemy) override;
private:
	float zeroGround;
};

class LaserTower : public Tower
{
public:
	LaserTower(const sf::Vector2f &pos);
	~LaserTower() override;

	void draw(sf::RenderTarget *const target) final;
	void update() final;

	void shoot(Enemy *target) final;

private:	
	sf::Vertex lineTower;

	struct LaserTarget
	{
		~LaserTarget();
		sf::Vertex lineTarget;
		GameObject *laser;
		Enemy *currentTarget;
		Timer damageTimer;
	};
	std::vector<LaserTarget*> targets;
	LaserTarget mainTarget;

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
	~ImprovedTower() override;

protected:
	void createdProjectile(Projectile *projectile) override;
};

#endif // TOWER_H
