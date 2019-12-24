#ifndef LEVELDEF_H
#define LEVELDEF_H

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

	SMALL_SLOW,//scorpion
	SMALL_MEDIUM,//car
	SMALL_FAST,//tricycle

	MID_SLOW,//tank
	MID_MEDIUM,//spider
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
