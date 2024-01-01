#include "arcsprite.h"
#include <ArcEngine>
#include "managers.h"

ArcSprite::ArcSprite(const std::string &name)
	: ArcObject(name)
{
	setType(ArcEngine::SPRITE);
	ColorProperty::setObject(this);
	BorderColorProperty::setObject(this);
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

sf::RectangleShape ArcSprite::hitBox() const
{
	return ArcObject::hitBox(sprite.getPosition(), sprite.getSize());
}

void ArcSprite::setTexture(TextureType textureID)
{
	m_textureID = textureID;
	if (textureID == -1)
		return;
	sprite.setTexture(&Engine::Instance().texturesManager()->getTexture(textureID), true);
}

void ArcSprite::setColor(const sf::Color &color)
{
	ColorProperty::setColor(color);
	sprite.setFillColor(actualColor());
}

void ArcSprite::setBorderColor(const sf::Color &color)
{
	BorderColorProperty::setBorderColor(color);
	sprite.setOutlineColor(actualBorderColor());
}

void ArcSprite::setBorderSize(const float size)
{
	BorderSizeProperty::setBorderSize(size);
	sprite.setOutlineThickness(size);
}

TextureType ArcSprite::textureID() const
{
	return m_textureID;
}


ArcClippedSprite::ArcClippedSprite(const std::string &name)
	: ArcSprite(name)
{

}

void ArcClippedSprite::setTexture(TextureType textureID)
{
	ArcSprite::setTexture(textureID);
	updateClip();
}

void ArcClippedSprite::setClip(const sf::IntRect &rect)
{
	clip = rect;
	updateClip();
}

void ArcClippedSprite::setClip(const sf::Vector2i &pos, const sf::Vector2i &size)
{
	setClip(sf::IntRect(pos, size));
}

void ArcClippedSprite::setClip(int x, int y, int width, int height)
{
	setClip(sf::IntRect(x, y, width, height));
}

void ArcClippedSprite::updateClip()
{
	sprite.setTextureRect(clip);
}

ArcMaskSprite::ArcMaskSprite(const std::string &name)
	: ArcSprite(name)
{

}

void ArcMaskSprite::setMask(TextureType id)
{
	maskID = id;
	updateMask();
}

void ArcMaskSprite::setTexture(TextureType textureID)
{
	ArcSprite::setTexture(textureID);
	updateMask();
}

sf::IntRect ArcMaskSprite::textureRect() const
{
	return m_textureRect;
}

void ArcMaskSprite::setTextureRect(const sf::IntRect &textureRect)
{
	if (m_textureRect == textureRect)
		return;
	m_textureRect = textureRect;
	sprite.setTextureRect(m_textureRect);
}

void ArcMaskSprite::updateMask()
{
	if (maskID == ArcEngine::undefType)
		return;
	sf::Texture &maskTexture = Engine::Instance().texturesManager()->getTexture(maskID);
	sf::Image maskImage = maskTexture.copyToImage();
	maskImage.createMaskFromColor(sf::Color::Black);

	sf::Image img = sprite.getTexture()->copyToImage();
	for (unsigned x = 0; x < maskImage.getSize().x; ++x) {
		for (unsigned y = 0; y < maskImage.getSize().y; ++y) {
			sf::Color color = maskImage.getPixel(x, y);
			if (color != sf::Color::White)
				img.setPixel(x, y, sf::Color::Transparent);
		}
	}

	texture.loadFromImage(img);
	sprite.setTexture(&texture);
}
