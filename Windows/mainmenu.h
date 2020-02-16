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
private:
	enum MENUS
	{
		CAMPAIGN,
		OPTIONS,
		MANUAL,
		EXIT
	};
};

#endif // MAINMENU_H
