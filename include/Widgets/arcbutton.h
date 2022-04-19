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

	enum class HOVER_TYPE
	{
		NONE,
		HOVER,
		RECT_HOVER,
		SCALE
	};

	HOVER_TYPE hoverType() const;
	void setHoverType(const HOVER_TYPE &hoverType);

	void setOpaqueClicks(bool opaqueClicks);
	void setView(sf::View *view);

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
	float lastAlpha = 1.f;
	sf::Vector2f lastScale = sf::Vector2f(1, 1);
	HOVER_TYPE m_hoverType = HOVER_TYPE::NONE;
	sf::RectangleShape hoverRect;
	bool m_opaqueClicks = false;
	sf::View *view = nullptr;
};

#endif // ARCBUTTON_H
