#include "objectsfactory.h"
#include "SpaceShip/Weapons/abstractweapon.h"
#include "SpaceShip/Weapons/Weapons/machinegun.h"
#include "SpaceShip/Weapons/Weapons/laser.h"
#include "SpaceShip/Weapons/Weapons/flamethrower.h"
#include "SpaceShip/Weapons/Weapons/rocket.h"
#include "SpaceShip/Weapons/Weapons/beam.h"
#include "SpaceShip/Weapons/Weapons/mine.h"
#include "SpaceShip/Weapons/Weapons/randomgun.h"
#include "SpaceShip/Weapons/Weapons/plasma.h"
#include "SpaceShip/Weapons/Weapons/cannon.h"
#include "SpaceShip/Weapons/Weapons/shotgun.h"
#include "SpaceShip/Weapons/Weapons/acoustic.h"
#include "SpaceShip/Weapons/Weapons/mortar.h"
#include "settings.h"

ObjectsFactory::ObjectsFactory()
{

}

void ObjectsFactory::setSpaceShipStats(const map<SPACESHIP_TYPES, SpaceShipStats> &spaceShipStats)
{
	m_spaceShipStats = spaceShipStats;
}

void ObjectsFactory::setWeaponsStats(const map<WEAPON_TYPES, WeaponStats> &weaponsStats)
{
	m_weaponsStats = weaponsStats;
}

ObjectsFactory &ObjectsFactory::Instance()
{
	static ObjectsFactory instance;
	return instance;
}

AbstractWeapon *ObjectsFactory::createWeapon(WEAPON_TYPES type)
{
	AbstractWeapon *weapon = nullptr;
	switch (type)
	{
	case DEFAULT:
		weapon = new MachineGun(RESOURCES::DEFAULT_PROJECTILE, Vector2i(6, 10), 3);
		break;
	case MINIGUN:
		weapon = new MachineGun(RESOURCES::BULLET_PROJECTILE, Vector2i(6, 10), 3);
		break;
	case LASER:
		weapon = new Laser(RESOURCES::LASER_PROJECTILE, Vector2i(12, 1080), 4);
		break;
	case FLAMETHOWER:
		weapon = new Flamethrower();
		break;
	case ROCKET:
		weapon = new Rocket();
		break;
	case PLASMA:
		weapon = new Plasma();
		break;
	case ACOUSTIC:
		weapon = new Acoustic();
		break;
	case SHOTGUN:
		weapon = new Shotgun();
		break;
	case BEAM:
		weapon = new Beam();
		break;
	case GATLING:
		weapon = new MachineGun(RESOURCES::BULLET_PROJECTILE, Vector2i(6, 10), 3);
		break;
	case CANNON:
		weapon = new Cannon();
		break;
	case MINE:
		weapon = new Mine();
		break;
	case RANDOM_GUN:
		weapon = new RandomGun();
		break;
	case MORTAR:
		weapon = new Mortar();
		break;
//	case ELECTRIC:
//		weapon = new Electric();
//		break;
	default:
		break;
	}
	weapon->setType(type);
	weapon->setStats(m_weaponsStats.at(type));
	return weapon;
}

WeaponStats ObjectsFactory::weaponStats(WEAPON_TYPES type) const
{
	return m_weaponsStats.at(type);
}

SpaceShipStats ObjectsFactory::getSpaceShipStats(SPACESHIP_TYPES type) const
{
	return m_spaceShipStats.at(type);
}

SpaceShip *ObjectsFactory::createSpaceShip(SPACESHIP_TYPES type, const Vector2f &pos)
{
	SpaceShip *spaceShip = nullptr;
	Characteristics characteristics;
	switch (type)
	{
	case SHIP_1:
	{
		spaceShip = new SpaceShip(getSpaceShipStats(type), RESOURCES::PLANE, RESOURCES::PLANE_ICON, pos, Vector2i(256, 124), 4);
	}
		break;
	case SHIP_2:
	{
		spaceShip = new SpaceShip(getSpaceShipStats(type), RESOURCES::ENEMY_0, RESOURCES::PLANE_ICON, pos, Vector2i(126, 126), 6);
	}
		break;
	case SHIP_3:
	{
		spaceShip = new SpaceShip(getSpaceShipStats(type), RESOURCES::ENEMY_1, RESOURCES::PLANE_ICON, pos, Vector2i(115, 160), 4);
	}
		break;
	case SHIP_4:
	{
		spaceShip = new SpaceShip(getSpaceShipStats(type), RESOURCES::ENEMY_2, RESOURCES::PLANE_ICON, pos, Vector2i(200, 149), 4);
	}
		break;
	case SHIP_5:
	{
		spaceShip = new SpaceShip(getSpaceShipStats(type), RESOURCES::ENEMY_3, RESOURCES::PLANE_ICON, pos, Vector2i(66, 200), 4);
	}
		break;
	case BOSS_1:
	{
		spaceShip = new SpaceShip(getSpaceShipStats(type), RESOURCES::BOSS_HELICOPTER, RESOURCES::PLANE_ICON, pos, Vector2i(90, 128), 2);
		characteristics.regen_rate = 0.5f;
	}
		break;
	default:
		break;
	}
	if (spaceShip != nullptr)
	{
		spaceShip->setStats(characteristics);
		spaceShip->setType(type);
	}
	return spaceShip;
}

