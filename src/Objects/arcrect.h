#ifndef ARC_RECT_H
#define ARC_RECT_H

#include "enginedef.h"
#include "arcobject.h"
#include "arcproperties.h"

class ArcRect : public ArcObject,
		public ColorProperty,
		public BorderColorProperty,
		public BorderSizeProperty
{
public:
	ArcRect(const std::string& name);

	void draw(sf::RenderTarget *const target) override;

	void setColor(const sf::Color& color) override;
	void setBorderColor(const sf::Color& color) override;
	void setBorderSize(const float size) override;

protected:
	friend class Intersection;
	void updatePos() override;
	void updateScale() override;
	void updateOrigin() override;
	void updateSize() override;
private:
	sf::RectangleShape rect;
};

#endif // ARC_RECT_H
