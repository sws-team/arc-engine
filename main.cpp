#include "mainwindow.h"
#include "globalvariables.h"
#include "settings.h"
#include "savedgame.h"
#include "Game/mission.h"
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

int main(int argc, char *argv[])
{
//    if ( SteamAPI_RestartAppIfNecessary( k_uAppIdInvalid ) ) // Replace with your App ID
//    {

//    }
	std::srand(std::time(nullptr));

    String path = String(argv[0]);

	const String appName = String("Game_") + String(APP_VERSION)
#ifdef TEST_BUILD
			+ String("_test")
#endif
#ifdef OS_WIN
			+ String(".exe")
#endif
	;
	path.replace(appName, String());

    GlobalVariables::Instance().setApplicationPath(path);

	GlobalVariables::Instance().loadGameSettings();

#ifdef STEAM_API
    if ( !SteamAPI_Init() )
    {
        printf( "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed).\n" );
        return 1;
    }
#endif
#ifdef CRASH_REPORTER
    Breakpad::CrashHandler::Init("desktop");
#endif

	Language::Instance().load();
//	if (!Campaign::Instance().loadWeapons("weapons"))
//	{
//		MessageBoxA(nullptr, "Cant read weapons file", "Error", 0);
//		return EXIT_FAILURE;
//	}
//	if (!Campaign::Instance().loadSpaceShips("spaceships"))
//	{
//		MessageBoxA(nullptr, "Cant read spaceships file", "Error", 0);
//		return EXIT_FAILURE;
//	}
//	if (!Campaign::Instance().loadMissions("campaign"))
//	{
//		MessageBoxA(nullptr, "Cant read campaign file", "Error", 0);
//		return EXIT_FAILURE;
//	}
	SavedGameLoader::Instance().load();

	MainWindow w;
	Settings::Instance().setMainWindow(&w);
	Settings::Instance().updateWindow();
    return w.exec();
}
