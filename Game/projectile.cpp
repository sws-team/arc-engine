#include "projectile.h"

Projectile::Projectile(const TextureType& texture_id,
					   const sf::Vector2f &pos,
					   const sf::Vector2i &frameSize,
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
