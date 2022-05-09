#ifndef GAMEACHIEVEMENTS_H
#define GAMEACHIEVEMENTS_H

#include <string>

typedef int AchievementType;
typedef int StatType;

class PlatformInterface
{
public:
	virtual bool initialize() {
		return true;
	}
	virtual void process() {

	}
	virtual bool terminate() {
		return true;
	}

	virtual std::string nick() const {
		return std::string();
	}

	virtual bool unlock(AchievementType type) {
		return true;
	}
	virtual bool setStat(StatType type, uint32_t value) {
		return true;
	}
	virtual bool incrementStat(StatType type) {
		return true;
	}
	virtual bool clear() {
		return true;
	}

	virtual bool saveFile(const std::string& fileName, const std::string& data) {
		return true;
	}
	virtual const std::string readFile(const std::string& fileName) {
		return std::string();
	}
	virtual bool deleteFile(const std::string& fileName) {
		return true;
	}

private:
};

class GAME_PLATFORM;
class GamePlatform
{
public:
	static GamePlatform &Instance();

	GAME_PLATFORM *platform();

	bool initialize();
	void process();
	bool terminate();
	bool isInitialized() const;

	bool unlock(AchievementType type);
	bool setStat(StatType type, uint32_t value);
	bool incrementStat(StatType type);
	bool clear();

	bool saveFile(const std::string& fileName, const std::string& data);
	const std::string readFile(const std::string& fileName);
	bool deleteFile(const std::string& fileName);

	std::string nick() const;

protected:
	bool initialized = false;
private:
	GamePlatform();

	GamePlatform(const GamePlatform& root) = delete;
	GamePlatform& operator=(const GamePlatform&) = delete;

	PlatformInterface *adapter = nullptr;
};

#endif // GAMEACHIEVEMENTS_H
