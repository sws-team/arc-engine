#include "mainwindow.h"
#include "globalvariables.h"
#include "Engine/engine.h"
#include "statewindow.h"

MainWindow::MainWindow()
	: RenderWindow ()
	,currentState(nullptr)
{
	GlobalVariables::Instance().loadControls();
	state = Engine::UNKNNOWN;
}

MainWindow::~MainWindow()
{
    GlobalVariables::Instance().saveGameSettings();
}

int MainWindow::exec()
{
	Engine::Instance().setWindow(this);
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
//			if (state == Engine::IN_GAME)
//				this->setView(this->getDefaultView());

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
#ifdef STEAM_API
		SteamAPI_RunCallbacks();
#endif
	}
    return EXIT_SUCCESS;
}
