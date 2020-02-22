#include "soundcontroller.h"
#include "settings.h"
#include "Engine/engine.h"

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
//	return;
	if (currentBackground == fileName)
		return;
	currentBackground = fileName;
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
	play(fileName, false);
}

void SoundController::playLooped(const string &fileName)
{
	play(fileName, true);
}

void SoundController::stop(const string &fileName)
{
	auto it = sounds.find(fileName);
	if (it != sounds.end())
		sounds[fileName].sound->stop();
}

void SoundController::play(const string &fileName, bool loop)
{
	//	return;
	if(fileName.empty())
		return;
	auto it = sounds.find(fileName);
	if (it == sounds.end())
	{
		SFX sfx;
		sfx.buffer = new SoundBuffer;
		sfx.sound = new Sound(*sfx.buffer);
		sfx.sound->setLoop(loop);
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
	for(const pair<string, SFX>& sfx : sounds)
		sfx.second.sound->setVolume(Settings::Instance().getSoundLevel());
}

void SoundController::pauseMusic()
{
	music->pause();
	for(const pair<string, SFX>& sfx : sounds)
	{
		if (sfx.second.sound->getStatus() == SoundSource::Playing)
			sfx.second.sound->pause();
	}
}

void SoundController::unpauseMusic()
{
	music->play();
	for(const pair<string, SFX>& sfx : sounds)
	{
		if (sfx.second.sound->getStatus() == SoundSource::Paused)
			sfx.second.sound->play();
	}
}

void SoundController::setMusicLooped(bool loop)
{
	music->setLoop(loop);
}

bool SoundController::isMusicFinished() const
{
	return music->getStatus() == Sound::Stopped;
}

