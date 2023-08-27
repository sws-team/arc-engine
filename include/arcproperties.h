#ifndef ARCPROPERTIES_H
#define ARCPROPERTIES_H

#include "enginedef.h"

class ArcProperty
{
public:
	ArcProperty();

protected:
	virtual void setObject(ArcObject *object);
	ArcObject *object = nullptr;
};

class ColorProperty : public ArcProperty
{
public:
	ColorProperty();

	virtual void setColor(const sf::Color& color);
	sf::Color color() const;

	static sf::Uint8 ratioToColor(float ratio);
	static float colorToRatio(sf::Uint8 color);

protected:
	void setObject(ArcObject *object) override;
	sf::Color actualColor() const;
	static constexpr float COLOR_CONST = 255.f;
	sf::Color m_color = sf::Color::White;
};

class BorderColorProperty : public ArcProperty
{
public:
	BorderColorProperty();

	virtual void setBorderColor(const sf::Color& color);
	sf::Color borderColor() const;

protected:
	void setObject(ArcObject *object) override;
	sf::Color actualBorderColor() const;
	sf::Color m_borderColor = sf::Color::White;
};

class BorderSizeProperty : public ArcProperty
{
public:
	BorderSizeProperty();

	virtual void setBorderSize(float size);
	float borderSize() const;

protected:
	void setObject(ArcObject *object) override;
	float m_borderSize = 1.f;
};

#endif // ARCPROPERTIES_H
