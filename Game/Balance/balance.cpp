#include "balance.h"
#include "balance_def.h"
#include "engine.h"
#include "managers.h"
#include "gamemanagers.h"

#include <json/json.h>

TowerStats::TowerStats()
{

}

TowerStats::TowerStats(float damage,
					   float attackSpeed,
					   float radius,
					   float projectileSpeed,
					   float cost)
	: damage(damage)
	,attackSpeed(attackSpeed)
	,radius(radius)
	,projectileSpeed(projectileSpeed)
	,cost(cost)
{

}

Balance::Balance() :
	towerUpgradeGain(0.25f)
  ,powerTowerCostOffset(10.f)
  ,blastCells(6.f)
  ,blastCount(5)
  ,baseTowerPenetration(0.00001f)
  ,freezeTowerValue(0.5f)
  ,freezeTowerCells(2)
  ,freezeTowerDuration(2.f * EngineDefs::MSEC)
  ,laserTowerMaxExtraTargets(4)
  ,rocketTowerCells(3)
  ,burnAttackSpeed(0.4f * EngineDefs::MSEC)
  ,burnDamage(20.f)
  ,burnDuration(5.f * EngineDefs::MSEC)
  ,drainValue(1.f)
  ,blindValue(0.3f)
  ,regressValue(0.3f)
  ,selfhealInterval(2.5f)
  ,selfhealValue(0.1f)
  ,healInterval(3.f)
  ,healValue(0.1f)
  ,shieldInterval(10.f)
  ,shieldValue(0.1f)
  ,strongInterval(4.5f)
  ,strongValue(0.05f)
  ,teleportInterval(5.f)
  ,spawnCount(2)
  ,spawnCountOffset(5)
  ,spawnInterval(7.f)
  ,shutdownInterval(5.f)
  ,shutdownDuration(10.f)
  ,shutdownCells(10.f)
  ,downgradeValue(0.5f)
  ,downgradeCells(10.f)
  ,downgradeDuration(9.f)
  ,downgradeInterval(5.f)
  ,killTowerInterval(10.f)
  ,downgradeTowerInterval(15.f)
  ,killTowerCells(15)
  ,downgradeTowerCells(20)
{

}

Balance &Balance::Instance()
{
	static Balance instance;
	return instance;
}

void Balance::load()
{
#ifdef LOAD_BALANCE
	std::string fileName = "balance.bal";
	std::ifstream file;
	file.open(fileName);
	if (!file.is_open())
		return;

	const std::string data((std::istreambuf_iterator<char>(file)),
							std::istreambuf_iterator<char>());
#else
	const FilesManager::OtherFile file = Engine::Instance().filesManager()->getData(GAME_FILES::BALANCE);
	const std::string data = std::string(static_cast<char*>(file.data));
#endif
	Json::Reader reader;
	Json::Value obj;
	if (!reader.parse(data, obj))
	{
		std::cout << "Can't read balance" << std::endl;
		return;
	}

	//load towers
	const Json::Value& towersObject = obj[BalanceDef::TOWERS_KEY];
	loadTowers(towersObject);

	//load enemies
	const Json::Value& enemiesObject = obj[BalanceDef::ENEMIES_KEY];
	loadEnemies(enemiesObject);

	//load limits
	const Json::Value& limitsObject = obj[BalanceDef::LIMITS_KEY];
	loadLimits(limitsObject);

	//load others
	const Json::Value& othersObject = obj[BalanceDef::OTHERS_KEY];
	loadOthers(othersObject);

	//load abilities
	const Json::Value& abilitiesObject = obj[BalanceDef::ABILITIES_KEY];
	loadAbilities(abilitiesObject);

	//loadMaps
	const Json::Value& mapsObject = obj[BalanceDef::MAPS_KEY];
	loadMaps(mapsObject);

	//load enemies abilities
	const Json::Value& enemiesAbilitiesObject = obj[BalanceDef::ENEMIES_ABILITIES_KEY];
	loadEnemiesAbilities(enemiesAbilitiesObject);

	//waves
	const Json::Value& wavesObject = obj[BalanceDef::WAVES_KEY];
	loadWaves(wavesObject);
}

