#include "mainwindow.h"
#include "gameoptions.h"
#include "gameplatform.h"
#include "gamestatemanager.h"

#ifdef OS_WIN
#include "windows.h"
#endif

#ifdef STEAM_API
#include "steam_api.h"
#endif

#include <iostream>
#include <fstream>

void loading();
void showLoading();

#ifdef OS_WIN
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nShowCmd)
#endif
#ifdef OS_UNIX
int main(int argc, char *argv[])
#endif
{
	std::srand(std::time(nullptr));
	Engine::Instance();
#ifdef DEMO_VERSION
	Engine::Instance().globalVariables()->setAppName("Arc Defence Demo");
#else
	Engine::Instance().globalVariables()->setAppName("Arc Defence");
#endif
	showLoading();
	const int steamAppId =
		#ifdef DEMO_VERSION
			1296240
		#else
			1262070
		#endif
			;
#ifdef STEAM_API
	if (SteamAPI_RestartAppIfNecessary(steamAppId))
		return EXIT_FAILURE;

	if (!SteamAPI_Init())
	{
#ifdef OS_WIN
		MessageBoxA(NULL, "Steam must be running to play this game", "Fatal Error!", MB_OK | MB_ICONERROR);
#endif
		return EXIT_FAILURE;
	}
#endif
	const std::string steamLanguage = std::string(SteamUtils()->GetSteamUILanguage());
	Engine::Instance().translationsManager()->setCurrentLanguage(steamLanguage);
	Engine::Instance().globalVariables()->loadGameSettings(GamePlatform::Instance().readFile(
															   EngineDefs::settingsFilePath));
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
	Engine::Instance().options<GameOptions>()->loadAchievements();
	GamePlatform::Instance().setStatsReceivedCallback(std::bind(&GameOptions::checkCompletedMissions, Engine::Instance().options<GameOptions>()));
	GamePlatform::Instance().requestStats();
	Engine::Instance().fontManager()->setFontModifier(0.75f);

#ifdef OS_WIN
	Engine::Instance().options<GameOptions>()->setDev(std::string(lpCmdLine) == std::string("-dev"));
#endif
	MainWindow w;
	Engine::Instance().options<GameOptions>()->setMainWindow(&w);
	Engine::Instance().getOptions()->updateWindow();
	w.exec();

	Engine::Instance().options<GameOptions>()->save();
	GamePlatform::Instance().saveFile(EngineDefs::settingsFilePath,
									  Engine::Instance().globalVariables()->saveGameSettings());

	Engine::Instance().clearInstance();

#ifdef STEAM_API
	SteamAPI_Shutdown();
#endif
	return EXIT_SUCCESS;
}

