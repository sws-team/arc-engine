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
				addTexture(GAME_TEXTURE::ENEMY_MECHSPIDER, out);
			else if (resource.name == "car")
				addTexture(GAME_TEXTURE::ENEMY_WAR_VEHICLE, out);
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
			else if (resource.name == "walker")
				addTexture(GAME_TEXTURE::ENEMY_WALKER, out);
			else if (resource.name == "big_tank")
				addTexture(GAME_TEXTURE::ENEMY_HEAVY_TANK, out);
			else if (resource.name == "giant_slug")
				addTexture(GAME_TEXTURE::ENEMY_GIANT_SLUG, out);
			else if (resource.name == "worm")
				addTexture(GAME_TEXTURE::ENEMY_WORM, out);
			else if (resource.name == "spawn_worm")
				addTexture(GAME_TEXTURE::ENEMY_SPAWN_WORM, out);
			else if (resource.name == "bugsaurus")
				addTexture(GAME_TEXTURE::ENEMY_BUGSAURUS, out);
			else if (resource.name == "tuatara")
				addTexture(GAME_TEXTURE::ENEMY_TUATARA, out);
			else if (resource.name == "sluggy")
				addTexture(GAME_TEXTURE::ENEMY_SLUGGY, out);
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
			else if (resource.name == "game_logo")
				addTexture(GAME_TEXTURE::LOGO, out);
			else if (resource.name == "direction")
				addTexture(GAME_TEXTURE::DIRECTION_TEXTURE, out);
			else if (resource.name == "empty_star")
				addTexture(GAME_TEXTURE::EMPTY_STAR_TEXTURE, out);
			else if (resource.name == "exit_icon")
				addTexture(GAME_TEXTURE::EXIT, out);
			else if (resource.name == "game_cursor")
				addTexture(GAME_TEXTURE::CURSOR_TEXTURE, out);
			else if (resource.name == "instruction_top")
				addTexture(GAME_TEXTURE::INSTRUCTIONS_TOP, out);
			else if (resource.name == "instruction_bottom")
				addTexture(GAME_TEXTURE::INSTRUCTIONS_BOTTOM, out);
			else if (resource.name == "instruction_vertical")
				addTexture(GAME_TEXTURE::INSTRUCTIONS_VERTICAL, out);
			else if (resource.name == "arrow")
				addTexture(GAME_TEXTURE::ARROW, out);
			else if (resource.name == "previous_icon")
			{
				addTexture(GAME_TEXTURE::PREVIOUS, out);
				addTexture(TexturesManager::CHOOSE_LIST_PREVIOUS, out);
			}
			else if (resource.name == "locked")
				addTexture(GAME_TEXTURE::LOCKED_ICON, out);
			else if (resource.name == "money_icon")
				addTexture(GAME_TEXTURE::MONEY_ICON, out);
			else if (resource.name == "panel")
				addTexture(GAME_TEXTURE::PANEL_TEXTURE, out);
			else if (resource.name == "panel_cursor")
				addTexture(GAME_TEXTURE::PANEL_CURSOR, out);
			else if (resource.name == "next_icon")
			{
				addTexture(GAME_TEXTURE::NEXT, out);
				addTexture(TexturesManager::CHOOSE_LIST_NEXT, out);
			}
			else if (resource.name == "sell_icon")
				addTexture(GAME_TEXTURE::SELL_TEXTURE, out);
			else if (resource.name == "star")
				addTexture(GAME_TEXTURE::STAR_TEXTURE, out);
			else if (resource.name == "upgrade_icon")
				addTexture(GAME_TEXTURE::UPGRADE_TEXTURE, out);
			else if (resource.name == "life_icon")
				addTexture(GAME_TEXTURE::LIFE_ICON, out);
			else if (resource.name == "audio_icon")
				addTexture(TexturesManager::AUDIO_ICON, out);
			else if (resource.name == "quiet_icon")
				addTexture(TexturesManager::QUIET_ICON, out);
			else if (resource.name == "character")
				addTexture(GAME_TEXTURE::CHARACTER, out);
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
			else if (resource.name == "enemy_rocket")
				addTexture(GAME_TEXTURE::ENEMY_ROCKET, out);
			else if (resource.name == "enemy_bullet")
				addTexture(GAME_TEXTURE::ENEMY_BULLET, out);
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
			else if (resource.name == "downgrade")
				addTexture(GAME_TEXTURE::DOWNGRADE, out);
			else if (resource.name == "tower_explosion")
				addTexture(GAME_TEXTURE::TOWER_EXPLOSION, out);
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
			else if (resource.name == "tank_shoot")
				addSound(GAME_SOUND::TANK_SHOOT, out);
			else if (resource.name == "downgrade_shoot")
				addSound(GAME_SOUND::DOWN_SHOOT, out);
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
			if (resource.name == "balance")
				addFile(GAME_FILES::BALANCE, out);
			else if (resource.name == "tiles")
				addFile(GAME_FILES::TILES, out);
