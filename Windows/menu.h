#ifndef MENU_H
#define MENU_H

#include "statewindow.h"

class Menu : public StateWindow
{
public:
	Menu();

	virtual void paint(RenderWindow *window) override;
	virtual void eventFilter(Event* event) override;
	virtual void back() override;

	void setPos(const Vector2f &pos);
	void setColor(const Color &color);
	void setCurrentColor(const Color &currentColor);

protected:
	virtual void accept() = 0;
	virtual void closeEvent() = 0;
	int currentMenu;
	void addItem(const String &str);
	vector<Text> menus;
private:
	void menuUp();

	void menuDown();

	int getMenuAtPos(const Vector2i &point) const;

	void updateColor();

	Vector2f m_pos;
	Color m_color;
	Color m_currentColor;

	static constexpr float Y_OFFSET = 30;
	float yPos;
};

#endif // MENU_H