vector<Vector2f> ObjectsFactory::getShootingPos(SHOOT_POSTITIONS type, SpaceShip* spaceShip) const
{

	vector<Vector2f> vecPos;
	if (type == SHOOT_POSTITIONS::CENTER || type == SHOOT_POSTITIONS::CENTER_AND_WINGS || type == SHOOT_POSTITIONS::ALL_POS)
	{
		vecPos.push_back(Vector2f(spaceShip->getSize().x * Settings::Instance().getScaleFactor().x/2, 0));
	}
	if (type == SHOOT_POSTITIONS::WINGS || type == SHOOT_POSTITIONS::CENTER_AND_WINGS || type == SHOOT_POSTITIONS::ALL_POS)
	{
		vecPos.push_back(Vector2f(spaceShip->getSize().x * Settings::Instance().getScaleFactor().x/3, 0));
		vecPos.push_back(Vector2f(spaceShip->getSize().x * Settings::Instance().getScaleFactor().x * 2/3, 0));
	}
	if (type == SHOOT_POSTITIONS::ALL_POS || type == SHOOT_POSTITIONS::BORDERS)
	{
		vecPos.push_back(Vector2f(0.f, 0.f));
		vecPos.push_back(Vector2f(spaceShip->getSize().x * Settings::Instance().getScaleFactor().x, 0));
	}
	return vecPos;
}

Character *ObjectsFactory::createCharacter(CHARACTERS type)
{
	Character *character = nullptr;
	switch (type)
	{
	case DOUBLE_DAMAGE_CHARACTER:
		character = new DoubleDamageCharacter(RESOURCES::DOUBLE_DAMAGE_CHARACTER_ICON, RESOURCES::DOUBLE_DAMAGE_ABILITY_ICON);
		break;
	case REPAIR_CHARACTER:
		character = new RepairCharacter(RESOURCES::REPAIR_CHARACTER_ICON, RESOURCES::REPAIR_ABILITY_ICON);
		break;
	case SUPLY_CHARACTER:
		character = new SuplyCharacter(RESOURCES::SUPLY_CHARACTER_ICON, RESOURCES::SUPLY_ABILITY_ICON);
		break;
	case CRITICAL_DAMAGE_CHARACTER:
		character = new CriticalDamageCharacter(RESOURCES::CRITICAL_DAMAGE_CHARACTER_ICON, RESOURCES::CRITICAL_DAMAGE_ABILITY_ICON);
		break;
	case LINE_CHARACTER:
		character = new LineCharacter(RESOURCES::LINE_CHARACTER_ICON, RESOURCES::ENEMY_ABILITY_ICON);
		break;
	case PLAYER_BOMB_CHARACTER:
		character = new PlayerBombCharacter(RESOURCES::BOMB_CHARACTER_ICON, RESOURCES::ENEMY_ABILITY_ICON);
		break;
	case ATTACK_SPEED_CHARACTER:
		character = new AttackSpeedCharacter(RESOURCES::ATTACK_SPEED_CHARACTER_ICON, RESOURCES::ENEMY_ABILITY_ICON);
		break;
	case VAMPIRISM_CHARACTER:
		character = new VampirismCharacter(RESOURCES::VAMPIRISM_CHARACTER_ICON, RESOURCES::ENEMY_ABILITY_ICON);
		break;
	case BACK_DAMAGE_CHARACTER:
		character = new BackDamageCharacter(RESOURCES::BACK_DAMAGE_CHARACTER_ICON, RESOURCES::ENEMY_ABILITY_ICON);
		break;
	case TRIPLE_SHOT_CHARACTER:
	{
		MultipleShotCharacter *multipleShotCharacter = new MultipleShotCharacter(RESOURCES::BACK_DAMAGE_CHARACTER_ICON, RESOURCES::ENEMY_ABILITY_ICON);
		multipleShotCharacter->setShotMultiple(1);
		multipleShotCharacter->setMultipleAngle(20.f);
		character = multipleShotCharacter;
	}
		break;
	case TRANSFUSION_CHARACTER:
		character = new TransfusionCharacter(RESOURCES::TRANSFUSION_CHARACTER_ICON, RESOURCES::ENEMY_ABILITY_ICON);
		break;
		//enemies
	case DECAY_CHARACTER:
		character = new DecayCharacter(RESOURCES::ENEMY_CHARACTER_ICON, RESOURCES::ENEMY_ABILITY_ICON);
		break;
	case SHELL_CHARACTER:
		character = new ShellCharacter(RESOURCES::ENEMY_CHARACTER_ICON, RESOURCES::ENEMY_ABILITY_ICON);
		break;
	case ENEMY_CHARACTER:
		character = new Character(RESOURCES::ENEMY_CHARACTER_ICON, RESOURCES::ENEMY_ABILITY_ICON);
		break;
	case TELEPORT_CHARACTER:
		character = new TeleportCharacter(RESOURCES::ENEMY_CHARACTER_ICON, RESOURCES::ENEMY_ABILITY_ICON);
		break;
	case GROW_CHARACTER:
		character = new GrowCharacter(RESOURCES::ENEMY_CHARACTER_ICON, RESOURCES::ENEMY_ABILITY_ICON);
		break;
	case ENEMY_BOMB_CHARACTER:
		character = new EnemyBombCharacter(RESOURCES::ENEMY_CHARACTER_ICON, RESOURCES::ENEMY_ABILITY_ICON);
		break;
	case BLAST_CHARACTER:
		character = new BlastCharacter(RESOURCES::ENEMY_CHARACTER_ICON, RESOURCES::ENEMY_ABILITY_ICON);
		break;
	default:
		character = new Character(RESOURCES::ENEMY_CHARACTER_ICON, RESOURCES::ENEMY_ABILITY_ICON);
		break;
	}
	character->setType(type);
	character->init();
	return character;
}

