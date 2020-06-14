#include "mainmenu.h"
#include "engine.h"
#include "managers.h"
#include "gamemanagers.h"
#include "gamestatemanager.h"
#include "gameplatform.h"
#include "gameoptions.h"
#include "mainwindow.h"

MainMenu::MainMenu()
	: Menu()
{
	setBackground(GAME_TEXTURE::MENU_BACKGROUND);
	currentMenu = static_cast<MENUS>(CAMPAIGN);

	const sf::Vector2f scaleFactor = Engine::Instance().settingsManager()->getScaleFactor();

	setPos(sf::Vector2f(120 * scaleFactor.x,
						550 * scaleFactor.y));
	setColor(GameOptions::primaryColor);
	setCurrentColor(GameOptions::alternativePrimaryColor);
	setOutlineColor(GameOptions::secondaryColor);
#ifdef GAME_NAME
	gameName.setFont(Engine::Instance().fontManager()->font());
	gameName.setString("Arc Defence");
	gameName.setFillColor(sf::Color::Cyan);
	gameName.setOutlineColor(sf::Color::Black);
	gameName.setOutlineThickness(5);
	gameName.setCharacterSize(150);
	gameName.setScale(scaleFactor);
	gameName.setPosition(Engine::Instance().settingsManager()->getResolution().x/2 - gameName.getGlobalBounds().width/2,
						 gameName.getGlobalBounds().height/2);
#endif
	logo.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::LOGO));
	logo.setScale(scaleFactor);
	logo.setPosition(512 * scaleFactor.x,
					 32 * scaleFactor.y);

#ifndef RELEASE_BUILD
	clearStats.setFont(Engine::Instance().fontManager()->font());
	clearStats.setString("Clear stats");
	clearStats.setFillColor(sf::Color::Cyan);
	clearStats.setOutlineColor(sf::Color::Black);
	clearStats.setOutlineThickness(5);
	clearStats.setCharacterSize(Engine::Instance().fontManager()->getCharSize(50));
	clearStats.setScale(scaleFactor);
	clearStats.setPosition(20, 200);

	clearSaves.setFont(Engine::Instance().fontManager()->font());
	clearSaves.setString("Clear saves");
	clearSaves.setFillColor(sf::Color::Cyan);
	clearSaves.setOutlineColor(sf::Color::Black);
	clearSaves.setOutlineThickness(5);
	clearSaves.setCharacterSize(Engine::Instance().fontManager()->getCharSize(50));
	clearSaves.setScale(scaleFactor);
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
	Engine::Instance().soundManager()->setMusicLooped(true);
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
#ifdef GAME_NAME
	window->draw(gameName);
#endif
	window->draw(logo);
#ifndef RELEASE_BUILD
	window->draw(clearStats);
	window->draw(clearSaves);
#endif
}

#ifndef RELEASE_BUILD
void MainMenu::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::MouseButtonPressed)
	{
		const sf::Vector2i pixelPos = sf::Vector2i(event->mouseButton.x, event->mouseButton.y);
		const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																			   *Engine::Instance().window()->view());
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