void Balance::loadTowers(const Json::Value &jsonTowers)
{
	towersStats.clear();
	loadTower(TOWER_TYPES::BASE, jsonTowers[BalanceDef::BASE_KEY]);
	loadTower(TOWER_TYPES::FREEZE, jsonTowers[BalanceDef::FREEZE_KEY]);
	loadTower(TOWER_TYPES::POWER, jsonTowers[BalanceDef::POWER_KEY]);
	loadTower(TOWER_TYPES::ROCKET, jsonTowers[BalanceDef::ROCKET_KEY]);
	loadTower(TOWER_TYPES::LASER, jsonTowers[BalanceDef::LASER_KEY]);
	loadTower(TOWER_TYPES::IMPROVED, jsonTowers[BalanceDef::IMPROVED_KEY]);
}

void Balance::loadTower(const TOWER_TYPES type, const Json::Value &jsonTower)
{
	TowerStats stats;
	stats.damage = jsonTower[BalanceDef::DAMAGE_KEY].asFloat();
	stats.attackSpeed = jsonTower[BalanceDef::ATTACK_SPEED_KEY].asFloat();
	stats.radius = jsonTower[BalanceDef::RADIUS_KEY].asFloat();
	stats.projectileSpeed = jsonTower[BalanceDef::PROJECTILE_SPEED_KEY].asFloat();
	stats.cost = jsonTower[BalanceDef::COST_KEY].asFloat();
	stats.damageOffset = jsonTower[BalanceDef::DAMAGE_OFFSET_KEY].asFloat();


	const Json::Value &jsonArmorDamage = jsonTower[BalanceDef::ARMOR_DAMAGE_KEY];
	const float infantryLightArmor = jsonArmorDamage[BalanceDef::INFANTRY_LIGHT_ARMOR_KEY].asFloat();
	const float infantryHeavyArmor = jsonArmorDamage[BalanceDef::INFANTRY_HEAVY_ARMOR_KEY].asFloat();

	const float vehicleLightArmor = jsonArmorDamage[BalanceDef::VEHICLE_LIGHT_ARMOR_KEY].asFloat();
	const float vehicleHeavyArmor = jsonArmorDamage[BalanceDef::VEHICLE_HEAVY_ARMOR_KEY].asFloat();

	const float monsterLightArmor = jsonArmorDamage[BalanceDef::MONSTER_LIGHT_ARMOR_KEY].asFloat();
	const float monsterHeavyArmor = jsonArmorDamage[BalanceDef::MONSTER_HEAVY_ARMOR_KEY].asFloat();

	stats.armorDamage.insert(std::pair<ARMOR_TYPE, float>(DEFAULT_ARMOR, 0.f));
	stats.armorDamage.insert(std::pair<ARMOR_TYPE, float>(INFANTRY_LIGHT, infantryLightArmor));
	stats.armorDamage.insert(std::pair<ARMOR_TYPE, float>(INFANTRY_HEAVY, infantryHeavyArmor));
	stats.armorDamage.insert(std::pair<ARMOR_TYPE, float>(VEHICLE_LIGHT, vehicleLightArmor));
	stats.armorDamage.insert(std::pair<ARMOR_TYPE, float>(VEHICLE_HEAVY, vehicleHeavyArmor));
	stats.armorDamage.insert(std::pair<ARMOR_TYPE, float>(MONSTER_LIGHT, monsterLightArmor));
	stats.armorDamage.insert(std::pair<ARMOR_TYPE, float>(MONSTER_HEAVY, monsterHeavyArmor));
	towersStats.insert(std::pair<TOWER_TYPES, TowerStats>(type, stats));
}

