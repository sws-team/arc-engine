#include "gamewindow.h"
#include "settings.h"
#include "controller.h"
#include "Game/Audio/soundcontroller.h"
#include "Game/Level/level.h"
#include "globalvariables.h"
#include "Game/Level/cursor.h"
#include "Game/Level/camera.h"
#include "Game/gamepanel.h"
#include "ResourcesManager/resourcesmanager.h"

GameWindow::GameWindow()
	: Menu()
{
	tick = 0;
	lastTime = 0;

	Engine::Instance();
	Engine::Instance().camera()->init();
	currentMenu = static_cast<GAME_MENU>(CONTINUE_GAME);

	m_state = PLAYING;

	paused.setString("Paused");
	paused.setFillColor(Color::Blue);
	paused.setCharacterSize(140);
	paused.setFont(GlobalVariables::Instance().font());

	menuImg.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::GAME_MENU_TEXTURE));
	gameOverImg.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::GAME_OVER_TEXTURE));
	finishedImg.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::MISSON_COMPLETED_TEXTURE));
	//score
	text.setFillColor(Color::Black);
	text.setFont(GlobalVariables::Instance().font());
	Engine::Instance().panel()->init();

	addItem("Continue");
	addItem("Exit from mission");
	addItem("Exit from game");
	addItem("Exit to OS");
	clock.restart();
}

void GameWindow::init()
{
	unsigned int missionNumber = Engine::Instance().getMission();
	Engine::Instance().controller()->setPauseFunc(bind(&GameWindow::pause, this));
	Engine::Instance().level()->startMission(missionNumber);
}

void GameWindow::paint(RenderWindow *window)
{
	Engine::Instance().level()->draw(window);
	switch (m_state)
	{
	case PLAYING:

		break;
	case PAUSED:
		window->draw(paused);
		break;
	case IN_MENU:
		window->draw(menuImg);
		Menu::paint(window);
		break;
	case FINISHED:
	{
		window->draw(finishedImg);
		window->draw(text);
	}
		break;
	case GAME_OVER:
		window->draw(gameOverImg);
		window->draw(text);
		break;
	}
}

void GameWindow::eventFilter(Event *event)
{
	if (event->type == Event::KeyPressed && event->key.code == Keyboard::F10)
		setState(IN_MENU);

	switch (m_state)
	{
	case PLAYING:
	{
		switch (event->type)
		{
		case Event::LostFocus:
			pause();
			break;
		case Event::Closed:
			Engine::Instance().setState(Engine::EXIT);
			break;
		default:
			break;
		}		
		Engine::Instance().controller()->keyEvent();
	}
		break;
	case PAUSED:
		Engine::Instance().controller()->pausedEvents();
		break;
	case IN_MENU:
		Menu::eventFilter(event);
		break;
	case FINISHED:
	case GAME_OVER:
		if (event->type == Event::KeyPressed
				|| event->type == Event::JoystickButtonPressed)
			finish();
		break;
	}
}

void GameWindow::update()
{	
	if (m_state != PLAYING)
		return;
	Engine::Instance().cursor()->update();
	Engine::Instance().level()->update();

	switch (Engine::Instance().level()->getState())
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
	case IN_MENU:
		closeEvent();
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
		setState(PLAYING);
	else if (m_state == PLAYING)	
		setState(PAUSED);	
}

void GameWindow::accept()
{
	switch (currentMenu)
	{
	case CONTINUE_GAME:
		setState(PLAYING);
		break;
	case EXIT_FROM_MISSION:
		Engine::Instance().setState(Engine::CHOOSE_MISSION);
		break;
	case EXIT_FROM_GAME:
		Engine::Instance().setState(Engine::MAIN_MENU);
		break;
	case EXIT_TO_OS:
		Engine::Instance().setState(Engine::CLOSING);
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
	case PLAYING:
		break;
	case PAUSED:
	{
		paused.setPosition(Vector2f(Settings::Instance().getResolution().x/2 - paused.getGlobalBounds().width/2,
									Settings::Instance().getResolution().y/2 - paused.getGlobalBounds().height/2));
	}
		break;
	case IN_MENU:
	{
		menuImg.setPosition(Vector2f(Settings::Instance().getResolution().x/2 - menuImg.getGlobalBounds().width/2,
									Settings::Instance().getResolution().y/2 - menuImg.getGlobalBounds().height/2));

		const float offsetX = 20.f * Settings::Instance().getScaleFactor().x;
		const float offsetY = 20.f * Settings::Instance().getScaleFactor().y;

		float posX = menuImg.getGlobalBounds().left + offsetX;
		float posY = menuImg.getGlobalBounds().top;

		for(Text& text : menus)
		{
			text.setPosition(posX, posY);
			posY += text.getGlobalBounds().height + offsetY;
		}
	}
		break;
	case FINISHED:
	{
		text.setString("Congratulations!");
		Engine::Instance().setMissionFinished(Engine::Instance().getMission(), 4);
		Engine::Instance().save();

		finishedImg.setPosition(Vector2f(Settings::Instance().getResolution().x/2 - finishedImg.getGlobalBounds().width/2,
										 Settings::Instance().getResolution().y/2 - finishedImg.getGlobalBounds().height/2));

		const float labelsOffset = 15 * Settings::Instance().getScaleFactor().y;
		float posX = finishedImg.getGlobalBounds().left;
		float posY = finishedImg.getGlobalBounds().top;

		posX += labelsOffset;
		posY += labelsOffset;

		text.setPosition(posX, posY);

		posY += text.getGlobalBounds().height;

		Engine::Instance().level()->clear();
	}
		break;
	case GAME_OVER:
	{
		text.setString("Game over!");
		gameOverImg.setPosition(Vector2f(Settings::Instance().getResolution().x/2 - gameOverImg.getGlobalBounds().width/2,
										 Settings::Instance().getResolution().y/2 - gameOverImg.getGlobalBounds().height/2));
	}
		break;
	}
	m_state = state;
}

void GameWindow::finish()
{
	Engine::Instance().setState(Engine::CHOOSE_MISSION);
}
