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
protected:
	virtual void accept() = 0;
	virtual void closeEvent() = 0;
	int currentMenu;
	void addItem(const String &str);
	vector<Text> menus;
private:
	constexpr static int offset = 60;
	constexpr static int x = 100;

	void menuUp();

	void menuDown();

	int getMenuAtPos(const Vector2i &point) const;

	void updateColor();
};

#endif // MENU_H
