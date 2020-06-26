#include "gamewindow.h"
#include "controller.h"
#include "Game/level.h"
#include "engine.h"
#include "Game/gamecursor.h"
#include "Game/camera.h"
#include "Game/gamepanel.h"
#include "gameoptions.h"
#include "gamestatemanager.h"
#include "gamemanagers.h"
#include "enginedef.h"
#include "gameplatform.h"
#include "Game/achievements.h"
#include "Game/instructions.h"
#include "mainwindow.h"

#include <algorithm>
#include <random>

GameWindow::GameWindow()
	: Menu()
	,isFinal(false)
{
	currentTrack = 0;
	tick = 0;
	lastTime = 0;

	Engine::Instance().options<GameOptions>()->camera()->init();
	currentMenu = static_cast<GAME_MENU>(CONTINUE_GAME);

	setColor(GameOptions::primaryColor);
	setCurrentColor(GameOptions::alternativePrimaryColor);
	setOutlineColor(GameOptions::secondaryColor);
	m_state = PLAYING;

	menuImg.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::GAME_MENU_TEXTURE));
	menuImg.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	windowSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::WINDOW_TEXTURE));
	windowSprite.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	text.setFillColor(GameOptions::primaryColor);
	text.setOutlineThickness(5);
	text.setOutlineColor(GameOptions::secondaryColor);
	text.setFont(Engine::Instance().fontManager()->font());
	text.setCharacterSize(Engine::Instance().fontManager()->getCharSize(100));
	text.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	skipText.setFillColor(GameOptions::primaryColor);
	skipText.setOutlineThickness(2);
	skipText.setOutlineColor(GameOptions::secondaryColor);
	skipText.setFont(Engine::Instance().fontManager()->font());
	skipText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(40));
	skipText.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	Engine::Instance().options<GameOptions>()->panel()->init();

	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::CONTINUE));
	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::RESTART));
	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::EXIT_TO_MENU));
	addItem(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::EXIT_FROM_GAME));

	sf::Vector2f pos = sf::Vector2f(0, 0);
	menuImg.setPosition(pos);

	const float offsetY = 20.f * Engine::Instance().settingsManager()->getScaleFactor().y;

	pos.x += 20.f * Engine::Instance().settingsManager()->getScaleFactor().x;
	pos.y += 290 * Engine::Instance().settingsManager()->getScaleFactor().y;

	for(sf::Text& menuText : menus)
	{
		menuText.setCharacterSize(40);
		menuText.setScale(Engine::Instance().settingsManager()->getScaleFactor());
		menuText.setPosition(pos);
		pos.y += menuText.getGlobalBounds().height + offsetY;
	}
	windowSprite.setPosition(Engine::Instance().settingsManager()->getResolution().x/2 - windowSprite.getGlobalBounds().width/2,
							 Engine::Instance().settingsManager()->getResolution().y/2 - windowSprite.getGlobalBounds().height/2);
	clock.restart();
}

GameWindow::~GameWindow()
{
	Engine::Instance().options<GameOptions>()->level()->clear();
	Engine::Instance().options<GameOptions>()->camera()->destroy();
}

void GameWindow::init()
{
	unsigned int missionNumber = Engine::Instance().options<GameOptions>()->getMission();
	Engine::Instance().options<GameOptions>()->controller()->setPauseFunc(std::bind(&GameWindow::pause, this));
	Engine::Instance().options<GameOptions>()->panel()->updateAbilitiesPos();
	Engine::Instance().soundManager()->endBackgroundSound();
	Engine::Instance().soundManager()->setMusicLooped(false);
	fillTracks(GAME_MUSIC::TRACKS_FILES);
	nextTrack();
	Engine::Instance().options<GameOptions>()->level()->startMission(missionNumber);
	TimersManager::Instance().setPaused(false);
}