void Balance::loadEnemies(const Json::Value &jsonEnemies)
{
	enemiesStats.clear();
	loadEnemy(ENEMY_TYPES::INFANTRY, jsonEnemies[BalanceDef::INFANTRY_KEY]);
	loadEnemy(ENEMY_TYPES::WAR_VEHICLE, jsonEnemies[BalanceDef::CAR_KEY]);
	loadEnemy(ENEMY_TYPES::TRICYCLE, jsonEnemies[BalanceDef::TRICYCLE_KEY]);
	loadEnemy(ENEMY_TYPES::WORM, jsonEnemies[BalanceDef::SMALL_KEY]);
	loadEnemy(ENEMY_TYPES::SPAWN_WORM, jsonEnemies[BalanceDef::SMALL_KEY]);
	loadEnemy(ENEMY_TYPES::SELFHEAL_ENEMY, jsonEnemies[BalanceDef::SELFHEAL_KEY]);
	loadEnemy(ENEMY_TYPES::TRACTOR, jsonEnemies[BalanceDef::TRACTOR_KEY]);
	loadEnemy(ENEMY_TYPES::SLUGGY, jsonEnemies[BalanceDef::ANOTHER_KEY]);
	loadEnemy(ENEMY_TYPES::TANK, jsonEnemies[BalanceDef::TANK_KEY]);
	loadEnemy(ENEMY_TYPES::MECHSPIDER, jsonEnemies[BalanceDef::SPIDER_KEY]);
	loadEnemy(ENEMY_TYPES::TUATARA, jsonEnemies[BalanceDef::MID_KEY]);
	loadEnemy(ENEMY_TYPES::REPAIR_ENEMY, jsonEnemies[BalanceDef::REPAIR_KEY]);
	loadEnemy(ENEMY_TYPES::SHIELD_ENEMY, jsonEnemies[BalanceDef::SHIELD_KEY]);
	loadEnemy(ENEMY_TYPES::WALKER, jsonEnemies[BalanceDef::TELEPORT_KEY]);
	loadEnemy(ENEMY_TYPES::GIANT_SLUG, jsonEnemies[BalanceDef::BIG_KEY]);
	loadEnemy(ENEMY_TYPES::HEAVY_TANK, jsonEnemies[BalanceDef::BIG_TANK_KEY]);
	loadEnemy(ENEMY_TYPES::BUGSAURUS, jsonEnemies[BalanceDef::SPAWN_KEY]);
	loadEnemy(ENEMY_TYPES::JUMPER, jsonEnemies[BalanceDef::JUMPER_KEY]);
	loadEnemy(ENEMY_TYPES::ROLLER, jsonEnemies[BalanceDef::CAR_KEY]);
}

void Balance::loadEnemy(const ENEMY_TYPES type, const Json::Value &jsonEnemy)
{
	EnemyStats stats;
	stats.speed = jsonEnemy[BalanceDef::SPEED_KEY].asFloat();
	stats.health = jsonEnemy[BalanceDef::HEALTH_KEY].asFloat();
	stats.damage = jsonEnemy[BalanceDef::LOSS_KEY].asFloat();
	stats.reflection = jsonEnemy[BalanceDef::REFLECTION_KEY].asFloat();
	stats.armorType = static_cast<ARMOR_TYPE>(jsonEnemy[BalanceDef::ARMOR_TYPE_KEY].asInt());
	enemiesStats.insert(std::pair<ENEMY_TYPES, EnemyStats>(type, stats));
}

