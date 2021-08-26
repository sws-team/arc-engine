#include "arcsprite.h"
#include "engine.h"
#include "managers.h"

ArcSprite::ArcSprite(const std::string &name)
	: ArcObject(name, ArcEngine::SPRITE)
{

}

void ArcSprite::draw(sf::RenderTarget * const target)
{
	ArcObject::draw(target);
	target->draw(sprite);
}

void ArcSprite::setPos(float x, float y)
{
	ArcObject::setPos(x, y);
	sprite.setPosition(x, y);
}

void ArcSprite::setOrigin(float x, float y)
{
	ArcObject::setOrigin(x, y);
	sprite.setOrigin(x, y);
}

void ArcSprite::setScale(float x, float y)
{
	ArcObject::setScale(x, y);
	sprite.setScale(x, y);
}

void ArcSprite::setSize(float x, float y)
{
	ArcObject::setSize(x, y);
	sprite.setSize(sf::Vector2f(x, y));
}

void ArcSprite::setTexture(TextureType textureID)
{
	m_textureID = textureID;
	sprite.setTexture(&GET_TEXTURE(textureID));
}

void ArcSprite::setColor(const sf::Color &color)
{
	sprite.setFillColor(color);
}

TextureType ArcSprite::textureID() const
{
	return m_textureID;
}

sf::Color ArcSprite::color() const
{
	return sprite.getFillColor();
}

void ArcSprite::setRotation(float angle)
{
	ArcObject::setRotation(angle);
	sprite.setRotation(angle);
}
