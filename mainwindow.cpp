#include "mainwindow.h"
#include "globalvariables.h"
#include "Engine/engine.h"
#include "statewindow.h"

#ifdef TEST_BUILD
#include "Game/gamecontroller.h"
#include "Game/objectsfactory.h"
#include "settings.h"
#endif

MainWindow::MainWindow()
	: RenderWindow ()
	,currentState(nullptr)
{
	GlobalVariables::Instance().loadControls();
	defaultView = this->getView();
	state = Engine::UNKNNOWN;

#ifdef TEST_BUILD
	SavedGameLoader::Instance().setCurrentSave(0);

	GameController::Instance().clear();
	const Vector2f startPos = Vector2f(Settings::Instance().getScaleFactor().x * static_cast<float>(Settings::Instance().getResolution().x)/2,
										Settings::Instance().getScaleFactor().y * static_cast<float>(Settings::Instance().getResolution().y)/2);

	const SavedGame savedGame = SavedGameLoader::Instance().getSavedGame();
	for(const GameState& data : savedGame.playersData)
	{
		SpaceShip *player = ObjectsFactory::Instance().createSpaceShip(data.spaceShip, startPos);
		player->loadData(data);
		GameController::Instance().addPlayer(player);
	}
	SavedGameLoader::Instance().setCurrentMission(0);
	Engine::Instance().setState(Engine::MAIN_LOCATION);
#endif
}

MainWindow::~MainWindow()
{
    GlobalVariables::Instance().saveGameSettings();
}

int MainWindow::exec()
{
	while (isOpen())
	{
		if (Engine::Instance().getState() != state)
		{
			if (currentState != nullptr)
				delete currentState;

			if (Engine::Instance().getState() == Engine::EXIT)
			{
				this->close();
//				SavedGameLoader::Instance().save();
				return EXIT_SUCCESS;
			}
			if (state == Engine::IN_GAME)
			{
				this->setView(this->getDefaultView());
			}
			currentState = Engine::Instance().createState(Engine::Instance().getState());
			currentState->init();
			state = Engine::Instance().getState();
		}

		if (currentState == nullptr)
			continue;

		Event event;
		while (pollEvent(event))
			currentState->eventFilter(&event);
		currentState->update();
		clear(Color::Black);
		currentState->paint(this);
		display();
	}
    return EXIT_SUCCESS;
}
