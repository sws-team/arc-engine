#ifndef GAMEMANAGERS_H
#define GAMEMANAGERS_H

#include "managers.h"

namespace GAME_TEXTURE {

enum TEXTURE_ID
{
	UNKNOWN_TEXTURE = TexturesManager::USER_TEXTURE_ID,
	MANUAL_BACKGROUND,
	MISSON_BACKGROUND,
	TILES,

	//ui
	PANEL_TEXTURE,
	CURSOR_TEXTURE,
	STAR_TEXTURE,
	EMPTY_STAR_TEXTURE,
	SELL_TEXTURE,
	UPGRADE_TEXTURE,
	FIX_TEXTURE,
	GAME_MENU_TEXTURE,
	WINDOW_TEXTURE,
	PANEL_CURSOR,
	MONEY_ICON,
	LIFE_ICON,
	DIRECTION_TEXTURE,
	LOCKED_ICON,
	INSTRUCTIONS_TOP,
	INSTRUCTIONS_BOTTOM,
	INSTRUCTIONS_VERTICAL,
	ARROW,
	NEXT,
	PREVIOUS,
	EXIT,
	CREDITS,
	LOGO,
	CHARACTER,
	FRAME,
	FRAME_DEMO,
	CHARACTER_FULL,
	NAME_FRAME,
	DEMO_FRAME,

	ABILITY_BOMB,
	ABILITY_FREEZE_BOMB,
	ABILITY_ACID,
	ABILITY_INCREASE_TOWER_DAMAGE,
	ABILITY_INCREASE_TOWER_ATTACK_SPEED,
	ABILITY_TIME_STOP,

	TOWER_BASE_ICON,
	TOWER_POWER_ICON,
	TOWER_ROCKET_ICON,
	TOWER_FREEZE_ICON,
	TOWER_LASER_ICON,
	TOWER_IMPROVED_ICON,

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
	HEAL_SMALL_EFFECT,
	HEAL_MID_EFFECT,
	HEAL_BIG_EFFECT,
	SHELL_EFFECT,
	WEB,
	WEB_EFFECT,
	DOWN_EFFECT,
	DOWNGRADE_PROJECTILE,
	UPGRADE,
	SMOKE,
	EXPLOSION,
	ENERGY_LEECH,
	REGRESS,
	BURN_SMALL,
	BURN_MID,
	BURN_BIG,
	BLAST,
	ENEMY_BULLET,
	ENEMY_ROCKET,
	DOWNGRADE,
	LAVA,
	INVISIBLITY,

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
	ENEMY_INFANTRY,
	ENEMY_WAR_VEHICLE,
	ENEMY_TRICYCLE,
	ENEMY_TANK,
	ENEMY_MECHSPIDER,
	ENEMY_TUATARA,
	ENEMY_GIANT_SLUG,
	ENEMY_HEAVY_TANK,
	ENEMY_SLUGGY,
	ENEMY_REPAIR,
	ENEMY_SHIELD,
	ENEMY_WALKER,
	ENEMY_SELF_HEAL,
	ENEMY_TRACTOR,
	ENEMY_WORM,
	ENEMY_BUGSAURUS,
	ENEMY_SPAWN_WORM,
	ENEMY_JUMPER,
	ENEMY_ROLLING,

	//objects
	OBJECT_TREE,

	//demonstrations
	//show towers
	SHOW_BASE_TOWER,
	SHOW_FREEZE_TOWER,
	SHOW_ENERGY_TOWER,
	SHOW_ROCKET_TOWER,
	SHOW_LASER_TOWER,
	SHOW_IMPROVED_TOWER,

	//show abilities
	SHOW_BOMB_ABILITY,
	SHOW_FREEZE_BOMB_ABILITY,
	SHOW_ACID_ABILITY,
	SHOW_INCREASE_TOWER_DAMAGE_ABILITY,
	SHOW_INCREASE_TOWER_ATTACK_SPEED_ABILITY,
	SHOW_TIME_STOP_ABILITY,

	//show enemies
	SHOW_INFANTRY,
	SHOW_WAR_VEHICLE,
	SHOW_TRICYCLE,
	SHOW_TANK,
	SHOW_MECHSPIDER,
	SHOW_TUATARA,
	SHOW_GIANT_SLUG,
	SHOW_HEAVY_TANK,
	SHOW_SLUGGY,
	SHOW_REPAIR,
	SHOW_SHIELD,
	SHOW_WALKER,
	SHOW_SELF_HEAL,
	SHOW_TRACTOR,
	SHOW_WORM,
	SHOW_BUGSAURUS,
	SHOW_JUMPER,
	SHOW_ROLLING,

	//show map effects
	SHOW_SMOKE,
	SHOW_DRAIN,
	SHOW_REGRESS,
	SHOW_TOWER_EXPLOSION,
	SHOW_LAVA,
	SHOW_INVISIBILITY,
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
	FREEZE_BOMB,
	STOP,
	TOWER_ABILITY,
	ACID,

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

