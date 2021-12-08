#include "arcrect.h"

ArcRect::ArcRect(const std::string &name)
	: ArcObject(name)
{
	setType(ArcEngine::RECT);
	ColorProperty::setObject(this);
	BorderColorProperty::setObject(this);
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
	ColorProperty::setColor(color);
	rect.setFillColor(actualColor());
}

void ArcRect::setBorderColor(const sf::Color &color)
{
	BorderColorProperty::setBorderColor(color);
	rect.setOutlineColor(actualBorderColor());
}

void ArcRect::setBorderSize(const float size)
{
	BorderSizeProperty::setBorderSize(size);
	rect.setOutlineThickness(size);
}
