#ifndef BUTTON_H
#define BUTTON_H

#include "widget.h"

class Button : public Widget
{
public:
	Button();

	void draw(RenderTarget *target) override;
	void event(Event *event) override;
	void update() override;

	void setCallback(function<void()> callback);
	void setText(const String &text);

	void setTextCharacterSize(unsigned int size);

	void setScale(const Vector2f& scaleFactor);

	void setFillColor(const Color &fillColor);
	void setBorderColor(const Color &borderColor);
	void setTextColor(const Color &textColor);

private:
	RectangleShape rect;
	Text m_text;
	function<void()> m_callback;
	RectangleShape currentRect;
	bool hovered;
};

#endif // BUTTON_H
