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
#endif
    const int screenH =
#ifdef OS_WIN
            GetSystemMetrics(SM_CYSCREEN);
#endif
    return Vector2i(screenW, screenH);
}

Vector2f GlobalVariables::tileSize() const
{
	return Vector2f(Settings::Instance().getScaleFactor().x * CELL_SIZE,
					Settings::Instance().getScaleFactor().y * CELL_SIZE);
}

void GlobalVariables::createDefaultControls()
{
	{//keyboard
		ControlSettings keyboardControls;

		keyboardControls.moveUp = Keyboard::W;
		keyboardControls.moveDown = Keyboard::S;
		keyboardControls.moveLeft = Keyboard::A;
		keyboardControls.moveRight = Keyboard::D;

		keyboardControls.action = Keyboard::T;
		keyboardControls.change = Keyboard::Y;
		keyboardControls.start = Keyboard::L;

		m_controls = keyboardControls;
	}	
//		ControlSettings joystickSettings;

//		joystickSettings.moveUp = Joystick::Y;
//		joystickSettings.moveDown = Joystick::Y;
//		joystickSettings.moveLeft = Joystick::X;
//		joystickSettings.moveRight = Joystick::X;

//		joystickSettings.action = 7;
//		joystickSettings.change = 8;
//		joystickSettings.start = 3;
}

bool GlobalVariables::getControl() const
{
	return m_control;
}

void GlobalVariables::setControl(bool moving)
{
	m_control = moving;
}

ControlSettings GlobalVariables::controls() const
{
	return m_controls;
}

void GlobalVariables::loadControls()
{
	const string fileName = applicationPath().toAnsiString() + string("/controls");
	ifstream stream(fileName);

	Json::Reader reader;
	Json::Value obj;
	if (!reader.parse(stream, obj))
	{
		cout << "cant read controls from: "<<fileName << endl;
		createDefaultControls();
		return;
	}

	Json::Value &jsonKeyboard = obj["keyboard"];
	Json::Value &jsonJoystick = obj["joystick"];

	ControlSettings controls;
	controls.moveUp = jsonKeyboard["moveUp"].asUInt();
	controls.moveDown = jsonKeyboard["moveDown"].asUInt();
	controls.moveLeft = jsonKeyboard["moveLeft"].asUInt();
	controls.moveRight = jsonKeyboard["moveRight"].asUInt();
	controls.action = jsonKeyboard["action"].asUInt();
	controls.change = jsonKeyboard["change"].asUInt();
	controls.start = jsonKeyboard["start"].asUInt();

	m_controls = controls;
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

    ifstream myfile;
	myfile.open(settingsFilePath);
    if (myfile.is_open())
    {
        myfile >> fullscreen
                >> width
                >> height
                >> soundLevel
                >> musicLevel
                ;
    }
    Settings::Instance().setFullscreen(fullscreen);
    Settings::Instance().setResolution(Vector2i(width, height));
    Settings::Instance().setSoundLevel(soundLevel);
    Settings::Instance().setMusicLevel(musicLevel);
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
