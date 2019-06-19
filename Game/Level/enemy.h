#ifndef ENEMY_H
#define ENEMY_H

#include "Game/gameobject.h"

struct EnemyStats
{
	float speed;
	float health;
	float damage;
};

class Enemy : public GameObject
{
public:
	Enemy(const RESOURCES::TEXTURE_TYPE& texture_id,
		  const Vector2f& startPos,
		  const EnemyStats &stats);

	EnemyStats getStats() const;

	bool moveStep();
	void moveNext(int direction);

	void update() override;

private:
	EnemyStats m_stats;

	constexpr static int RIGHT_ANGLE = 90;

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
