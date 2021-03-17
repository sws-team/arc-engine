#include "gameplatform.h"
#include <iostream>

#ifdef STEAM_PLATFORM
#include "steamplatform.h"
#endif

GamePlatform::GamePlatform()
{
	adapter = new GAME_PLATFORM();
}

GamePlatform &GamePlatform::Instance()
{
	static GamePlatform singleton;
	return singleton;
}

GAME_PLATFORM *GamePlatform::platform()
{
	return static_cast<GAME_PLATFORM*>(adapter);
}

bool GamePlatform::initialize()
{
	initialized = adapter->initialize();
	return initialized;
}

void GamePlatform::process()
{
	return adapter->process();
}

bool GamePlatform::terminate()
{
	return adapter->terminate();
}

bool GamePlatform::clear()
{
	return adapter->clear();
}

bool GamePlatform::isInitialized() const
{
	return initialized;
}

bool GamePlatform::unlock(AchievementType type)
{
	if (!initialized)
		return false;
	return adapter->unlock(type);
}

bool GamePlatform::setStat(StatType type, uint32_t value)
{
	if (!initialized)
		return false;
	return adapter->setStat(type, value);
}

bool GamePlatform::incrementStat(StatType type)
{
	if (!initialized)
		return false;
	return adapter->incrementStat(type);
}


bool GamePlatform::saveFile(const std::string &fileName, const std::string &data)
{
	return adapter->saveFile(fileName, data);
}

const std::string GamePlatform::readFile(const std::string &fileName)
{
	return adapter->readFile(fileName);
}

bool GamePlatform::deleteFile(const std::string &fileName)
{
	return adapter->deleteFile(fileName);
}

std::string GamePlatform::nick() const
{
	return adapter->nick();
}
