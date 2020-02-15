#ifndef VALUESCALE_H
#define VALUESCALE_H

#include "widget.h"

class ValueScale : public Widget
{
public:
	ValueScale();

	void draw(RenderTarget *target) override;
	void event(Event *event) override;
	void update() override;

	void setScalesCount(int scales);
	void setRange(float min, float max);
	void setMinValue(float min);
	void setMaxValue(float max);

	float value() const;
	void setValue(float value);

	void setColorActive(const Color &colorActive);
	void setColorInactive(const Color &colorInactive);

private:
	int m_scales;
	int currentTriangle;

	float m_max;
	float m_min;
	void updateTriangle();
	void updateValue();
	vector<ConvexShape> triangles;

	Color m_colorActive;
	Color m_colorInactive;

	int getCurrentTriangle(const Vector2f& pos);
};

#endif // VALUESCALE_H
