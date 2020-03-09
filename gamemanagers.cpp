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
			else if (resource.name == "window")
				addTexture(GAME_TEXTURE::WINDOW_TEXTURE, out);
			//enemies
			else if (resource.name == "spider")
				addTexture(GAME_TEXTURE::ENEMY_SPIDER, out);
			else if (resource.name == "car")
				addTexture(GAME_TEXTURE::ENEMY_CAR, out);
			else if (resource.name == "tricycle")
				addTexture(GAME_TEXTURE::ENEMY_TRICYCLE, out);
		}
			break;
		case GameResource::SOUND:
		{
//			if (resource.name == "intro")
//				addSound(SoundManager::INTRO, out);
//			else if (resource.name == "click")
//				addSound(SoundManager::CLICK, out);
//			else if (resource.name == "hover")
//				addSound(SoundManager::HOVER, out);
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
	Engine::Instance().texturesManager()->addTexture(TexturesManager::FOCUS_ICON, "images/ui/focus.png");

	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::PANEL_TEXTURE, "images/ui/panel.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::CURSOR_TEXTURE, "images/ui/game_cursor.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::STAR_TEXTURE, "images/ui/star.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::EMPTY_STAR_TEXTURE, "images/ui/empty_star.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::SELL_TEXTURE, "images/ui/sell.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::UPGRADE_TEXTURE, "images/ui/upgrade.png");

	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::PANEL_CURSOR, "images/ui/panel_cursor.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MONEY_ICON, "images/ui/money.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::LIFE_ICON, "images/ui/money.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::DIRECTION_TEXTURE, "images/ui/direction.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::LOCKED_ICON, "images/ui/locked.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::INSTRUCTIONS_TEXTURE, "images/ui/instructions.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::NEXT, "images/ui/right.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::PREVIOUS, "images/ui/left.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::EXIT, "images/ui/exit.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::CREDITS, "images/ui/credits.png");

	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ABILITY_BOMB, "images/ui/Abilities/ability_bomb.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ABILITY_FREEZE_BOMB, "images/ui/Abilities/ability_freeze_bomb.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ABILITY_ACID, "images/ui/Abilities/ability_acid.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ABILITY_INCREASE_TOWER_DAMAGE, "images/ui/Abilities/ability_increase_tower_damage.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ABILITY_INCREASE_TOWER_ATTACK_SPEED, "images/ui/Abilities/ability_increase_tower_speed.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ABILITY_TIME_STOP, "images/ui/Abilities/ability_stop.png");

	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::TOWER_BASE, "images/ui/Towers/tower_base.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::TOWER_POWER, "images/ui/Towers/tower_power.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::TOWER_ROCKET, "images/ui/Towers/tower_rocket.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::TOWER_FREEZE, "images/ui/Towers/tower_freeze.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::TOWER_LASER, "images/ui/Towers/tower_laser.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::TOWER_IMPROVED, "images/ui/Towers/tower_improved.png");

	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::BASE_PROJECTILE, "images/Weapons/bullet.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::IMPROVED_PROJECTILE, "images/Weapons/improved_bullet.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::FREEZE_PROJECTILE, "images/Weapons/freeze_bullet.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ROCKET_PROJECTILE, "images/Weapons/rocket.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::LASER_PROJECTILE, "images/Weapons/laser.png");

	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MANUAL_BACKGROUND, "images/mission.png");

	//effects
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::BASE_EXPLOSION_EFFECT, "images/Effects/base_explosion.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::FREEZE_EXPLOSION_EFFECT, "images/Effects/freeze_explosion.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ROCKET_EXPLOSION_EFFECT, "images/Effects/rocket_explosion.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::IMPROVED_EXPLOSION_EFFECT, "images/Effects/improved_explosion.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::VENOM_EFFECT, "images/Effects/venom.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::BOMB_EXPLOSION, "images/Effects/bomb_explosion.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::FREEZE_BOMB_EXPLOSION, "images/Effects/freeze_bomb_explosion.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::HEAL_EFFECT, "images/Effects/heal.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::SHELL_EFFECT, "images/Effects/shell.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::WEB, "images/Effects/web.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::DOWN_EFFECT, "images/Effects/down.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::DOWNGRADE_PROJECTILE, "images/Weapons/electric.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::UPGRADE, "images/Effects/upgrade.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::SMOKE, "images/Effects/smoke.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::EXPLOSION, "images/Effects/explosion.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::DRAIN, "images/Effects/drain.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::REGRESS, "images/Effects/regress.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::BURN, "images/Effects/burn.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::BLAST, "images/Effects/blast.png");

	//map icons
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_1, "images/maps_icons/mission1.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_2, "images/maps_icons/mission2.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_3, "images/maps_icons/mission3.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_4, "images/maps_icons/mission4.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_5, "images/maps_icons/mission5.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_6, "images/maps_icons/mission6.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_7, "images/maps_icons/mission7.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_8, "images/maps_icons/mission8.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_9, "images/maps_icons/mission9.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_10, "images/maps_icons/mission10.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_11, "images/maps_icons/mission11.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_12, "images/maps_icons/mission12.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_13, "images/maps_icons/mission13.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_14, "images/maps_icons/mission14.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_15, "images/maps_icons/mission15.png");

	//enemies
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_SCORPION, "images/Enemies/scorpion.png");
//	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_CAR, "images/Enemies/car.png");
//	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_TRICYCLE, "images/Enemies/tricycle.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_TANK, "images/Enemies/tank.png");
//	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_SPIDER, "images/Enemies/spider.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_HELICOPTER, "images/Enemies/helicopter.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_AIRCARRIER, "images/Enemies/aircarrier.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_BIG_TANK, "images/Enemies/big_tank.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_PLANE, "images/Enemies/plane.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_REPAIR, "images/Enemies/repair.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_SHIELD, "images/Enemies/shield.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_TELEPORT, "images/Enemies/teleport.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_SELF_HEAL, "images/Enemies/enemy.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_DOWN_TOWER, "images/Enemies/tractor.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_ANT, "images/Enemies/ant.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_COW, "images/Enemies/cow.png");

	//objects
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::OBJECT_TREE, "images/maps_objects/tree.png");
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
	Engine::Instance().soundManager()->addSound(SoundManager::INTRO, "sounds/intro.ogg");
	Engine::Instance().soundManager()->addSound(SoundManager::CLICK, "sounds/ui/click.ogg");
	Engine::Instance().soundManager()->addSound(SoundManager::HOVER, "sounds/ui/hover.ogg");

	Engine::Instance().soundManager()->addSound(GAME_SOUND::SELL, "sounds/sell.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::TARGET_LOCK, "sounds/target_lock.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::TOWER_EXPLOSION, "sounds/tower_explosion.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::SETUP, "sounds/setup.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::WARNING, "sounds/warning.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::WIN, "sounds/win.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::UPGRADE, "sounds/upgrade.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::INTRO, "sounds/intro.ogg");

	//enemies
	Engine::Instance().soundManager()->addSound(GAME_SOUND::CATCH, "sounds/enemies/catch.ogg");
	//abilities
	Engine::Instance().soundManager()->addSound(GAME_SOUND::BOMB, "sounds/abilities/bomb.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::STOP, "sounds/abilities/stop.ogg");
	//towers
	Engine::Instance().soundManager()->addSound(GAME_SOUND::FREEZE_SHOT, "sounds/towers/freeze_shot.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::BASE_SHOT, "sounds/towers/base_shot.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::ROCKET_SHOT, "sounds/towers/rocket_shot.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::LASER_SHOT, "sounds/towers/laser_shot.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::IMPROVED_SHOT, "sounds/towers/improved_shot.ogg");

}

void GameManagers::loadMusic()
{
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_1, "sounds/music/track_1.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_2, "sounds/music/track_2.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_3, "sounds/music/track_3.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_4, "sounds/music/track_4.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_5, "sounds/music/track_5.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_6, "sounds/music/track_6.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_7, "sounds/music/track_7.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_8, "sounds/music/track_8.ogg");


	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::FINAL_1, "sounds/music/final_1.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::FINAL_2, "sounds/music/final_2.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::FINAL_3, "sounds/music/final_3.ogg");


	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::MENU, "sounds/main.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::CREDITS, "sounds/credits.ogg");
}