void GameWindow::paint(sf::RenderWindow *window)
{
	Engine::Instance().options<GameOptions>()->level()->draw(window);
	switch (m_state)
	{
	case PLAYING:
		break;
	case PAUSED:
		window->draw(menuImg);
		Menu::paint(window);
		break;
	case FINISHED:
	case GAME_OVER:
		window->draw(windowSprite);
		window->draw(text);
		window->draw(skipText);
		break;
	default:
		break;
	}
}

void GameWindow::eventFilter(sf::Event *event)
{
	switch (m_state)
	{
	case PLAYING:
	{
		switch (event->type)
		{
		case sf::Event::LostFocus:
			pause();
			break;
//		case Event::GainedFocus:
//			pause();
//			break;
		case sf::Event::Closed:
			Engine::Instance().stateManager()->setState(StateManager::EXIT);
			break;
		case sf::Event::MouseMoved:
			Engine::Instance().options<GameOptions>()->cursor()->updatePanel();
			if (!Engine::Instance().options<GameOptions>()->cursor()->inPanel())
				Engine::Instance().options<GameOptions>()->cursor()->initCell();
			Engine::Instance().options<GameOptions>()->panel()->updateCursor();
			break;
		default:
			break;
		}
		Engine::Instance().options<GameOptions>()->controller()->eventFilter(event);
	}
		break;
	case PAUSED:
	{
		if (event->type == sf::Event::KeyPressed)
			if (event->key.code == sf::Keyboard::Escape)
			{
				pause();
				return;
			}

		if (event->type == sf::Event::JoystickButtonPressed)
			if (event->joystickButton.button == Controller::KEY_ESCAPE)
			{
				pause();
				return;
			}
		Menu::eventFilter(event);
	}
		break;
	case FINISHED:
	case GAME_OVER:
		if (event->type == sf::Event::KeyPressed
				|| event->type == sf::Event::JoystickButtonPressed)
			finish();
		break;
	}
}

void GameWindow::update()
{	
	if (m_state != PLAYING)
		return;
	if (!isFinal && Engine::Instance().options<GameOptions>()->level()->isFinalWave())
	{
		Engine::Instance().soundManager()->endBackgroundSound();
		fillTracks(GAME_MUSIC::FINAL_TRACKS_FILES);
		isFinal = true;
	}
	if (Engine::Instance().soundManager()->isMusicFinished())
		nextTrack();

	Engine::Instance().options<GameOptions>()->cursor()->update();
	Engine::Instance().options<GameOptions>()->level()->update();
	Engine::Instance().options<GameOptions>()->panel()->update();
	Engine::Instance().options<GameOptions>()->instructions()->update();

	switch (Engine::Instance().options<GameOptions>()->level()->getState())
	{
	case Level::WIN:	
		setState(FINISHED);	
		break;
	case Level::LOSE:
		setState(GAME_OVER);
		break;
	default:
		break;
	}
}

void GameWindow::back()
{
	switch (m_state)
	{
	case PLAYING:
		pause();
		break;
	case PAUSED:
		pause();
		break;
	case FINISHED:
	case GAME_OVER:
		finish();
		break;
	}
}

void GameWindow::pause()
{
	if (m_state == PAUSED)
	{
		setState(PLAYING);
		TimersManager::Instance().setPaused(false);
		Engine::Instance().soundManager()->unpauseMusic();
	}
	else if (m_state == PLAYING)	
	{
		setState(PAUSED);
		TimersManager::Instance().setPaused(true);
		Engine::Instance().soundManager()->pauseMusic();
	}
}

void GameWindow::accept()
{
	switch (currentMenu)
	{
	case CONTINUE_GAME:
		pause();
		break;
	case RESET_MISSION:
	{
		Engine::Instance().options<GameOptions>()->level()->clear();
		init();
		pause();
	}
		break;
	case EXIT_FROM_MISSION:
		Engine::Instance().stateManager()->setState(GameStateManager::CHOOSE_MISSION);
		break;
	case EXIT_FROM_GAME:
		Engine::Instance().stateManager()->setState(StateManager::CLOSING);
		break;
	}
}

