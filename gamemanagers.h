#ifndef GAMEMANAGERS_H
#define GAMEMANAGERS_H

#include "managers.h"

namespace GAME_TEXTURE {

enum TEXTURE_ID
{
	UNKNOWN_TEXTURE = TexturesManager::USER_TEXTURE_ID,
	MANUAL_BACKGROUND,
	MISSON_BACKGROUND,

	//ui
	PANEL_TEXTURE,
	CURSOR_TEXTURE,
	STAR_TEXTURE,
	EMPTY_STAR_TEXTURE,
	SELL_TEXTURE,
	UPGRADE_TEXTURE,
	GAME_MENU_TEXTURE,
	WINDOW_TEXTURE,
	PANEL_CURSOR,
	MONEY_ICON,
	LIFE_ICON,
	DIRECTION_TEXTURE,
	LOCKED_ICON,
	INSTRUCTIONS_TEXTURE,
	NEXT,
	PREVIOUS,
	EXIT,
	CREDITS,

	ABILITY_BOMB,
	ABILITY_FREEZE_BOMB,
	ABILITY_ACID,
	ABILITY_INCREASE_TOWER_DAMAGE,
	ABILITY_INCREASE_TOWER_ATTACK_SPEED,
	ABILITY_TIME_STOP,

	TOWER_BASE,
	TOWER_POWER,
	TOWER_ROCKET,
	TOWER_FREEZE,
	TOWER_LASER,
	TOWER_IMPROVED,

	BASE_PROJECTILE,
	IMPROVED_PROJECTILE,
	ROCKET_PROJECTILE,
	FREEZE_PROJECTILE,
	LASER_PROJECTILE,

	MENU_BACKGROUND,

	//effects
	BASE_EXPLOSION_EFFECT,
	FREEZE_EXPLOSION_EFFECT,
	ROCKET_EXPLOSION_EFFECT,
	IMPROVED_EXPLOSION_EFFECT,
	VENOM_EFFECT,
	BOMB_EXPLOSION,
	FREEZE_BOMB_EXPLOSION,
	HEAL_EFFECT,
	SHELL_EFFECT,
	WEB,
	DOWN_EFFECT,
	DOWNGRADE_PROJECTILE,
	UPGRADE,
	SMOKE,
	EXPLOSION,
	DRAIN,
	REGRESS,
	BURN,
	BLAST,

	//map icons
	MAP_ICON_MISSION_1,
	MAP_ICON_MISSION_2,
	MAP_ICON_MISSION_3,
	MAP_ICON_MISSION_4,
	MAP_ICON_MISSION_5,
	MAP_ICON_MISSION_6,
	MAP_ICON_MISSION_7,
	MAP_ICON_MISSION_8,
	MAP_ICON_MISSION_9,
	MAP_ICON_MISSION_10,
	MAP_ICON_MISSION_11,
	MAP_ICON_MISSION_12,
	MAP_ICON_MISSION_13,
	MAP_ICON_MISSION_14,
	MAP_ICON_MISSION_15,

	//enemies
	ENEMY_SCORPION,
	ENEMY_CAR,
	ENEMY_TRICYCLE,
	ENEMY_TANK,
	ENEMY_SPIDER,
	ENEMY_HELICOPTER,
	ENEMY_AIRCARRIER,
	ENEMY_BIG_TANK,
	ENEMY_PLANE,
	ENEMY_REPAIR,
	ENEMY_SHELL,
	ENEMY_TELEPORT,
	ENEMY_SELF_HEAL,
	ENEMY_DOWN_TOWER,
	ENEMY_ANT,
	ENEMY_COW,

	//objects
	OBJECT_TREE,
};
}
namespace GAME_SOUND {
enum SOUND_ID
{
	UNKNOWN_SOUND = SoundManager::USER_SOUND_ID,

	INTRO,
	SELL,
	TARGET_LOCK,
	TOWER_EXPLOSION,
	BOMB,
	STOP,
	CATCH,
	WARNING,
	WIN,
	SETUP,
	UPGRADE,

	BASE_SHOT,
	FREEZE_SHOT,
	ROCKET_SHOT,
	IMPROVED_SHOT,
	LASER_SHOT,
};
}

