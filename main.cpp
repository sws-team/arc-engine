
#include <iostream>
#include <SFML/Window/Cursor.hpp>
#include <SFML/Window.hpp>






#include "mainwindow.h"
#include "engine.h"
#include "managers.h"
#include "gameoptions.h"
#include "gamestatemanager.h"
#include "gamemanagers.h"
#include "gameachievements.h"

#ifdef OS_WIN
#include "windows.h"
#endif

#ifdef STEAM_API
#include "steam_api.h"
#endif

#include <iostream>
#include <fstream>

/*
int main()
{
#ifdef STEAM_API
	if (SteamAPI_RestartAppIfNecessary(1262070))
		return EXIT_FAILURE;

	if (!SteamAPI_Init())
	{
//		MessageBoxA(NULL, "Steam must be running to play this game", "Fatal Error!", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
#endif
	std::srand(std::time(nullptr));
	sf::String path = sf::String("argv[0]");
	const sf::String appName = sf::String("TowerDefence_") + sf::String(APP_VERSION)
#ifdef OS_WIN
			+ sf::String(".exe")
#endif
	;
	path.replace(appName, sf::String());


	Engine::Instance();



	// Declare and create a new window
	sf::Window window(sf::VideoMode(800, 600), "SFML window");
	// Limit the framerate to 60 frames per second (this step is optional)
	window.setFramerateLimit(60);
	// The main loop - ends as soon as the window is closed
	while (window.isOpen())
	{
	   // Event processing
	   sf::Event event;
	   while (window.pollEvent(event))
	   {
		   // Request for closing the window
		   if (event.type == sf::Event::Closed)
			   window.close();
	   }
	   // Activate the window for OpenGL rendering
	   window.setActive();
	   // OpenGL drawing commands go here...
	   // End the current frame and display its contents on screen
	   window.display();
	}
	return 0;
}
*/

int main(int argc, char *argv[])
{
#ifdef STEAM_API
	if (SteamAPI_RestartAppIfNecessary(1262070))
		return EXIT_FAILURE;

	if (!SteamAPI_Init())
	{
//		MessageBoxA(NULL, "Steam must be running to play this game", "Fatal Error!", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
#endif
	std::srand(std::time(nullptr));
	sf::String path = sf::String("argv[0]");
	const sf::String appName = sf::String("TowerDefence_") + sf::String(APP_VERSION)
#ifdef OS_WIN
			+ sf::String(".exe")
#endif
	;
	path.replace(appName, sf::String());

	Engine::Instance();

	GameManagers::loadResources();
	GameManagers::loadTranslations();

	Engine::Instance().translationsManager()->setCurrentLanguage("rus");
	Engine::Instance().globalVariables()->setApplicationPath(path);
	Engine::Instance().globalVariables()->loadGameSettings();

	Engine::Instance().setStateManager(new GameStateManager);
	Engine::Instance().setOptions(new GameOptions);
	Engine::Instance().options<GameOptions>()->load();
//	Engine::Instance().soundManager()->setQuiet(true);


//!!! cout << SteamUtils()->GetSteamUILanguage()<<endl;

	if (!Engine::Instance().options<GameOptions>()->checkMaps("maps"))
	{
#ifdef OS_WIN
//		MessageBoxA(NULL, "Checksum error", "Fatal Error!", MB_OK | MB_ICONERROR);
#endif
		return EXIT_FAILURE;
	}

	Engine::Instance().globalVariables()->setAppName("TowerDefence");
	Engine::Instance().options<GameOptions>()->loadAchievements();
	GameAchievements::Instance().requestStats();

	MainWindow w;
	Engine::Instance().options<GameOptions>()->setMainWindow(&w);
	Engine::Instance().getOptions()->updateWindow();

	const int result = w.exec();
	Engine::Instance().clearInstance();
#ifdef STEAM_API
	SteamAPI_Shutdown();
#endif
	return result;
}
