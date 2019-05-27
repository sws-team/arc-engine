#include "lifebar.h"
#include "settings.h"

LifeBar::LifeBar()
{

}

void LifeBar::init(const Vector2f &size, const Color& color)
{
	fullSize = size;
	currentValue.setSize(fullSize);
	currentValue.setFillColor(color);

	fullValue.setSize(fullSize);
	fullValue.setOutlineColor(Color::White);
	fullValue.setFillColor(Color::Transparent);
	fullValue.setOutlineThickness(1 * Settings::Instance().getScaleFactor().y);
}

void LifeBar::draw(RenderWindow *window)
{
	window->draw(currentValue);
	window->draw(fullValue);
}

void LifeBar::setPos(const Vector2f &pos)
{
	fullValue.setPosition(pos);
	currentValue.setPosition(pos);
}

void LifeBar::setValue(float rate)
{
	currentValue.setSize(Vector2f(rate * fullSize.x,  fullSize.y));
}