void Balance::loadLimits(const Json::Value &jsonLimits)
{
	const int BASE_MAX = jsonLimits[BalanceDef::BASE_LIMIT_KEY].asInt();
	towerLimits.insert(std::pair<TOWER_TYPES, int>(BASE, BASE_MAX));

	const int FREEZE_MAX = jsonLimits[BalanceDef::FREEZE_LIMIT_KEY].asInt();
	towerLimits.insert(std::pair<TOWER_TYPES, int>(FREEZE, FREEZE_MAX));

	const int POWER_MAX = jsonLimits[BalanceDef::POWER_LIMIT_KEY].asInt();
	towerLimits.insert(std::pair<TOWER_TYPES, int>(POWER, POWER_MAX));

	const int ROCKET_MAX = jsonLimits[BalanceDef::ROCKET_LIMIT_KEY].asInt();
	towerLimits.insert(std::pair<TOWER_TYPES, int>(ROCKET, ROCKET_MAX));

	const int LASER_MAX = jsonLimits[BalanceDef::LASER_LIMIT_KEY].asInt();
	towerLimits.insert(std::pair<TOWER_TYPES, int>(LASER, LASER_MAX));

	const int IMPROVED_MAX = jsonLimits[BalanceDef::IMPROVED_LIMIT_KEY].asInt();
	towerLimits.insert(std::pair<TOWER_TYPES, int>(IMPROVED, IMPROVED_MAX));
}

void Balance::loadOthers(const Json::Value &jsonOthers)
{
	towerUpgradeGain = jsonOthers[BalanceDef::UPGRADE_GAIN_KEY].asFloat();
	powerTowerCostOffset = jsonOthers[BalanceDef::POWER_TOWER_COST_OFFSET_KEY].asFloat();
	blastCells = jsonOthers[BalanceDef::BLAST_CELLS_KEY].asFloat();
	blastCount = jsonOthers[BalanceDef::BLAST_COUNT_KEY].asInt();
	baseTowerPenetration = jsonOthers[BalanceDef::BASE_TOWER_PENETRATION_KEY].asFloat();
	freezeTowerValue = jsonOthers[BalanceDef::FREEZE_TOWER_VALUE_KEY].asFloat();
	freezeTowerCells = jsonOthers[BalanceDef::FREEZE_TOWER_CELLS_KEY].asFloat();
	freezeTowerDuration = jsonOthers[BalanceDef::FREEZE_TOWER_DURATION_KEY].asFloat() * EngineDefs::MSEC;
	laserTowerMaxExtraTargets = jsonOthers[BalanceDef::LASER_TOWER_MAX_EXTRA_TARGETS_KEY].asInt();
	rocketTowerCells = jsonOthers[BalanceDef::ROCKET_TOWER_CELLS_KEY].asFloat();
	burnAttackSpeed = jsonOthers[BalanceDef::BURN_ATTACK_SPEED_KEY].asFloat() * EngineDefs::MSEC;
	burnDamage = jsonOthers[BalanceDef::BURN_DAMAGE_KEY].asFloat();
	burnDuration = jsonOthers[BalanceDef::BURN_DURATION_KEY].asFloat() * EngineDefs::MSEC;
	drainValue = jsonOthers[BalanceDef::DRAIN_VALUE_KEY].asFloat();
	blindValue = jsonOthers[BalanceDef::BLIND_VALUE_KEY].asFloat();
	regressValue = jsonOthers[BalanceDef::REGRESS_VALUE_KEY].asFloat();
}

