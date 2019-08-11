#include "resourcesmanager.h"

ResourcesManager::ResourcesManager()
{
	loadTextures();
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

void ResourcesManager::loadTextures()
{
	addTexture(RESOURCES::PANEL_TEXTURE, "images/ui/Panel/panel.png");
	addTexture(RESOURCES::CURSOR_TEXTURE, "images/cursor.png");
	addTexture(RESOURCES::STAR_TEXTURE, "images/ui/star.png");
	addTexture(RESOURCES::EMPTY_STAR_TEXTURE, "images/ui/empty_star.png");
	addTexture(RESOURCES::SELL_TEXTURE, "images/ui/Panel/sell.png");
	addTexture(RESOURCES::UPGRADE_TEXTURE, "images/ui/Panel/upgrade.png");
	addTexture(RESOURCES::READY_TEXTURE, "images/ui/ready.png");
	addTexture(RESOURCES::GAME_MENU_TEXTURE, "images/gameMenu.png");
	addTexture(RESOURCES::GAME_OVER_TEXTURE, "images/GameOver.png");
	addTexture(RESOURCES::MISSON_COMPLETED_TEXTURE, "images/missionCompleted.png");

	addTexture(RESOURCES::ENEMY_TEXTURE, "images/Enemies/enemy1.png");

	addTexture(RESOURCES::ABILITY_BOMB, "images/ui/Panel/Abilities/ability_bomb.png");
	addTexture(RESOURCES::ABILITY_FREEZE_BOMB, "images/ui/Panel/Abilities/ability_freeze_bomb.png");
	addTexture(RESOURCES::ABILITY_CARPET_BOMBING, "images/ui/Panel/Abilities/ability_carpet_bombing.png");
	addTexture(RESOURCES::ABILITY_INCREASE_TOWER_DAMAGE, "images/ui/Panel/Abilities/ability_increase_tower_damage.png");
	addTexture(RESOURCES::ABILITY_INCREASE_TOWER_ATTACK_SPEED, "images/ui/Panel/Abilities/ability_increase_tower_speed.png");
	addTexture(RESOURCES::ABILITY_TIME_STOP, "images/ui/Panel/Abilities/ability_time_stop.png");

	addTexture(RESOURCES::TOWER_BASE, "images/ui/Panel/Towers/tower_base.png");
	addTexture(RESOURCES::TOWER_POWER, "images/ui/Panel/Towers/tower_power.png");
	addTexture(RESOURCES::TOWER_ROCKET, "images/ui/Panel/Towers/tower_rocket.png");
	addTexture(RESOURCES::TOWER_FREEZE, "images/ui/Panel/Towers/tower_freeze.png");
	addTexture(RESOURCES::TOWER_LASER, "images/ui/Panel/Towers/tower_laser.png");
	addTexture(RESOURCES::TOWER_IMPROVED, "images/ui/Panel/Towers/tower_improved.png");

	addTexture(RESOURCES::BASE_PROJECTILE, "images/Weapons/bullet.png");
	addTexture(RESOURCES::IMPROVED_PROJECTILE, "images/Weapons/bullet.png");
	addTexture(RESOURCES::FREEZE_PROJECTILE, "images/Weapons/bullet.png");
	addTexture(RESOURCES::ROCKET_PROJECTILE, "images/Weapons/rocket.png");
	addTexture(RESOURCES::LASER_PROJECTILE, "images/Weapons/laser.png");

	addTexture(RESOURCES::ABOUT_BACKGROUND, "images/about.png");
	addTexture(RESOURCES::SETTINGS_BACKGROUND, "images/options.png");
	addTexture(RESOURCES::MENU_BACKGROUND, "images/menu.png");
	addTexture(RESOURCES::CLOSE_BACKGROUND, "images/close.png");
	addTexture(RESOURCES::INTRO_BACKGROUND, "images/logo.png");
}

void ResourcesManager::addTexture(RESOURCES::TEXTURE_TYPE type, const string &path)
{
	Texture texture;
	texture.loadFromFile(path);
	m_textures.insert(pair<RESOURCES::TEXTURE_TYPE, Texture>(type, texture));
}
