#ifndef ARC_POLYGON_H
#define ARC_POLYGON_H

#include "enginedef.h"
#include "arcobject.h"
#include "arcproperties.h"

class ArcPolygon : public ArcObject,
		public ColorProperty,
		public BorderColorProperty,
		public BorderSizeProperty
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

	void setColor(const sf::Color& color) override;
	void setBorderColor(const sf::Color& color) override;
	void setBorderSize(const float size) override;

protected:
	void initTransform() override;
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
