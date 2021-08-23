#ifndef ARCLABEL_H
#define ARCLABEL_H

#include "arcobject.h"

class ArcLabel : public ArcObject
{
public:
	ArcLabel();
	void setPos(float x, float y) override;
	void setOrigin(float x, float y) override;

private:
	sf::Text text;

	float m_fontSize = 10.f;
	float m_borderSize = 1.f;
	sf::Color m_color = sf::Color::White;
	sf::Color m_borderColor = sf::Color::Black;
};

#endif // ARCLABEL_H
