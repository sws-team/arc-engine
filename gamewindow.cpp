#include "gamewindow.h"
#include "settings.h"
#include "controller.h"
#include "Game/Audio/soundcontroller.h"
#include "Game/level.h"
#include "savedgame.h"
#include "globalvariables.h"
#include "Game/Level/Cursor/cursor.h"

GameWindow::GameWindow()
	: Menu()
{	
	currentMenu = static_cast<GAME_MENU>(CONTINUE_GAME);

	m_state = PLAYING;

	level = new Level();
	cursor = new Cursor();
	controller = new Controller(level, cursor);

	paused.setString("Paused");
	paused.setFillColor(Color::Blue);
	paused.setCharacterSize(140);
	paused.setFont(GlobalVariables::Instance().font());

	menuImg.texture.loadFromFile("images/ui/game_menu.png");
	menuImg.sprite.setTexture(menuImg.texture);

	finishedImg.texture.loadFromFile("images/Locations/score.png");
	finishedImg.sprite.setTexture(finishedImg.texture);

	gameOverImg.texture.loadFromFile("images/Windows/GameOver.png");
	gameOverImg.sprite.setTexture(gameOverImg.texture);
	//score
	scoreText.setFillColor(Color::Black);
	scoreText.setFont(GlobalVariables::Instance().font());
	scoreText.setString("Congratulations!");

//	for (unsigned int i = 0; i < GameController::Instance().players().size(); ++i)
//	{
//		ScoreInfo score;

//		score.kills.text.setFillColor(Color::Blue);
//		score.kills.text.setFont(GlobalVariables::Instance().font());
//	//	score.kills.text.setString(Language::Instance().translate(Language::DAMAGE) + delimer);
//		score.kills.text.setString("Kills: ");
//		score.kills.valueText.setFillColor(Color::Black);
//		score.kills.valueText.setFont(GlobalVariables::Instance().font());

//		score.gears.text.setFillColor(Color::Blue);
//		score.gears.text.setFont(GlobalVariables::Instance().font());
//	//	score.score.text.setString(Language::Instance().translate(Language::DAMAGE) + delimer);
//		score.gears.text.setString("Gears: ");
//		score.gears.valueText.setFillColor(Color::Black);
//		score.gears.valueText.setFont(GlobalVariables::Instance().font());

//		score.points.text.setFillColor(Color::Blue);
//		score.points.text.setFont(GlobalVariables::Instance().font());
//	//	score.points.text.setString(Language::Instance().translate(Language::DAMAGE) + delimer);
//		score.points.text.setString("Points: ");
//		score.points.valueText.setFillColor(Color::Black);
//		score.points.valueText.setFont(GlobalVariables::Instance().font());

//		scores.push_back(score);
//	}

	addItem("Continue");
	addItem("Exit from mission");
	addItem("Exit from game");
	addItem("Exit to OS");
}

void GameWindow::init()
{
	unsigned int missionNumber = SavedGameLoader::Instance().getCurrentMission();
	controller->setPauseFunc(bind(&GameWindow::pause, this));
	level->startMission(missionNumber);
}

void GameWindow::paint(RenderWindow *window)
{
	level->draw(window);
	cursor->draw(window);
	switch (m_state)
	{
	case PLAYING:

		break;
	case PAUSED:
		window->draw(paused);
		break;
	case IN_MENU:
		window->draw(menuImg.sprite);
		Menu::paint(window);
		break;
	case FINISHED:
	{
		window->draw(finishedImg.sprite);
		window->draw(scoreText);
//		for (unsigned int i = 0; i < GameController::Instance().players().size(); ++i)
//		{
//			SpaceShip *player = GameController::Instance().players().at(i);
//			window->draw(player->getIcon());
//		}
		for (unsigned int i = 0; i < scores.size(); ++i)
		{
			const ScoreInfo score = scores.at(i);
			window->draw(score.kills.text);
			window->draw(score.kills.valueText);
			window->draw(score.gears.text);
			window->draw(score.gears.valueText);
			window->draw(score.points.text);
			window->draw(score.points.valueText);
		}
	}
		break;
	case GAME_OVER:
		window->draw(gameOverImg.sprite);
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
		controller->keyEvent();
	}
		break;
	case PAUSED:
		controller->pausedEvents();
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

	if (level->isFinished())
	{
		const bool isFailed = level->isFailed();
		if (isFailed)
			setState(GAME_OVER);
		else
			setState(FINISHED);
		//save
		SavedGameLoader::Instance().save();
		return;
	}
	cursor->update();
	level->update();
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
		Engine::Instance().setState(Engine::MAIN_LOCATION);
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
		const Vector2f offset = Vector2f(paused.getGlobalBounds().width/2, paused.getGlobalBounds().height/2);
		paused.setPosition(level->getCenter() + offset);
	}
		break;
	case IN_MENU:
	{
		const Vector2f offset = Vector2f(menuImg.sprite.getGlobalBounds().width/2, menuImg.sprite.getGlobalBounds().height/2);
		menuImg.sprite.setPosition(level->getCenter() - offset);

		const float offsetX = 20.f * Settings::Instance().getScaleFactor().x;
		const float offsetY = 20.f * Settings::Instance().getScaleFactor().y;

		float posX = menuImg.sprite.getGlobalBounds().left + offsetX;
		float posY = menuImg.sprite.getGlobalBounds().top;

		for(Text& text : menus)
		{
			text.setPosition(posX, posY);
			posY += text.getGlobalBounds().height + offsetY;
		}
	}
		break;
	case FINISHED:
	{
		SavedGameLoader::Instance().addCompletedLevel(SavedGameLoader::Instance().getCurrentMission());

		const Vector2f offset = Vector2f(finishedImg.sprite.getGlobalBounds().width/2, finishedImg.sprite.getGlobalBounds().height/2);
		finishedImg.sprite.setPosition(level->getCenter() - offset);

		const float labelsOffset = 15 * Settings::Instance().getScaleFactor().y;
		float posX = finishedImg.sprite.getGlobalBounds().left;
		float posY = finishedImg.sprite.getGlobalBounds().top;

		posX += labelsOffset;
		posY += labelsOffset;

		scoreText.setPosition(posX, posY);

		posY += scoreText.getGlobalBounds().height;

//		for (unsigned int i = 0; i < GameController::Instance().players().size(); ++i)
//		{
//			SpaceShip *player = GameController::Instance().players().at(i);

//			player->getIcon().setPosition(posX, posY);

//			const float gears = level->getLevelBonuses().at(player);

//			scores[i].gears.valueText.setString((String(GlobalVariables::to_string_with_precision(gears, 1))));

//			scores[i].gears.text.setPosition(posX, posY);
//			scores[i].gears.valueText.setPosition(posX + scores.at(i).gears.text.getGlobalBounds().width, posY);

//			posY +=  scores.at(i).gears.text.getGlobalBounds().height + labelsOffset;
//		}
	}
		break;
	case GAME_OVER:
	{
		const Vector2f offset = Vector2f(gameOverImg.sprite.getGlobalBounds().width/2, gameOverImg.sprite.getGlobalBounds().height/2);
		gameOverImg.sprite.setPosition(level->getCenter() - offset);
	}
		break;
	}
	m_state = state;
}

void GameWindow::finish()
{
	Engine::Instance().setState(Engine::MAIN_LOCATION);
}

