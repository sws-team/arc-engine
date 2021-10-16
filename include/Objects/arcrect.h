#ifndef ARC_RECT_H
#define ARC_RECT_H

#include "enginedef.h"
#include "arcobject.h"

class ArcRect : public ArcObject
{
public:
	ArcRect(const std::string& name);

	void draw(sf::RenderTarget *const target) override;

	void setColor(const sf::Color& color);
	void setBorderColor(const sf::Color& color);
	void setBorderSize(const float size);

	sf::Color color() const;
	sf::Color borderColor() const;
	float borderSize() const;

	void setRotation(float angle) override;
protected:
	void updatePos() override;
	void updateScale() override;
	void updateOrigin() override;
	void updateSize() override;
private:
	sf::RectangleShape rect;
};

#endif // ARC_RECT_H
