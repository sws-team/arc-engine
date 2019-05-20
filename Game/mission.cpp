#include "mission.h"
#include "json/json.h"
#include "globalvariables.h"
#include "objectsfactory.h"

Campaign::Campaign()
{

}

Campaign &Campaign::Instance()
{
	static Campaign instance;
	return instance;
}

bool Campaign::loadMissions(const string &fileName)
{
	ifstream stream(fileName, std::ifstream::in);

	Json::Reader reader;
	Json::Value obj;
	if (!reader.parse(stream, obj))
	{
		cout << "cant read " << fileName << endl;
		return false;
	}

	for (unsigned int i = 0; i < obj.size(); i++)
	{
		Mission mission;
		mission.backgroundSound = obj[i]["sound"].asString();
		mission.id = obj[i]["id"].asInt();

		mission.speed = obj[i]["speed"].asFloat();
//		mission.loop = obj[i]["loop"].asString();
//		const Json::Value& maps = obj[i]["maps"];
//		for (unsigned int j = 0; j < maps.size(); ++j)
//			mission.sprites.push_back(maps[j].asString());

		mission.maps = obj[i]["maps"].asInt();

		const Json::Value& boss = obj[i]["boss"];
		mission.boss.x = boss["x"].asFloat();
		mission.boss.type = static_cast<SPACESHIP_TYPES>(boss["type"].asInt());

		const Json::Value& enemies = obj[i]["enemies"];

		for (unsigned int j = 0; j < enemies.size(); ++j)
		{
			Mission::Enemy enemy;			
			const Json::Value& jsonEnemy = enemies[j];

			enemy.spaceShipType =  static_cast<SPACESHIP_TYPES>(jsonEnemy["type"].asInt());
			enemy.pos.x = jsonEnemy["startX"].asFloat();
			enemy.pos.y = jsonEnemy["startY"].asFloat();
			if (jsonEnemy.isMember("artifact"))
				enemy.artifact = static_cast<ARTIFACT_TYPES>(jsonEnemy["artifact"].asInt());
			else
				enemy.artifact = ARTIFACT_TYPES::UNKNOWN_ARTIFACT;

			if (jsonEnemy.isMember("crew"))
			{
				const Json::Value& crew = jsonEnemy["crew"];
				for (unsigned int k = 0; k < crew.size(); ++k)
					enemy.characters.push_back(static_cast<CHARACTERS>(crew[k].asInt()));
			}
			if (jsonEnemy.isMember("weapons"))
			{
				const Json::Value& weapons = jsonEnemy["weapons"];
				for (unsigned int k = 0; k < weapons.size(); ++k)
					enemy.weapons.push_back(static_cast<WEAPON_TYPES>(weapons[k].asInt()));
			}
			if (jsonEnemy.isMember("stats"))
			{
				const Json::Value& characteristics = jsonEnemy["stats"];
				enemy.stats.attack_speed = characteristics["attack_speed"].asFloat();
				enemy.stats.crit_chance = characteristics["crit_chance"].asFloat();
				enemy.stats.crit_damage = characteristics["crit_damage"].asFloat();
				enemy.stats.reflection = characteristics["reflection"].asFloat();
				enemy.stats.regen_rate = characteristics["regen_rate"].asFloat();
				enemy.stats.regen_energy_rate = characteristics["regen_energy_rate"].asFloat();
				enemy.stats.health = characteristics["health"].asFloat();
				enemy.stats.energy = characteristics["energy"].asFloat();
				enemy.stats.shield = characteristics["shield"].asFloat();
				enemy.stats.ability_cost = characteristics["ability_cost"].asFloat();
			}

			const Json::Value& move = jsonEnemy["actions"];
			enemy.move.looped = move["looped"].asBool();
			enemy.move.count = move["steps"].asInt();

			const Json::Value& directions = move["directions"];
			for (unsigned int k = 0; k < directions.size(); ++k)
					enemy.move.directions.push_back(static_cast<ENEMY_ACTIONS>(directions[k].asInt()));

			mission.enemies.push_back(enemy);
		}
		missions.push_back(mission);
	}
	return true;
}

bool Campaign::loadSpaceShips(const string &fileName)
{
	ifstream stream(fileName, std::ifstream::in);

	Json::Reader reader;
	Json::Value obj;
	if (!reader.parse(stream, obj))
	{
		cout << "cant read " << fileName << endl;
		return false;
	}
	map<SPACESHIP_TYPES, SpaceShipStats> spaceShipStats;
	for (unsigned int i = 0; i < obj.size(); i++)
	{
		const SPACESHIP_TYPES type = static_cast<SPACESHIP_TYPES>(obj[i]["type"].asInt());

		SpaceShipStats stats;
		stats.isStatic = obj[i]["static"].asBool();
		const Json::Value& data = obj[i]["data"];
		stats.defaultData.health = data["health"].asFloat();
		stats.defaultData.shield = data["shield"].asFloat();
		stats.defaultData.energy = data["energy"].asFloat();
		stats.enginePower = obj[i]["enginePower"].asFloat();
		stats.shootPosCount = static_cast<SHOOT_POSTITIONS>(obj[i]["shootPos"].asInt());

		spaceShipStats.insert(pair<SPACESHIP_TYPES, SpaceShipStats>(type, stats));
	}
	ObjectsFactory::Instance().setSpaceShipStats(spaceShipStats);
	return true;
}

bool Campaign::loadWeapons(const string &fileName)
{
	ifstream stream(fileName, std::ifstream::in);

	Json::Reader reader;
	Json::Value obj;
	if (!reader.parse(stream, obj))
	{
		cout << "cant read " << fileName << endl;
		return false;
	}
	map<WEAPON_TYPES, WeaponStats> weaponsStats;
	for (unsigned int i = 0; i < obj.size(); i++)
	{
		const WEAPON_TYPES type = static_cast<WEAPON_TYPES>(obj[i]["type"].asInt());
		WeaponStats stats;
		stats.attackSpeed = obj[i]["attackSpeed"].asFloat();
		stats.maxCount = obj[i]["maxCount"].asUInt();
		stats.damage = obj[i]["damage"].asFloat();
		stats.projectileSpeed = obj[i]["projectileSpeed"].asFloat();
		stats.sound = obj[i]["sound"].asString();
		stats.icon = obj[i]["icon"].asString();
		stats.ammoCost = obj[i]["ammoCost"].asFloat();
		stats.ammoSuply = obj[i]["ammoSuply"].asInt();
		stats.cost = obj[i]["cost"].asFloat();
		weaponsStats.insert(pair<WEAPON_TYPES, WeaponStats>(type, stats));
	}
	ObjectsFactory::Instance().setWeaponsStats(weaponsStats);
	return true;
}
