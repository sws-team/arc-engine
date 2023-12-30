#include "arcline.h"
#include <arcengineutils.h>

ArcLine::ArcLine(const std::string &name)
	: ArcObject(name)
{
	setType(ArcEngine::LINE);
	ColorProperty::setObject(this);
}

void ArcLine::setFirstPoint(const sf::Vector2f &point)
{
	m_objects = false;
	if (m_firstPoint == point)
		return;
	m_firstPoint = point;
	updateTarget();
}

void ArcLine::setFirstPoint(float x, float y)
{
	setFirstPoint(sf::Vector2f(x, y));
}

void ArcLine::setFirstX(float x)
{
	setFirstPoint(x, m_firstPoint.y);
}

void ArcLine::setFirstY(float y)
{
	setFirstPoint(m_firstPoint.x, y);
}

void ArcLine::setSecondPoint(const sf::Vector2f &point)
{
	m_objects = false;
	if (m_secondPoint == point)
		return;
	m_secondPoint = point;
	updateTarget();
}

void ArcLine::setSecondPoint(float x, float y)
{
	setSecondPoint(sf::Vector2f(x, y));
}

void ArcLine::setSecondX(float x)
{
	setFirstPoint(x, m_secondPoint.y);
}

void ArcLine::setSecondY(float y)
{
	setFirstPoint(m_secondPoint.x, y);
}

void ArcLine::setLineWidth(float lineWidth)
{
	m_lineWidth = lineWidth;
	line.setOutlineThickness(lineWidth);
}

void ArcLine::setObjects(ArcObject *object, ArcObject *target)
{
	if (object == nullptr || target == nullptr) {
		m_objects = false;
		return;
	}
	m_objects = true;
	this->object = object;
	this->target = target;
	updateTarget();
}

void ArcLine::setObjects(const std::string& object, const std::string& target)
{
	ArcObject* obj = this->parent()->findChild(object, false);
	ArcObject* tar = this->parent()->findChild(target, false);
	setObjects(obj, tar);
}

float ArcLine::lineWidth() const
{
	return m_lineWidth;
}

sf::Vector2f ArcLine::firstPoint() const
{
	return m_firstPoint;
}

sf::Vector2f ArcLine::secondPoint() const
{
	return m_secondPoint;
}

void ArcLine::draw(sf::RenderTarget * const target)
{
	target->draw(line, m_transform);
	ArcObject::draw(target);
}

void ArcLine::update()
{
	if(m_objects) {
		const sf::Vector2f lastFirstPoint = object->pos();
		const sf::Vector2f lastSecondPoint= target->pos();
		if (lastFirstPoint != m_firstPoint || lastSecondPoint != m_secondPoint) {
			updateTarget();
		}
	}
	ArcObject::update();
}

void ArcLine::setColor(const sf::Color &color)
{
	ColorProperty::setColor(color);
	line.setFillColor(actualColor());
	line.setOutlineColor(actualColor());
}

void ArcLine::updateScale()
{
	line.setScale(scaledGlobalScale());
	ArcObject::updateScale();
}

sf::RectangleShape ArcLine::hitBox() const
{
	return line;
}

void ArcLine::updateTarget()
{
	if (m_objects) {
		m_firstPoint = object->pos();
		m_secondPoint = target->pos();
	}
	line.setPosition(scaledGlobalPos(m_firstPoint));

	const float angle = ArcEngine::getAngle(m_firstPoint, m_secondPoint);
	line.setRotation(angle);

	sf::Vector2f objectPos = m_firstPoint;
	if (float dif = m_secondPoint.x - m_firstPoint.x; dif < 0) {
		objectPos += sf::Vector2f(dif, 0);
	}
	if (float dif = m_secondPoint.y - m_firstPoint.y; dif < 0) {
		objectPos += sf::Vector2f(0, dif);
	}
	setPos(objectPos);

	float w = std::fabs(m_secondPoint.x - m_firstPoint.x);
	float h = std::fabs(m_secondPoint.y - m_firstPoint.y);
	w = std::max(w, m_lineWidth);
	h = std::max(h, m_lineWidth);
	setSize(w, h);

	const float lineLength = std::sqrt(std::pow(w, 2) + std::pow(h, 2));
	line.setSize(sf::Vector2f(lineLength, m_lineWidth));
}