	WEB_SHOOT,
	ELECTRIC_SHOCK,
	STRONG,
	SHIELD,
	BLAST,
	TELEPORT,
	DRAIN_DROPPPED,
	ACTIVATE_ABILITY,
	GAME_OVER,
	REPAIR,
	HEAL,
	SPAWN_ROAR,
	ALARM,
	REGRESS,
	TANK_SHOOT,
	DOWN_SHOOT,
};
}

namespace GAME_MUSIC {
enum MUSIC_ID
{
	UNKNOWN_SOUND = SoundManager::USER_MUSIC_ID,

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
	START,
	OPTIONS,
	MANUAL,
	EXIT,

	//panel
	ABILITY_ACID,
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
	DPS,
	RADIUS,
	KILLS,
	COST,
	SELL_COST,
	WAVE,
	LIMIT,

	BASE_TOWER_DESCRIPTION,
	FREEZE_TOWER_DESCRIPTION,
	ENERGY_TOWER_DESCRIPTION,
	ROCKET_TOWER_DESCRIPTION,
	LASER_TOWER_DESCRIPTION,
	IMPROVED_TOWER_DESCRIPTION,

	BOMB_ABILITY_DESCRIPTION,
	FREEZE_BOMB_ABILITY_DESCRIPTION,
	ACID_ABILITY_DESCRIPTION,
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
	SKIP_TEXT,

	//Instructions
	INSTRUCTION_WELCOME,
	INSTRUCTION_MONEY,
	INSTRUCTION_HEALTH,
	INSTRUCTION_PROGRESS,
	INSTRUCTION_TOWERS,
	INSTRUCTION_UPGRADE_SELL,
	INSTRUCTION_GOOD_LUCK,
	INSTRUCTION_SKIP,	

	//enemies
	INFANTRY,
	WAR_VEHICLE,
	TRICYCLE,
	TANK,
	MECHSPIDER,
	TUATARA,
	GIANT_SLUG,
	HEAVY_TANK,
	SLUGGY,
	REPAIR,
	SHELL,
	WALKER,
	ROLLER,
	TRACTOR,
	WORM,
	BUGSAURUS,
	JUMPER,
	SELFHEAL,

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
	ARMOR_LIGHT,
	ARMOR_MEDIUM,
	ARMOR_STRONG,
	COOLDOWN_TIME,
	DAMAGE_ATTRIBUTE,
	DURATION_ATTRIBUTE,

	BASE_TOWER_ABILITY,
	FREEZE_TOWER_ABILITY,
	POWER_TOWER_ABILITY,
	ROCKET_TOWER_ABILITY,
	LASER_TOWER_ABILITY,
	IMPROVED_TOWER_ABILITY,
	POWER_TOWER_TRAIT,
	CAN_BUILD_ANYWHERE,
	CAN_BUILD_NEAR_ENERGY_TOWER,

	//enemies abilities
	ENEMY_ABILITY_NONE,
	ENEMY_ABILITY_RAGE,
	ENEMY_ABILITY_SPAWN,
	ENEMY_ABILITY_TELEPORT,
	ENEMY_ABILITY_SHELL_NEAR,
	ENEMY_ABILITY_HEAL_NEAR,
	ENEMY_ABILITY_SHUTDOWN_TOWER,
	ENEMY_ABILITY_STRONG,
	ENEMY_ABILITY_DOWN_TOWER,
	ENEMY_ABILITY_SELF_HEAL,
	ENEMY_ABILITY_FASTER,
	ENEMY_ABILITY_KILL_TOWER,
	ENEMY_ABILITY_DOWNGRADE_TOWER,
	ENEMY_ABILITY_ROLLING,
	ENEMY_ABILITY_JUMPING,
	//map effects
	SMOKE_NAME,
	DRAIN_NAME,
	REGRESS_NAME,
	TOWER_EXPLOSION_NAME,
	LAVA_NAME,
	INVISIBILITY_NAME,
	SMOKE_DESCRIPTION,
	DRAIN_DESCRIPTION,
	REGRESS_DESCRIPTION,
	TOWER_EXPLOSION_DESCRIPTION,
	LAVA_DESCRIPTION,
	INVISIBILITY_DESCRIPTION,
};
}
namespace GAME_FILES {

enum FILE_ID
{
	TEST_MISSION,
	MISSON_1 = 1,
	MISSON_2,
	MISSON_3,
	MISSON_4,
	MISSON_5,
	MISSON_6,
	MISSON_7,
	MISSON_8,
	MISSON_9,
	MISSON_10,
	MISSON_11,
	MISSON_12,
	MISSON_13,
	MISSON_14,
	MISSON_15,
	BALANCE,
	TILES,
};

}
namespace GAME_SHADERS {

enum SHADER_ID
{
	WAVE,
	MOVING,
	SMOKE
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
	static void addFile(FileType type, const std::string& data);
	static void addShader(ShaderType type, const std::string& data);

	static void loadTranslations();
	static std::vector<sf::String> creators();

	static const std::string resourcesFileName;
	static const std::string checksum;

	static void loadExtraResources();
};

#endif // GAMEMANAGERS_H
