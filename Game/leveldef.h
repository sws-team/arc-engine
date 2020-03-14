#ifndef LEVELDEF_H
#define LEVELDEF_H

#include "stdheader.h"

enum ACTION_STATE
{
	READY,

	ADD_TOWER,

	ABILITY_VENOM,
	ABILITY_BOMB,
	ABILITY_FREEZE_BOMB,
	ABILITY_INCREASE_TOWER_ATTACK_SPEED,
	ABILITY_INCREASE_TOWER_DAMAGE,
	ABILITY_STOP,

	SELL,
	UPGRADE
};

enum ENEMY_TYPES
{
	UNKNOWN,

	INFANTRY,
	CAR,
	TRICYCLE,
	SMALL_NEXT,
	SELFHEAL_ENEMY,
	TRACTOR,
	ANOTHER_ENEMY,

	TANK,//tank
	SPIDER,//spider
	MID_FAST,//helicopter
	REPAIR_ENEMY,
	SHIELD_ENEMY,
	TELEPORT_ENEMY,

	BIG_SLOW,//aircarrier
	BIG_TANK,//big tank
	SPAWN_ENEMY,
};

struct Wave
{
	std::vector<ENEMY_TYPES> spawnEnemies;
	float protection;
	float respawnTime;
};

enum TOWER_TYPES
{
	BASE,
	POWER,
	ROCKET,
	FREEZE,
	LASER,
	IMPROVED,
};

#endif // LEVELDEF_H
