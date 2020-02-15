#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include "stdheader.h"

namespace GameAchievements
{
enum AchievmentsTypes
{
	COMPLETE_ALL_LEVELS,
	COMPLETE_ALL_LEVELS_WITH_5_STARS,
	COMPLETE_LEVEL_WITHOUT_DAMAGE,
	KILL_5_WITH_ONE_BOMB,
	FREEZE_5_WITH_ONE_BOMB,
	KILL_100_BY_ACID,
	KILL_10000_ENEMIES,
	KILL_100_BY_BASE_TOWER,
	KILL_100_BY_FREEZE_TOWER,
	KILL_100_BY_POWER_TOWER,
	KILL_100_BY_ROCKET_TOWER,
	KILL_100_BY_LASER_TOWER,
	KILL_100_BY_IMPROVED_TOWER,

};

const static string COMPLETE_ALL_LEVELS_STR = "complete_all_levels";
const static string COMPLETE_ALL_LEVELS_WITH_5_STARS_STR = "complete_all_levels_with_5_stars";
const static string COMPLETE_LEVEL_WITHOUT_DAMAGE_STR = "complete_level_without_damage";
const static string KILL_5_WITH_ONE_BOMB_STR = "kill_5_with_one_bomb";
const static string FREEZE_5_WITH_ONE_BOMB_STRs = "freeze_5_with_one_bomb";

}

#endif // ACHIEVEMENTS_H
