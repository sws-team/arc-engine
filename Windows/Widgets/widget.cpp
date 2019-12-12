#include "widget.h"

Widget::Widget():
	m_pos(Vector2f(0,0))
  ,m_size(Vector2i(32, 32))
{

}

Widget::~Widget()
{

}

void Widget::draw(RenderTarget *target)
{

}

void Widget::event(Event *event)
{

}

void Widget::update()
{

}


void Widget::setPos(const Vector2f &pos)
{
	m_pos = pos;
	update();
}

void Widget::setPos(float x, float y)
{
	setPos(Vector2f(x, y));
}

void Widget::setSize(const Vector2f &size)
{
	m_size = size;
	update();
}

void Widget::setSize(float x, float y)
{
	setSize(Vector2f(x, y));
}

Vector2f Widget::size() const
{
    return m_size;
}