#ifndef LOAD_MAPS
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
#endif
		}
			break;
		default:
			break;
		}
	}

	addShader(GAME_SHADERS::WAVE, ShadersFactory::WAVE_SHADER);
	addShader(GAME_SHADERS::MOVING, ShadersFactory::MOVING_SHADER);
	addShader(GAME_SHADERS::SMOKE, ShadersFactory::SMOKE_SHADER);

#ifdef LOAD_MAPS
	for (int i = 0; i < 15; ++i)
	{
		const int n = i + 1;
		const std::string fileName = std::string("maps/mission") + std::to_string(n) + std::string(".tmx");

		std::ifstream file;
		file.open(fileName);
		if (!file.is_open())
			return;

		std::string data((std::istreambuf_iterator<char>(file)),
						 std::istreambuf_iterator<char>());

		file >> data;
		file.close();

		addFile(n, data);
	}
#endif

	loadExtraResources();
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

void GameManagers::loadTranslations()
{
	std::map<int, sf::String> russainTranslation;
	std::map<int, sf::String> englishTranslation;
	std::map<int, sf::String> frenchTranslation;

	russainTranslation.insert(std::pair<int, sf::String>(TranslationsManager::LANGUAGE_ID, L"Русский"));
	englishTranslation.insert(std::pair<int, sf::String>(TranslationsManager::LANGUAGE_ID, L"English"));
	frenchTranslation.insert(std::pair<int, sf::String>(TranslationsManager::LANGUAGE_ID, L"French"));

	//menu
	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::START, L"Play"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::START, L"Играть"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::START, L"Jouer"));

	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::OPTIONS, L"Опции"));
	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::OPTIONS, L"Options"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::OPTIONS, L"Options"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::MANUAL, L"Manual"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::MANUAL, L"Справочник"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::MANUAL, L"Manual"));

	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::EXIT, L"Выход"));
	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::EXIT, L"Exit"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::EXIT, L"Sortir du jeu"));

	//manual
	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SPEED, L"Speed"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SPEED, L"Скорость"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SPEED, L"Скорость"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SIZE, L"Size"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SIZE, L"Размер"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SIZE, L"Размер"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ARMOR, L"Armour"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ARMOR, L"Броня"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ARMOR, L"Броня"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::HEALTH, L"Health"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::HEALTH, L"Здоровье"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::HEALTH, L"Здоровье"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY, L"Abilities"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY, L"Особые способности"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY, L"Особые способности"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SMALL, L"Small"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SMALL, L"Мелкий"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SMALL, L"Мелкий"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::MID, L"Mid"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::MID, L"Средний"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::MID, L"Средний"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BIG, L"Big"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BIG, L"Крупный"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BIG, L"Крупный"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SLOW, L"Slow"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SLOW, L"Медленный"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SLOW, L"Медленный"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::NORMAL_SPEED, L"Normal"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::NORMAL_SPEED, L"Нормальная"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::NORMAL_SPEED, L"Нормальная"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::FAST, L"Fast"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::FAST, L"Быстрая"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::FAST, L"Быстрая"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::NEXT, L"Next"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::NEXT, L"Вперед"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::NEXT, L"Вперед"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::PREVIOUS, L"Previous"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::PREVIOUS, L"Назад"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::PREVIOUS, L"Назад"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::CREDITS, L"Credits"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::CREDITS, L"Авторы"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::CREDITS, L"Credits"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BACK, L"Back"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BACK, L"Вернуться"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BACK, L"Вернуться"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ARMOR_LIGHT, L"Light"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ARMOR_LIGHT, L"Легкая"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ARMOR_LIGHT, L"Light"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ARMOR_MEDIUM, L"Medium"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ARMOR_MEDIUM, L"Средняя"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ARMOR_MEDIUM, L"Medium"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ARMOR_STRONG, L"Strong"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ARMOR_STRONG, L"Крепкая"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ARMOR_STRONG, L"Strong"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::COOLDOWN_TIME, L"Cooldown time"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::COOLDOWN_TIME, L"Время перезарядки"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::COOLDOWN_TIME, L"??"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::DAMAGE_ATTRIBUTE, L"Damage"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::DAMAGE_ATTRIBUTE, L"Урон"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::DAMAGE_ATTRIBUTE, L"Damage"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::DURATION_ATTRIBUTE, L"Duration"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::DURATION_ATTRIBUTE, L"Время действия"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::DURATION_ATTRIBUTE, L"Duration"));

	//settings
	englishTranslation.insert(std::pair<int, sf::String>(TranslationsManager::SETTINGS, L"Settings"));
	russainTranslation.insert(std::pair<int, sf::String>(TranslationsManager::SETTINGS, L"Настройки"));
	frenchTranslation.insert(std::pair<int, sf::String>(TranslationsManager::SETTINGS, L"Parametres"));

	englishTranslation.insert(std::pair<int, sf::String>(TranslationsManager::AUDIO, L"Audio"));
	russainTranslation.insert(std::pair<int, sf::String>(TranslationsManager::AUDIO, L"Аудио"));
	frenchTranslation.insert(std::pair<int, sf::String>(TranslationsManager::AUDIO, L"Audio"));

	englishTranslation.insert(std::pair<int, sf::String>(TranslationsManager::SOUND, L"Sound"));
	russainTranslation.insert(std::pair<int, sf::String>(TranslationsManager::SOUND, L"Звуки"));
	frenchTranslation.insert(std::pair<int, sf::String>(TranslationsManager::SOUND, L"Sons"));

	englishTranslation.insert(std::pair<int, sf::String>(TranslationsManager::MUSIC, L"Music"));
	russainTranslation.insert(std::pair<int, sf::String>(TranslationsManager::MUSIC, L"Музыка"));
	frenchTranslation.insert(std::pair<int, sf::String>(TranslationsManager::MUSIC, L"Musique"));

	englishTranslation.insert(std::pair<int, sf::String>(TranslationsManager::VIDEO, L"Video"));
	russainTranslation.insert(std::pair<int, sf::String>(TranslationsManager::VIDEO, L"Видео"));
	frenchTranslation.insert(std::pair<int, sf::String>(TranslationsManager::VIDEO, L"Video"));

	englishTranslation.insert(std::pair<int, sf::String>(TranslationsManager::FULLSCREEN, L"Fullscreen"));
	russainTranslation.insert(std::pair<int, sf::String>(TranslationsManager::FULLSCREEN, L"Полноэкранный режим"));
	frenchTranslation.insert(std::pair<int, sf::String>(TranslationsManager::FULLSCREEN, L"Plein ecran"));

	englishTranslation.insert(std::pair<int, sf::String>(TranslationsManager::RESOLUTION, L"Resolution"));
	russainTranslation.insert(std::pair<int, sf::String>(TranslationsManager::RESOLUTION, L"Разрешение"));
	frenchTranslation.insert(std::pair<int, sf::String>(TranslationsManager::RESOLUTION, L"Resolution"));

	englishTranslation.insert(std::pair<int, sf::String>(TranslationsManager::MISC, L"Misc"));
	russainTranslation.insert(std::pair<int, sf::String>(TranslationsManager::MISC, L"Разное"));
	frenchTranslation.insert(std::pair<int, sf::String>(TranslationsManager::MISC, L"Autres"));

	englishTranslation.insert(std::pair<int, sf::String>(TranslationsManager::LANGUAGE, L"Language"));
	russainTranslation.insert(std::pair<int, sf::String>(TranslationsManager::LANGUAGE, L"Язык"));
	frenchTranslation.insert(std::pair<int, sf::String>(TranslationsManager::LANGUAGE, L"Langue"));

	englishTranslation.insert(std::pair<int, sf::String>(TranslationsManager::ACCEPT, L"Accept"));
	russainTranslation.insert(std::pair<int, sf::String>(TranslationsManager::ACCEPT, L"Принять"));
	frenchTranslation.insert(std::pair<int, sf::String>(TranslationsManager::ACCEPT, L"Confirme"));

	englishTranslation.insert(std::pair<int, sf::String>(TranslationsManager::CANCEL, L"Cancel"));
	russainTranslation.insert(std::pair<int, sf::String>(TranslationsManager::CANCEL, L"Отмена"));
	frenchTranslation.insert(std::pair<int, sf::String>(TranslationsManager::CANCEL, L"Rejet"));

	//game
	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::PAUSED, L"Paused"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::PAUSED, L"Пауза"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::PAUSED, L"Pause"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::CONTINUE, L"Continue"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::CONTINUE, L"Продолжить"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::CONTINUE, L"Continuer"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::RESTART, L"Restart"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::RESTART, L"Рестарт"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::RESTART, L"Recommencer"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::EXIT_TO_MENU, L"Exit to menu"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::EXIT_TO_MENU, L"Выход в меню"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::EXIT_TO_MENU, L"Sortir au menu"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::EXIT_FROM_GAME, L"Exit from game"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::EXIT_FROM_GAME, L"Выход из игры"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::EXIT_FROM_GAME, L"Sortir du jeu"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::GAME_OVER, L"Game Over!"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::GAME_OVER, L"Игра окончена"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::GAME_OVER, L"Tu as perdu, desole"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::CONGRATULATIONS, L"Congratulations!"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::CONGRATULATIONS, L"Поздравляем"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::CONGRATULATIONS, L"Congratulations"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::START_GAME, L"Press Space to start"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::START_GAME, L"Нажмите Пробел чтобы начать"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::START_GAME, L"Tapez l'Espace afin de commencer le jeu"));

	//instructions
	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Welcome!"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Добро пожаловать в игру!"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Welcome!"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Money"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Валюта"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Money"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Health"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Здоровье"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Health"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Progress"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Прогресс уровня"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Progress"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Press Return to continue or Space to skip"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Нажмите Ввод чтобы продолжить или Пробел чтобы пропустить"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Taper la Touche d'entree pour continuer ou l'Espace pour passer"));

	//enemies
	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INFANTRY, L"Infantry"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INFANTRY, L"Пехота"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INFANTRY, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::WAR_VEHICLE, L"Машина"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::WAR_VEHICLE, L"Машина"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::WAR_VEHICLE, L"Машина"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TRICYCLE, L"Трицикл"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TRICYCLE, L"Tricycle de reconnaissance"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TRICYCLE, L"Трицикл"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TANK, L"Танк"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TANK, L"Танк"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TANK, L"Танк"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::MECHSPIDER, L"Паук"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::MECHSPIDER, L"Мехпаук"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::MECHSPIDER, L"Паук"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TUATARA, L"Вертолет"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TUATARA, L"Вертолет"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TUATARA, L"Вертолет"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::GIANT_SLUG, L"Авианосец"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::GIANT_SLUG, L"Авианосец"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::GIANT_SLUG, L"Авианосец"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::HEAVY_TANK, L"Большой танк"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::HEAVY_TANK, L"Большой танк"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::HEAVY_TANK, L"Большой танк"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SLUGGY, L"Самолет"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SLUGGY, L"Самолет"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SLUGGY, L"Самолет"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::REPAIR, L"Скорая"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::REPAIR, L"Скорая"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::REPAIR, L"Скорая"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SHELL, L"Ракушка"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SHELL, L"Ракушка"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SHELL, L"Ракушка"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::WALKER, L"Телепорт"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::WALKER, L"Телепорт"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::WALKER, L"Телепорт"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SELF_HEAL, L"Самолечит"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SELF_HEAL, L"Самолечит"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SELF_HEAL, L"Самолечит"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TRACTOR, L"Ухудшение башни"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TRACTOR, L"Ухудшение башни"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TRACTOR, L"Ухудшение башни"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::WORM, L"Larva"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::WORM, L"Личинка"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::WORM, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BUGSAURUS, L"Корова"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BUGSAURUS, L"Корова"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BUGSAURUS, L"Корова"));
