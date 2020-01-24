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

	m_state = PLAYING;

	menuImg.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::GAME_MENU_TEXTURE));
	menuImg.setScale(Settings::Instance().getGameScaleFactor());

	gameOverImg.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::GAME_OVER_TEXTURE));
	gameOverImg.setScale(Settings::Instance().getScaleFactor());

	finishedImg.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::MISSON_COMPLETED_TEXTURE));
	finishedImg.setScale(Settings::Instance().getScaleFactor());
	//score
	text.setFillColor(Color::Magenta);
	text.setFont(GlobalVariables::Instance().font());
	text.setCharacterSize(100);
	text.setScale(Settings::Instance().getScaleFactor());
	Engine::Instance().panel()->init();

	addItem(Language::Instance().translate(Language::CONTINUE));
	addItem(Language::Instance().translate(Language::RESTART));
	addItem(Language::Instance().translate(Language::EXIT_TO_MENU));
	addItem(Language::Instance().translate(Language::EXIT_FROM_GAME));

	for(Text& text : menus)
	{
		text.setCharacterSize(30);
		text.setScale(Settings::Instance().getScaleFactor());
	}

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
}

void GameWindow::paint(RenderWindow *window)
{
	viewPos = Vector2f(window->getView().getCenter().x - window->getView().getSize().x/2,
					   window->getView().getCenter().y - window->getView().getSize().y/2);

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
	{
		window->draw(finishedImg);
		window->draw(text);
	}
		break;
	case GAME_OVER:
		window->draw(gameOverImg);
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
//	Engine::Instance().window()->setMouseCursorVisible(true);
	switch (state)
	{
	case PLAYING:
//		Engine::Instance().window()->setMouseCursorVisible(false);
		break;
	case PAUSED:
	{	
		Vector2f pos = viewPos;
		menuImg.setPosition(pos);

		const float offsetY = 20.f * Settings::Instance().getScaleFactor().y;

		pos.x += GlobalVariables::Instance().tileSize().x;
		pos.y += Settings::Instance().getResolution().y/2 * Settings::GAME_SCALE;
		pos.y -= menuImg.getGlobalBounds().height/2;

		for(Text& text : menus)
		{
			text.setPosition(pos);
			pos.y += text.getGlobalBounds().height + offsetY;
		}
	}
		break;
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

		Engine::Instance().setMissionFinished(Engine::Instance().getMission(), stars);
		Engine::Instance().save();


		Vector2f pos = viewPos;
		pos.x += Settings::Instance().getResolution().x/2 * Settings::GAME_SCALE;
		pos.y += Settings::Instance().getResolution().y/2 * Settings::GAME_SCALE;
		finishedImg.setPosition(pos - Vector2f(finishedImg.getGlobalBounds().width/2,
											   finishedImg.getGlobalBounds().height/2));
		text.setString(Language::Instance().translate(Language::CONGRATULATIONS));

		text.setPosition(pos - Vector2f(text.getGlobalBounds().width/2,
										text.getGlobalBounds().height/2));
	}
		break;
	case GAME_OVER:
	{
		Vector2f pos = viewPos;
		pos.x += Settings::Instance().getResolution().x/2 * Settings::GAME_SCALE;
		pos.y += Settings::Instance().getResolution().y/2 * Settings::GAME_SCALE;
		gameOverImg.setPosition(pos - Vector2f(gameOverImg.getGlobalBounds().width/2,
											   gameOverImg.getGlobalBounds().height/2));

		text.setString(Language::Instance().translate(Language::GAME_OVER));
		text.setPosition(pos - Vector2f(text.getGlobalBounds().width/2,
										text.getGlobalBounds().height/2));
	}
		break;
	}
	m_state = state;
}

void GameWindow::finish()
{
	Engine::Instance().setState(Engine::CHOOSE_MISSION);
}

