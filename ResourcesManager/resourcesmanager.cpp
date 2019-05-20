#include "resourcesmanager.h"

ResourcesManager::ResourcesManager()
{
	loadTextures();
	loadMapTextures();
}

ResourcesManager &ResourcesManager::Instance()
{
	static ResourcesManager instance;
	return instance;
}

Texture &ResourcesManager::getTexture(RESOURCES::TEXTURE_TYPE type)
{
	return m_textures.at(type);
}

Texture &ResourcesManager::getMapTexture(int id, int n)
{
	return maps_textures.at(id).at(n);
}

void ResourcesManager::loadTextures()
{
	addTexture(RESOURCES::PANEL_TEXTURE, "images/panel.png");

	addTexture(RESOURCES::GEAR, "images/SpaceShips/gear.png");

	addTexture(RESOURCES::GARAGE_BACKGROUND, "images/Locations/garage.png");
	addTexture(RESOURCES::ABOUT_BACKGROUND, "images/about.png");
	addTexture(RESOURCES::SETTINGS_BACKGROUND, "images/options.png");
	addTexture(RESOURCES::MENU_BACKGROUND, "images/menu.png");
	addTexture(RESOURCES::LOAD_GAME_BACKGROUND, "images/Windows/SavedGameWindow.png");
	addTexture(RESOURCES::NEW_GAME_BACKGROUND, "images/newGameBackground.png");
	addTexture(RESOURCES::CLOSE_BACKGROUND, "images/close.png");
	addTexture(RESOURCES::INTRO_BACKGROUND, "images/logo.png");
	addTexture(RESOURCES::SHOP_LOCATION_BACKGROUND, "images/Locations/bar.png");
	addTexture(RESOURCES::PLANETS_BACKGROUND, "images/Locations/planets.png");

	addTexture(RESOURCES::MINIGUN_ICON, "images/icons/Weapons/minigun_icon.png");
	addTexture(RESOURCES::ROCKET_ICON, "images/icons/Weapons/rocket_icon.png");
	addTexture(RESOURCES::LASER_ICON, "images/icons/Weapons/laser_icon.png");
	addTexture(RESOURCES::FLAMETHOWER_ICON, "images/icons/Weapons/flamethower_icon.png");

	addTexture(RESOURCES::ARTIFACT_1, "images/icons/Artifacts/artifact_1.png");
	addTexture(RESOURCES::ARTIFACT_2, "images/icons/Artifacts/artifact_2.png");
	addTexture(RESOURCES::ARTIFACT_3, "images/icons/Artifacts/artifact_3.png");
	addTexture(RESOURCES::ARTIFACT_4, "images/icons/Artifacts/artifact_4.png");

	addTexture(RESOURCES::CRITICAL_EFFECT, "images/Effects/critical.png");
	addTexture(RESOURCES::BURN_EFFECT, "images/Weapons/burn.png");
	addTexture(RESOURCES::EXPLOSION_EFFECT, "images/Effects/explosion.png");

	addTexture(RESOURCES::PLANE, "images/SpaceShips/plane.png");
	addTexture(RESOURCES::ENEMY_0, "images/Enemies/enemy.png");
	addTexture(RESOURCES::ENEMY_1, "images/Enemies/SPOILER_enemy1p.png");
	addTexture(RESOURCES::ENEMY_2, "images/Enemies/SPOILER_enemy2p.png");
	addTexture(RESOURCES::ENEMY_3, "images/Enemies/SPOILER_enemy3.png");
	addTexture(RESOURCES::BOSS_HELICOPTER, "images/Enemies/Bosses/boss1.png");

	addTexture(RESOURCES::PLANE_ICON, "images/icons/SpaceShips/plane_icon.png");

	addTexture(RESOURCES::MINE_PROJECTILE, "images/Weapons/mine.png");
	addTexture(RESOURCES::FLAMETHOWER_PROJECTILE, "images/Weapons/flamethrower.png");
	addTexture(RESOURCES::BEAM_PROJECTILE, "images/Weapons/beam.png");
	addTexture(RESOURCES::PLASMA_PROJECTILE, "images/Weapons/plasma.png");
	addTexture(RESOURCES::CANNON_PROJECTILE, "images/Weapons/cannon.png");
	addTexture(RESOURCES::RANDOM_GUN_PROJECTILE, "images/Weapons/randomgun.png");
	addTexture(RESOURCES::ROCKET_PROJECTILE, "images/Weapons/rocket.png");
	addTexture(RESOURCES::LASER_PROJECTILE, "images/Weapons/laser.png");
	addTexture(RESOURCES::BULLET_PROJECTILE, "images/Weapons/bullet.png");
	addTexture(RESOURCES::DEFAULT_PROJECTILE, "images/Weapons/default.png");
	addTexture(RESOURCES::SHOTGUN_PROJECTILE, "images/Weapons/shotgun.png");
	addTexture(RESOURCES::BLAST_PROJECTILE, "images/Weapons/blast.png");
	addTexture(RESOURCES::MORTAR_AIM, "images/Weapons/mortar.png");

	addTexture(RESOURCES::ENEMY_CHARACTER_ICON, "images/icons/Characters/enemy.png");
	addTexture(RESOURCES::RANDOM_CHARACTER_ICON, "images/icons/Characters/random_character.png");
	addTexture(RESOURCES::DOUBLE_DAMAGE_CHARACTER_ICON, "images/icons/Characters/double_damage_character.png");
	addTexture(RESOURCES::REPAIR_CHARACTER_ICON, "images/icons/Characters/repair_character.png");
	addTexture(RESOURCES::SUPLY_CHARACTER_ICON, "images/icons/Characters/suply_character.png");
	addTexture(RESOURCES::CRITICAL_DAMAGE_CHARACTER_ICON, "images/icons/Characters/critical_damage_character.png");
	addTexture(RESOURCES::ATTACK_SPEED_CHARACTER_ICON, "images/icons/Characters/attack_speed_character.png");
	addTexture(RESOURCES::BOMB_CHARACTER_ICON, "images/icons/Characters/bomb_character.png");
	addTexture(RESOURCES::LINE_CHARACTER_ICON, "images/icons/Characters/line_character.png");
	addTexture(RESOURCES::VAMPIRISM_CHARACTER_ICON, "images/icons/Characters/vampirism_character.png");
	addTexture(RESOURCES::BACK_DAMAGE_CHARACTER_ICON, "images/icons/Characters/back_damage_character.png");
	addTexture(RESOURCES::TRIPLE_SHOT_CHARACTER_ICON, "images/icons/Characters/triple_shoot_character.png");
	addTexture(RESOURCES::TRANSFUSION_CHARACTER_ICON, "images/icons/Characters/transfusion_character.png");

	addTexture(RESOURCES::ENEMY_ABILITY_ICON, "images/icons/Abilities/enemy_ability.png");
	addTexture(RESOURCES::DOUBLE_DAMAGE_ABILITY_ICON, "images/icons/Abilities/ability_6doubledamage.png");
	addTexture(RESOURCES::REPAIR_ABILITY_ICON, "images/icons/Abilities/ability_7repair.png");
	addTexture(RESOURCES::SUPLY_ABILITY_ICON, "images/icons/Abilities/ability_4ammo.png");
	addTexture(RESOURCES::CRITICAL_DAMAGE_ABILITY_ICON, "images/icons/Abilities/ability_3critical.png");

	addTexture(RESOURCES::UI_UP, "images/Locations/up.png");
	addTexture(RESOURCES::UI_DOWN, "images/Locations/down.png");
	addTexture(RESOURCES::PLAYER_1, "images/ui/player1.png");
	addTexture(RESOURCES::PLAYER_2, "images/ui/player2.png");
	addTexture(RESOURCES::PLAYER_3, "images/ui/player3.png");
	addTexture(RESOURCES::PLAYER_4, "images/ui/player4.png");
	addTexture(RESOURCES::CONTROL_1, "images/ui/control0.png");
	addTexture(RESOURCES::CONTROL_2, "images/ui/control1.png");
	addTexture(RESOURCES::CONTROL_3, "images/ui/control2.png");
	addTexture(RESOURCES::CONTROL_4, "images/ui/control3.png");
	addTexture(RESOURCES::FRAME_0, "images/ui/frame0.png");
	addTexture(RESOURCES::FRAME_1, "images/ui/frame1.png");
	addTexture(RESOURCES::FRAME_2, "images/ui/frame2.png");
	addTexture(RESOURCES::FRAME_3, "images/ui/frame3.png");
	addTexture(RESOURCES::FRAME_4, "images/ui/frame4.png");

//	addTexture(RESOURCES::ENEMY_CHARACTER_ICON, "images/SpaceShips/gear.png");
//	addTexture(RESOURCES::ENEMY_CHARACTER_ICON, "images/SpaceShips/gear.png");
//	addTexture(RESOURCES::ENEMY_CHARACTER_ICON, "images/SpaceShips/gear.png");
//	addTexture(RESOURCES::ENEMY_CHARACTER_ICON, "images/SpaceShips/gear.png");
//	addTexture(RESOURCES::ENEMY_CHARACTER_ICON, "images/SpaceShips/gear.png");
//	addTexture(RESOURCES::ENEMY_CHARACTER_ICON, "images/SpaceShips/gear.png");
//	addTexture(RESOURCES::ENEMY_CHARACTER_ICON, "images/SpaceShips/gear.png");
//	addTexture(RESOURCES::ENEMY_CHARACTER_ICON, "images/SpaceShips/gear.png");
//	addTexture(RESOURCES::ENEMY_CHARACTER_ICON, "images/SpaceShips/gear.png");
//	addTexture(RESOURCES::ENEMY_CHARACTER_ICON, "images/SpaceShips/gear.png");
//	addTexture(RESOURCES::ENEMY_CHARACTER_ICON, "images/SpaceShips/gear.png");


}

void ResourcesManager::addTexture(RESOURCES::TEXTURE_TYPE type, const string &path)
{
	Texture texture;
	texture.loadFromFile(path);
	m_textures.insert(pair<RESOURCES::TEXTURE_TYPE, Texture>(type, texture));
}

void ResourcesManager::loadMapTextures()
{
	for (int i = 1; i <= 5; ++i)
	{
		vector<Texture> textures;
		Texture texture;
		for (int j = 0; j <= 3; ++j)
		{
			char buffer[64];
			sprintf (buffer, "images/Maps/map%d/s%d.png", i, j);
			const string fileName = string(buffer);
			texture.loadFromFile(fileName);
			textures.push_back(texture);
		}
		textures.push_back(texture);
		maps_textures.insert(pair<int, vector<Texture>>(i, textures));
		textures.clear();
	}
}
