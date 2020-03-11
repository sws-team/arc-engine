#include "gamemanagers.h"
#include "engine.h"
#include "managers.h"
#include "base64.h"
#include "gameresource.h"
#include "Game/levelobject.h"

const std::string GameManagers::resourcesFileName = "resources.dat";
const std::string GameManagers::checksum = "dd";

GameManagers::GameManagers()
{

}

void GameManagers::loadResources()
{
	const std::vector<GameResource::Resource> resources = GameResource::Resource::loadResources(resourcesFileName);
	for(const GameResource::Resource &resource : resources)
	{
		std::string out;
		Base64::Decode(resource.data, &out);

		switch (resource.type)
		{
		case GameResource::TEXTURE:
		{
			if (resource.name == "tiles")
				addTexture(GAME_TEXTURE::TILES, out);
			else if (resource.name == "close")
				addTexture(TexturesManager::CLOSE_BACKGROUND, out);
			else if (resource.name == "about")
				addTexture(TexturesManager::ABOUT_BACKGROUND, out);
			else if (resource.name == "game_menu")
				addTexture(GAME_TEXTURE::GAME_MENU_TEXTURE, out);
			else if (resource.name == "logo")
				addTexture(TexturesManager::INTRO_BACKGROUND, out);
			else if (resource.name == "menu")
				addTexture(GAME_TEXTURE::MENU_BACKGROUND, out);
			else if (resource.name == "mission")
				addTexture(GAME_TEXTURE::MISSON_BACKGROUND, out);
			else if (resource.name == "options")
				addTexture(TexturesManager::SETTINGS_BACKGROUND, out);
			else if (resource.name == "manual")
				addTexture(GAME_TEXTURE::MANUAL_BACKGROUND, out);
			else if (resource.name == "window")
				addTexture(GAME_TEXTURE::WINDOW_TEXTURE, out);
			//enemies
			else if (resource.name == "spider")
				addTexture(GAME_TEXTURE::ENEMY_SPIDER, out);
			else if (resource.name == "car")
				addTexture(GAME_TEXTURE::ENEMY_CAR, out);
			else if (resource.name == "tricycle")
				addTexture(GAME_TEXTURE::ENEMY_TRICYCLE, out);
			else if (resource.name == "tank")
				addTexture(GAME_TEXTURE::ENEMY_TANK, out);
			else if (resource.name == "tractor")
				addTexture(GAME_TEXTURE::ENEMY_TRACTOR, out);
			else if (resource.name == "infantry")
				addTexture(GAME_TEXTURE::ENEMY_INFANTRY, out);
			else if (resource.name == "shield")
				addTexture(GAME_TEXTURE::ENEMY_SHIELD, out);
			else if (resource.name == "teleport")
				addTexture(GAME_TEXTURE::ENEMY_TELEPORT, out);
			else if (resource.name == "big_tank")
				addTexture(GAME_TEXTURE::ENEMY_BIG_TANK, out);
			else if (resource.name == "aircarrier")
				addTexture(GAME_TEXTURE::ENEMY_AIRCARRIER, out);
			else if (resource.name == "ant")
				addTexture(GAME_TEXTURE::ENEMY_ANT, out);
			else if (resource.name == "cow")
				addTexture(GAME_TEXTURE::ENEMY_COW, out);
			else if (resource.name == "helicopter")
				addTexture(GAME_TEXTURE::ENEMY_HELICOPTER, out);
			else if (resource.name == "plane")
				addTexture(GAME_TEXTURE::ENEMY_PLANE, out);
			else if (resource.name == "repair")
				addTexture(GAME_TEXTURE::ENEMY_REPAIR, out);
			else if (resource.name == "selfheal")
				addTexture(GAME_TEXTURE::ENEMY_SELF_HEAL, out);
			//map icons
			else if (resource.name == "mission1_icon")
				addTexture(GAME_TEXTURE::MAP_ICON_MISSION_1, out);
			else if (resource.name == "mission2_icon")
				addTexture(GAME_TEXTURE::MAP_ICON_MISSION_2, out);
			else if (resource.name == "mission3_icon")
				addTexture(GAME_TEXTURE::MAP_ICON_MISSION_3, out);
			else if (resource.name == "mission4_icon")
				addTexture(GAME_TEXTURE::MAP_ICON_MISSION_4, out);
			else if (resource.name == "mission5_icon")
				addTexture(GAME_TEXTURE::MAP_ICON_MISSION_5, out);
			else if (resource.name == "mission6_icon")
				addTexture(GAME_TEXTURE::MAP_ICON_MISSION_6, out);
			else if (resource.name == "mission7_icon")
				addTexture(GAME_TEXTURE::MAP_ICON_MISSION_7, out);
			else if (resource.name == "mission8_icon")
				addTexture(GAME_TEXTURE::MAP_ICON_MISSION_8, out);
			else if (resource.name == "mission9_icon")
				addTexture(GAME_TEXTURE::MAP_ICON_MISSION_9, out);
			else if (resource.name == "mission10_icon")
				addTexture(GAME_TEXTURE::MAP_ICON_MISSION_10, out);
			else if (resource.name == "mission11_icon")
				addTexture(GAME_TEXTURE::MAP_ICON_MISSION_11, out);
			else if (resource.name == "mission12_icon")
				addTexture(GAME_TEXTURE::MAP_ICON_MISSION_12, out);
			else if (resource.name == "mission13_icon")
				addTexture(GAME_TEXTURE::MAP_ICON_MISSION_13, out);
			else if (resource.name == "mission14_icon")
				addTexture(GAME_TEXTURE::MAP_ICON_MISSION_14, out);
			else if (resource.name == "mission15_icon")
				addTexture(GAME_TEXTURE::MAP_ICON_MISSION_15, out);
			//towers
			else if (resource.name == "tower_base")
				addTexture(GAME_TEXTURE::TOWER_BASE, out);
			else if (resource.name == "tower_freeze")
				addTexture(GAME_TEXTURE::TOWER_FREEZE, out);
			else if (resource.name == "tower_power")
				addTexture(GAME_TEXTURE::TOWER_POWER, out);
			else if (resource.name == "tower_rocket")
				addTexture(GAME_TEXTURE::TOWER_ROCKET, out);
			else if (resource.name == "tower_laser")
				addTexture(GAME_TEXTURE::TOWER_LASER, out);
			else if (resource.name == "tower_improved")
				addTexture(GAME_TEXTURE::TOWER_IMPROVED, out);
			//icons
			else if (resource.name == "ability_bomb_icon")
				addTexture(GAME_TEXTURE::ABILITY_BOMB, out);
			else if (resource.name == "ability_freeze_bomb_icon")
				addTexture(GAME_TEXTURE::ABILITY_FREEZE_BOMB, out);
			else if (resource.name == "ability_acid_icon")
				addTexture(GAME_TEXTURE::ABILITY_ACID, out);
			else if (resource.name == "ability_increase_tower_damage_icon")
				addTexture(GAME_TEXTURE::ABILITY_INCREASE_TOWER_DAMAGE, out);
			else if (resource.name == "ability_increase_tower_speed_icon")
				addTexture(GAME_TEXTURE::ABILITY_INCREASE_TOWER_ATTACK_SPEED, out);
			else if (resource.name == "ability_stop_icon")
				addTexture(GAME_TEXTURE::ABILITY_TIME_STOP, out);
			else if (resource.name == "tower_base_icon")
				addTexture(GAME_TEXTURE::TOWER_BASE_ICON, out);
			else if (resource.name == "tower_freeze_icon")
				addTexture(GAME_TEXTURE::TOWER_FREEZE_ICON, out);
			else if (resource.name == "tower_power_icon")
				addTexture(GAME_TEXTURE::TOWER_POWER_ICON, out);
			else if (resource.name == "tower_rocket_icon")
				addTexture(GAME_TEXTURE::TOWER_ROCKET_ICON, out);
			else if (resource.name == "tower_laser_icon")
				addTexture(GAME_TEXTURE::TOWER_LASER_ICON, out);
			else if (resource.name == "tower_improved_icon")
				addTexture(GAME_TEXTURE::TOWER_IMPROVED_ICON, out);
			//ui
			else if (resource.name == "focus_icon")
				addTexture(TexturesManager::FOCUS_ICON, out);
			else if (resource.name == "credits_icon")
				addTexture(GAME_TEXTURE::CREDITS, out);
			else if (resource.name == "cursor")
				addTexture(TexturesManager::CURSOR, out);
			else if (resource.name == "icon")
				addTexture(TexturesManager::ICON, out);
			else if (resource.name == "direction")
				addTexture(GAME_TEXTURE::DIRECTION_TEXTURE, out);
			else if (resource.name == "empty_star")
				addTexture(GAME_TEXTURE::EMPTY_STAR_TEXTURE, out);
			else if (resource.name == "exit_icon")
				addTexture(GAME_TEXTURE::EXIT, out);
			else if (resource.name == "game_cursor")
				addTexture(GAME_TEXTURE::CURSOR_TEXTURE, out);
			else if (resource.name == "instructions")
				addTexture(GAME_TEXTURE::INSTRUCTIONS_TEXTURE, out);
			else if (resource.name == "left_icon")
				addTexture(GAME_TEXTURE::PREVIOUS, out);
			else if (resource.name == "locked")
				addTexture(GAME_TEXTURE::LOCKED_ICON, out);
			else if (resource.name == "money_icon")
				addTexture(GAME_TEXTURE::MONEY_ICON, out);
			else if (resource.name == "panel")
				addTexture(GAME_TEXTURE::PANEL_TEXTURE, out);
			else if (resource.name == "panel_cursor")
				addTexture(GAME_TEXTURE::PANEL_CURSOR, out);
			else if (resource.name == "right_icon")
				addTexture(GAME_TEXTURE::NEXT, out);
			else if (resource.name == "sell_icon")
				addTexture(GAME_TEXTURE::SELL_TEXTURE, out);
			else if (resource.name == "star")
				addTexture(GAME_TEXTURE::STAR_TEXTURE, out);
			else if (resource.name == "upgrade_icon")
				addTexture(GAME_TEXTURE::UPGRADE_TEXTURE, out);
			else if (resource.name == "life_icon")
				addTexture(GAME_TEXTURE::LIFE_ICON, out);
			//weapons
			else if (resource.name == "bullet")
				addTexture(GAME_TEXTURE::BASE_PROJECTILE, out);
			else if (resource.name == "freeze_bullet")
				addTexture(GAME_TEXTURE::FREEZE_PROJECTILE, out);
			else if (resource.name == "improved_bullet")
				addTexture(GAME_TEXTURE::IMPROVED_PROJECTILE, out);
			else if (resource.name == "laser_projectile")
				addTexture(GAME_TEXTURE::LASER_PROJECTILE, out);
			else if (resource.name == "rocket_projectile")
				addTexture(GAME_TEXTURE::ROCKET_PROJECTILE, out);
			else if (resource.name == "electric_projectile")
				addTexture(GAME_TEXTURE::DOWNGRADE_PROJECTILE, out);
			//effects
			else if (resource.name == "base_explosion")
				addTexture(GAME_TEXTURE::BASE_EXPLOSION_EFFECT, out);
			else if (resource.name == "blast")
				addTexture(GAME_TEXTURE::BLAST, out);
			else if (resource.name == "bomb_explosion")
				addTexture(GAME_TEXTURE::BOMB_EXPLOSION, out);
			else if (resource.name == "burn")
				addTexture(GAME_TEXTURE::BURN, out);
			else if (resource.name == "down_effect")
				addTexture(GAME_TEXTURE::DOWN_EFFECT, out);
			else if (resource.name == "drain")
				addTexture(GAME_TEXTURE::DRAIN, out);
			else if (resource.name == "explosion")
				addTexture(GAME_TEXTURE::EXPLOSION, out);
			else if (resource.name == "freeze_bomb_explosion")
				addTexture(GAME_TEXTURE::FREEZE_BOMB_EXPLOSION, out);
			else if (resource.name == "freeze_explosion")
				addTexture(GAME_TEXTURE::FREEZE_EXPLOSION_EFFECT, out);
			else if (resource.name == "heal_effect")
				addTexture(GAME_TEXTURE::HEAL_EFFECT, out);
			else if (resource.name == "improved_explosion")
				addTexture(GAME_TEXTURE::IMPROVED_EXPLOSION_EFFECT, out);
			else if (resource.name == "regress")
				addTexture(GAME_TEXTURE::REGRESS, out);
			else if (resource.name == "rocket_explosion")
				addTexture(GAME_TEXTURE::ROCKET_EXPLOSION_EFFECT, out);
			else if (resource.name == "shell_effect")
				addTexture(GAME_TEXTURE::SHELL_EFFECT, out);
			else if (resource.name == "smoke_effect")
				addTexture(GAME_TEXTURE::SMOKE, out);
			else if (resource.name == "upgrade_effect")
				addTexture(GAME_TEXTURE::UPGRADE, out);
			else if (resource.name == "acid")
				addTexture(GAME_TEXTURE::VENOM_EFFECT, out);
			else if (resource.name == "web")
				addTexture(GAME_TEXTURE::WEB, out);
			//map objects
			else if (resource.name == "tree")
				addTexture(GAME_TEXTURE::OBJECT_TREE, out);

		}
			break;
		case GameResource::FONT:
		{
			if (resource.name == "default_font")
				Engine::Instance().fontManager()->addFont(FontManager::DEFAULT_FONT, out.c_str(), out.size());
		}
			break;
		case GameResource::SOUND:
		{
			if (resource.name == "intro")
				addSound(SoundManager::INTRO, out);
			else if (resource.name == "click")
				addSound(SoundManager::CLICK, out);
			else if (resource.name == "hover")
				addSound(SoundManager::HOVER, out);
			else if (resource.name == "acid")
				addSound(GAME_SOUND::ACID, out);
			else if (resource.name == "activate_ability")
				addSound(GAME_SOUND::ACTIVATE_ABILITY, out);
			else if (resource.name == "bomb")
				addSound(GAME_SOUND::BOMB, out);
			else if (resource.name == "freeze")
				addSound(GAME_SOUND::FREEZE_BOMB, out);
			else if (resource.name == "stop")
				addSound(GAME_SOUND::STOP, out);
			else if (resource.name == "tower_ability")
				addSound(GAME_SOUND::TOWER_ABILITY, out);
			else if (resource.name == "drain")
				addSound(GAME_SOUND::DRAIN_DROPPPED, out);
			else if (resource.name == "regress")
				addSound(GAME_SOUND::REGRESS, out);
			else if (resource.name == "catch")
				addSound(GAME_SOUND::CATCH, out);
			else if (resource.name == "electric_shock")
				addSound(GAME_SOUND::ELECTRIC_SHOCK, out);
			else if (resource.name == "electrick_shoot")
				addSound(GAME_SOUND::ELECTRIC_SHOOT, out);
			else if (resource.name == "heal")
				addSound(GAME_SOUND::HEAL, out);
			else if (resource.name == "repair")
				addSound(GAME_SOUND::REPAIR, out);
			else if (resource.name == "roar")
				addSound(GAME_SOUND::SPAWN_ROAR, out);
			else if (resource.name == "shield")
				addSound(GAME_SOUND::SHIELD, out);
			else if (resource.name == "shoot_web")
				addSound(GAME_SOUND::WEB_SHOOT, out);
			else if (resource.name == "strong")
				addSound(GAME_SOUND::STRONG, out);
			else if (resource.name == "teleport")
				addSound(GAME_SOUND::TELEPORT, out);
			else if (resource.name == "base_shot")
				addSound(GAME_SOUND::BASE_SHOT, out);
			else if (resource.name == "blast")
				addSound(GAME_SOUND::BLAST, out);
			else if (resource.name == "freeze_shot")
				addSound(GAME_SOUND::FREEZE_SHOT, out);
			else if (resource.name == "improved_shot")
				addSound(GAME_SOUND::IMPROVED_SHOT, out);
			else if (resource.name == "laser_shot")
				addSound(GAME_SOUND::LASER_SHOT, out);
			else if (resource.name == "rocket_shot")
				addSound(GAME_SOUND::ROCKET_SHOT, out);
			else if (resource.name == "alarm")
				addSound(GAME_SOUND::ALARM, out);
			else if (resource.name == "game_over")
				addSound(GAME_SOUND::GAME_OVER, out);
			else if (resource.name == "sell")
				addSound(GAME_SOUND::SELL, out);
			else if (resource.name == "setup")
				addSound(GAME_SOUND::SETUP, out);
			else if (resource.name == "target_lock")
				addSound(GAME_SOUND::TARGET_LOCK, out);
			else if (resource.name == "tower_explosion")
				addSound(GAME_SOUND::TOWER_EXPLOSION, out);
			else if (resource.name == "upgrade")
				addSound(GAME_SOUND::UPGRADE, out);
			else if (resource.name == "warning")
				addSound(GAME_SOUND::WARNING, out);
			else if (resource.name == "win")
				addSound(GAME_SOUND::WIN, out);
		}
			break;
		case GameResource::MUSIC:
		{
			if (resource.name == "main")
				addMusic(SoundManager::MAIN_MENU_MUSIC, out);
			else if (resource.name == "credits")
				addMusic(SoundManager::CREDITS_MUSIC, out);
			else if (resource.name == "final_1")
				addMusic(GAME_MUSIC::FINAL_1, out);
			else if (resource.name == "final_2")
				addMusic(GAME_MUSIC::FINAL_2, out);
			else if (resource.name == "final_3")
				addMusic(GAME_MUSIC::FINAL_3, out);
			else if (resource.name == "track_1")
				addMusic(GAME_MUSIC::TRACK_1, out);
			else if (resource.name == "track_2")
				addMusic(GAME_MUSIC::TRACK_2, out);
			else if (resource.name == "track_3")
				addMusic(GAME_MUSIC::TRACK_3, out);
			else if (resource.name == "track_4")
				addMusic(GAME_MUSIC::TRACK_4, out);
			else if (resource.name == "track_5")
				addMusic(GAME_MUSIC::TRACK_5, out);
			else if (resource.name == "track_6")
				addMusic(GAME_MUSIC::TRACK_6, out);
			else if (resource.name == "track_7")
				addMusic(GAME_MUSIC::TRACK_7, out);
			else if (resource.name == "track_8")
				addMusic(GAME_MUSIC::TRACK_8, out);
		}
			break;
		case GameResource::OTHER:
		{
			if (resource.name == "tiles")
				addFile(GAME_FILES::TILES, out);
			else if (resource.name == "mission1")
				addFile(GAME_FILES::MISSON_1, out);
			else if (resource.name == "mission2")
				addFile(GAME_FILES::MISSON_2, out);
			else if (resource.name == "mission3")
				addFile(GAME_FILES::MISSON_3, out);
			else if (resource.name == "mission4")
				addFile(GAME_FILES::MISSON_4, out);
			else if (resource.name == "mission5")
				addFile(GAME_FILES::MISSON_5, out);
			else if (resource.name == "mission6")
				addFile(GAME_FILES::MISSON_6, out);
			else if (resource.name == "mission7")
				addFile(GAME_FILES::MISSON_7, out);
			else if (resource.name == "mission8")
				addFile(GAME_FILES::MISSON_8, out);
			else if (resource.name == "mission9")
				addFile(GAME_FILES::MISSON_9, out);
			else if (resource.name == "mission10")
				addFile(GAME_FILES::MISSON_10, out);
			else if (resource.name == "mission11")
				addFile(GAME_FILES::MISSON_11, out);
			else if (resource.name == "mission12")
				addFile(GAME_FILES::MISSON_12, out);
			else if (resource.name == "mission13")
				addFile(GAME_FILES::MISSON_13, out);
			else if (resource.name == "mission14")
				addFile(GAME_FILES::MISSON_14, out);
			else if (resource.name == "mission15")
				addFile(GAME_FILES::MISSON_15, out);
		}
			break;
		default:
			break;
		}

	}

	addShader(GAME_SHADERS::WAVE, ShadersFactory::WAVE_SHADER);
	addShader(GAME_SHADERS::MOVING, ShadersFactory::MOVING_SHADER);

	loadSounds();
	loadMusic();
}