void Balance::loadAbilities(const Json::Value &jsonAbilities)
{
	bombDamage = jsonAbilities[BalanceDef::BOMB_DAMAGE_KEY].asFloat();
	bombCooldown = jsonAbilities[BalanceDef::BOMB_COOLDOWN_KEY].asFloat();
	freezeValue = jsonAbilities[BalanceDef::FREEZE_VALUE_KEY].asFloat();
	freezeDuration = jsonAbilities[BalanceDef::FREEZE_DURATION_KEY].asFloat();
	freezeCooldown = jsonAbilities[BalanceDef::FREEZE_COOLDOWN_KEY].asFloat();
	acidDamage = jsonAbilities[BalanceDef::ACID_DAMAGE_KEY].asFloat();
	acidAttackSpeed = jsonAbilities[BalanceDef::ACID_ATTACK_SPEED_KEY].asFloat();
	acidCount = jsonAbilities[BalanceDef::ACID_COUNT_KEY].asInt();
	acidCooldown = jsonAbilities[BalanceDef::ACID_COOLDOWN_KEY].asFloat();
	increaseAttackSpeedDuration = jsonAbilities[BalanceDef::INCREASE_ATTACK_SPEED_DURATION_KEY].asFloat();
	increaseAttackSpeedValue = jsonAbilities[BalanceDef::INCREASE_ATTACK_SPEED_VALUE_KEY].asFloat();
	increaseAttackSpeedCooldown = jsonAbilities[BalanceDef::INCREASE_ATTACK_SPEED_COOLDOWN_KEY].asFloat();
	increaseDamageDuration = jsonAbilities[BalanceDef::INCREASE_DAMAGE_DURATION_KEY].asFloat();
	increaseDamageValue = jsonAbilities[BalanceDef::INCREASE_DAMAGE_VALUE_KEY].asFloat();
	increaseDamageCooldown = jsonAbilities[BalanceDef::INCREASE_DAMAGE_COOLDOWN_KEY].asFloat();
	stopDuration = jsonAbilities[BalanceDef::STOP_DURATION_KEY].asFloat();
	stopCooldown = jsonAbilities[BalanceDef::STOP_COOLDOWN_KEY].asFloat();
}

void Balance::loadMaps(const Json::Value &jsonMaps)
{
	mapsStats.clear();
	for (int number = 1; number <= 15; ++number)
	{
		MapStats stats;

		const Json::Value& mapObject = jsonMaps[BalanceDef::MAP_KEY + std::to_string(number)];

		stats.life = mapObject[BalanceDef::MAP_LIFE_KEY].asDouble();
		stats.money = mapObject[BalanceDef::MAP_MONEY_KEY].asDouble();

		//drain
		{
			const Json::Value& drainObject = mapObject[BalanceDef::MAP_DRAIN_KEY];
			stats.moneyDrain.duration = drainObject[BalanceDef::MAP_EFFECT_DURATION_KEY].asDouble();
			stats.moneyDrain.time = drainObject[BalanceDef::MAP_EFFECT_TIME_KEY].asDouble();
			stats.moneyDrain.count = drainObject[BalanceDef::MAP_EFFECT_COUNT_KEY].asInt();
			stats.moneyDrain.enabled = drainObject[BalanceDef::MAP_EFFECT_ENABLED_KEY].asBool();
		}
		//explosions
		{
			const Json::Value& explosionsObject = mapObject[BalanceDef::MAP_EXPLOSIONS_KEY];
			stats.explosions.duration = explosionsObject[BalanceDef::MAP_EFFECT_DURATION_KEY].asDouble();
			stats.explosions.time = explosionsObject[BalanceDef::MAP_EFFECT_TIME_KEY].asDouble();
			stats.explosions.count = explosionsObject[BalanceDef::MAP_EFFECT_COUNT_KEY].asInt();
			stats.explosions.enabled = explosionsObject[BalanceDef::MAP_EFFECT_ENABLED_KEY].asBool();
		}
		//regress
		{
			const Json::Value& regressObject = mapObject[BalanceDef::MAP_REGRESS_KEY];
			stats.regress.duration = regressObject[BalanceDef::MAP_EFFECT_DURATION_KEY].asDouble();
			stats.regress.time = regressObject[BalanceDef::MAP_EFFECT_TIME_KEY].asDouble();
			stats.regress.count = regressObject[BalanceDef::MAP_EFFECT_COUNT_KEY].asInt();
			stats.regress.enabled = regressObject[BalanceDef::MAP_EFFECT_ENABLED_KEY].asBool();
		}
		//smoke
		{
			const Json::Value& smokeObject = mapObject[BalanceDef::MAP_SMOKE_KEY];
			stats.smoke.duration = smokeObject[BalanceDef::MAP_EFFECT_DURATION_KEY].asDouble();
			stats.smoke.time = smokeObject[BalanceDef::MAP_EFFECT_TIME_KEY].asDouble();
			stats.smoke.count = smokeObject[BalanceDef::MAP_EFFECT_COUNT_KEY].asInt();
			stats.smoke.enabled = smokeObject[BalanceDef::MAP_EFFECT_ENABLED_KEY].asBool();
		}
		//lava
		{
			const Json::Value& lavaObject = mapObject[BalanceDef::MAP_LAVA_KEY];
			stats.lava.duration = lavaObject[BalanceDef::MAP_EFFECT_DURATION_KEY].asDouble();
			stats.lava.time = lavaObject[BalanceDef::MAP_EFFECT_TIME_KEY].asDouble();
			stats.lava.count = lavaObject[BalanceDef::MAP_EFFECT_COUNT_KEY].asInt();
			stats.lava.enabled = lavaObject[BalanceDef::MAP_EFFECT_ENABLED_KEY].asBool();
		}
		stats.smoke.time *= EngineDefs::MSEC;
		stats.smoke.duration *= EngineDefs::MSEC;
		stats.regress.time *= EngineDefs::MSEC;
		stats.regress.duration *= EngineDefs::MSEC;
		stats.moneyDrain.time *= EngineDefs::MSEC;
		stats.moneyDrain.duration *= EngineDefs::MSEC;
		stats.explosions.time *= EngineDefs::MSEC;
		stats.explosions.duration *= EngineDefs::MSEC;
		stats.lava.time *= EngineDefs::MSEC;
		stats.lava.duration *= EngineDefs::MSEC;

		mapsStats.insert(std::pair<int, MapStats>(number, stats));
	}
}

