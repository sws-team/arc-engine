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
	addTexture(RESOURCES::PANEL_TEXTURE, "images/ui/panel.png");
	addTexture(RESOURCES::CURSOR_TEXTURE, "images/ui/game_cursor.png");
	addTexture(RESOURCES::STAR_TEXTURE, "images/ui/star.png");
	addTexture(RESOURCES::EMPTY_STAR_TEXTURE, "images/ui/empty_star.png");
	addTexture(RESOURCES::SELL_TEXTURE, "images/ui/sell.png");
	addTexture(RESOURCES::UPGRADE_TEXTURE, "images/ui/upgrade.png");
	addTexture(RESOURCES::GAME_MENU_TEXTURE, "images/gameMenu.png");
	addTexture(RESOURCES::WINDOW_TEXTURE, "images/window.png");
	addTexture(RESOURCES::PANEL_CURSOR, "images/ui/panel_cursor.png");
	addTexture(RESOURCES::MONEY_ICON, "images/ui/money.png");
	addTexture(RESOURCES::LIFE_ICON, "images/ui/money.png");
	addTexture(RESOURCES::DIRECTION_TEXTURE, "images/ui/direction.png");
	addTexture(RESOURCES::FOCUS_ICON, "images/ui/focus.png");
	addTexture(RESOURCES::LOCKED_ICON, "images/ui/locked.png");
	addTexture(RESOURCES::INSTRUCTIONS_TEXTURE, "images/ui/instructions.png");
	addTexture(RESOURCES::NEXT, "images/ui/right.png");
	addTexture(RESOURCES::PREVIOUS, "images/ui/left.png");
	addTexture(RESOURCES::EXIT, "images/ui/exit.png");
	addTexture(RESOURCES::CREDITS, "images/ui/credits.png");

	addTexture(RESOURCES::ABILITY_BOMB, "images/ui/Abilities/ability_bomb.png");
	addTexture(RESOURCES::ABILITY_FREEZE_BOMB, "images/ui/Abilities/ability_freeze_bomb.png");
	addTexture(RESOURCES::ABILITY_ACID, "images/ui/Abilities/ability_acid.png");
	addTexture(RESOURCES::ABILITY_INCREASE_TOWER_DAMAGE, "images/ui/Abilities/ability_increase_tower_damage.png");
	addTexture(RESOURCES::ABILITY_INCREASE_TOWER_ATTACK_SPEED, "images/ui/Abilities/ability_increase_tower_speed.png");
	addTexture(RESOURCES::ABILITY_TIME_STOP, "images/ui/Abilities/ability_stop.png");

	addTexture(RESOURCES::TOWER_BASE, "images/ui/Towers/tower_base.png");
	addTexture(RESOURCES::TOWER_POWER, "images/ui/Towers/tower_power.png");
	addTexture(RESOURCES::TOWER_ROCKET, "images/ui/Towers/tower_rocket.png");
	addTexture(RESOURCES::TOWER_FREEZE, "images/ui/Towers/tower_freeze.png");
	addTexture(RESOURCES::TOWER_LASER, "images/ui/Towers/tower_laser.png");
	addTexture(RESOURCES::TOWER_IMPROVED, "images/ui/Towers/tower_improved.png");

	addTexture(RESOURCES::BASE_PROJECTILE, "images/Weapons/bullet.png");
	addTexture(RESOURCES::IMPROVED_PROJECTILE, "images/Weapons/improved_bullet.png");
	addTexture(RESOURCES::FREEZE_PROJECTILE, "images/Weapons/freeze_bullet.png");
	addTexture(RESOURCES::ROCKET_PROJECTILE, "images/Weapons/rocket.png");
	addTexture(RESOURCES::LASER_PROJECTILE, "images/Weapons/laser.png");

	addTexture(RESOURCES::ABOUT_BACKGROUND, "images/about.png");
	addTexture(RESOURCES::SETTINGS_BACKGROUND, "images/options.png");
	addTexture(RESOURCES::MENU_BACKGROUND, "images/menu.png");
	addTexture(RESOURCES::CLOSE_BACKGROUND, "images/close.png");
	addTexture(RESOURCES::INTRO_BACKGROUND, "images/logo.png");

	//effects
	addTexture(RESOURCES::BASE_EXPLOSION_EFFECT, "images/Effects/base_explosion.png");
	addTexture(RESOURCES::FREEZE_EXPLOSION_EFFECT, "images/Effects/freeze_explosion.png");
	addTexture(RESOURCES::ROCKET_EXPLOSION_EFFECT, "images/Effects/rocket_explosion.png");
	addTexture(RESOURCES::IMPROVED_EXPLOSION_EFFECT, "images/Effects/improved_explosion.png");
	addTexture(RESOURCES::VENOM_EFFECT, "images/Effects/venom.png");
	addTexture(RESOURCES::BOMB_EXPLOSION, "images/Effects/bomb_explosion.png");
	addTexture(RESOURCES::FREEZE_BOMB_EXPLOSION, "images/Effects/freeze_bomb_explosion.png");
	addTexture(RESOURCES::HEAL_EFFECT, "images/Effects/heal.png");
	addTexture(RESOURCES::SHELL_EFFECT, "images/Effects/shell.png");
	addTexture(RESOURCES::WEB, "images/Effects/web.png");
	addTexture(RESOURCES::DOWN_EFFECT, "images/Effects/down.png");
	addTexture(RESOURCES::DOWNGRADE_PROJECTILE, "images/Weapons/electric.png");
	addTexture(RESOURCES::UPGRADE, "images/Effects/upgrade.png");
	addTexture(RESOURCES::SMOKE, "images/Effects/smoke.png");
	addTexture(RESOURCES::EXPLOSION, "images/Effects/explosion.png");
	addTexture(RESOURCES::DRAIN, "images/Effects/drain.png");
	addTexture(RESOURCES::REGRESS, "images/Effects/regress.png");
	addTexture(RESOURCES::BURN, "images/Effects/burn.png");
	addTexture(RESOURCES::BLAST, "images/Effects/blast.png");

	//map icons
	addTexture(RESOURCES::MAP_ICON_MISSION_1, "images/maps_icons/mission1.png");
	addTexture(RESOURCES::MAP_ICON_MISSION_2, "images/maps_icons/mission2.png");
	addTexture(RESOURCES::MAP_ICON_MISSION_3, "images/maps_icons/mission3.png");
	addTexture(RESOURCES::MAP_ICON_MISSION_4, "images/maps_icons/mission4.png");
	addTexture(RESOURCES::MAP_ICON_MISSION_5, "images/maps_icons/mission5.png");
	addTexture(RESOURCES::MAP_ICON_MISSION_6, "images/maps_icons/mission6.png");
	addTexture(RESOURCES::MAP_ICON_MISSION_7, "images/maps_icons/mission7.png");
	addTexture(RESOURCES::MAP_ICON_MISSION_8, "images/maps_icons/mission8.png");
	addTexture(RESOURCES::MAP_ICON_MISSION_9, "images/maps_icons/mission9.png");
	addTexture(RESOURCES::MAP_ICON_MISSION_10, "images/maps_icons/mission10.png");
	addTexture(RESOURCES::MAP_ICON_MISSION_11, "images/maps_icons/mission11.png");
	addTexture(RESOURCES::MAP_ICON_MISSION_12, "images/maps_icons/mission12.png");
	addTexture(RESOURCES::MAP_ICON_MISSION_13, "images/maps_icons/mission13.png");
	addTexture(RESOURCES::MAP_ICON_MISSION_14, "images/maps_icons/mission14.png");
	addTexture(RESOURCES::MAP_ICON_MISSION_15, "images/maps_icons/mission15.png");

	//enemies
	addTexture(RESOURCES::ENEMY_SCORPION, "images/Enemies/scorpion.png");
	addTexture(RESOURCES::ENEMY_CAR, "images/Enemies/car.png");
	addTexture(RESOURCES::ENEMY_TRICYCLE, "images/Enemies/tricycle.png");
	addTexture(RESOURCES::ENEMY_TANK, "images/Enemies/tank.png");
	addTexture(RESOURCES::ENEMY_SPIDER, "images/Enemies/spider.png");
	addTexture(RESOURCES::ENEMY_HELICOPTER, "images/Enemies/helicopter.png");
	addTexture(RESOURCES::ENEMY_AIRCARRIER, "images/Enemies/aircarrier.png");
	addTexture(RESOURCES::ENEMY_BIG_TANK, "images/Enemies/big_tank.png");
	addTexture(RESOURCES::ENEMY_PLANE, "images/Enemies/plane.png");
	addTexture(RESOURCES::ENEMY_REPAIR, "images/Enemies/repair.png");
	addTexture(RESOURCES::ENEMY_SHELL, "images/Enemies/shell.png");
	addTexture(RESOURCES::ENEMY_TELEPORT, "images/Enemies/teleport.png");
	addTexture(RESOURCES::ENEMY_SELF_HEAL, "images/Enemies/enemy.png");
	addTexture(RESOURCES::ENEMY_DOWN_TOWER, "images/Enemies/enemy1.png");
	addTexture(RESOURCES::ENEMY_ANT, "images/Enemies/ant.png");
	addTexture(RESOURCES::ENEMY_COW, "images/Enemies/cow.png");

	//objects
	addTexture(RESOURCES::OBJECT_TREE, "images/maps_objects/tree.png");
}

void ResourcesManager::addTexture(RESOURCES::TEXTURE_TYPE type, const string &path)
{
	Texture texture;
	texture.loadFromFile(path);
	m_textures.insert(pair<RESOURCES::TEXTURE_TYPE, Texture>(type, texture));
}
