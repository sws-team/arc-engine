#ifndef BALANCE_H
#define BALANCE_H

#include "stdheader.h"
#include "Game/leveldef.h"

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
};

struct EnemyStats
{
	float speed;
	float health;
	float damage;
	float reflection;
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

	//maps
	MapStats getMapStats(int number) const;

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


	void loadTowers(const Json::Value& jsonTowers);
	void loadTower(const TOWER_TYPES type, const Json::Value& jsonTower);

	void loadEnemies(const Json::Value& jsonEnemies);
	void loadEnemy(const ENEMY_TYPES type, const Json::Value& jsonEnemy);

	void loadLimits(const Json::Value& jsonLimits);
	void loadOthers(const Json::Value& jsonOthers);
	void loadAbilities(const Json::Value& jsonAbilities);

	void loadMaps(const Json::Value& jsonMaps);
};

#endif // BALANCE_H
