#ifndef SOUNDCONTROLLER_H
#define SOUNDCONTROLLER_H

#include <stdheader.h>
#include "soundsdef.h"

class SoundController
{
public:
    static SoundController &Instance();
    ~SoundController();
    void startBackgroundSound(const string& fileName);
    void endBackgroundSound();

	void playOnce(const string& fileName);
	void playLooped(const string& fileName);
	void stop(const string& fileName);

    void updateVolume();

	void pauseMusic();
	void unpauseMusic();

private:
    SoundController();
    SoundController(const SoundController& root);
    SoundController& operator=(const SoundController&);

	Music *music;

	struct SFX
	{
		SoundBuffer *buffer;
		Sound *sound;
	};
	map<string, SFX> sounds;
	string currentBackground;

	void play(const string& fileName, bool loop);
};

#endif // SOUNDCONTROLLER_H
