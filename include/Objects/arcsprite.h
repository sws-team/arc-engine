#ifndef ARCSPRITE_H
#define ARCSPRITE_H

#include "enginedef.h"
#include "arcobject.h"

class ArcSprite : public ArcObject
{
public:
	ArcSprite(const std::string& name);

	void draw(sf::RenderTarget *const target) override;

	virtual void setTexture(TextureType textureID);
	void setColor(const sf::Color& color);
	void setBorderColor(const sf::Color& color);
	void setBorderSize(const float size);

	TextureType textureID() const;
	sf::Color color() const;
	sf::Color borderColor() const;
	float borderSize() const;

protected:
	void updatePos() override;
	void updateScale() override;
	void updateOrigin() override;
	void updateSize() override;

	sf::RectangleShape sprite;
	TextureType m_textureID = -1;
};

class ArcClippedSprite : ArcSprite
{
public:
	ArcClippedSprite(const std::string& name);
	void setTexture(TextureType textureID) override;

	void setClip(const sf::IntRect& rect);
	void setClip(const sf::Vector2i& pos, const sf::Vector2i& size);
	void setClip(float x, float y, float width, float height);

private:
	sf::IntRect clip;
	void updateClip();
};

#endif // ARCSPRITE_H