Artifact *ObjectsFactory::createArtifact(ARTIFACT_TYPES type)
{
	string icon;
	icon = "images/icons/Artifacts/default.png";
	Characteristics stats;
	switch (type)
	{
	case CRITICAL_DAMAGE_ARTIFACT:
		stats.crit_damage = 0.15f;
		icon = "images/icons/Artifacts/artifact_2.png";
		break;
	case DAMAGE_ARTIFACT:
		stats.damage = 0.15f;
		icon = "images/icons/Artifacts/artifact_1.png";
		break;
	case REFLECTION_ARTIFACT:
		stats.reflection = 0.2f;
		icon = "images/icons/Artifacts/artifact_4.png";
		break;
	case CRITICAL_CHANCE_ARTIFACT:
		stats.crit_chance = 0.2f;
		icon = "images/icons/Artifacts/artifact_4.png";
		break;
	case ATTACK_SPEED_ARTIFACT:
		stats.attack_speed = 0.2f;
		icon = "images/icons/Artifacts/artifact_4.png";
		break;
	case REGEN_RATE_ARTIFACT:
		stats.regen_rate = 0.2f;
		icon = "images/icons/Artifacts/artifact_4.png";
		break;
	case REGEN_ENERGY_ARTIFACT:
		stats.regen_energy_rate = 0.2f;
		icon = "images/icons/Artifacts/artifact_4.png";
		break;
	case HEALTH_ARTIFACT:
		stats.health = 0.2f;
		stats.shield = 0.2f;
		icon = "images/icons/Artifacts/artifact_4.png";
		break;
	case ENERGY_ARTIFACT:
		stats.energy = 0.2f;
		icon = "images/icons/Artifacts/artifact_4.png";
		break;
	case ABILITY_COST_ARTIFACT:
		stats.ability_cost = 0.2f;
		icon = "images/icons/Artifacts/artifact_4.png";
		break;
	case ALL_STATS_ARTIFACT:
		stats.damage = 0.05f;
		stats.attack_speed = 0.05f;
		stats.crit_chance = 0.05f;
		stats.crit_damage = 0.05f;
		stats.reflection = 0.05f;
		stats.regen_rate = 0.05f;
		stats.regen_energy_rate = 0.05f;
		stats.health = 0.05f;
		stats.energy = 0.05f;
		stats.shield = 0.05f;
		icon = "images/icons/Artifacts/artifact_4.png";
		break;
	default:
		break;
	}

	Artifact *artifact = new Artifact(stats, icon);
	artifact->setType(type);
	return artifact;
}
