#ifndef ENEMY_H
#define ENEMY_H

#include "Game/gameobject.h"

struct EnemyStats
{
	float speed;
	float health;
	float damage;
	float reflection;
};

class Enemy : public GameObject
{
public:
	Enemy(const RESOURCES::TEXTURE_TYPE& texture_id,
		  const Vector2f& startPos,
		  const EnemyStats &stats);
	~Enemy() override;

	EnemyStats getData() const;

	bool moveStep();
	void moveNext(int direction);

	void update() override;	
	void draw(RenderTarget *const target) override;

	void hit(float damage);

	bool isAlive() const;

	void freeze(float k, int duration);

	Vector2f enemyPos() const;
	Vector2f getOriginalPos() const;

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

	Vector2f offset;
	Vector2f movePos;
	Vector2f centerOffset;
	void moveEnemy(const Vector2f& d);
};

class EnemiesFactory
{
public:
	enum TYPES
	{
		UNKNOWN,

		SMALL_SLOW,
		SMALL_MEDIUM,
		SMALL_FAST,

		MID_SLOW,
		MID_MEDIUM,
		MID_FAST,

		BIG_SLOW,
		BIG_MEDIUM,
		BIG_FAST,

		VERY_FAST,
		VERY_BIG
	};

	static Enemy *createEnemy(TYPES type, const Vector2f &startPos);
};

#endif // ENEMY_H
