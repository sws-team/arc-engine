#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "stdheader.h"
#include "gameobject.h"

class Projectile : public GameObject
{
public:
	Projectile(const TextureType &texture_id,
			   const sf::Vector2f &pos,
			   const sf::Vector2i &frameSize,
			   int frameCount);
	constexpr static sf::Int64 PROJECTILE_GAME_SPEED = 10;

	class Enemy *target;
	void setAngle(float angle);
	float angle() const;

private:
	float m_angle;
};

#endif // PROJECTILE_H
