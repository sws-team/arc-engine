#ifndef LIFEBAR_H
#define LIFEBAR_H

#include "stdheader.h"

class LifeBar
{
public:
	LifeBar();

	void init(const Vector2i &size, const Color& color);
	RectangleShape currentValue;
	RectangleShape fullValue;

	void draw(RenderTarget *target);
	void setPos(const Vector2f& pos);
	Vector2f pos() const;

	void setValue(float rate);

	static constexpr float LIFE_BAR_HEIGHT = 5.f;
private:
	Vector2i fullSize;
};

#endif // LIFEBAR_H
