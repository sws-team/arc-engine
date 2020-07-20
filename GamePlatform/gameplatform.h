#ifndef GAMEACHIEVEMENTS_H
#define GAMEACHIEVEMENTS_H

#include "stdheader.h"
#include "steam_api.h"

typedef int AchievementType;
typedef int StatType;

class GamePlatform
{
public:
	static GamePlatform &Instance();

	void addAchievement(AchievementType type, const std::string& name);
	void addStat(StatType type, const std::string& name);

	bool unlock(AchievementType type);
	bool setValue(StatType type, int32 value);
	bool incrementValue(StatType type);

	bool requestStats();

	bool saveFile(const std::string& fileName, const std::string& data);
	const std::string readFile(const std::string& fileName);

	bool deleteFile(const std::string& fileName);
	bool clearAll();

	void setPlatformState(const std::string& state, const std::string& text);
	void setStatsReceivedCallback(const std::function<void ()> &func);

private:
	GamePlatform();

	GamePlatform(const GamePlatform& root) = delete;
	GamePlatform& operator=(const GamePlatform&) = delete;

	struct Achievement
	{
		std::string name;
		bool achieved;
	};
	uint64 appId;
	bool initialized;

	std::map<AchievementType, Achievement> m_achievements;

	struct Stat
	{
		std::string name;
//		enum StatTypes
//		{
//			STAT_INT,
//			STAT_FLOAT,
//			STAT_STRING,
//		};
//		StatTypes type;
//		union StatValue
//		{
//			int32 intVal;
//			float floatVal;
//		};
//		StatValue value;
		int32 value;
	};
	std::map<StatType, Stat> m_stats;

	STEAM_CALLBACK( GamePlatform, OnUserStatsReceived, UserStatsReceived_t,
					m_CallbackUserStatsReceived );
//	STEAM_CALLBACK( GameAchievements, OnUserStatsStored, UserStatsStored_t,
//					m_CallbackUserStatsStored );
	STEAM_CALLBACK( GamePlatform, OnAchievementStored,
					UserAchievementStored_t, m_CallbackAchievementStored );

	std::function<void()> statsReceivedFunc;
};

#endif // GAMEACHIEVEMENTS_H
