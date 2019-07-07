#include "projectile.h"

Projectile::Projectile(const RESOURCES::TEXTURE_TYPE& texture_id,
					   const Vector2f &pos,
					   const Vector2i &frameSize,
					   int frameCount,
					   float angle):
	GameObject(texture_id, pos, frameSize, frameCount)
  ,angle(angle)
{
	sprite.setRotation(angle);
	animationSpeed = 5;
}
