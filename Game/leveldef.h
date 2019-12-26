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
	ABILITY_UNKNOWN,

	SELL,
	UPGRADE
};

enum ENEMY_TYPES
{
	UNKNOWN,

	SCORPION,//scorpion
	SMALL_MEDIUM,//car
	SMALL_FAST,//tricycle

	MID_SLOW,//tank
	SPIDER,//spider
	MID_FAST,//helicopter

	BIG_SLOW,//aircarrier
	BIG_MEDIUM,//big tank
	BIG_FAST,//plane

	//with abilities
	REPAIR_ENEMY,
	SHELL_ENEMY,
	TELEPORT_ENEMY,
	SELFHEAL_ENEMY,
	DOWN_TOWER_ENEMY,
	SMALL_NEXT,
};

struct Wave
{
	vector<ENEMY_TYPES> spawnEnemies;
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
