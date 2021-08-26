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
	void setPos(float x, float y) override;
	void setOrigin(float x, float y) override;
	void setScale(float x, float y) override;
	void setSize(float x, float y) override;

	sf::RectangleShape sprite;
	TextureType m_textureID;
};

#endif // ARCSPRITE_H
