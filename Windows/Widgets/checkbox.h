#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "widget.h"

class CheckBox : public Widget
{
public:
	CheckBox();

	void draw(RenderTarget *target) override;
	void event(Event *event) override;
	void update() override;

	bool isChecked() const;
	void setChecked(bool checked);

private:
	RectangleShape rect;
	CircleShape circle;

	bool m_isChecked;
};

#endif // CHECKBOX_H
