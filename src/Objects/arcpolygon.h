#ifndef ARC_POLYGON_H
#define ARC_POLYGON_H

#include <ArcObject>
#include <ArcProperties>

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
		RECTANGLE,
		CUSTOM,
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

	void setCustomVertexCount(int count);

protected:
	void initTransform() override;
	void updatePos() override;
	void updateScale() override;
	void updateOrigin() override;
	void updateSize() override;

private:
	sf::CircleShape polygon;
	SHAPE m_shapeType = RECTANGLE;
	int customVertexCount = 0;
	unsigned vertexCount(SHAPE shapeType);
};

#endif // ARC_POLYGON_H
