#ifndef ARCLINE_H
#define ARCLINE_H

#include <ArcObject>
#include <ArcProperties>

class ArcLine : public ArcObject,
		public ColorProperty
{
public:
	ArcLine(const std::string& name);

	void setFirstPoint(const sf::Vector2f& point);
	void setFirstPoint(float x, float y);
	void setFirstX(float x);
	void setFirstY(float y);

	void setSecondPoint(const sf::Vector2f& point);
	void setSecondPoint(float x, float y);
	void setSecondX(float x);
	void setSecondY(float y);

	void setLineWidth(float lineWidth);

	void setObjects(ArcObject* object, ArcObject *target);
	void setObjects(const std::string& object, const std::string& target);

	float lineWidth() const;
	sf::Vector2f firstPoint() const;
	sf::Vector2f secondPoint() const;

	void draw(sf::RenderTarget *const target) override;
	void update() override;

	void setColor(const sf::Color& color) override;

	void updateTarget();

protected:
	void updateScale() override;

private:
	sf::RectangleShape line;
	float m_lineWidth = 1.f;
	sf::Vector2f m_firstPoint;
	sf::Vector2f m_secondPoint;
	bool m_objects = false;
	ArcObject* object = nullptr;
	ArcObject* target = nullptr;
};

#endif // ARCLINE_H
