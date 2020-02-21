#include "gamewindow.h"
#include "settings.h"
#include "controller.h"
#include "Game/Audio/soundcontroller.h"
#include "Game/Level/level.h"
#include "globalvariables.h"
#include "Game/Level/gamecursor.h"
#include "Game/Level/camera.h"
#include "Game/gamepanel.h"
#include "ResourcesManager/resourcesmanager.h"
#include "Translations/language.h"

GameWindow::GameWindow()
	: Menu()
{
	tick = 0;
	lastTime = 0;

	Engine::Instance();
	Engine::Instance().camera()->init();
	currentMenu = static_cast<GAME_MENU>(CONTINUE_GAME);

	setColor(Color(64,224,208, 100));
	setCurrentColor(Color(64,224,208));

	m_state = PLAYING;

	menuImg.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::GAME_MENU_TEXTURE));
	menuImg.setScale(Settings::Instance().getScaleFactor());

	windowSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::WINDOW_TEXTURE));
	windowSprite.setScale(Settings::Instance().getScaleFactor());

	text.setFillColor(Color::Magenta);
	text.setOutlineThickness(5);
	text.setOutlineColor(Color::Yellow);
	text.setFont(GlobalVariables::Instance().font());
	text.setCharacterSize(100);
	text.setScale(Settings::Instance().getScaleFactor());

	Engine::Instance().panel()->init();

	addItem(Language::Instance().translate(Language::CONTINUE));
	addItem(Language::Instance().translate(Language::RESTART));
	addItem(Language::Instance().translate(Language::EXIT_TO_MENU));
	addItem(Language::Instance().translate(Language::EXIT_FROM_GAME));

	Vector2f pos = Vector2f(0, 0);
	menuImg.setPosition(pos);

	const float offsetY = 20.f * Settings::Instance().getScaleFactor().y;

	pos.x += GlobalVariables::Instance().tileSize().x;
	pos.y += Settings::Instance().getResolution().y/2;

	for(Text& menuText : menus)
	{
		menuText.setCharacterSize(40);
		menuText.setScale(Settings::Instance().getScaleFactor());
		menuText.setPosition(pos);
		pos.y += menuText.getGlobalBounds().height + offsetY;
	}
	windowSprite.setPosition(Settings::Instance().getResolution().x/2 - windowSprite.getGlobalBounds().width/2,
							 Settings::Instance().getResolution().y/2 - windowSprite.getGlobalBounds().height/2);
	clock.restart();
}

GameWindow::~GameWindow()
{
	Engine::Instance().level()->clear();
	Engine::Instance().camera()->destroy();
}

void GameWindow::init()
{
	unsigned int missionNumber = Engine::Instance().getMission();
	Engine::Instance().controller()->setPauseFunc(bind(&GameWindow::pause, this));
	Engine::Instance().level()->startMission(missionNumber);
	TimersManager::Instance().setPaused(false);
}

void GameWindow::paint(RenderWindow *window)
{
	Engine::Instance().level()->draw(window);
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
		break;
	default:
		break;
	}
}

void GameWindow::eventFilter(Event *event)
{
	switch (m_state)
	{
	case PLAYING:
	{
		switch (event->type)
		{
		case Event::LostFocus:
			pause();
			break;
//		case Event::GainedFocus:
//			pause();
//			break;
		case Event::Closed:
			Engine::Instance().setState(Engine::EXIT);
			break;
		case Event::MouseMoved:
			Engine::Instance().cursor()->updatePanel();
			if (!Engine::Instance().cursor()->inPanel())
				Engine::Instance().cursor()->initCell();
			Engine::Instance().panel()->updateCursor();
			break;
		default:
			break;
		}
		Engine::Instance().controller()->eventFilter(event);
	}
		break;
	case PAUSED:
	{
		if (event->type == Event::KeyPressed)
			if (event->key.code == Keyboard::Escape)
			{
				pause();
				return;
			}

		if (event->type == Event::JoystickButtonPressed)
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
	Engine::Instance().panel()->update();

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
		SoundController::Instance().unpauseMusic();
	}
	else if (m_state == PLAYING)	
	{
		setState(PAUSED);
		TimersManager::Instance().setPaused(true);
		SoundController::Instance().pauseMusic();
	}
}

void GameWindow::accept()
{
	switch (currentMenu)
	{
	case CONTINUE_GAME:
		setState(PLAYING);
		break;
	case RESET_MISSION:
	{
		Engine::Instance().level()->clear();
		init();
		setState(PLAYING);
	}
		break;
	case EXIT_FROM_MISSION:
		Engine::Instance().setState(Engine::CHOOSE_MISSION);
		break;
	case EXIT_FROM_GAME:
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
	case FINISHED:
	{
		const float fullLife = Engine::Instance().level()->getStartLife();
		const float currentLife = Engine::Instance().level()->getLifeCount();
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

		text.setString(Language::Instance().translate(Language::CONGRATULATIONS));
		updateTextPos();

		Engine::Instance().setMissionFinished(Engine::Instance().getMission(), stars);
		Engine::Instance().save();
	}
		break;
	case GAME_OVER:
	{
		text.setString(Language::Instance().translate(Language::GAME_OVER));
		updateTextPos();
	}
		break;
	default:
		break;
	}
	m_state = state;
}

void GameWindow::finish()
{
	Engine::Instance().setState(Engine::CHOOSE_MISSION);
}

void GameWindow::updateTextPos()
{
	text.setPosition(Settings::Instance().getResolution().x/2 - text.getGlobalBounds().width/2,
					 Settings::Instance().getResolution().y/2 - text.getGlobalBounds().height/2);
}

