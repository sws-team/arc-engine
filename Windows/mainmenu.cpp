#include "mainmenu.h"
#include "engine.h"
#include "managers.h"
#include "gamemanagers.h"
#include "gamestatemanager.h"
#include "gameplatform.h"
#include "gameoptions.h"

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
#ifndef RELEASE_BUILD
	clearStats.setFont(Engine::Instance().fontManager()->font());
	clearStats.setString("Clear stats");
	clearStats.setFillColor(sf::Color::Cyan);
	clearStats.setOutlineColor(sf::Color::Black);
	clearStats.setOutlineThickness(5);
	clearStats.setCharacterSize(50);
	clearStats.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	clearStats.setPosition(20, 200);

	clearSaves.setFont(Engine::Instance().fontManager()->font());
	clearSaves.setString("Clear saves");
	clearSaves.setFillColor(sf::Color::Cyan);
	clearSaves.setOutlineColor(sf::Color::Black);
	clearSaves.setOutlineThickness(5);
	clearSaves.setCharacterSize(50);
	clearSaves.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	clearSaves.setPosition(20, 250);
#endif

	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::START));
	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::OPTIONS));
	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::MANUAL));
	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::EXIT));

	GamePlatform::Instance().setPlatformState("steam_display", "#Status_AtMainMenu");
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
	window->draw(clearStats);
	window->draw(clearSaves);
}

#ifndef RELEASE_BUILD
void MainMenu::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::MouseButtonPressed)
	{
		const sf::Vector2f pos = sf::Vector2f(event->mouseButton.x, event->mouseButton.y);
		if (clearStats.getGlobalBounds().contains(pos))
		{
			if (GamePlatform::Instance().clearAll())
				clearStats.setString("Cleared!");
		}
		if (clearSaves.getGlobalBounds().contains(pos))
		{
			if (GamePlatform::Instance().deleteFile(EngineDefs::saveFileName))
			{
				Engine::Instance().options<GameOptions>()->clearSaves();
				clearSaves.setString("Cleared!");
			}
		}
	}
	return Menu::eventFilter(event);
}
#endif