void GameManagers::addTexture(TextureType type, const std::string &data)
{
	Engine::Instance().texturesManager()->addTexture(type, data.c_str(), data.size());
}

void GameManagers::addSound(SoundType type, const std::string &data)
{
	Engine::Instance().soundManager()->addSound(type, data.c_str(), data.size());
}

void GameManagers::addMusic(MusicType type, const std::string &data)
{
	Engine::Instance().soundManager()->addMusic(type, data.c_str(), data.size());
}

void GameManagers::addFile(FileType type, const std::string &data)
{
	Engine::Instance().filesManager()->addFile(type, data.c_str(), data.size());
}

void GameManagers::addShader(ShaderType type, const std::string &data)
{
	Engine::Instance().shadersManager()->addShader(type, data);
}

void GameManagers::loadSounds()
{
//	Engine::Instance().soundManager()->addSound(SoundManager::INTRO, "sounds/intro.ogg");
//	Engine::Instance().soundManager()->addSound(SoundManager::CLICK, "sounds/ui/click.ogg");
//	Engine::Instance().soundManager()->addSound(SoundManager::HOVER, "sounds/ui/hover.ogg");

//	Engine::Instance().soundManager()->addSound(GAME_SOUND::SELL, "sounds/sell.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::TARGET_LOCK, "sounds/target_lock.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::TOWER_EXPLOSION, "sounds/tower_explosion.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::SETUP, "sounds/setup.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::WARNING, "sounds/warning.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::WIN, "sounds/win.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::UPGRADE, "sounds/upgrade.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::INTRO, "sounds/intro.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::GAME_OVER, "sounds/game_over.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::ALARM, "sounds/alarm.ogg");

//	//enemies
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::CATCH, "sounds/enemies/catch.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::WEB_SHOOT, "sounds/enemies/shoot_web.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::ELECTRIC_SHOCK, "sounds/enemies/electric_shock.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::ELECTRIC_SHOOT, "sounds/enemies/electric_shoot.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::STRONG, "sounds/enemies/strong.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::SHIELD, "sounds/enemies/shield.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::TELEPORT, "sounds/enemies/teleport.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::REPAIR, "sounds/enemies/repair.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::HEAL, "sounds/enemies/heal.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::SPAWN_ROAR, "sounds/enemies/roar.ogg");
//	//abilities
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::ACTIVATE_ABILITY, "sounds/abilities/activate_ability.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::BOMB, "sounds/abilities/bomb.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::FREEZE_BOMB, "sounds/abilities/freeze.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::STOP, "sounds/abilities/stop.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::TOWER_ABILITY, "sounds/abilities/tower_ability.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::ACID, "sounds/abilities/acid.ogg");
//	//towers
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::FREEZE_SHOT, "sounds/towers/freeze_shot.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::BASE_SHOT, "sounds/towers/base_shot.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::ROCKET_SHOT, "sounds/towers/rocket_shot.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::LASER_SHOT, "sounds/towers/laser_shot.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::IMPROVED_SHOT, "sounds/towers/improved_shot.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::BLAST, "sounds/towers/blast.ogg");

//	Engine::Instance().soundManager()->addSound(GAME_SOUND::DRAIN_DROPPPED, "sounds/effects/drain.ogg");
//	Engine::Instance().soundManager()->addSound(GAME_SOUND::REGRESS, "sounds/effects/regress.ogg");
}

