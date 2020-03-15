#include "mainmenu.h"
#include "engine.h"
#include "managers.h"
#include "gamemanagers.h"
#include "gamestatemanager.h"

#ifdef STEAM_API
#include "steam_api.h"
#endif

MainMenu::MainMenu()
	: Menu()
{
	setBackground(GAME_TEXTURE::MENU_BACKGROUND);
	currentMenu = static_cast<MENUS>(CAMPAIGN);

	setPos(sf::Vector2f(120 * Engine::Instance().settingsManager()->getScaleFactor().x,
					640 * Engine::Instance().settingsManager()->getScaleFactor().y));
	setColor(sf::Color(64,224,208, 100));
	setCurrentColor(sf::Color(64,224,208));

	gameName.setFont(Engine::Instance().fontManager()->font());
	gameName.setString("Game Name");
	gameName.setFillColor(sf::Color::Cyan);
	gameName.setOutlineColor(sf::Color::Black);
	gameName.setOutlineThickness(5);
	gameName.setCharacterSize(150);
	gameName.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	gameName.setPosition(Engine::Instance().settingsManager()->getResolution().x/2 - gameName.getGlobalBounds().width/2,
						 gameName.getGlobalBounds().height/2);

//	const sf::String name = sf::String(SteamFriends()->GetPersonaName());
//	playerName.setFont(Engine::Instance().fontManager()->font());
//	playerName.setString(name);
//	playerName.setFillColor(sf::Color::Cyan);
//	playerName.setOutlineColor(sf::Color::Black);
//	playerName.setOutlineThickness(5);
//	playerName.setCharacterSize(50);
//	playerName.setScale(Engine::Instance().settingsManager()->getScaleFactor());
//	playerName.setPosition(Engine::Instance().settingsManager()->getResolution().x/2 - playerName.getGlobalBounds().width/2,
//						 playerName.getGlobalBounds().height/2);

	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::START));
	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::OPTIONS));
	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::MANUAL));
	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::EXIT));
}

void MainMenu::init()
{
	Engine::Instance().soundManager()->startBackgroundSound(SoundManager::MAIN_MENU_MUSIC);
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

void MainMenu::paint(sf::RenderWindow *window)
{
	Menu::paint(window);
	window->draw(gameName);
//	window->draw(playerName);
}
