#include "mainmenu.h"
#include "Game/Audio/soundcontroller.h"
#include "Engine/engine.h"
#include "Translations/language.h"
#include "globalvariables.h"
#include "settings.h"

MainMenu::MainMenu()
	: Menu()

{
	setBackground(RESOURCES::MENU_BACKGROUND);
	currentMenu = static_cast<MENUS>(CAMPAIGN);

	setPos(Vector2f(120 * Settings::Instance().getScaleFactor().x,
					640 * Settings::Instance().getScaleFactor().y));
	setColor(Color(64,224,208, 100));
	setCurrentColor(Color(64,224,208));

	gameName.setFont(GlobalVariables::Instance().font());
	gameName.setString("Game Name");
	gameName.setFillColor(Color::Cyan);
	gameName.setOutlineColor(Color::Black);
	gameName.setOutlineThickness(5);
	gameName.setCharacterSize(150);
	gameName.setPosition(Settings::Instance().getResolution().x/2 - gameName.getGlobalBounds().width/2,
						 gameName.getGlobalBounds().height/2);

	addItem(Language::Instance().translate(Language::CAMPAIGN));
	addItem(Language::Instance().translate(Language::OPTIONS));
	addItem(Language::Instance().translate(Language::MANUAL));
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
	case CAMPAIGN:
		Engine::Instance().setState(Engine::CHOOSE_MISSION);
		break;
	case OPTIONS:
		Engine::Instance().setState(Engine::OPTIONS);
		break;
	case MANUAL:
		Engine::Instance().setState(Engine::MANUAL);
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

void MainMenu::paint(RenderWindow *window)
{
	Menu::paint(window);
	window->draw(gameName);
}
