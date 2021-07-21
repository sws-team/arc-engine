#include "widget.h"

const sf::Color Widget::HOVERED_COLOR = sf::Color(32, 32, 32, 96);
const sf::Color Widget::DISABLED_COLOR = sf::Color(64, 64, 64, 150);

Widget::Widget():
	m_pos(sf::Vector2f(0,0))
	,m_size(sf::Vector2i(32, 32))
	,m_rotation(0.f)
{

}

Widget::~Widget()
{

}

void Widget::draw(sf::RenderTarget *target)
{

}

void Widget::event(sf::Event *event)
{

}

void Widget::update()
{

}

void Widget::setPos(const sf::Vector2f &pos)
{
	m_pos = pos;
	update();
}

void Widget::setPos(float x, float y)
{
	setPos(sf::Vector2f(x, y));
}

void Widget::setSize(const sf::Vector2f &size)
{
	m_size = size;
	update();
}

void Widget::setSize(float x, float y)
{
	setSize(sf::Vector2f(x, y));
}

void Widget::setRotation(float angle)
{
	m_rotation = angle;
	update();
}

sf::Vector2f Widget::size() const
{
	return m_size;
}

sf::Vector2f Widget::pos() const
{
	return m_pos;
}

float Widget::rotation() const
{
	return m_rotation;
}
