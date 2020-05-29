#include "gamemanagers.h"
#include "engine.h"
#include "managers.h"
#include "base64.h"
#include "gameresource.h"
#include "Game/levelobject.h"

const std::string GameManagers::resourcesFileName = "resources.dat";
const std::string GameManagers::checksum = "9c393757bde52138f3e763dce9e949cd57f534a13329a468c64fcfc086121219";

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
			}
			//enemies
			{
			if (resource.name == "spider")
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
			else if (resource.name == "jumper")
				addTexture(GAME_TEXTURE::ENEMY_JUMPER, out);
			else if (resource.name == "roller")
				addTexture(GAME_TEXTURE::ENEMY_ROLLING, out);
			}
			//map icons
			{
			if (resource.name == "mission1_icon")
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
			}
			//towers
			{
			if (resource.name == "tower_base")
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
			}
			//icons
			{
			if (resource.name == "ability_bomb_icon")
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
			}
			//ui
			{
			if (resource.name == "focus_icon")
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
			else if (resource.name == "frame")
				addTexture(GAME_TEXTURE::FRAME, out);
			else if (resource.name == "frame_demo")
				addTexture(GAME_TEXTURE::FRAME_DEMO, out);
			else if (resource.name == "character_full")
				addTexture(GAME_TEXTURE::CHARACTER_FULL, out);
			else if (resource.name == "name_frame")
				addTexture(GAME_TEXTURE::NAME_FRAME, out);
			else if (resource.name == "demo_frame")
				addTexture(GAME_TEXTURE::DEMO_FRAME, out);
			}
			//weapons
			{
			if (resource.name == "bullet")
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
			}
			//effects
			{
			if (resource.name == "base_explosion")
				addTexture(GAME_TEXTURE::BASE_EXPLOSION_EFFECT, out);
			else if (resource.name == "blast")
				addTexture(GAME_TEXTURE::BLAST, out);
			else if (resource.name == "bomb_explosion")
				addTexture(GAME_TEXTURE::BOMB_EXPLOSION, out);
			else if (resource.name == "burn")
				addTexture(GAME_TEXTURE::BURN, out);
			else if (resource.name == "down_effect")
				addTexture(GAME_TEXTURE::DOWN_EFFECT, out);
			else if (resource.name == "energyleech")
				addTexture(GAME_TEXTURE::ENERGY_LEECH, out);
			else if (resource.name == "explosion")
				addTexture(GAME_TEXTURE::EXPLOSION, out);
			else if (resource.name == "freeze_bomb_explosion")
				addTexture(GAME_TEXTURE::FREEZE_BOMB_EXPLOSION, out);
			else if (resource.name == "freeze_explosion")
				addTexture(GAME_TEXTURE::FREEZE_EXPLOSION_EFFECT, out);
			else if (resource.name == "heal_small_effect")
				addTexture(GAME_TEXTURE::HEAL_SMALL_EFFECT, out);
			else if (resource.name == "heal_mid_effect")
				addTexture(GAME_TEXTURE::HEAL_MID_EFFECT, out);
			else if (resource.name == "heal_big_effect")
				addTexture(GAME_TEXTURE::HEAL_BIG_EFFECT, out);
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
			else if (resource.name == "web_effect")
				addTexture(GAME_TEXTURE::WEB_EFFECT, out);
			else if (resource.name == "downgrade")
				addTexture(GAME_TEXTURE::DOWNGRADE, out);
			else if (resource.name == "tower_explosion")
				addTexture(GAME_TEXTURE::TOWER_EXPLOSION, out);
			else if (resource.name == "lava")
				addTexture(GAME_TEXTURE::LAVA, out);
			//map objects
			}
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
			else if (resource.name == "mission0")
				addFile(GAME_FILES::TEST_MISSION, out);
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

std::vector<sf::String> GameManagers::creators()
{
	std::vector<sf::String> creatorsVec;
	creatorsVec.push_back(Engine::Instance().globalVariables()->appName() +
						  sf::String(" Version: ") + APP_VERSION);
	creatorsVec.push_back(Engine::EngineName() + ": " + Engine::EngineVersion());

	creatorsVec.push_back(sf::String("Developer:"));
	creatorsVec.push_back(sf::String("Marat Turmukhambetov"));

	creatorsVec.push_back(sf::String("Designer:"));
	creatorsVec.push_back(sf::String("Olesya Chmireva"));

	creatorsVec.push_back(sf::String("Testers:"));
	creatorsVec.push_back(sf::String("Konstantin Grigorenko"));
	creatorsVec.push_back(sf::String("frtkv"));
	creatorsVec.push_back(sf::String("Dmitrii Sevastyanov"));

	creatorsVec.push_back(sf::String("Proofreader:"));
	creatorsVec.push_back(sf::String("Evgeniia Sevastyanova"));

	creatorsVec.push_back(sf::String("Translators:"));
	creatorsVec.push_back(sf::String("English: Olesya Chmireva,\nKonstantin Grigorenko"));
	creatorsVec.push_back(sf::String("Russian: Marat Turmukhambetov"));
	creatorsVec.push_back(sf::String("French: Olesya Chmireva"));
	creatorsVec.push_back(sf::String("German: Konstantin Grigorenko"));

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
			if (resource.name == "show_bomb_ability")
				addTexture(GAME_TEXTURE::SHOW_BOMB_ABILITY, out);
			else if (resource.name == "show_freeze_bomb_ability")
				addTexture(GAME_TEXTURE::SHOW_FREEZE_BOMB_ABILITY, out);
			else if (resource.name == "show_base_tower")
				addTexture(GAME_TEXTURE::SHOW_BASE_TOWER, out);
			else if (resource.name == "show_freeze_tower")
				addTexture(GAME_TEXTURE::SHOW_FREEZE_TOWER, out);
		}
	}
}

