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

    void updateVolume();

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
};

#endif // SOUNDCONTROLLER_H