void GameWindow::closeEvent()
{
	setState(PLAYING);
}

void GameWindow::setState(const GAME_STATE &state)
{
	if (state == m_state)
		return;

	switch (state)
	{
	case FINISHED:
	{
		const float fullLife = Engine::Instance().options<GameOptions>()->level()->getStartLife();
		const float currentLife = Engine::Instance().options<GameOptions>()->level()->getLifeCount();
		const float k = currentLife/fullLife;
		unsigned int stars = 0;
		if (k > 0.9f) // 0.9 1 - 5
			stars = 5;
		else if (k > 0.8f) // 0.8 0.9 - 4
			stars = 4;
		else if (k > 0.6f) // 0.6 0.8 - 3
			stars = 3;
		else if (k > 0.4f) // 0.4 0.6 - 2
			stars = 2;
		else if (k > 0.1f) // 0.1 0.4 - 1
			stars = 1;
		else // 0 0.1 - 0
			stars = 0;

		Engine::Instance().soundManager()->endBackgroundSound();
		text.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::CONGRATULATIONS));
		skipText.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::SKIP_TEXT));
		updateTextPos();
		const unsigned int mission = Engine::Instance().options<GameOptions>()->getMission();
		const int starsValue = Engine::Instance().options<GameOptions>()->missionStars(mission);
		if (k == 1.f)
			GamePlatform::Instance().unlock(ACHIEVEMENT_COMPLETE_LEVEL_WITHOUT_DAMAGE);
		if (starsValue == -1)
		{
			GamePlatform::Instance().incrementValue(STAT_COMPLETED_LEVELS);
			if (stars == 5)
				GamePlatform::Instance().incrementValue(STAT_5_STARS_COMPLETED_LEVELS);

			Engine::Instance().options<GameOptions>()->setMissionFinished(mission, stars);
			Engine::Instance().options<GameOptions>()->save();
		}
		else
		{
			if (stars > starsValue)
			{
				if (stars == 5)
					GamePlatform::Instance().incrementValue(STAT_5_STARS_COMPLETED_LEVELS);
				Engine::Instance().options<GameOptions>()->setMissionFinished(mission, stars);
				Engine::Instance().options<GameOptions>()->save();
			}
		}
	}
		break;
	case GAME_OVER:
	{
		text.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::GAME_OVER));
		skipText.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::SKIP_TEXT));
		updateTextPos();
		Engine::Instance().soundManager()->endBackgroundSound();
	}
		break;
	default:
		break;
	}
	m_state = state;
}

void GameWindow::finish()
{
	Engine::Instance().stateManager()->setState(GameStateManager::CHOOSE_MISSION);
}

void GameWindow::updateTextPos()
{
	text.setPosition(Engine::Instance().settingsManager()->getResolution().x/2 -
					 text.getGlobalBounds().width/2,
					 Engine::Instance().settingsManager()->getResolution().y/2 -
					 text.getGlobalBounds().height/2);

	skipText.setPosition(Engine::Instance().settingsManager()->getResolution().x/2 -
						 skipText.getGlobalBounds().width/2,
						 windowSprite.getGlobalBounds().top + windowSprite.getGlobalBounds().height -
						 Engine::Instance().options<GameOptions>()->tileSize().y);
}

void GameWindow::nextTrack()
{
	currentTrack++;
	if (currentTrack >= tracks.size())
		currentTrack = 0;
	const MusicType trackType = tracks.at(currentTrack);
	Engine::Instance().soundManager()->startBackgroundSound(trackType);
}

void GameWindow::fillTracks(const std::vector<MusicType> &trackList)
{
	tracks.clear();
	currentTrack = 0;
	tracks = trackList;
	std::random_device rd;
	std::mt19937 g(rd());
	shuffle(tracks.begin(), tracks.end(), g);
}

#ifdef STEAM_API
void GameWindow::OnGameOverlayActivated(GameOverlayActivated_t *pCallback)
{
	if (pCallback->m_bActive)
		pause();
	else
		pause();
}
#endif
