#ifndef LIFEBAR_H
#define LIFEBAR_H

#include "stdheader.h"

class LifeBar
{
public:
	LifeBar();

	void init(const Vector2f& size, const Color& color);
	RectangleShape currentValue;
	RectangleShape fullValue;

	void draw(RenderWindow *window);
	void setPos(const Vector2f& pos);

	void setValue(float rate);

private:
	Vector2f fullSize;
};

#endif // LIFEBAR_H
