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

private:
	int m_scales;
	int currentTriangle;

	float m_max;
	float m_min;
	void updateTriangle();
	void updateValue();
	vector<ConvexShape> triangles;
};

#endif // VALUESCALE_H
