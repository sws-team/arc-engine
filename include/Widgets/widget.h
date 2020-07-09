#ifndef WIDGET_H
#define WIDGET_H

#include "stdheader.h"

class Widget
{
public:
	Widget();
	virtual ~Widget();

	virtual void draw(sf::RenderTarget *target);
	virtual void event(sf::Event *event);
	virtual void update();

	void setPos(const sf::Vector2f& pos);
	void setPos(float x, float y);
	void setSize(const sf::Vector2f& size);
	void setSize(float x, float y);

	sf::Vector2f size() const;
	sf::Vector2f pos() const;

	static const sf::Color DISABLED_COLOR;
protected:
	sf::Vector2f m_pos;
	sf::Vector2f m_size;

	static const sf::Color HOVERED_COLOR;
};

#endif // WIDGET_H