void GameManagers::loadTranslations()
{
	std::map<int, std::wstring> russainTranslation;
	std::map<int, std::wstring> englishTranslation;
	std::map<int, std::wstring> frenchTranslation;


	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::LANGUAGE_ID, L"�������"));
	englishTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::LANGUAGE_ID, L"English"));
	frenchTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::LANGUAGE_ID, L"French"));

	//menu
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CAMPAIGN, L"��������"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CAMPAIGN, L"Play"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CAMPAIGN, L"Jouer"));

	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::OPTIONS, L"�����"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::OPTIONS, L"Options"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::OPTIONS, L"Options"));

	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::MANUAL, L"����������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EXIT, L"�����"));

	//settings
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::SETTINGS, L"���������"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::AUDIO, L"�����"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::SOUND, L"�����"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::MUSIC, L"������"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::VIDEO, L"�����"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::FULLSCREEN, L"������������� �����"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::RESOLUTION, L"����������"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::MISC, L"������"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::LANGUAGE, L"����"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::ACCEPT, L"�������"));
	russainTranslation.insert(std::pair<int, std::wstring>(TranslationsManager::CANCEL, L"������"));

	//panel
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_VENOM, L"�������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_BOMB, L"�����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"�������� �����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"��������� �����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"���������� ����� �����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY_STOP, L"���������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_BASE, L"������� �����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_POWER, L"�������������� �����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_ROCKET, L"�������� �����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_FREEZE, L"�������������� �����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_LASER, L"�������� �����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TOWER_IMPROVED, L"���������� �����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::LEVEL, L"�������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"���� � �������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::RADIUS, L"������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::KILLS, L"�����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::COST, L"����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SELL_COST, L"���� �������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::WAVE, L"�����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"����� ������������ ������\n�� �������� ��������� �����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"��������� �����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::POWER_TOWER_DESCRIPTION, L"���������� �������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"����� �� ������ ������� ����\n�� �������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"������� ����������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"�������� ���������� �� ����\n�����������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"������� ���� ���������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"��������� ������ ���������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::VENOM_ABILITY_DESCRIPTION, L"���������� ������� ����\n� �������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"�������� ����������� ����\n�����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"�������� ����������� ��������\n����� �����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"������������� �������� ����\n������"));

	//game
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::PAUSED, L"�����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CONTINUE, L"����������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::RESTART, L"�������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EXIT_TO_MENU, L"����� � ����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EXIT_FROM_GAME, L"����� �� ����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::GAME_OVER, L"���� ��������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CONGRATULATIONS, L"�����������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::START_GAME, L"������� ������ ����� ������"));

	//instructions
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"����� ����������!"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"�����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_ABILITIES, L"�����������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_MONEY, L"������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"��������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"�������� ������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::INSTRUCTION_SKIP, L"������� ���� ����� ���������� ��� ������ ����� ����������"));

	//enemies
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SCORPION, L"��������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CAR, L"������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TRICYCLE, L"�������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TANK, L"����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SPIDER, L"����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HELICOPTER, L"��������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::AIRCARRIER, L"���������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BIG_TANK, L"������� ����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::PLANE, L"�������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::REPAIR, L"������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SHELL, L"�������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TELEPORT, L"��������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SELF_HEAL, L"���������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::DOWN_TOWER, L"��������� �����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ANT, L"�������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::COW, L"������"));

	//manual
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SPEED, L"��������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SIZE, L"������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ARMOR, L"�����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HEALTH, L"��������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY, L"������ �����������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SMALL, L"������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::MID, L"�������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BIG, L"�������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SLOW, L"���������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::NORMAL_SPEED, L"����������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::FAST, L"�������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::NEXT, L"������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::PREVIOUS, L"�����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CREDITS, L"������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BACK, L"���������"));

	//difficult
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EASY, L"�����"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::NORMAL, L"���������"));
	russainTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HARD, L"������"));





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
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SCORPION, L"�����"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CAR, L"������"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TRICYCLE, L"�������"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TANK, L"����"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SPIDER, L"����"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HELICOPTER, L"��������"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::AIRCARRIER, L"���������"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BIG_TANK, L"������� ����"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::PLANE, L"�������"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::REPAIR, L"������"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SHELL, L"�������"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TELEPORT, L"��������"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SELF_HEAL, L"���������"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::DOWN_TOWER, L"��������� �����"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ANT, L"�������"));
	englishTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::COW, L"������"));

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
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SCORPION, L"�����"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CAR, L"������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TRICYCLE, L"�������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TANK, L"����"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SPIDER, L"����"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HELICOPTER, L"��������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::AIRCARRIER, L"���������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BIG_TANK, L"������� ����"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::PLANE, L"�������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::REPAIR, L"������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SHELL, L"�������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::TELEPORT, L"��������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SELF_HEAL, L"���������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::DOWN_TOWER, L"��������� �����"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ANT, L"�������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::COW, L"������"));

	//manual
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SPEED, L"��������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SIZE, L"������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ARMOR, L"�����"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HEALTH, L"��������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::ABILITY, L"������ �����������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SMALL, L"������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::MID, L"�������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BIG, L"�������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::SLOW, L"���������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::NORMAL_SPEED, L"����������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::FAST, L"�������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::NEXT, L"������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::PREVIOUS, L"�����"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::CREDITS, L"������"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::BACK, L"���������"));

	//difficult
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::EASY, L"Easy"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::NORMAL, L"Normal"));
	frenchTranslation.insert(std::pair<int, std::wstring>(GAME_TRANSLATION::HARD, L"Hard"));


	Engine::Instance().translationsManager()->addTranslation(std::string("russian"), russainTranslation);
	Engine::Instance().translationsManager()->addTranslation(std::string("english"), englishTranslation);
	Engine::Instance().translationsManager()->addTranslation(std::string("french"), frenchTranslation);
}
