#ifndef ARCSPRITE_H
#define ARCSPRITE_H

#include "enginedef.h"
#include "arcobject.h"

class ArcSprite : public ArcObject
{
public:
	ArcSprite(const std::string& name);

	void draw(sf::RenderTarget *const target) override;

	void setPos(float x, float y) override;
	void setOrigin(float x, float y) override;
	void setScale(float x, float y) override;
	void setSize(float x, float y) override;

	void setTexture(TextureType textureID);
	void setColor(const sf::Color& color);

	sf::Color color() const;

protected:
	sf::RectangleShape sprite;
};

#endif // ARCSPRITE_H
