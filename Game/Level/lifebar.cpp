#include "lifebar.h"
#include "settings.h"

LifeBar::LifeBar()
{

}

void LifeBar::init(const Vector2i &size, const Color& color)
{
	fullSize = size;
	currentValue.setSize(Vector2f(fullSize.x, fullSize.y));
	currentValue.setFillColor(color);

	fullValue.setSize(currentValue.getSize());
	fullValue.setOutlineColor(Color::White);
	fullValue.setFillColor(Color::Transparent);
	fullValue.setOutlineThickness(1 * Settings::Instance().getScaleFactor().y);
}

void LifeBar::draw(RenderTarget *target)
{
	target->draw(currentValue);
	target->draw(fullValue);
}

void LifeBar::setPos(const Vector2f &pos)
{
	fullValue.setPosition(pos);
	currentValue.setPosition(pos);
}

Vector2f LifeBar::pos() const
{
	return fullValue.getPosition();
}

void LifeBar::setValue(float rate)
{
	currentValue.setSize(Vector2f(rate * fullSize.x,  fullSize.y));
}
