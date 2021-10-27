#include "arcrect.h"

ArcRect::ArcRect(const std::string &name)
	: ArcObject(name)
{
	setType(ArcEngine::RECT);
}

void ArcRect::draw(sf::RenderTarget * const target)
{
	target->draw(rect, m_transform);
	ArcObject::draw(target);
}

void ArcRect::updatePos()
{
	rect.setPosition(scaledGlobalPos());
	ArcObject::updatePos();
}

void ArcRect::updateScale()
{
	rect.setScale(scaledGlobalScale());
	ArcObject::updateScale();
}

void ArcRect::updateOrigin()
{
	rect.setOrigin(globalOrigin());
	ArcObject::updateOrigin();
}

void ArcRect::updateSize()
{
	rect.setSize(size());
	ArcObject::updateSize();
}

void ArcRect::setColor(const sf::Color &color)
{
	rect.setFillColor(color);
}

void ArcRect::setBorderColor(const sf::Color &color)
{
	rect.setOutlineColor(color);
}

void ArcRect::setBorderSize(const float size)
{
	rect.setOutlineThickness(size);
}

sf::Color ArcRect::color() const
{
	return rect.getFillColor();
}

sf::Color ArcRect::borderColor() const
{
	return rect.getOutlineColor();
}

float ArcRect::borderSize() const
{
	return rect.getOutlineThickness();
}