void Balance::loadEnemiesAbilities(const Json::Value &jsonEnemiesAbilities)
{
	selfhealInterval = jsonEnemiesAbilities[BalanceDef::SELFHEAL_INTERVAL_KEY].asFloat() * EngineDefs::MSEC;
	selfhealValue = jsonEnemiesAbilities[BalanceDef::SELFHEAL_VALUE_KEY].asFloat();
	healInterval = jsonEnemiesAbilities[BalanceDef::HEAL_INTERVAL_KEY].asFloat() * EngineDefs::MSEC;
	healValue = jsonEnemiesAbilities[BalanceDef::HEAL_VALUE_KEY].asFloat();
	shieldInterval = jsonEnemiesAbilities[BalanceDef::SHIELD_INTERVAL_KEY].asFloat() * EngineDefs::MSEC;
	shieldValue = jsonEnemiesAbilities[BalanceDef::SHIELD_VALUE_KEY].asFloat();
	strongInterval = jsonEnemiesAbilities[BalanceDef::STRONG_INTERVAL_KEY].asFloat() * EngineDefs::MSEC;
	strongValue = jsonEnemiesAbilities[BalanceDef::STRONG_VALUE_KEY].asFloat();
	teleportInterval = jsonEnemiesAbilities[BalanceDef::TELEPORT_INTERVAL_KEY].asFloat() * EngineDefs::MSEC;
	spawnCount = jsonEnemiesAbilities[BalanceDef::SPAWN_COUNT_KEY].asInt();
	spawnCountOffset = jsonEnemiesAbilities[BalanceDef::SPAWN_COUNT_OFFSET_KEY].asInt();
	spawnInterval = jsonEnemiesAbilities[BalanceDef::SPAWN_INTERVAL_KEY].asFloat() * EngineDefs::MSEC;
	shutdownInterval = jsonEnemiesAbilities[BalanceDef::SHUTDOWN_INTERVAL_KEY].asFloat() * EngineDefs::MSEC;
	shutdownDuration = jsonEnemiesAbilities[BalanceDef::SHUTDOWN_DURATION_KEY].asFloat() * EngineDefs::MSEC;
	shutdownCells = jsonEnemiesAbilities[BalanceDef::SHUTDOWN_CELLS_KEY].asFloat();
	downgradeValue = jsonEnemiesAbilities[BalanceDef::DOWNGRADE_VALUE_KEY].asFloat();
	downgradeCells = jsonEnemiesAbilities[BalanceDef::DOWNGRADE_CELLS_KEY].asFloat();
	downgradeDuration = jsonEnemiesAbilities[BalanceDef::DOWNGRADE_DURATION_KEY].asFloat() * EngineDefs::MSEC;
	downgradeInterval = jsonEnemiesAbilities[BalanceDef::DOWNGRADE_INTERVAL_KEY].asFloat() * EngineDefs::MSEC;	
	killTowerInterval = jsonEnemiesAbilities[BalanceDef::KILL_TOWER_INTERVAL_KEY].asFloat() * EngineDefs::MSEC;
	downgradeTowerInterval = jsonEnemiesAbilities[BalanceDef::DOWNGRADE_TOWER_INTERVAL_KEY].asFloat() * EngineDefs::MSEC;
	killTowerCells = jsonEnemiesAbilities[BalanceDef::KILL_TOWER_CELLS_KEY].asFloat();
	downgradeTowerCells = jsonEnemiesAbilities[BalanceDef::DOWNGRADE_TOWER_CELLS_KEY].asFloat();
}

