#include "gameplatform.h"

#ifdef OS_WIN
#include "windows.h"
#endif

GamePlatform::GamePlatform()
	: appId(0)
	,initialized(false)
	,m_CallbackUserStatsReceived( this, &GamePlatform::OnUserStatsReceived )
//	,m_CallbackUserStatsStored( this, &GameAchievements::OnUserStatsStored )
	,m_CallbackAchievementStored( this, &GamePlatform::OnAchievementStored )
	,statsReceivedFunc(nullptr)
{
	appId = SteamUtils()->GetAppID();
}

GamePlatform &GamePlatform::Instance()
{
	static GamePlatform singleton;
	return singleton;
}

void GamePlatform::addAchievement(AchievementType type, const std::string &name)
{
	Achievement achievement;
	achievement.name = name;
	achievement.achieved = false;
	m_achievements.insert(std::pair<AchievementType, Achievement>(type, achievement));
}

void GamePlatform::addStat(StatType type, const std::string &name)
{
	Stat stat;
	stat.name = name;
	stat.value = 0;
	m_stats.insert(std::pair<StatType, Stat>(type, stat));
}

bool GamePlatform::unlock(AchievementType type)
{
	// Have we received a call back from Steam yet?
	if (!initialized)
		return false;

	const Achievement achievement = m_achievements.at(type);
	if (achievement.achieved)
		return true;

	const std::string name = achievement.name;
	SteamUserStats()->SetAchievement(name.c_str());
	m_achievements[type].achieved = true;
	return SteamUserStats()->StoreStats();
}

bool GamePlatform::setValue(StatType type, int32 value)
{
	if (!initialized)
		return false;

	const Stat stat = m_stats.at(type);
	const std::string name = stat.name;
	const bool isSet = SteamUserStats()->SetStat(name.c_str(), value);
	if (isSet)
		m_stats[type].value = value;
	return SteamUserStats()->StoreStats();
}

bool GamePlatform::incrementValue(StatType type)
{
	const Stat stat = m_stats.at(type);
	return setValue(type, stat.value + 1);
}

bool GamePlatform::requestStats()
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

bool GamePlatform::saveFile(const std::string &fileName, const std::string &data)
{
	if (SteamRemoteStorage() == nullptr)
		return false;
	return SteamRemoteStorage()->FileWrite(fileName.c_str(), data.c_str(), data.size());
}

const std::string GamePlatform::readFile(const std::string &fileName)
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

bool GamePlatform::deleteFile(const std::string &fileName)
{
	if (SteamRemoteStorage() == nullptr)
		return false;
	if (!SteamRemoteStorage()->FileExists(fileName.c_str()))
		return true;
	return SteamRemoteStorage()->FileDelete(fileName.c_str());
}

bool GamePlatform::clearAll()
{
	if (SteamUserStats() == nullptr || SteamUser() == nullptr)
		return false;
	// Is the user logged on?  If not we can't get stats.
	if ( !SteamUser()->BLoggedOn() )
		return false;

	return SteamUserStats()->ResetAllStats(true);
}

void GamePlatform::setPlatformState(const std::string &state, const std::string &text)
{
	SteamFriends()->SetRichPresence(state.c_str(), text.c_str());
}

void GamePlatform::setStatsReceivedCallback(const std::function<void ()> &func)
{
	statsReceivedFunc = func;
}

void GamePlatform::OnAchievementStored(UserAchievementStored_t *pCallback)
{
	if (appId != pCallback->m_nGameID)
		return;

//	std::cout << "Stored Achievement for Steam"<<std::endl;
}

void GamePlatform::OnUserStatsReceived( UserStatsReceived_t *pCallback )
{
	if (appId != pCallback->m_nGameID)
		return;

	if (pCallback->m_eResult != k_EResultOK)
		return;

	//	char buffer[128];
	//	_snprintf( buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult );
	//	OutputDebugString( buffer );

	initialized = true;
	for(const std::pair<AchievementType, Achievement>& achievement : m_achievements)
	{
		bool achieved;
		SteamUserStats()->GetAchievement(achievement.second.name.c_str(), &achieved);
		m_achievements[achievement.first].achieved = achieved;
//		std::cout << achievement.second.name << " - " << achieved<<std::endl;
	}
	for(const std::pair<StatType, Stat>& stat : m_stats)
	{
		int32 value;
		SteamUserStats()->GetStat(stat.second.name.c_str(), &value);
		m_stats[stat.first].value = value;
//		std::cout << stat.second.name << " - " << value<<std::endl;
	}
	if (statsReceivedFunc != nullptr)
		statsReceivedFunc();
}

//void GameAchievements::OnUserStatsStored( UserStatsStored_t *pCallback )
//{
//	if (appId != pCallback->m_nGameID)
//		return;

////	if (pCallback->m_eResult == k_EResultOK)
////	{
////		OutputDebugString( "Stored stats for Steam\n" );
////	}
////	else
////	{
////		char buffer[128];
////		_snprintf( buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult );
////		OutputDebugString( buffer );
////	}
//}

