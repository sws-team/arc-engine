#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "stdheader.h"
#include "Game/gameobject.h"

class Projectile : public GameObject
{
public:
	Projectile(const RESOURCES::TEXTURE_TYPE &texture_id,
			   const Vector2f &pos,
			   const Vector2i &frameSize,
			   int frameCount);
	constexpr static Int64 PROJECTILE_GAME_SPEED = 10;

	class Enemy *target;
	void setAngle(float angle);
	float angle() const;

private:
	float m_angle;
};

#endif // PROJECTILE_H