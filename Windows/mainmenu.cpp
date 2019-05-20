#include "mainmenu.h"
#include "Game/Audio/soundcontroller.h"
#include "Engine/engine.h"
#include "Translations/language.h"

MainMenu::MainMenu()
	: Menu()

{
	setBackground(RESOURCES::MENU_BACKGROUND);
	currentMenu = static_cast<MENUS>(PLAY);

	addItem(Language::Instance().translate(Language::PLAY));
	addItem(Language::Instance().translate(Language::OPTIONS));
	addItem(Language::Instance().translate(Language::CREDITS));
	addItem(Language::Instance().translate(Language::EXIT));
}

void MainMenu::init()
{
	SoundController::Instance().startBackgroundSound(MENU_SOUND_FILE);
}

void MainMenu::accept()
{
	switch (currentMenu)
	{
	case PLAY:
		Engine::Instance().setState(Engine::LOAD_GAME);
		break;
	case OPTIONS:
		Engine::Instance().setState(Engine::OPTIONS);
		break;
	case CREDITS:
		Engine::Instance().setState(Engine::ABOUT);
		break;
	case EXIT:
		Engine::Instance().setState(Engine::CLOSING);
		break;
	}
}

void MainMenu::closeEvent()
{
	currentMenu = EXIT;
	accept();
}
