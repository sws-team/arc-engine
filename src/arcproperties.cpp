#include "arcproperties.h"
#include "arcobject.h"

ArcProperty::ArcProperty()
{

}

void ArcProperty::setObject(ArcObject *object)
{
	this->object = object;
}

ColorProperty::ColorProperty()
	: ArcProperty()
{

}

void ColorProperty::setColor(const sf::Color &color)
{
	if (m_color == color)
		return;

	m_color = color;
	const float alpha = colorToRatio(color.a);
	object->setAlpha(alpha);
}

sf::Color ColorProperty::color() const
{
	return m_color;
}

sf::Color ColorProperty::actualColor() const
{
	sf::Color result = m_color;
	result.a = ColorProperty::ratioToColor(object->globalAlpha());
	return result;
}

sf::Uint8 ColorProperty::ratioToColor(float ratio)
{
	return static_cast<sf::Uint8>(ratio * COLOR_CONST);
}

float ColorProperty::colorToRatio(sf::Uint8 color)
{
	return static_cast<float>(color) / COLOR_CONST;
}

void ColorProperty::setObject(ArcObject *object)
{
	ArcProperty::setObject(object);
	setColor(color());
}

BorderColorProperty::BorderColorProperty()
	: ArcProperty()
{

}

void BorderColorProperty::setBorderColor(const sf::Color &color)
{
	if (m_borderColor == color)
		return;

	m_borderColor = color;
}

sf::Color BorderColorProperty::borderColor() const
{
	return m_borderColor;
}

void BorderColorProperty::setObject(ArcObject *object)
{
	ArcProperty::setObject(object);
	setBorderColor(sf::Color::Black);
}

sf::Color BorderColorProperty::actualBorderColor() const
{
	sf::Color result = m_borderColor;
	result.a = ColorProperty::ratioToColor(object->globalAlpha());
	return result;
}

BorderSizeProperty::BorderSizeProperty()
	: ArcProperty()
{

}

void BorderSizeProperty::setBorderSize(float size)
{
	m_borderSize = size;
}

float BorderSizeProperty::borderSize() const
{
	return m_borderSize;
}

void BorderSizeProperty::setObject(ArcObject *object)
{
	ArcProperty::setObject(object);
	setBorderSize(m_borderSize);
}
