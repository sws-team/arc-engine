#include "mainwindow.h"
#include "globalvariables.h"
#include "Engine/engine.h"
#include "statewindow.h"
#include <SFML/Window/Cursor.hpp>
#include "settings.h"

MainWindow::MainWindow()
	: RenderWindow ()
	,currentState(nullptr)
{
	state = Engine::UNKNNOWN;
}

MainWindow::~MainWindow()
{
    GlobalVariables::Instance().saveGameSettings();
}

int MainWindow::exec()
{
	Engine::Instance().setWindow(this);
	setMouseCursorGrabbed(true);
	setMouseCursorVisible(false);

	Image img;
	img.loadFromFile("images/ui/cursor.png");
	Cursor cursor;
	const bool cursorLoaded = cursor.loadFromPixels(img.getPixelsPtr(), Vector2u(32,32), Vector2u(0,0));
	while (isOpen())
	{
		if (Engine::Instance().getState() != state)
		{			
			if (currentState != nullptr)
				delete currentState;

			if (cursorLoaded)
				setMouseCursor(cursor);

			if (Engine::Instance().getState() == Engine::EXIT)
			{
				this->close();
//				SavedGameLoader::Instance().save();
				return EXIT_SUCCESS;
			}
			currentState = Engine::Instance().createState(Engine::Instance().getState());
			currentState->init();
			state = Engine::Instance().getState();
			setMouseCursorVisible(state != Engine::INTRO);
		}

		if (currentState == nullptr)
			continue;

		Event event;
		while (pollEvent(event))
			currentState->eventFilter(&event);
		currentState->update();
#ifdef STEAM_API
		SteamAPI_RunCallbacks();
#endif
		clear(Color::Black);
		currentState->paint(this);
		if (GlobalVariables::Instance().isEnabledFPS())
			drawFPS();

		display();
	}
	return EXIT_SUCCESS;
}

void MainWindow::drawFPS()
{
	Text fps;
	fps.setFont(GlobalVariables::Instance().font());
	fps.setCharacterSize(50);
	fps.setFillColor(Color::Yellow);
	const float currentTime = clock.restart().asSeconds();
	const float fpsValue = 1.f / currentTime;
	fps.setString(to_string(static_cast<int>(floor(fpsValue))));
	fps.setPosition(Settings::Instance().getResolution().x - fps.getGlobalBounds().width, 0);
	draw(fps);
}
