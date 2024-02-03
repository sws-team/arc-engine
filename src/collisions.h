#ifndef COLLISION_H
#define COLLISION_H

#include "enginedef.h"

class Intersection
{
public:
	static bool contains(ArcObject* object, const sf::Vector2f& pos);
	static bool contains(ArcObject* object, ArcObject* target);

	static bool contains(const sf::RectangleShape& rect,
						 const sf::Vector2f& pos,
						 const sf::Transform& transform = sf::Transform());

	static bool contains(const sf::FloatRect &rect,
						 const sf::Vector2f& pos,
						 const sf::Transform& transform = sf::Transform());


	static bool intersects(ArcObject* object, ArcObject* other);
	static bool intersects(ArcObject* object, const sf::FloatRect& rect);

	static bool intersects(const sf::RectangleShape& objectRect,
						   const sf::RectangleShape& targetRect,
						   const sf::Transform& objectTransform = sf::Transform(),
						   const sf::Transform& targetTransform = sf::Transform());

	static bool intersects(const sf::FloatRect& objectRect,
						   const sf::FloatRect& targetRect,
						   const sf::Transform& objectTransform = sf::Transform(),
						   const sf::Transform& targetTransform = sf::Transform());

	static sf::Vector2f getTranslatedPoint(ArcObject* object, const sf::Vector2f& point);

private:
	static bool isParallelogramContainsPoint(const sf::Vector2f& point,
									  const std::vector<sf::Vector2f>& coords);
};

#endif
