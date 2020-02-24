#include "mainwindow.h"
#include "engine.h"
#include "managers.h"
#include "gameoptions.h"
#include "gamestatemanager.h"
#include "gamemanagers.h"

#ifdef OS_WIN
#include "windows.h"
#endif

#ifdef STEAM_API
#include "steam_api.h"
#endif

#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
#ifdef STEAM_API
//	if (SteamAPI_RestartAppIfNecessary(48))
//		return EXIT_FAILURE;
	if ( !SteamAPI_Init() )
	{
		MessageBoxA(NULL, "Steam must be running to play this game", "Fatal Error!", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
#endif
	std::srand(std::time(nullptr));
    String path = String(argv[0]);
	const String appName = String("TowerDefence_") + String(APP_VERSION)
#ifdef OS_WIN
			+ String(".exe")
#endif
	;
	path.replace(appName, String());

	Engine::Instance();

	GameManagers::loadTextures();
	GameManagers::loadTranslations();
	GameManagers::loadSounds();
	GameManagers::loadMusic();

	Engine::Instance().translationsManager()->setCurrentLanguage("rus");
	Engine::Instance().globalVariables()->setApplicationPath(path);
	Engine::Instance().globalVariables()->loadGameSettings();

	Engine::Instance().setStateManager(new GameStateManager);
	Engine::Instance().setOptions(new GameOptions);
	Engine::Instance().options<GameOptions>()->load();


	if (!Engine::Instance().options<GameOptions>()->checkMaps("maps"))
	{
#ifdef OS_WIN
		MessageBoxA(NULL, "Checksum error", "Fatal Error!", MB_OK | MB_ICONERROR);
#endif
		return EXIT_FAILURE;
	}

	Engine::Instance().globalVariables()->setAppName("TowerDefence");

	MainWindow w;
	Engine::Instance().options<GameOptions>()->setMainWindow(&w);
	Engine::Instance().options<GameOptions>()->updateWindow();

	const int result = w.exec();
	Engine::Instance().clearInstance();
#ifdef STEAM_API
	SteamAPI_Shutdown();
#endif
	return result;
}