void Balance::loadWaves(const Json::Value &jsonWaves)
{
	waves.clear();
	for (unsigned int number = 0; number < jsonWaves.size(); ++number)
	{
		const Json::Value &jsonWave = jsonWaves[number];
		std::vector<Wave> wave_s;
		for (unsigned int i = 0; i < jsonWave.size(); ++i)
		{
			const Json::Value &jsonWaveWave = jsonWave[i];

			Wave w;
			w.protection = jsonWaveWave[BalanceDef::PROTECTION_KEY].asFloat();
			w.respawnTime = jsonWaveWave[BalanceDef::RESPAWN_TIME_KEY].asFloat();

			const Json::Value &jsonEnemies = jsonWaveWave[BalanceDef::SPAWN_ENEMIES_KEY];
			for (unsigned int j = 0; j < jsonEnemies.size(); ++j)
			{
				const Json::Value &jsonEnemy = jsonEnemies[j];
				const ENEMY_TYPES type = static_cast<ENEMY_TYPES>(jsonEnemy[BalanceDef::ENEMY_TYPE_KEY].asInt());
				const int count = jsonEnemy[BalanceDef::ENEMIES_COUNT_KEY].asInt();
				for (int k = 0; k < count; ++k)
					w.spawnEnemies.push_back(type);
			}
			wave_s.push_back(w);
		}
		waves.push_back(wave_s);
	}
}

TowerStats Balance::getTowerStats(const TOWER_TYPES type) const
{
	return towersStats.at(type);
}

EnemyStats Balance::getEnemyStats(const ENEMY_TYPES type) const
{
	return enemiesStats.at(type);
}

int Balance::getTowerLimit(const TOWER_TYPES type) const
{
	return towerLimits.at(type);
}

float Balance::getTowerUpgradeGain() const
{
	return towerUpgradeGain;
}

float Balance::getPowerTowerCostOffset() const
{
	return powerTowerCostOffset;
}

float Balance::getBlastCells() const
{
	return blastCells;
}

int Balance::getBlastCount() const
{
	return blastCount;
}

float Balance::getbaseTowerPenetration() const
{
	return baseTowerPenetration;
}

float Balance::getFreezeTowerValue() const
{
	return freezeTowerValue;
}

float Balance::getFreezeTowerCells() const
{
	return freezeTowerCells;
}

float Balance::getFreezeTowerDuration() const
{
	return freezeTowerDuration;
}

int Balance::getLaserTowerMaxExtraTargets() const
{
	return laserTowerMaxExtraTargets;
}

