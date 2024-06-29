#include "arcpolygon.h"

ArcPolygon::ArcPolygon(const std::string &name)
	: ArcObject(name)
{
	setType(ArcEngine::POLYGON);
	ColorProperty::setObject(this);
	BorderColorProperty::setObject(this);
}

void ArcPolygon::draw(sf::RenderTarget * const target)
{
	target->draw(polygon, m_transform);
	ArcObject::draw(target);
}

ArcPolygon::SHAPE ArcPolygon::shapeType() const
{
	return m_shapeType;
}

void ArcPolygon::setShapeType(const SHAPE &shapeType)
{
	m_shapeType = shapeType;
	initTransform();
}

void ArcPolygon::initTransform()
{
	ArcObject::initTransform();
	polygon.setPointCount(vertexCount(m_shapeType));
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
	case CUSTOM:
		return customVertexCount;
	case RECTANGLE:
		return 4;
	case CIRCLE:
		return 30;
	case TRIANGLE:
		return 3;
	case HEXAGON:
		return 6;
	case OCTAGON:
		return 8;
	default:
		break;
	}
	return 0;
}

void ArcPolygon::setColor(const sf::Color &color)
{
	ColorProperty::setColor(color);
	polygon.setFillColor(actualColor());
}

void ArcPolygon::setBorderColor(const sf::Color &color)
{
	BorderColorProperty::setBorderColor(color);
	polygon.setOutlineColor(actualBorderColor());
}

void ArcPolygon::setBorderSize(const float size)
{
	BorderSizeProperty::setBorderSize(size);
	polygon.setOutlineThickness(size);
}

void ArcPolygon::setCustomVertexCount(int count)
{
	customVertexCount = count;
}
