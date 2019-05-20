#include "settings.h"

const Vector2i Settings::defaultResolution = Vector2i(1920, 1080);

Settings &Settings::Instance()
{
    static Settings s;
    return s;
}

Settings::Settings():
	resolution(defaultResolution)
  ,soundLevel(100)
  ,musicLevel(100)
  ,fullscreen(true)
  ,mainWindow(nullptr)
{
	resolution = defaultResolution;
}

int Settings::getMusicLevel() const
{
    return musicLevel;
}

void Settings::setMusicLevel(int value)
{
	musicLevel = value;
}

Vector2f Settings::getScaleFactor() const
{
	const Vector2i res = getInscribedResolution();
	return Vector2f(static_cast<float>(res.x)/defaultResolution.x,
					static_cast<float>(res.y)/defaultResolution.y);
}

Vector2f Settings::getOriginalScaleFactor() const
{
	return Vector2f(static_cast<float>(resolution.x)/defaultResolution.x,
					static_cast<float>(resolution.y)/defaultResolution.y);
}

Vector2i Settings::getInscribedResolution() const
{
	return Vector2i(resolution.x, resolution.x * 9/16);
}

void Settings::setMainWindow(RenderWindow *window)
{
	mainWindow = window;
}

void Settings::updateWindow()
{
	mainWindow->create(VideoMode(resolution.x, resolution.y),
					   String("window - ") + String(APP_VERSION),
					   fullscreen?Style::Fullscreen:Style::Default,
					   ContextSettings(0, 0, 8)
					   );
	mainWindow->setVerticalSyncEnabled(true);
//	mainWindow->setFramerateLimit(60); // Set a framrate limit to reduce the CPU load
//	mainWindow->setMouseCursorVisible(false); // Hide the cursor
}

bool Settings::getFullscreen() const
{
	return fullscreen;
}

void Settings::setFullscreen(bool value)
{
    fullscreen = value;
}

int Settings::getSoundLevel() const
{
    return soundLevel;
}

void Settings::setSoundLevel(int value)
{
    soundLevel = value;
}

Vector2i Settings::getResolution() const
{
	return resolution;
}

void Settings::setResolution(const Vector2i &value)
{
    resolution = value;
}
