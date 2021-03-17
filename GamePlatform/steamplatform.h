#ifndef STEAMPLATFORM_H
#define STEAMPLATFORM_H

#include "steam/steam_api.h"
#include "gameplatform.h"
#include <map>

class SteamPlatform : public PlatformInterface
{
public:
	SteamPlatform();

	bool initialize() override;
	void process() override;
	bool terminate() override;

	bool unlock(AchievementType type) override;
	bool setStat(StatType type, uint32_t value) override;
	bool incrementStat(StatType type) override;
	bool clear() override;

	std::string nick() const override;

	bool saveFile(const std::string& fileName, const std::string& data);
	std::string readFile(const std::string& fileName);
	bool deleteFile(const std::string& fileName);

	void addAchievement(AchievementType type, const std::string& name);
	void addStat(StatType type, const std::string& name);

	bool requestStats();
	void setPlatformState(const std::string& state, const std::string& text);

private:
	uint64_t appId;

	struct Achievement
	{
		std::string name;
		bool achieved;
	};
	std::map<AchievementType, Achievement> m_achievements;

	struct Stat
	{
		std::string name;
		uint32_t value;
	};
	std::map<StatType, Stat> m_stats;

	STEAM_CALLBACK(SteamPlatform, OnUserStatsReceived, UserStatsReceived_t,
					m_CallbackUserStatsReceived);
	STEAM_CALLBACK(SteamPlatform, OnUserStatsStored, UserStatsStored_t,
					m_CallbackUserStatsStored);
	STEAM_CALLBACK(SteamPlatform, OnAchievementStored,
					UserAchievementStored_t, m_CallbackAchievementStored);
};

#endif // STEAMPLATFORM_H