namespace GAME_MUSIC {
enum MUSIC_ID
{
	UNKNOWN_SOUND = SoundManager::USER_MUSIC_ID,

	MENU,
	CREDITS,

	TRACK_1,
	TRACK_2,
	TRACK_3,
	TRACK_4,
	TRACK_5,
	TRACK_6,
	TRACK_7,
	TRACK_8,
	FINAL_1,
	FINAL_2,
	FINAL_3,
};
const std::vector<MusicType> TRACKS_FILES =
{
	TRACK_1,
	TRACK_2,
	TRACK_3,
	TRACK_4,
	TRACK_5,
	TRACK_6,
	TRACK_7,
	TRACK_8,
};
const std::vector<MusicType> FINAL_TRACKS_FILES =
{
	FINAL_1,
	FINAL_2,
	FINAL_3,
};

}

namespace GAME_TRANSLATION {
enum TRANSLATION_ID
{
	UNKNOWN = TranslationsManager::USER_TRANSLATION_ID,

	//menu
	CAMPAIGN,
	OPTIONS,
	MANUAL,
	EXIT,

	//panel
	ABILITY_VENOM,
	ABILITY_BOMB,
	ABILITY_FREEZE_BOMB,
	ABILITY_INCREASE_TOWER_ATTACK_SPEED,
	ABILITY_INCREASE_TOWER_DAMAGE,
	ABILITY_STOP,
	TOWER_BASE,
	TOWER_POWER,
	TOWER_ROCKET,
	TOWER_FREEZE,
	TOWER_LASER,
	TOWER_IMPROVED,

	LEVEL,
	DAMAGE_PER_SECOND,
	RADIUS,
	KILLS,
	COST,
	SELL_COST,
	WAVE,

	BASE_TOWER_DESCRIPTION,
	FREEZE_TOWER_DESCRIPTION,
	POWER_TOWER_DESCRIPTION,
	ROCKET_TOWER_DESCRIPTION,
	LASER_TOWER_DESCRIPTION,
	IMPROVED_TOWER_DESCRIPTION,

	BOMB_ABILITY_DESCRIPTION,
	FREEZE_BOMB_ABILITY_DESCRIPTION,
	VENOM_ABILITY_DESCRIPTION,
	INC_DMG_ABILITY_DESCRIPTION,
	INC_AS_ABILITY_DESCRIPTION,
	STOP_ABILITY_DESCRIPTION,

	//Game
	PAUSED,
	CONTINUE,
	RESTART,
	EXIT_TO_MENU,
	EXIT_FROM_GAME,

	GAME_OVER,
	CONGRATULATIONS,
	START_GAME,

	//Instructions
	INSTRUCTION_WELCOME,
	INSTRUCTION_TOWERS,
	INSTRUCTION_ABILITIES,
	INSTRUCTION_MONEY,
	INSTRUCTION_HEALTH,
	INSTRUCTION_PROGRESS,
	INSTRUCTION_SKIP,

	//enemies
	SCORPION,
	CAR,
	TRICYCLE,
	TANK,
	SPIDER,
	HELICOPTER,
	AIRCARRIER,
	BIG_TANK,
	PLANE,
	REPAIR,
	SHELL,
	TELEPORT,
	SELF_HEAL,
	DOWN_TOWER,
	ANT,
	COW,

	//manual
	SPEED,
	SIZE,
	ARMOR,
	HEALTH,
	ABILITY,
	SMALL,
	MID,
	BIG,
	SLOW,
	NORMAL_SPEED,
	FAST,
	NEXT,
	PREVIOUS,
	CREDITS,
	BACK,

	//difficult
	EASY,
	NORMAL,
	HARD,
};
}

class GameManagers
{
public:
	GameManagers();

	static void loadResources();
	static void addTexture(TextureType type, const std::string& data);
	static void addSound(SoundType type, const std::string& data);
	static void addMusic(MusicType type, const std::string& data);

	static void loadTranslations();

	static void loadSounds();
	static void loadMusic();
};

#endif // GAMEMANAGERS_H
