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

	TextureType textureID() const;
	sf::Color color() const;

	void setRotation(float angle) override;
protected:
	void updatePos() override;
	void updateScale() override;
	void updateOrigin() override;
	void updateSize() override;

	sf::RectangleShape sprite;
	TextureType m_textureID = -1;
};

#endif // ARCSPRITE_H
