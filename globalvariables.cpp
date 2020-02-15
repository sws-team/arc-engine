#include "globalvariables.h"
#include "settings.h"
#include "json/json.h"

#ifdef OS_WIN
#include "windows.h"
#include <shlobj.h>
#endif

//#include <tinydir.h>

const Color GlobalVariables::GrayColor = Color(133, 133, 133);
const string settingsFilePath = string("settings.ini");

GlobalVariables &GlobalVariables::Instance()
{
    static GlobalVariables singleton;
    return singleton;
}

GlobalVariables::GlobalVariables()
{
    Settings::Instance().setResolution(getScreenResolution());

	m_font.loadFromFile("font.otf");
}

Vector2i GlobalVariables::getScreenResolution() const
{
	const int screenW =
#ifdef OS_WIN
            GetSystemMetrics(SM_CXSCREEN);
#else
		1920;
#endif
	const int screenH =
#ifdef OS_WIN
            GetSystemMetrics(SM_CYSCREEN);
#else
			1080;
		#endif
    return Vector2i(screenW, screenH);
}

Vector2f GlobalVariables::tileSize() const
{
	return Vector2f(Settings::Instance().getScaleFactor().x * CELL_SIZE,
					Settings::Instance().getScaleFactor().y * CELL_SIZE);
}

Vector2f GlobalVariables::mapTileSize() const
{
	return Vector2f(Settings::Instance().getScaleFactor().x * MAP_CELL_SIZE,
					Settings::Instance().getScaleFactor().y * MAP_CELL_SIZE);
}

void GlobalVariables::saveGameSettings()
{
    ofstream myfile;
	myfile.open(settingsFilePath);
    myfile << Settings::Instance().getFullscreen() << endl
           << Settings::Instance().getResolution().x << endl
           << Settings::Instance().getResolution().y << endl
           << Settings::Instance().getSoundLevel() << endl
           << Settings::Instance().getMusicLevel() << endl
		   << Settings::Instance().difficult() << endl
                 ;
	myfile.close();
}

void GlobalVariables::loadGameSettings()
{
    bool fullscreen = true;
    int width = Settings::Instance().getResolution().x;
    int height = Settings::Instance().getResolution().y;
    int soundLevel = 100;
    int musicLevel = 50;
	float difficult = 1.f;

    ifstream myfile;
	myfile.open(settingsFilePath);
    if (myfile.is_open())
    {
        myfile >> fullscreen
                >> width
                >> height
                >> soundLevel
                >> musicLevel
				>> difficult
                ;
    }
    Settings::Instance().setFullscreen(fullscreen);
    Settings::Instance().setResolution(Vector2i(width, height));
    Settings::Instance().setSoundLevel(soundLevel);
    Settings::Instance().setMusicLevel(musicLevel);
	Settings::Instance().setDifficult(difficult);
    myfile.close();
}

String GlobalVariables::applicationPath() const
{
    return m_applicationPath;
}

void GlobalVariables::setApplicationPath(const String &applicationPath)
{
    m_applicationPath = applicationPath;
}

Font &GlobalVariables::font()
{
    return m_font;
}