void GameManagers::loadMusic()
{
//	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_1, "music/track_1.ogg");
//	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_2, "music/track_2.ogg");
//	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_3, "music/track_3.ogg");
//	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_4, "music/track_4.ogg");
//	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_5, "music/track_5.ogg");
//	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_6, "music/track_6.ogg");
//	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_7, "music/track_7.ogg");
//	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_8, "music/track_8.ogg");


//	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::FINAL_1, "music/final_1.ogg");
//	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::FINAL_2, "music/final_2.ogg");
//	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::FINAL_3, "music/final_3.ogg");

//	Engine::Instance().soundManager()->addMusic(SoundManager::MAIN_MENU_MUSIC, "music/main.ogg");
//	Engine::Instance().soundManager()->addMusic(SoundManager::CREDITS_MUSIC, "music/credits.ogg");
}

void GameManagers::loadTranslations()
{
	std::map<int, std::wstring> russainTranslation;
	std::map<int, std::wstring> englishTranslation;
	std::map<int, std::wstring> frenchTranslation;


	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::LANGUAGE_ID, L"Русский"));
	englishTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::LANGUAGE_ID, L"English"));
	frenchTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::LANGUAGE_ID, L"French"));

	//menu
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CAMPAIGN, L"Кампания"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CAMPAIGN, L"Play"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CAMPAIGN, L"Jouer"));

	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::OPTIONS, L"Опции"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::OPTIONS, L"Options"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::OPTIONS, L"Options"));

	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::MANUAL, L"Справочник"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EXIT, L"Выход"));

	//settings
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::SETTINGS, L"Настройки"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::AUDIO, L"Аудио"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::SOUND, L"Звуки"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::MUSIC, L"Музыка"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::VIDEO, L"Видео"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::FULLSCREEN, L"Полноэкранный режим"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::RESOLUTION, L"Разрешение"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::MISC, L"Разное"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::LANGUAGE, L"Язык"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::ACCEPT, L"Принять"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::CANCEL, L"Отмена"));

	//panel
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_VENOM, L"Кислота"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_BOMB, L"Бомба"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Морозная бомба"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Ускорение башни"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Увеличение урона башни"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_STOP, L"Остановка"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_BASE, L"Обычная башня"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_POWER, L"Энергетическая башня"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_ROCKET, L"Ракетная башня"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_FREEZE, L"Замораживающая башня"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_LASER, L"Лазерная башня"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_IMPROVED, L"Улучшенная башня"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::LEVEL, L"Уровень"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Урон в секунду"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::RADIUS, L"Радиус"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::KILLS, L"Фраги"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::COST, L"Цена"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SELL_COST, L"Цена продажи"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::WAVE, L"Волна"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Имеет кумулятивный эффект\nпо снижению вражеской брони"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Замедляет врага"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::POWER_TOWER_DESCRIPTION, L"Производит ресурсы"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Взрыв от ракеты наносит урон\nпо площади"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Атакует непрерывно"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Наиболее эффективна по всем\nпоказателям"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Наносит урон мгновенно"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Замедляет врагов мгновенно"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::VENOM_ABILITY_DESCRIPTION, L"Непрерывно наносит урон\nв области"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Временно увеличивает урон\nбашни"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Временно увеличивает скорость\nатаки башни"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Останавливает движение всех\nврагов"));

	//game
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::PAUSED, L"Пауза"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CONTINUE, L"Продолжить"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::RESTART, L"Рестарт"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EXIT_TO_MENU, L"Выход в меню"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EXIT_FROM_GAME, L"Выход из игры"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::GAME_OVER, L"Игра окончена"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CONGRATULATIONS, L"Поздравляем"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::START_GAME, L"Нажмите Пробел чтобы начать"));

	//instructions
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Добро пожаловать!"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"Башни"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_ABILITIES, L"Способности"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Валюта"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Здоровье"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Прогресс уровня"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Нажмите Ввод чтобы продолжить или Пробел чтобы пропустить"));

	//enemies
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SCORPION, L"Скорпион"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CAR, L"Машина"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TRICYCLE, L"Трицикл"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TANK, L"Танк"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SPIDER, L"Паук"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HELICOPTER, L"Вертолет"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::AIRCARRIER, L"Авианосец"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BIG_TANK, L"Большой танк"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::PLANE, L"Самолет"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::REPAIR, L"Скорая"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SHELL, L"Ракушка"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TELEPORT, L"Телепорт"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SELF_HEAL, L"Самолечит"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::DOWN_TOWER, L"Ухудшение башни"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ANT, L"Муравей"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::COW, L"Корова"));

	//manual
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SPEED, L"Скорость"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SIZE, L"Размер"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ARMOR, L"Броня"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HEALTH, L"Здоровье"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY, L"Особые способности"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SMALL, L"Мелкий"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::MID, L"Средний"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BIG, L"Крупный"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SLOW, L"Медленный"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::NORMAL_SPEED, L"Нормальная"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::FAST, L"Быстрая"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::NEXT, L"Вперед"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::PREVIOUS, L"Назад"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CREDITS, L"Авторы"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BACK, L"Вернуться"));

	//difficult
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EASY, L"Легко"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::NORMAL, L"Нормально"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HARD, L"Трудно"));





	//menu
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::MANUAL, L"Manual"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EXIT, L"Exit"));

	//settings
	englishTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::SETTINGS, L"Settings"));
	englishTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::AUDIO, L"Audio"));
	englishTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::SOUND, L"Sound"));
	englishTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::MUSIC, L"Music"));
	englishTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::VIDEO, L"Video"));
	englishTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::FULLSCREEN, L"Fullscreen"));
	englishTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::RESOLUTION, L"Resolution"));
	englishTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::MISC, L"Misc"));
	englishTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::LANGUAGE, L"Language"));
	englishTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::ACCEPT, L"Accept"));
	englishTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::CANCEL, L"Cancel"));

	//panel
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_VENOM, L"Venom"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_BOMB, L"Bomb"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Freeze bomb"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Speed up tower"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Strengthen tower"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_STOP, L"Stop all"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_BASE, L"Tower base"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_POWER, L"Tower power"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_ROCKET, L"Tower rocket"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_FREEZE, L"Tower freeze"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_LASER, L"Tower laser"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_IMPROVED, L"Tower improved"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::LEVEL, L"Level"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Damage per seconds"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::RADIUS, L"Radius"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::KILLS, L"Kills"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::COST, L"Cost"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SELL_COST, L"Sell cost"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::WAVE, L"Wave"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Has a cumulative effect of reducing enemy armor"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Slows down the enemy"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::POWER_TOWER_DESCRIPTION, L"Produces resources"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Rocket explosion deals area damage"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Attacks continuously"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Most effective in all stats"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Deals damage instantly"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Slows down enemies instantly"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::VENOM_ABILITY_DESCRIPTION, L"Continuously deals damage in an area."));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Temporarily increases tower damage"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Temporarily increases tower attack speed"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Stops the movement of all enemies"));

	//game
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::PAUSED, L"Paused"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CONTINUE, L"Continue"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::RESTART, L"Restart"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EXIT_TO_MENU, L"Exit to menu"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EXIT_FROM_GAME, L"Exit from game"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::GAME_OVER, L"Game Over!"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CONGRATULATIONS, L"Congratulations!"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::START_GAME, L"Press Space to start"));

	//instructions
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Welcome!"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"Towers"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_ABILITIES, L"Abilities"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Money"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Health"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Progress"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Press Return to continue or Space to skip"));

	//enemies
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SCORPION, L"Башни"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CAR, L"Машина"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TRICYCLE, L"Трицикл"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TANK, L"Танк"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SPIDER, L"Паук"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HELICOPTER, L"Вертолет"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::AIRCARRIER, L"Авианосец"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BIG_TANK, L"Большой танк"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::PLANE, L"Самолет"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::REPAIR, L"Скорая"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SHELL, L"Ракушка"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TELEPORT, L"Телепорт"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SELF_HEAL, L"Самолечит"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::DOWN_TOWER, L"Ухудшение башни"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ANT, L"Муравей"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::COW, L"Корова"));

	//manual
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SPEED, L"Speed"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SIZE, L"Size"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ARMOR, L"Armour"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HEALTH, L"Health"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY, L"Abilities"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SMALL, L"Small"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::MID, L"Mid"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BIG, L"Big"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SLOW, L"Slow"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::NORMAL_SPEED, L"Normal"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::FAST, L"Fast"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::NEXT, L"Next"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::PREVIOUS, L"Previous"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CREDITS, L"Credits"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BACK, L"Back"));

	//difficult
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EASY, L"Easy"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::NORMAL, L"Normal"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HARD, L"Hard"));


	//menu
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::MANUAL, L"Manual"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CREDITS, L"Credits"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EXIT, L"Sortir du jeu"));

	//settings
	frenchTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::SETTINGS, L"Parametres"));
	frenchTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::AUDIO, L"Audio"));
	frenchTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::SOUND, L"Sons"));
	frenchTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::MUSIC, L"Musique"));
	frenchTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::VIDEO, L"Video"));
	frenchTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::FULLSCREEN, L"Plein ecran"));
	frenchTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::RESOLUTION, L"Resolution"));
	frenchTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::MISC, L"Autres"));
	frenchTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::LANGUAGE, L"Langue"));
	frenchTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::ACCEPT, L"Confirme"));
	frenchTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::CANCEL, L"Rejet"));

	//panel
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_VENOM, L"Acide"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_BOMB, L"Bombe"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Bombe-glace"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Tour a vitesse d'attaque augmentee"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Tour aux dommages augmentes"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_STOP, L"Stopper"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_BASE, L"Tour de base"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_POWER, L"Tour de production d'energie"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_ROCKET, L"Tour reacteur-fusee"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_FREEZE, L"Tour a obus glacants"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_LASER, L"Tour laser"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_IMPROVED, L"Tour de base amelioree"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::LEVEL, L"Niveau"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Dommages"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::RADIUS, L"Rayon"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::KILLS, L"Ennemis tues"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::COST, L"Prix"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SELL_COST, L"Prix de la vente"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::WAVE, L"Vague"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Effet cumulatif de reduire la cuirasse d'un ennemi"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Ralentissement d'un ennemi"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::POWER_TOWER_DESCRIPTION, L"Extraire les ressourses"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Dommage d'une certaine territoire"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Attaquer continuellement"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Attaquer avec les meilleurs indices"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Exploision instantanee"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Ralentissement instantane d'un ennemi"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::VENOM_ABILITY_DESCRIPTION, L"Dommages prolonge d'une certaine territoire"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Amelioration temporaire des dommages causes"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Acceleration temporaire des tirs"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Stopper tous les ennemis"));

	//game
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::PAUSED, L"Pause"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CONTINUE, L"Continuer"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::RESTART, L"Recommencer"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EXIT_TO_MENU, L"Sortir au menu"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EXIT_FROM_GAME, L"Sortir du jeu"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::GAME_OVER, L"Tu as perdu, desole"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CONGRATULATIONS, L"Congratulations"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::START_GAME, L"Tapez l'Espace afin de commencer le jeu"));

	//instructions
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Welcome!"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"Towers"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_ABILITIES, L"Abilities"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Money"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Health"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Progress"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Taper la Touche d'entree pour continuer ou l'Espace pour passer"));

	//enemies
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SCORPION, L"Башни"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CAR, L"Машина"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TRICYCLE, L"Трицикл"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TANK, L"Танк"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SPIDER, L"Паук"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HELICOPTER, L"Вертолет"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::AIRCARRIER, L"Авианосец"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BIG_TANK, L"Большой танк"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::PLANE, L"Самолет"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::REPAIR, L"Скорая"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SHELL, L"Ракушка"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TELEPORT, L"Телепорт"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SELF_HEAL, L"Самолечит"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::DOWN_TOWER, L"Ухудшение башни"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ANT, L"Муравей"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::COW, L"Корова"));

	//manual
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SPEED, L"Скорость"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SIZE, L"Размер"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ARMOR, L"Броня"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HEALTH, L"Здоровье"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY, L"Особые способности"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SMALL, L"Мелкий"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::MID, L"Средний"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BIG, L"Крупный"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SLOW, L"Медленный"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::NORMAL_SPEED, L"Нормальная"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::FAST, L"Быстрая"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::NEXT, L"Вперед"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::PREVIOUS, L"Назад"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CREDITS, L"Авторы"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BACK, L"Вернуться"));

	//difficult
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EASY, L"Easy"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::NORMAL, L"Normal"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HARD, L"Hard"));


	Engine::Instance().translationsManager()->addTranslation(std::string("russian"), russainTranslation);
	Engine::Instance().translationsManager()->addTranslation(std::string("english"), englishTranslation);
	Engine::Instance().translationsManager()->addTranslation(std::string("french"), frenchTranslation);
}
