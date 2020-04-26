#include "mainwindow.h"
#include "engine.h"
#include "managers.h"
#include "gameoptions.h"
#include "gamestatemanager.h"
#include "gamemanagers.h"
#include "gameplatform.h"
#include "Game/Balance/balance.h"

#ifdef OS_WIN
#include "windows.h"
#endif

#ifdef STEAM_API
#include "steam_api.h"
#endif

#include <iostream>
#include <fstream>

#ifdef OS_WIN
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nShowCmd)
#endif
#ifdef OS_UNIX
int main(int argc, char *argv[])
#endif
{
#ifdef STEAM_API
	if (SteamAPI_RestartAppIfNecessary(1262070))
		return EXIT_FAILURE;

	if (!SteamAPI_Init())
	{
		MessageBoxA(NULL, "Steam must be running to play this game", "Fatal Error!", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
#endif

	std::srand(std::time(nullptr));

	Engine::Instance();
	GameManagers::loadResources();
	GameManagers::loadTranslations();
	Balance::Instance().load();

	const std::string steamLanguage = std::string(SteamUtils()->GetSteamUILanguage());
	Engine::Instance().translationsManager()->setCurrentLanguage(steamLanguage);
	Engine::Instance().globalVariables()->loadGameSettings(GamePlatform::Instance().readFile(EngineDefs::settingsFilePath));
	Engine::Instance().setStateManager(new GameStateManager);
	Engine::Instance().setOptions(new GameOptions);
	Engine::Instance().options<GameOptions>()->load();
	if (!Engine::Instance().options<GameOptions>()->verifyChecksum())
	{
#ifdef OS_WIN
		MessageBoxA(NULL, "Checksum error", "Fatal Error!", MB_OK | MB_ICONERROR);
#endif
		return EXIT_FAILURE;
	}
#ifdef DEMO_DEMO_VERSION
	Engine::Instance().globalVariables()->setAppName("Arc Defence Demo");
#else
	Engine::Instance().globalVariables()->setAppName("Arc Defence");
#endif
	Engine::Instance().options<GameOptions>()->loadAchievements();
	GamePlatform::Instance().requestStats();
	Engine::Instance().fontManager()->setFontModifier(0.75f);

	MainWindow w;
	Engine::Instance().options<GameOptions>()->setMainWindow(&w);
	Engine::Instance().getOptions()->updateWindow();
	w.exec();

	Engine::Instance().options<GameOptions>()->save();
	GamePlatform::Instance().saveFile(EngineDefs::settingsFilePath, Engine::Instance().globalVariables()->saveGameSettings());

	Engine::Instance().clearInstance();

#ifdef STEAM_API
	SteamAPI_Shutdown();
#endif
	return EXIT_SUCCESS;
}
