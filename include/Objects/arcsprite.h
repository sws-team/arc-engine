#ifndef ARCSPRITE_H
#define ARCSPRITE_H

#include "arcobject.h"
#include "arcproperties.h"

class ArcSprite : public ArcObject,
		public ColorProperty,
		public BorderColorProperty,
		public BorderSizeProperty
{
public:
	ArcSprite(const std::string& name);

	void draw(sf::RenderTarget *const target) override;

	virtual void setTexture(TextureType textureID);
	void setColor(const sf::Color& color) override;
	void setBorderColor(const sf::Color& color) override;
	void setBorderSize(const float size) override;

	TextureType textureID() const;

protected:
	friend class Intersection;
	void updatePos() override;
	void updateScale() override;
	void updateOrigin() override;
	void updateSize() override;

	sf::RectangleShape sprite;
	TextureType m_textureID = -1;
};

class ArcClippedSprite : public ArcSprite
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

class ArcMaskSprite : public ArcSprite
{
public:
	ArcMaskSprite(const std::string& name);

	void setMask(TextureType id);

	void setTexture(TextureType textureID) override;

private:
	sf::Texture texture;
	TextureType maskID = ArcEngine::undefType;
	void updateMask();
};

#endif // ARCSPRITE_H