#ifdef WITH_DIFFICULT
	//difficult
	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::EASY, L"Easy"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::EASY, L"Легко"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::EASY, L"Easy"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::NORMAL, L"Normal"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::NORMAL, L"Нормально"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::NORMAL, L"Normal"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::HARD, L"Hard"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::HARD, L"Трудно"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::HARD, L"Hard"));
#endif
	//towers abilities
	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BASE_TOWER_ABILITY, L"Invulnerability"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BASE_TOWER_ABILITY, L"Неуязвимость"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BASE_TOWER_ABILITY, L"Invulnerability"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::FREEZE_TOWER_ABILITY, L"Freeze on area"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::FREEZE_TOWER_ABILITY, L"Заморозка по площади"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::FREEZE_TOWER_ABILITY, L"Freeze on area"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::POWER_TOWER_ABILITY, L"???"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::POWER_TOWER_ABILITY, L"Периодично испускает энергетические импульсы,\n наносящие урон врагам рядом"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::POWER_TOWER_ABILITY, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ROCKET_TOWER_ABILITY, L"Burn enemy"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ROCKET_TOWER_ABILITY, L"Поджигает врага"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ROCKET_TOWER_ABILITY, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::LASER_TOWER_ABILITY, L"Multi-targeting"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::LASER_TOWER_ABILITY, L"Несколько целей"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::LASER_TOWER_ABILITY, L"Multi-targeting"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY, L"Shoot on 4 sides"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY, L"Стрельба на 4 стороны"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY, L"Shoot on 4 sides"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::POWER_TOWER_TRAIT, L"Price"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::POWER_TOWER_TRAIT, L"Цена за постройки увеличивается\nс каждой построенное башней"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::POWER_TOWER_TRAIT, L"???"));

	//enemies abilities
	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_NONE, L"???"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_NONE, L"Отсутствует"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_NONE, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_RAGE, L"Rage - "));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_RAGE, L"Ярость - броня увеличивается\nобратно пропорционально здоровью"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_RAGE, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN, L"Spawn"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN, L"Может родить новых личинок"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT, L"Teleport"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT, L"Телепорт"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT, L"Teleport"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR, L"shell"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR, L"Генератор силового поля\nувеличивает броню окружающих"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR, L"Heal"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR, L"Ремонтное оборудование\nпозволяет восстанавливать некоторое\nколичество здоровья"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER, L"???"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER, L"Может выпускать паутину из наноботов,\nкоторая временно выводит из строя башню"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_STRONG, L"???"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_STRONG, L"Оснащен энергетическим щитом,\nвеличина заряда которого зависит от времени"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_STRONG, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER, L"???"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER, L"Оснащен излучателем направленного действия,\nкоторый может вызывать поломки башен"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL, L"???"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL, L"Усиленная регенерация\nпозволяет восстанавливать здоровье"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_FASTER, L"???"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_FASTER, L"Ускоряется пропорционально\nотнятому здоровью"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_FASTER, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER, L"???"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER, L"Может уничтожить башню,\nвыстрелив в нее"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER, L"???"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER, L"Может ухудшить башню,\nвыстрелив в нее"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER, L"???"));

	//panel
	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_ACID, L"Acid"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_ACID, L"Кислота"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_ACID, L"Acide"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_BOMB, L"Bomb"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_BOMB, L"Бомба"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_BOMB, L"Bombe"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Freeze bomb"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Морозная бомба"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Bombe-glace"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Speed up tower"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Ускорение башни"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Tour a vitesse d'attaque augmentee"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Strengthen tower"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Усиление башни"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Tour aux dommages augmentes"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_STOP, L"Stop all"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_STOP, L"Остановка"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ABILITY_STOP, L"Stopper"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_BASE, L"Base tower"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_BASE, L"Обычная башня"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_BASE, L"Tour de base"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_POWER, L"Energy tower"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_POWER, L"Энергетическая башня"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_POWER, L"Tour de production d'energie"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_ROCKET, L"Rocket tower"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_ROCKET, L"Ракетная башня"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_ROCKET, L"Tour reacteur-fusee"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_FREEZE, L"Tower freeze"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_FREEZE, L"Замораживающая башня"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_FREEZE, L"Tour a obus glacants"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_LASER, L"Tower laser"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_LASER, L"Лазерная башня"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_LASER, L"Tour laser"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_IMPROVED, L"Tower improved"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_IMPROVED, L"Улучшенная башня"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_IMPROVED, L"Tour de base amelioree"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::LEVEL, L"Level"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::LEVEL, L"Уровень"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::LEVEL, L"Niveau"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Damage per seconds"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Урон в секунду"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Dommages"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::RADIUS, L"Radius"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::RADIUS, L"Радиус"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::RADIUS, L"Rayon"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::KILLS, L"Kills"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::KILLS, L"Фраги"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::KILLS, L"Ennemis tues"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::COST, L"Cost"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::COST, L"Цена"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::COST, L"Prix"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SELL_COST, L"Sell cost"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SELL_COST, L"Цена продажи"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SELL_COST, L"Prix de la vente"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::WAVE, L"Wave"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::WAVE, L"Волна"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::WAVE, L"Vague"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::LIMIT, L"Limit"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::LIMIT, L"Лимит"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::LIMIT, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Has a cumulative effect of reducing enemy armor"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Имеет кумулятивный эффект\nпо снижению вражеской брони"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Effet cumulatif de reduire la cuirasse d'un ennemi"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Slows down the enemy"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Замедляет врага"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Ralentissement d'un ennemi"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION, L"Produces resources"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION, L"Производит ресурсы"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION, L"Extraire les ressourses"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Rocket explosion deals area damage"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Взрыв от ракеты наносит урон\nпо площади"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Dommage d'une certaine territoire"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Attacks continuously"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Атакует непрерывно"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Attaquer continuellement"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Most effective in all stats"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Наиболее эффективна по всем\nпоказателям"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Attaquer avec les meilleurs indices"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Deals damage instantly"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Наносит урон мгновенно"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Exploision instantanee"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Slows down enemies instantly"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Замедляет врагов мгновенно"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Ralentissement instantane d'un ennemi"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION, L"Continuously deals damage in an area."));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION, L"Непрерывно наносит урон\nв области"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION, L"Dommages prolonge d'une certaine territoire"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Temporarily increases tower damage"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Временно увеличивает урон\nбашни"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Amelioration temporaire des dommages causes"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Temporarily increases tower attack speed"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Временно увеличивает скорость\nатаки башни"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Acceleration temporaire des tirs"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Stops the movement of all enemies"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Останавливает движение всех\nврагов"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Stopper tous les ennemis"));

	//map effects
	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SMOKE_NAME, L"Mist"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SMOKE_NAME, L"Туман"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SMOKE_NAME, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::DRAIN_NAME, L"Energy leech"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::DRAIN_NAME, L"Энергопиявка"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::DRAIN_NAME, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::REGRESS_NAME, L"???"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::REGRESS_NAME, L"???"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::REGRESS_NAME, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_EXPLOSION_NAME, L"ICBM"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_EXPLOSION_NAME, L"МБР"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_EXPLOSION_NAME, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SMOKE_DESCRIPTION, L"???"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SMOKE_DESCRIPTION, L"При попадании на башню ограничивет\nточность приборов наведения.\nУменьшает радиус стрельбы башни"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::SMOKE_DESCRIPTION, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::DRAIN_DESCRIPTION, L"???"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::DRAIN_DESCRIPTION, L"Выкачивает энергию из энергетических башен.\nМожет быть уничтожен взрывом бомбы"));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::DRAIN_DESCRIPTION, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::REGRESS_DESCRIPTION, L"???"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::REGRESS_DESCRIPTION, L"Вызывает помехи в работе башен,\nснижается урон и скорость стрельбы."));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::REGRESS_DESCRIPTION, L"???"));

	englishTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION, L"???"));
	russainTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION, L"Уничтожает башню независимо от уровня."));
	frenchTranslation.insert(std::pair<int, sf::String>(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION, L"???"));

	Engine::Instance().translationsManager()->addTranslation(std::string("russian"), russainTranslation);
	Engine::Instance().translationsManager()->addTranslation(std::string("english"), englishTranslation);
	Engine::Instance().translationsManager()->addTranslation(std::string("french"), frenchTranslation);
}

