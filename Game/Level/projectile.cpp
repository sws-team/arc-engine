#include "projectile.h"

Projectile::Projectile(const RESOURCES::TEXTURE_TYPE& texture_id,
					   const Vector2f &pos,
					   const Vector2i &frameSize,
					   int frameCount):
	GameObject(texture_id, pos, frameSize, frameCount)
{
	m_angle = 0.f;
	animationSpeed = 5;
}

void Projectile::setAngle(float angle)
{
	m_angle = angle;
	sprite.setRotation(angle);
}

float Projectile::angle() const
{
	return m_angle;
}