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
			   int frameCount,
			   float angle);
	constexpr static Int64 PROJECTILE_GAME_SPEED = 10;

	float angle;
};

#endif // PROJECTILE_H
