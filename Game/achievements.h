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
	KILL_10_WITH_ONE_BOMB,
	FREEZE_10_WITH_ONE_BOMB
};

const static string COMPLETE_ALL_LEVELS_STR = "complete_all_levels";
const static string COMPLETE_ALL_LEVELS_WITH_5_STARS_STR = "complete_all_levels_with_5_stars";
const static string COMPLETE_LEVEL_WITHOUT_DAMAGE_STR = "complete_level_without_damage";
const static string KILL_10_WITH_ONE_BOMB_STR = "kill_10_with_one_bomb";
const static string FREEZE_10_WITH_ONE_BOMB_STRs = "freeze_10_with_one_bomb";
}

#endif // ACHIEVEMENTS_H
