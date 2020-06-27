#ifndef MAINMENU_H
#define MAINMENU_H

#include "Windows/menu.h"
//#define GAME_NAME

class MainMenu : public Menu
{
public:
	MainMenu();

	void init() override;
	void accept() override;
	void closeEvent() override;
	void paint(sf::RenderWindow *window) override;
#ifdef DEV_BUILD
	void eventFilter(sf::Event* event) override;
#endif
private:
	enum MENUS
	{
		CAMPAIGN,
		OPTIONS,
		MANUAL,
		EXIT
	};
#ifdef GAME_NAME
	sf::Text gameName;
#endif
#ifdef DEV_BUILD
	sf::Text clearStats;
	sf::Text clearSaves;
#endif
	sf::Sprite logo;
};

#endif // MAINMENU_H
