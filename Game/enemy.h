#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.h"
#include "enginedef.h"
#include "Balance/balance.h"

class EnemyAbility;

class Enemy : public GameObject
{
public:
	Enemy(const TextureType& texture_id,
		  const sf::Vector2f& startPos,
		  const EnemyStats &stats,
		  const sf::Vector2i &cellSize,
		  const float frameCount,
		  const float animationSpeed);
	~Enemy() override;

	void setAbility(EnemyAbility *ability);

	EnemyStats getData() const;
	EnemyStats getPureStats() const;

	void moveEnemy();

	void moveNext(int direction);

	void update() override;	
	void draw(sf::RenderTarget *const target) override;

	void hit(float damage);

	bool isAlive() const;

	void useAbility();
	void freeze(float k, int duration);
	void heal(float health);
	void protect(float shell, bool show = true);
	void setReflection(const float reflection);

	sf::Vector2f enemyPos() const;
	sf::Vector2f enemyCenter() const;

	void setStopped(bool stopped);
	bool isStopped() const;

	bool isVisible() const;
	void setVisible(bool visible);

	void startBurn();

	static constexpr float ENEMY_SCALE = 3.f;

	sf::Vector2f getEnemySize() const;

	sf::FloatRect enemyRect() const;

	ENEMY_TYPES type() const;
	void setType(const ENEMY_TYPES &type);

	sf::Vector2i getLastCell() const;
	void setLastCell(const sf::Vector2i &lastCell);

	bool getLastUp() const;

private:
	ENEMY_TYPES m_type;
	EnemyStats m_stats;
	EnemyStats m_data;

	constexpr static int RIGHT_ANGLE = 90;

	class LifeBar *lifeBar;
	void drawLifeBar(sf::RenderTarget *target);

	//move
	Timer moveTimer;
	int currentDirection;

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

	sf::Vector2f m_size;

	sf::Vector2f m_spritePos;//enemy pos in rect
	sf::Vector2f m_pos;//global pos

	constexpr static float LIFEBAR_OFFSET = 4;
	EnemyAbility *ability;
	bool m_stopped;
	bool m_visible;
	bool m_burned;
	Timer burnTimer;
	Timer burnAttack;
	GameObject *burnAnimation;
public:
	sf::Vector2f actualMoveStep() const;
private:
	sf::Vector2f moveStep;
	sf::Vector2i m_lastCell;

	bool lastUp;
};

class EnemyAbility : public GameDrawable
{
public:
	EnemyAbility(float msec);
	virtual void draw(sf::RenderTarget *const target) override;
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
};

class ShellNearAbility : public AreaAbility
{
public:
	ShellNearAbility();
protected:
	void use() override;
};

class SelfHealAbility : public EnemyAbility
{
public:
	SelfHealAbility();
protected:
	void use() override;
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
	constexpr static int TELEPORT_DISAPPEAR_ROW = 1;
	constexpr static int TELEPORT_APPEAR_ROW = 2;
};

class TowerEffectAbility : public EnemyAbility
{
public:
	TowerEffectAbility(float msec);
	void draw(sf::RenderTarget *const target) override;
	void update() override;
protected:
	virtual void effect(bool isActive) = 0;
	void use() override;
	class Tower *targetTower;

	struct AbilityInfo
	{
		TextureType enemyTextureId;
		sf::Vector2i animationSize;
		TextureType pojectileTextureId;
		sf::Vector2i projectileSize;
		float duration;
		SoundType catchSound;
		float cells;
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
	void getBack();
	float reflection;
	constexpr static float REFLECTION_MODIFIER = 0.5f;
};

class ShutdownTowerAbility : public TowerEffectAbility
{
public:
	ShutdownTowerAbility();
	~ShutdownTowerAbility();

protected:
	void effect(bool isActive) override;
};

class DownTowerAbility : public TowerEffectAbility
{
public:
	DownTowerAbility();
	~DownTowerAbility();

protected:
	void effect(bool isActive) override;
};

class SpawnEnemy : public EnemyAbility
{
public:
	SpawnEnemy();
protected:
	void use() override;
private:
	enum STATES
	{
		READY,
		SPAWN,
		WAIT_SPAWN,
		FINISHED,
	};
	STATES m_state;
	constexpr static float SPAWN_INTERVAL = 700;
	int currentSpawnCount;
	int spawnCount;
	constexpr static float ACTIVATE_SPAWN_ROW = 1;
	constexpr static float DEACTIVATE_SPAWN_ROW = 2;
};

class StrongAbility : public EnemyAbility
{
public:
	StrongAbility();

protected:
	void use() override;
};

class RageAbility : public EnemyAbility
{
public:
	RageAbility();

protected:
	void use() override;
	constexpr static float RAGE_INTERVAL = 100;
};


class EnemiesFactory
{
public:

	struct EnemyInfo
	{
		enum ABILITY_TYPE
		{
			NONE,
			RAGE,
			SPAWN,
			TELEPORT,
			SHELL_NEAR,
			HEAL_NEAR,
			SHUTDOWN_TOWER,
			STRONG,
			DOWN_TOWER,
			SELF_HEAL,
		};

		EnemyStats stats;
		sf::Vector2i size;
		ABILITY_TYPE abilityType;
		TextureType texture_id;
		float animationSpeed;
		float frameCount;
	};

	static Enemy *createEnemy(ENEMY_TYPES type, const sf::Vector2f &startPos);
	static std::vector<Wave> generateEnemies(unsigned int n);
	static EnemyInfo getEnemyInfo(ENEMY_TYPES type);
private:
	static Wave createWave(float protection, float time, const std::map<ENEMY_TYPES, int>& enemies);
};

#endif // ENEMY_H
