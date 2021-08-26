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
	sprite.setTexture(&GET_TEXTURE(textureID));
}

void ArcSprite::setColor(const sf::Color &color)
{
	sprite.setFillColor(color);
}

sf::Color ArcSprite::color() const
{
	return sprite.getFillColor();
}
