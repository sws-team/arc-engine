#ifndef BALANCE_H
#define BALANCE_H

#include "stdheader.h"
#include "Game/leveldef.h"

enum ARMOR_TYPE
{
	DEFAULT_ARMOR,

	INFANTRY_LIGHT,
	INFANTRY_HEAVY,

	VEHICLE_LIGHT,
	VEHICLE_HEAVY,

	MONSTER_LIGHT,
	MONSTER_HEAVY
};

typedef std::map<ARMOR_TYPE, float> TowerArmorDamage;

struct TowerStats
{
	TowerStats();
	TowerStats(float damage,
			   float attackSpeed,
			   float radius,
			   float projectileSpeed,
			   float cost);

	float damage;
	float attackSpeed;
	float radius;
	float projectileSpeed;
	float cost;
	TowerArmorDamage armorDamage;
};

struct EnemyStats
{
	float speed;
	float health;
	float damage;
	float reflection;
	ARMOR_TYPE armorType;
};

struct MapStats
{
	float money;
	float life;

	struct MapDanger
	{
		MapDanger()
			: enabled(false)
			,time(10000)
			,duration(2000)
			,count(1)
		{

		}
		bool enabled;
		float time;
		float duration;
		int count;
	};

	MapDanger explosions;
	MapDanger smoke;
	MapDanger moneyDrain;
	MapDanger regress;
};

namespace Json {
class Value;
}

class Balance
{
public:
	static Balance &Instance();

	void load();

	//getters
	TowerStats getTowerStats(const TOWER_TYPES type) const;
	EnemyStats getEnemyStats(const ENEMY_TYPES type) const;

	//limits
	int getTowerLimit(const TOWER_TYPES type) const;

	//other
	float getTowerUpgradeGain() const;
	float getEnergyGain() const;
	float getPowerTowerCostOffset() const;
	float getBlastCells() const;
	int getBlastCount() const;
	float getbaseTowerPenetration() const;
	float getFreezeTowerValue() const;
	float getFreezeTowerCells() const;
	float getFreezeTowerDuration() const;
	int getLaserTowerMaxExtraTargets() const;
	float getRocketTowerCells() const;
	float getBurnAttackSpeed() const;
	float getBurnDamage() const;
	float getBurnDuration() const;
	float getDrainValue() const;
	float getBlindValue() const;
	float getRegressValue() const;

	//abilities
	float getBombDamage() const;
	float getBombCooldown() const;
	float getFreezeValue() const;
	float getFreezeDuration() const;
	float getFreezeCooldown() const;
	float getAcidDamage() const;
	float getAcidAttackSpeed() const;
	int getAcidCount() const;
	float getAcidCooldown() const;
	float getIncreaseAttackSpeedDuration() const;
	float getIncreaseAttackSpeedValue() const;
	float getIncreaseAttackSpeedCooldown() const;
	float getIncreaseDamageDuration() const;
	float getIncreaseDamageValue() const;
	float getIncreaseDamageCooldown() const;
	float getStopDuration() const;
	float getStopCooldown() const;

	//enemies abilities
	float getSelfhealInterval() const;
	float getSelfhealValue() const;
	float getHealInterval() const;
	float getHealValue() const;
	float getShieldInterval() const;
	float getShieldValue() const;
	float getStrongInterval() const;
	float getStrongValue() const;
	float getTeleportInterval() const;
	int getSpawnCount() const;
	int getSpawnCountOffset() const;
	float getSpawnInterval() const;
	float getShutdownInterval() const;
	float getShutdownDuration() const;
	float getShutdownCells() const;
	float getDowngradeValue() const;
	float getDowngradeCells() const;
	float getDowngradeDuration() const;
	float getDowngradeInterval() const;
	float getKillTowerInterval() const;
	float getKillAreaTowersInterval() const;
	float getKillTowerCells() const;
	float getKillAreaTowersCells() const;

	//maps
	MapStats getMapStats(int number) const;
	//waves
	std::vector<Wave> getWave(int number) const;

private:
	Balance();
	Balance(const Balance& root) = delete;
	Balance& operator=(const Balance&) = delete;

	std::map<TOWER_TYPES, TowerStats> towersStats;
	std::map<ENEMY_TYPES, EnemyStats> enemiesStats;
	std::map<TOWER_TYPES, int> towerLimits;

	//others
	float towerUpgradeGain;
	float energyGain;
	float powerTowerCostOffset;
	float blastCells;
	int blastCount;
	float baseTowerPenetration;
	float freezeTowerValue;
	float freezeTowerCells;
	float freezeTowerDuration;
	int laserTowerMaxExtraTargets;
	float rocketTowerCells;
	float burnAttackSpeed;
	float burnDamage;
	float burnDuration;
	float drainValue;
	float blindValue;
	float regressValue;

	//abilities
	float bombDamage;
	float bombCooldown;
	float freezeValue;
	float freezeDuration;
	float freezeCooldown;
	float acidDamage;
	float acidAttackSpeed;
	int acidCount;
	float acidCooldown;
	float increaseAttackSpeedDuration;
	float increaseAttackSpeedValue;
	float increaseAttackSpeedCooldown;
	float increaseDamageDuration;
	float increaseDamageValue;
	float increaseDamageCooldown;
	float stopDuration;
	float stopCooldown;
	//maps
	std::map<int, MapStats> mapsStats;
	//enemies abilities
	float selfhealInterval;
	float selfhealValue;
	float healInterval;
	float healValue;
	float shieldInterval;
	float shieldValue;
	float strongInterval;
	float strongValue;
	float teleportInterval;
	int spawnCount;
	int spawnCountOffset;
	float spawnInterval;
	float shutdownInterval;
	float shutdownDuration;
	float shutdownCells;
	float downgradeValue;
	float downgradeCells;
	float downgradeDuration;
	float downgradeInterval;
	float killTowerInterval;
	float killAreaTowersInterval;
	float killTowerCells;
	float killAreaTowersCells;

	//waves

	std::vector<std::vector<Wave> > waves;

	void loadTowers(const Json::Value& jsonTowers);
	void loadTower(const TOWER_TYPES type, const Json::Value& jsonTower);

	void loadEnemies(const Json::Value& jsonEnemies);
	void loadEnemy(const ENEMY_TYPES type, const Json::Value& jsonEnemy);

	void loadLimits(const Json::Value& jsonLimits);
	void loadOthers(const Json::Value& jsonOthers);
	void loadAbilities(const Json::Value& jsonAbilities);

	void loadMaps(const Json::Value& jsonMaps);
	void loadEnemiesAbilities(const Json::Value& jsonEnemiesAbilities);
	void loadWaves(const Json::Value& jsonWaves);
};

#endif // BALANCE_H