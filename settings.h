#ifndef SETTINGS_H
#define SETTINGS_H

#include "stdheader.h"

class Settings
{
public:
    static Settings &Instance();

    Vector2i getResolution() const;
    void setResolution(const Vector2i &value);

	float gameScale() const;


    int getSoundLevel() const;
    void setSoundLevel(int value);

    bool getFullscreen() const;
    void setFullscreen(bool value);

    int getMusicLevel() const;
    void setMusicLevel(int value);

	Vector2f getScaleFactor() const;	
	Vector2f getOriginalScaleFactor() const;

	Vector2i getInscribedResolution() const;

	void setMainWindow(RenderWindow *window);
	void updateWindow();

	static const Vector2i defaultResolution;
private:
	constexpr static float GAME_SCALE = 0.85f;
    Settings();
    Settings(Settings const&) = delete;
    Settings& operator= (Settings const&) = delete;

    Vector2i resolution;
    int soundLevel;
    int musicLevel;
    bool fullscreen;

	RenderWindow *mainWindow;
};

#endif // SETTINGS_H
