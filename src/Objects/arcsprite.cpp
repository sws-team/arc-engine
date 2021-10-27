#include "arcsprite.h"
#include "engine.h"
#include "managers.h"

ArcSprite::ArcSprite(const std::string &name)
	: ArcObject(name)
{
	setType(ArcEngine::SPRITE);
}

void ArcSprite::draw(sf::RenderTarget * const target)
{
	target->draw(sprite, m_transform);
	ArcObject::draw(target);
}

void ArcSprite::updatePos()
{
	sprite.setPosition(scaledGlobalPos());
	ArcObject::updatePos();
}

void ArcSprite::updateScale()
{
	sprite.setScale(scaledGlobalScale());
	ArcObject::updateScale();
}

void ArcSprite::updateOrigin()
{
	sprite.setOrigin(globalOrigin());
	ArcObject::updateOrigin();
}

void ArcSprite::updateSize()
{
	sprite.setSize(size());
	ArcObject::updateSize();
}

void ArcSprite::setTexture(TextureType textureID)
{
	m_textureID = textureID;
	if (textureID == -1)
		return;
	sprite.setTexture(&GET_TEXTURE(textureID));
}

void ArcSprite::setColor(const sf::Color &color)
{
	sprite.setFillColor(color);
}

void ArcSprite::setBorderColor(const sf::Color &color)
{
	sprite.setOutlineColor(color);
}

void ArcSprite::setBorderSize(const float size)
{
	sprite.setOutlineThickness(size);
}

TextureType ArcSprite::textureID() const
{
	return m_textureID;
}

sf::Color ArcSprite::color() const
{
	return sprite.getFillColor();
}

sf::Color ArcSprite::borderColor() const
{
	return sprite.getOutlineColor();
}

float ArcSprite::borderSize() const
{
	return sprite.getOutlineThickness();
}