std::vector<sf::String> GameManagers::creators()
{
	std::vector<sf::String> creatorsVec;
	creatorsVec.push_back(Engine::Instance().globalVariables()->appName() +
						  sf::String(" Version: ") + APP_VERSION);
	creatorsVec.push_back(Engine::EngineName() + ": " + Engine::EngineVersion());

	creatorsVec.push_back(sf::String("Developers:"));
	creatorsVec.push_back(sf::String("FIO"));

	creatorsVec.push_back(sf::String("Testers:"));
	creatorsVec.push_back(sf::String("FIO"));

	creatorsVec.push_back(sf::String("Desiner:"));
	creatorsVec.push_back(sf::String("FIO"));

	creatorsVec.push_back(sf::String("Translators:"));
	creatorsVec.push_back(sf::String("Russian: "));
	creatorsVec.push_back(sf::String("French: "));
	creatorsVec.push_back(sf::String("German: "));

	creatorsVec.push_back(sf::String("Libraries:"));
	creatorsVec.push_back(sf::String("SFML"));
	creatorsVec.push_back(sf::String("https://www.sfml-dev.org/"));
	creatorsVec.push_back(sf::String("jsoncpp"));
	creatorsVec.push_back(sf::String("https://github.com/open-source-parsers/jsoncpp"));
	creatorsVec.push_back(sf::String("TinyXML"));
	creatorsVec.push_back(sf::String("http://www.grinninglizard.com/tinyxml/"));
	creatorsVec.push_back(sf::String("Base64"));
	creatorsVec.push_back(sf::String("https://github.com/tkislan/base64"));

	creatorsVec.push_back(sf::String("Music:"));
	creatorsVec.push_back(sf::String("https://opengameart.org"));
	creatorsVec.push_back(sf::String("https://freesound.org/"));
	creatorsVec.push_back(sf::String("Main theme: \"The Adventure Begins\"\nby Bart Kelsey licensed by CC-BY-SA 3.0\nhttps://opengameart.org/content/adventure-begins"));

	creatorsVec.push_back(sf::String("Shaders:"));
	creatorsVec.push_back(sf::String("http://glslsandbox.com"));

	return creatorsVec;
}

void GameManagers::loadExtraResources()
{
	const std::vector<GameResource::Resource> resources = GameResource::Resource::loadResources("display.dat");
	for(const GameResource::Resource &resource : resources)
	{
		std::string out;
		Base64::Decode(resource.data, &out);

		if (resource.type == GameResource::TEXTURE)
		{
//			if (resource.name == "")
//				addTexture(GAME_TEXTURE::SHOW_BASE_TOWER_ABILITY, out);
		}
	}
}

