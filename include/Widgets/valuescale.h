#ifndef VALUESCALE_H
#define VALUESCALE_H

#include "widget.h"

class ValueScale : public Widget
{
public:
	ValueScale();

	void draw(sf::RenderTarget *target) override;
	void event(sf::Event *event) override;
	void update() override;

	void setScalesCount(int scales);
	void setRange(float min, float max);
	void setMinValue(float min);
	void setMaxValue(float max);

	float value() const;
	void setValue(float value);

	void setColorActive(const sf::Color &colorActive);
	void setColorInactive(const sf::Color &colorInactive);

private:
	int m_scales;
	int currentTriangle;

	float m_max;
	float m_min;
	void updateTriangle();
	void updateValue();
	std::vector<sf::ConvexShape> triangles;

	sf::Color m_colorActive;
	sf::Color m_colorInactive;

	int getCurrentTriangle(const sf::Vector2f& pos);
};

#endif // VALUESCALE_H
