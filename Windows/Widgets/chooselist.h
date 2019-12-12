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

	void setCharacterSize(unsigned int characterSize);

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
};

#endif // CHOOSELIST_H
