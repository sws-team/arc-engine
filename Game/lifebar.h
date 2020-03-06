#ifndef LIFEBAR_H
#define LIFEBAR_H

#include "stdheader.h"

class LifeBar
{
public:
	LifeBar();

	void init(const sf::Vector2i &size, const sf::Color& color);
	sf::RectangleShape currentValue;
	sf::RectangleShape fullValue;

	void draw(sf::RenderTarget *target);
	void setPos(const sf::Vector2f& pos);
	sf::Vector2f pos() const;

	void setValue(float rate);

	static constexpr float LIFE_BAR_HEIGHT = 5.f;
private:
	sf::Vector2i fullSize;
};

#endif // LIFEBAR_H