float Balance::getRocketTowerCells() const
{
	return rocketTowerCells;
}

float Balance::getBurnAttackSpeed() const
{
	return burnAttackSpeed;
}

float Balance::getBurnDamage() const
{
	return burnDamage;
}

float Balance::getBurnDuration() const
{
	return burnDuration;
}

float Balance::getDrainValue() const
{
	return drainValue;
}

float Balance::getBlindValue() const
{
	return blindValue;
}

float Balance::getRegressValue() const
{
	return regressValue;
}

float Balance::getBombDamage() const
{
	return bombDamage;
}

float Balance::getBombCooldown() const
{
	return bombCooldown;
}

float Balance::getFreezeValue() const
{
	return freezeValue;
}

float Balance::getFreezeDuration() const
{
	return freezeDuration;
}

float Balance::getFreezeCooldown() const
{
	return freezeCooldown;
}

float Balance::getAcidDamage() const
{
	return acidDamage;
}

float Balance::getAcidAttackSpeed() const
{
	return acidAttackSpeed;
}

int Balance::getAcidCount() const
{
	return acidCount;
}

float Balance::getAcidCooldown() const
{
	return acidCooldown;
}

float Balance::getIncreaseAttackSpeedDuration() const
{
	return increaseAttackSpeedDuration;
}

float Balance::getIncreaseAttackSpeedValue() const
{
	return increaseAttackSpeedValue;
}

float Balance::getIncreaseAttackSpeedCooldown() const
{
	return increaseAttackSpeedCooldown;
}

float Balance::getIncreaseDamageDuration() const
{
	return increaseDamageDuration;
}

float Balance::getIncreaseDamageValue() const
{
	return increaseDamageValue;
}

float Balance::getIncreaseDamageCooldown() const
{
	return increaseDamageCooldown;
}

float Balance::getStopDuration() const
{
	return stopDuration;
}

float Balance::getStopCooldown() const
{
	return stopCooldown;
}

float Balance::getSelfhealInterval() const
{
	return selfhealInterval;
}

float Balance::getSelfhealValue() const
{
	return selfhealValue;
}

float Balance::getHealInterval() const
{
	return healInterval;
}

float Balance::getHealValue() const
{
	return healValue;
}

float Balance::getShieldInterval() const
{
	return shieldInterval;
}

float Balance::getShieldValue() const
{
	return shieldValue;
}

float Balance::getStrongInterval() const
{
	return strongInterval;
}

float Balance::getStrongValue() const
{
	return strongValue;
}

float Balance::getTeleportInterval() const
{
	return teleportInterval;
}

int Balance::getSpawnCount() const
{
	return spawnCount;
}

int Balance::getSpawnCountOffset() const
{
	return spawnCountOffset;
}

float Balance::getSpawnInterval() const
{
	return spawnInterval;
}

float Balance::getShutdownInterval() const
{
	return shutdownInterval;
}

float Balance::getShutdownDuration() const
{
	return shutdownDuration;
}

float Balance::getShutdownCells() const
{
	return shutdownCells;
}

float Balance::getDowngradeValue() const
{
	return downgradeValue;
}

float Balance::getDowngradeCells() const
{
	return downgradeCells;
}

float Balance::getDowngradeDuration() const
{
	return downgradeDuration;
}

float Balance::getDowngradeInterval() const
{
	return downgradeInterval;
}

float Balance::getKillTowerInterval() const
{
	return killTowerInterval;
}

float Balance::getDowngradeTowerInterval() const
{
	return downgradeTowerInterval;
}

float Balance::getKillTowerCells() const
{
	return killTowerCells;
}

float Balance::getDowngradeTowerCells() const
{
	return downgradeTowerCells;
}

MapStats Balance::getMapStats(int number) const
{
	return mapsStats.at(number);
}

std::vector<Wave> Balance::getWave(int number) const
{
	return waves.at(number);
}

