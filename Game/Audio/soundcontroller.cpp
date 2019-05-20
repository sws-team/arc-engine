#include "soundcontroller.h"
#include "settings.h"

SoundController::SoundController()
{
    music = new Music;
	music->setLoop(true);
}

SoundController &SoundController::Instance()
{
    static SoundController instance;
    return instance;
}

SoundController::~SoundController()
{
    endBackgroundSound();
	delete music;
	for(auto it : sounds)
	{
		delete it.second.sound;
		delete it.second.buffer;
	}
}

void SoundController::startBackgroundSound(const string &fileName)
{
	return;
    endBackgroundSound();
	const bool ok = music->openFromFile(fileName);
	if (!ok)
		return;
    updateVolume();
    music->play();
}

void SoundController::endBackgroundSound()
{
    music->stop();
}

void SoundController::playOnce(const string &fileName)
{
	return;
	auto it = sounds.find(fileName);
	if (it == sounds.end())
	{
		SFX sfx;
		sfx.buffer = new SoundBuffer;
		sfx.sound = new Sound(*sfx.buffer);
		sfx.buffer->loadFromFile(fileName);
		sfx.sound->setVolume(Settings::Instance().getSoundLevel());
		sfx.sound->play();
		sounds.insert(pair<string, SFX>(fileName, sfx));
		return;
	}
	it->second.sound->play();
}

void SoundController::updateVolume()
{
	music->setVolume(Settings::Instance().getMusicLevel());
}

