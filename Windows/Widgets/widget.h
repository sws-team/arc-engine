#ifndef WIDGET_H
#define WIDGET_H

#include "stdheader.h"

class Widget
{
public:
	Widget();
	virtual ~Widget();

	virtual void draw(RenderTarget *target);
	virtual void event(Event *event);
	virtual void update();

	void setPos(const Vector2f& pos);
	void setPos(float x, float y);
	void setSize(const Vector2f& size);
	void setSize(float x, float y);

protected:
	Vector2f m_pos;
	Vector2f m_size;
};

#endif // WIDGET_H
