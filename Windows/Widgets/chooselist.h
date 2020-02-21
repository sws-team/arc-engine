#ifndef CHOOSELIST_H
#define CHOOSELIST_H

#include "widget.h"

class ChooseList : public Widget
{
public:
	ChooseList();

	void draw(RenderTarget *target) override;
	void event(Event *event) override;
	void update() override;

	void setList(const vector<String>& list);
	void addItem(const String& item);
	void setCurrent(int n);
	void setCurrent(const String& text);
	String currentText() const;
	void setScale(const Vector2f& scaleFactor);

	void setCharacterSize(unsigned int characterSize);

	void setTextColor(const Color &textColor);

	void setFillColor(const Color &fillColor);

	void setBorderColor(const Color &borderColor);

	void setCurrentColor(const Color &currentColor);

private:
	struct ChooseValue
	{
		RectangleShape rect;
		Text text;
	};
	vector<ChooseValue> rects;
	vector<String> m_list;
	void updateList();
	int current;
	unsigned int m_characterSize;

	Color m_textColor;
	Color m_fillColor;
	Color m_borderColor;
	Color m_currentColor;

	RectangleShape currentRect;
	bool hovered;
};

#endif // CHOOSELIST_H
