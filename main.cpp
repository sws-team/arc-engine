#include "mainwindow.h"
#include "globalvariables.h"
#include "settings.h"
#include "Translations/language.h"

#ifdef OS_WIN
#include "windows.h"
#endif

#ifdef CRASH_REPORTER
#include "CrashHandler.h"
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
//	std::cout << path.toAnsiString() << std::endl << appName.toAnsiString() << std::endl;
	path.replace(appName, String());
//	std::cout << path.toAnsiString() << std::endl;

    GlobalVariables::Instance().setApplicationPath(path);
	GlobalVariables::Instance().loadGameSettings();

#ifdef CRASH_REPORTER
    Breakpad::CrashHandler::Init("desktop");
#endif

	Language::Instance().load();
	Engine::Instance().load();

	if (!Engine::Instance().checkMaps("maps"))
	{
		MessageBoxA(NULL, "Checksum error", "Fatal Error!", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}

	MainWindow w;
	Settings::Instance().setMainWindow(&w);
	Settings::Instance().updateWindow();
	const int result = w.exec();
#ifdef STEAM_API
	SteamAPI_Shutdown();
#endif
	return result;
}
