#ifndef HOTKEYSWINDOW_H
#define HOTKEYSWINDOW_H

#include "statewindow.h"

class HotkeysWindow : public StateWindow
{
public:
	HotkeysWindow();
	~HotkeysWindow();

	void back() override;

protected:
	void paint(sf::RenderWindow *window) override;

private:
	sf::Text headerText;
	sf::Text text;
	sf::Sprite rect;
};

#endif // HOTKEYSWINDOW_H
