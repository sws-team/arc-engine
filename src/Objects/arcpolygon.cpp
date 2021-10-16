#include "arcpolygon.h"

ArcPolygon::ArcPolygon(const std::string &name)
	: ArcObject(name)
{
	setType(ArcEngine::POLYGON);
}

void ArcPolygon::draw(sf::RenderTarget * const target)
{
	target->draw(polygon);
	ArcObject::draw(target);
}

ArcPolygon::SHAPE ArcPolygon::shapeType() const
{
	return m_shapeType;
}

void ArcPolygon::setShapeType(const SHAPE &shapeType)
{
	m_shapeType = shapeType;
	init();
}

void ArcPolygon::init()
{
	ArcObject::init();
	polygon.setPointCount(vertexCount(m_shapeType));
}

void ArcPolygon::setRotation(float angle)
{
	ArcObject::setRotation(angle);
	polygon.setRotation(angle);
}

void ArcPolygon::updatePos()
{
	polygon.setPosition(scaledGlobalPos());
	ArcObject::updatePos();
}

void ArcPolygon::updateScale()
{
	polygon.setScale(scaledGlobalScale());
	ArcObject::updateScale();
}

void ArcPolygon::updateOrigin()
{
	polygon.setOrigin(globalOrigin());
	ArcObject::updateOrigin();
}

void ArcPolygon::updateSize()
{
	polygon.setRadius(width() / 2.f);
	ArcObject::updateSize();
}

unsigned ArcPolygon::vertexCount(ArcPolygon::SHAPE shapeType)
{
	switch (shapeType)
	{
	case SQUARE:
		return 4;
		break;
	case CIRCLE:
		return 0;
		break;
	case TRIANGLE:
		return 3;
		break;
	case HEXAGON:
		return 6;
		break;
	case OCTAGON:
		return 8;
		break;
	default:
		break;
	}
	return 0;
}

void ArcPolygon::setColor(const sf::Color &color)
{
	polygon.setFillColor(color);
}

void ArcPolygon::setBorderColor(const sf::Color &color)
{
	polygon.setOutlineColor(color);
}

void ArcPolygon::setBorderSize(const float size)
{
	polygon.setOutlineThickness(size);
}

sf::Color ArcPolygon::color() const
{
	return polygon.getFillColor();
}

sf::Color ArcPolygon::borderColor() const
{
	return polygon.getOutlineColor();
}

float ArcPolygon::borderSize() const
{
	return polygon.getOutlineThickness();
}
