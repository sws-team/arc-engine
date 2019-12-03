#ifndef SETTINGS_H
#define SETTINGS_H

#include "stdheader.h"

class Settings
{
public:
    static Settings &Instance();

    Vector2i getResolution() const;
    void setResolution(const Vector2i &value);

	constexpr static float GAME_SCALE = 0.85f;

	float getSoundLevel() const;
	void setSoundLevel(float value);

    bool getFullscreen() const;
    void setFullscreen(bool value);

	float getMusicLevel() const;
	void setMusicLevel(float value);

	Vector2f getScaleFactor() const;	
	Vector2f getOriginalScaleFactor() const;
	Vector2f getGameScaleFactor() const;

	Vector2i getInscribedResolution() const;

	void setMainWindow(RenderWindow *window);
	void updateWindow();

	static const Vector2i defaultResolution;
private:
    Settings();
    Settings(Settings const&) = delete;
    Settings& operator= (Settings const&) = delete;

    Vector2i resolution;
	float soundLevel;
	float musicLevel;
    bool fullscreen;

	RenderWindow *mainWindow;
};

#endif // SETTINGS_H
