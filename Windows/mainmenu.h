#ifndef MAINMENU_H
#define MAINMENU_H

#include "Windows/menu.h"

class MainMenu : public Menu
{
public:
	MainMenu();

	void init() override;
	void accept() override;
	void closeEvent() override;
	void paint(sf::RenderWindow *window) override;

private:
	enum MENUS
	{
		CAMPAIGN,
		OPTIONS,
		MANUAL,
		EXIT
	};
	sf::Text gameName;
//	sf::Text playerName;
};

#endif // MAINMENU_H
