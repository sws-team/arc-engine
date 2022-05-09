#include "steamplatform.h"

SteamPlatform::SteamPlatform()
	: PlatformInterface()
	,m_CallbackUserStatsReceived(this, &SteamPlatform::OnUserStatsReceived)
	,m_CallbackUserStatsStored(this, &SteamPlatform::OnUserStatsStored)
	,m_CallbackAchievementStored(this, &SteamPlatform::OnAchievementStored)
{
	appId = GAME_ID;
}

bool SteamPlatform::initialize()
{
	if (SteamAPI_RestartAppIfNecessary(GAME_ID))
		return false;
	if (!SteamAPI_Init())
		return false;
	return true;
}

void SteamPlatform::process()
{
	SteamAPI_RunCallbacks();
}

bool SteamPlatform::terminate()
{
	SteamAPI_Shutdown();
	return true;
}

bool SteamPlatform::clear()
{
	if (SteamUserStats() == nullptr || SteamUser() == nullptr)
		return false;
	// Is the user logged on?  If not we can't get stats.
	if ( !SteamUser()->BLoggedOn() )
		return false;

	return SteamUserStats()->ResetAllStats(true);
}

std::string SteamPlatform::nick() const
{
	return std::string(SteamFriends()->GetFriendPersonaName(SteamUser()->GetSteamID()));
}

bool SteamPlatform::saveFile(const std::string &fileName, const std::string &data)
{
	if (SteamRemoteStorage() == nullptr)
		return false;
	return SteamRemoteStorage()->FileWrite(fileName.c_str(), data.c_str(), data.size());
}

const std::string SteamPlatform::readFile(const std::string &fileName)
{
	std::string result;
	if (SteamRemoteStorage() == nullptr)
		return result;
	if (!SteamRemoteStorage()->FileExists(fileName.c_str()))
		return result;

	const int32 size = SteamRemoteStorage()->GetFileSize(fileName.c_str());
	void *data = malloc(size);
	SteamRemoteStorage()->FileRead(fileName.c_str(), data, size);

	result = std::string((char*)data);
	free(data);
	return result;
}

bool SteamPlatform::deleteFile(const std::string &fileName)
{
	if (SteamRemoteStorage() == nullptr)
		return false;
	if (!SteamRemoteStorage()->FileExists(fileName.c_str()))
		return true;
	return SteamRemoteStorage()->FileDelete(fileName.c_str());
}

bool SteamPlatform::unlock(AchievementType type)
{
	const Achievement achievement = m_achievements.at(type);
	if (achievement.achieved)
		return true;

	const std::string name = achievement.name;
	SteamUserStats()->SetAchievement(name.c_str());
	m_achievements[type].achieved = true;
	return SteamUserStats()->StoreStats();
}

bool SteamPlatform::setStat(StatType type, uint32_t value)
{
	const Stat stat = m_stats.at(type);
	const std::string name = stat.name;
	const bool isSet = SteamUserStats()->SetStat(name.c_str(), static_cast<int32>(value));
	if (isSet)
		m_stats[type].value = value;
	return SteamUserStats()->StoreStats();
}

bool SteamPlatform::incrementStat(StatType type)
{
	const Stat stat = m_stats.at(type);
	return setStat(type, stat.value + 1);
}

void SteamPlatform::addAchievement(AchievementType type, const std::string &name)
{
	Achievement achievement;
	achievement.name = name;
	achievement.achieved = false;
	m_achievements.insert(std::pair<AchievementType, Achievement>(type, achievement));
}

void SteamPlatform::addStat(StatType type, const std::string &name)
{
	Stat stat;
	stat.name = name;
	stat.value = 0;
	m_stats.insert(std::pair<StatType, Stat>(type, stat));
}

bool SteamPlatform::requestStats()
{
	// Is Steam loaded? If not we can't get stats.
	if (SteamUserStats() == nullptr || SteamUser() == nullptr)
		return false;
	// Is the user logged on?  If not we can't get stats.
	if ( !SteamUser()->BLoggedOn() )
		return false;
	// Request user stats.
	return SteamUserStats()->RequestCurrentStats();
}

void SteamPlatform::setPlatformState(const std::string &state, const std::string &text)
{
	SteamFriends()->SetRichPresence(state.c_str(), text.c_str());
}

void SteamPlatform::OnAchievementStored(UserAchievementStored_t *pCallback)
{
	if (appId != pCallback->m_nGameID)
		return;

//	std::cout << "Stored Achievement for Steam"<<std::endl;
}

void SteamPlatform::OnUserStatsReceived(UserStatsReceived_t *pCallback)
{
	if (appId != pCallback->m_nGameID)
		return;

	if (pCallback->m_eResult != k_EResultOK)
		return;

	for(const std::pair<AchievementType, Achievement>& achievement : m_achievements)
	{
		bool achieved;
		SteamUserStats()->GetAchievement(achievement.second.name.c_str(), &achieved);
		m_achievements[achievement.first].achieved = achieved;
//		std::cout << achievement.second.name << " - " << achieved<<std::endl;
	}
	for(const std::pair<StatType, Stat> stat : m_stats)
	{
		int32 value;
		SteamUserStats()->GetStat(stat.second.name.c_str(), &value);
		m_stats[stat.first].value = value;
//		std::cout << stat.second.name << " - " << value<<std::endl;
	}
}

void SteamPlatform::OnUserStatsStored( UserStatsStored_t *pCallback )
{
	if (appId != pCallback->m_nGameID)
		return;

//	if (pCallback->m_eResult == k_EResultOK)
//	{
//		OutputDebugString( "Stored stats for Steam\n" );
//	}
//	else
//	{
//		char buffer[128];
//		_snprintf( buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult );
//		OutputDebugString( buffer );
//	}
}
