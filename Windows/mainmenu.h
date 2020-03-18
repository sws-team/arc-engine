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
#ifndef RELEASE_BUILD
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
	sf::Text gameName;
#ifndef RELEASE_BUILD
	sf::Text clearStats;
	sf::Text clearSaves;
#endif
};

#endif // MAINMENU_H
