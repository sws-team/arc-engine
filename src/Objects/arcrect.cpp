#include "arcrect.h"

ArcRect::ArcRect(const std::string &name)
	: ArcObject(name)
{
	setType(ArcEngine::RECT);
}

void ArcRect::setRotation(float angle)
{
	ArcObject::setRotation(angle);
	polygon.setRotation(angle);
}

void ArcRect::updatePos()
{
	polygon.setPosition(scaledGlobalPos());
	ArcObject::updatePos();
}

void ArcRect::updateScale()
{
	polygon.setScale(scaledGlobalScale());
	ArcObject::updateScale();
}

void ArcRect::updateOrigin()
{
	polygon.setOrigin(globalOrigin());
	ArcObject::updateOrigin();
}

void ArcRect::updateSize()
{
	polygon.setSize(size());
	ArcObject::updateSize();
}

void ArcRect::setColor(const sf::Color &color)
{
	polygon.setFillColor(color);
}

void ArcRect::setBorderColor(const sf::Color &color)
{
	polygon.setOutlineColor(color);
}

void ArcRect::setBorderSize(const float size)
{
	polygon.setOutlineThickness(size);
}

sf::Color ArcRect::color() const
{
	return polygon.getFillColor();
}

sf::Color ArcRect::borderColor() const
{
	return polygon.getOutlineColor();
}

float ArcRect::borderSize() const
{
	return polygon.getOutlineThickness();
}
