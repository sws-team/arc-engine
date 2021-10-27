#ifndef ARC_POLYGON_H
#define ARC_POLYGON_H

#include "enginedef.h"
#include "arcobject.h"

class ArcPolygon : public ArcObject
{
public:
	ArcPolygon(const std::string& name);

	void draw(sf::RenderTarget *const target) override;

	enum SHAPE
	{
		SQUARE,
		CIRCLE,
		TRIANGLE,
		HEXAGON,
		OCTAGON,
	};

	SHAPE shapeType() const;
	void setShapeType(const ArcPolygon::SHAPE &shapeType);

	void setColor(const sf::Color& color);
	void setBorderColor(const sf::Color& color);
	void setBorderSize(const float size);

	sf::Color color() const;
	sf::Color borderColor() const;
	float borderSize() const;

	void init() override;
protected:
	void updatePos() override;
	void updateScale() override;
	void updateOrigin() override;
	void updateSize() override;
private:
	sf::CircleShape polygon;
	SHAPE m_shapeType = SQUARE;
	static unsigned vertexCount(SHAPE shapeType);
};

#endif // ARC_POLYGON_H
