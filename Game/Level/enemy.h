#ifndef ENEMY_H
#define ENEMY_H

#include "Game/gameobject.h"
#include "Game/leveldef.h"

struct EnemyStats
{
	float speed;
	float health;
	float damage;
	float reflection;
};

class EnemyAbility;

class Enemy : public GameObject
{
public:
	Enemy(const RESOURCES::TEXTURE_TYPE& texture_id,
		  const Vector2f& startPos,
		  const EnemyStats &stats,
		  const Vector2i &cellSize);
	~Enemy() override;

	void setAbility(EnemyAbility *ability);

	EnemyStats getData() const;
	EnemyStats getPureStats() const;

	bool moveStep();
	void moveNext(int direction);

	void update() override;	
	void draw(RenderTarget *const target) override;

	void hit(float damage);

	bool isAlive() const;

	void useAbility();
	void freeze(float k, int duration);
	void heal(float health);
	void protect(float shell, bool show = true);

	Vector2f enemyPos() const;
	Vector2f enemyCenter() const;

	void setStopped(bool stopped);
	bool isStopped() const;

	bool isVisible() const;
	void setVisible(bool visible);

private:
	EnemyStats m_stats;
	EnemyStats m_data;

	constexpr static int RIGHT_ANGLE = 90;

	class LifeBar *lifeBar;
	void drawLifeBar(RenderTarget *target);

	//move
	int moveCounter;
	Vector2f currentStep;
	int currentDirection;
	Vector2f targetPos;

	enum SPRITE_DIRECTION
	{
		DEFAULT_DOWN,
		SPRITE_LEFT,
		SPRITE_UP,
		SPRITE_RIGHT,
	};
	SPRITE_DIRECTION spriteDirection;

	Timer freezeTimer;
	int freezeDuration;
	float freezeK;
	bool isFreezed;
	bool startFreeze;
	void moveEnemy(const Vector2f& d);
	Vector2f m_size;

	Vector2f m_spritePos;//enemy pos in rect
	Vector2f m_pos;//global pos

	constexpr static float LIFEBAR_OFFSET = 4;
	EnemyAbility *ability;
	bool m_stopped;
	bool m_visible;
};

class EnemyAbility : public GameDrawable
{
public:
	EnemyAbility(float msec);
	virtual void draw(RenderTarget *const target) override;
	void update() override;

	void setOwner(Enemy *owner);

protected:
	virtual void use() = 0;
	Enemy *owner;
	Timer abilityTimer;
	float m_interval;
	float m_abilityInterval;
};

class AreaAbility : public EnemyAbility
{
public:
	AreaAbility(float msec);

protected:
	bool inArea(Enemy *enemy) const;
	float area;
};

class HealNearAbility : public AreaAbility
{
public:
	HealNearAbility();
protected:
	void use() override;
	constexpr static float HEAL_INTERVAL = 3000;
};

class ShellNearAbility : public AreaAbility
{
public:
	ShellNearAbility();
protected:
	void use() override;
	constexpr static float SHELL_INTERVAL = 10000;
};

class SelfHealAbility : public EnemyAbility
{
public:
	SelfHealAbility();
protected:
	void use() override;
	constexpr static float SELF_HEAL_INTERVAL = 2500;
};

class TeleportAbility : public EnemyAbility
{
public:
	TeleportAbility();
protected:
	void use() override;
private:
	enum STATES
	{
		READY,
		DISAPPEAR,
		APPEAR,
		FINISH,
	};
	STATES m_state;
	constexpr static float TELEPORT_INTERVAL = 5000;
	constexpr static int TELEPORT_DISAPPEAR_ROW = 1;
	constexpr static int TELEPORT_APPEAR_ROW = 2;
};

class TowerEffectAbility : public EnemyAbility
{
public:
	TowerEffectAbility();
	void draw(RenderTarget *const target) override;
	void update() override;
protected:
	virtual void effect(bool isActive) = 0;
	void use() override;
	class Tower *targetTower;

	struct AbilityInfo
	{
		RESOURCES::TEXTURE_TYPE enemyTextureId;
		Vector2i animationSize;
		RESOURCES::TEXTURE_TYPE pojectileTextureId;
		Vector2i projectileSize;
		float duration;
	};
	AbilityInfo info;
private:
	enum STATES
	{
		READY,
		SHOOT,
		MOVE,
		WAIT,
		FINISHED,
	};
	STATES m_state;
	Timer moveTimer;
	GameObject *projectile;
	float m_angle;
	constexpr static float SHUTDOWN_INTERVAL = 5000;
	void getBack();
};

class ShutdownTowerAbility : public TowerEffectAbility
{
public:
	ShutdownTowerAbility();

	constexpr static float TOWER_DISABLED_DURATION = 10000;
protected:
	void effect(bool isActive) override;
};

class DownTowerAbility : public TowerEffectAbility
{
public:
	DownTowerAbility();
	constexpr static float DOWNGRADE_VALUE = 0.3f;
	constexpr static float TOWER_DOWNGRADED_DURATION = 9000;
protected:
	void effect(bool isActive) override;
};

class EnemiesFactory
{
public:
	static Enemy *createEnemy(ENEMY_TYPES type, const Vector2f &startPos);
	static vector<Wave> generateEnemies(unsigned int n);
private:
	static Wave createWave(float protection, float time, const map<ENEMY_TYPES, int>& enemies);
};

#endif // ENEMY_H
