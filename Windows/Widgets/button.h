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

private:
	RectangleShape rect;
	Text m_text;
	function<void()> m_callback;
};

#endif // BUTTON_H