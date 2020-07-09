#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "widget.h"

class CheckBox : public Widget
{
public:
	CheckBox();

	void draw(sf::RenderTarget *target) override;
	void event(sf::Event *event) override;
	void update() override;

	bool isChecked() const;
	void setChecked(bool checked);

	void setBorderColor(const sf::Color &borderColor);
	void setCheckedColor(const sf::Color& checkedColor);

private:
	sf::RectangleShape rect;
	sf::CircleShape circle;

	bool m_isChecked;
	sf::RectangleShape currentRect;
	bool hovered;
};

#endif // CHECKBOX_H
