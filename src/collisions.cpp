#include "collisions.h"
#include <SFML/Graphics.hpp>
#include <ArcButton>
#include <ArcSprite>
#include <ArcRect>
#include <ArcLine>

bool Intersection::isParallelogramContainsPoint(const sf::Vector2f& point,
												const std::vector<sf::Vector2f>& coords)
{
	if (coords.size() != 4)
		return false;

	const sf::Vector2f A = coords.at(0);
	const sf::Vector2f B = coords.at(1);
	const sf::Vector2f C = coords.at(2);
	const sf::Vector2f D = coords.at(3);

	const float P1 = (A.x - point.x) * (B.y - A.y) - (A.y - point.y) * (B.x - A.x);
	const float P2 = (B.x - point.x) * (C.y - B.y) - (B.y - point.y) * (C.x - B.x);
	const float P3 = (C.x - point.x) * (D.y - C.y) - (C.y - point.y) * (D.x - C.x);
	const float P4 = (D.x - point.x) * (A.y - D.y) - (D.y - point.y) * (A.x - D.x);

	if ((P1 < 0 && P2 < 0 && P3 < 0 && P4 < 0) ||
			(P1 > 0 && P2 > 0 && P3 > 0 && P4 > 0))
		return true;
	return false;
}

bool Intersection::contains(const sf::RectangleShape& rect,
							const sf::Vector2f& pos,
							const sf::Transform& transform)
{
	return contains(rect.getGlobalBounds(), pos, transform);
}

bool Intersection::contains(ArcObject* object, ArcObject* target)
{
	return contains(object->hitBox(), target->scaledGlobalPos(), object->m_transform);
}

bool Intersection::contains(ArcObject* object, const sf::Vector2f& pos)
{
	return contains(object->hitBox(), pos, object->m_transform);
}

bool Intersection::contains(const sf::FloatRect &rect, const sf::Vector2f& pos, const sf::Transform& transform)
{
	const float x = rect.left;
	const float y = rect.top;
	const float w = x + rect.width;
	const float h = y + rect.height;

	const sf::Vector2f p1 = transform.transformPoint(x, y);
	const sf::Vector2f p2 = transform.transformPoint(x, h);
	const sf::Vector2f p3 = transform.transformPoint(w, h);
	const sf::Vector2f p4 = transform.transformPoint(w, y);

	const std::vector<sf::Vector2f> points = {p1, p2, p3, p4};

	return isParallelogramContainsPoint(pos, points);
}

bool Intersection::intersects(const sf::FloatRect& objectRect,
							  const sf::FloatRect& targetRect,
							  const sf::Transform& objectTransform,
							  const sf::Transform& targetTransform)
{
	std::vector<sf::Vector2f> points = {
		targetTransform.transformPoint(targetRect.left, targetRect.top),
		targetTransform.transformPoint(targetRect.left + targetRect.width, targetRect.top),
		targetTransform.transformPoint(targetRect.left + targetRect.width, targetRect.top + targetRect.height),
		targetTransform.transformPoint(targetRect.left, targetRect.top + targetRect.height)
	};
	for(const sf::Vector2f& pos : points) {
		if (contains(objectRect, pos, objectTransform))
			return true;
	}
	return false;
}

bool Intersection::intersects(ArcObject* object, ArcObject* other)
{
	return intersects(object->hitBox(), other->hitBox(), object->m_transform, other->m_transform) ||
			intersects(other->hitBox(), object->hitBox(), other->m_transform, object->m_transform);
}

bool Intersection::intersects(ArcObject* object, const sf::FloatRect& rect)
{
	return intersects(object->hitBox().getGlobalBounds(), rect, object->m_transform);
}

bool Intersection::intersects(const sf::RectangleShape& objectRect,
							  const sf::RectangleShape& targetRect,
							  const sf::Transform& objectTransform,
							  const sf::Transform& targetTransform)
{
	return intersects(objectRect.getGlobalBounds(), targetRect.getGlobalBounds(),
					  objectTransform, targetTransform);
}

sf::Vector2f Intersection::getTranslatedPoint(ArcObject* object, const sf::Vector2f& point)
{
	return object->m_transform.transformPoint(point);
}
