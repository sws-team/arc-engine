#ifndef ARCBUTTON_H
#define ARCBUTTON_H

#include "arcsprite.h"

class ArcButton : public ArcSprite
{
public:
	ArcButton(const std::string& name);

	void draw(sf::RenderTarget *const target) override;

	void setCallback(const std::function<void()>& func);

	void setClickable(bool enabled);
	bool isClickable() const;

protected:
	bool eventFilter(sf::Event *event) override;

	void updatePos() override;
	void updateScale() override;
	void updateOrigin() override;
	void updateSize() override;

private:
	std::function<void()> m_callback = nullptr;
	bool m_clickable = true;
	bool hovered = false;
	sf::RectangleShape hoverRect;
	static const sf::Color HOVERED_COLOR;
};

#endif // ARCBUTTON_H
