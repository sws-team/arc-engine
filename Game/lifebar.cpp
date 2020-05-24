#include "lifebar.h"
#include "engine.h"
#include "managers.h"

LifeBar::LifeBar()
{

}

void LifeBar::init(const sf::Vector2i &size,
				   const sf::Color& color,
				   const sf::Color &borderColor)
{
	fullSize = size;
	currentValue.setSize(sf::Vector2f(fullSize.x, fullSize.y));
	currentValue.setFillColor(color);

	fullValue.setSize(currentValue.getSize());
	fullValue.setOutlineColor(borderColor);
	fullValue.setFillColor(sf::Color::Transparent);
	fullValue.setOutlineThickness(1 * Engine::Instance().settingsManager()->getScaleFactor().y);
}

void LifeBar::draw(sf::RenderTarget *target)
{
	target->draw(currentValue);
	target->draw(fullValue);
}

void LifeBar::setPos(const sf::Vector2f &pos)
{
	fullValue.setPosition(pos);
	currentValue.setPosition(pos);
}

sf::Vector2f LifeBar::pos() const
{
	return fullValue.getPosition();
}

void LifeBar::setValue(float rate)
{
	currentValue.setSize(sf::Vector2f(rate * fullSize.x,  fullSize.y));
}
