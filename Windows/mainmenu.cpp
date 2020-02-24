#include "mainmenu.h"
#include "engine.h"
#include "managers.h"
#include "gamemanagers.h"
#include "gamestatemanager.h"

MainMenu::MainMenu()
	: Menu()
{
	setBackground(GAME_TEXTURE::MENU_BACKGROUND);
	currentMenu = static_cast<MENUS>(CAMPAIGN);

	setPos(Vector2f(120 * Engine::Instance().settingsManager()->getScaleFactor().x,
					640 * Engine::Instance().settingsManager()->getScaleFactor().y));
	setColor(Color(64,224,208, 100));
	setCurrentColor(Color(64,224,208));

	gameName.setFont(Engine::Instance().fontManager()->font());
	gameName.setString("Game Name");
	gameName.setFillColor(Color::Cyan);
	gameName.setOutlineColor(Color::Black);
	gameName.setOutlineThickness(5);
	gameName.setCharacterSize(150);
	gameName.setPosition(Engine::Instance().settingsManager()->getResolution().x/2 - gameName.getGlobalBounds().width/2,
						 gameName.getGlobalBounds().height/2);

	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::CAMPAIGN));
	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::OPTIONS));
	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::MANUAL));
	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::EXIT));
}

void MainMenu::init()
{
	Engine::Instance().soundManager()->startBackgroundSound(GAME_MUSIC::MENU);
}

void MainMenu::accept()
{
	switch (currentMenu)
	{
	case CAMPAIGN:
		Engine::Instance().stateManager()->setState(GameStateManager::CHOOSE_MISSION);
		break;
	case OPTIONS:
		Engine::Instance().stateManager()->setState(StateManager::SETTINGS);
		break;
	case MANUAL:
		Engine::Instance().stateManager()->setState(GameStateManager::MANUAL);
		break;
	case EXIT:
		Engine::Instance().stateManager()->setState(GameStateManager::CLOSING);
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
