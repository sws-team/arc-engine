#ifndef CPU_H
#define CPU_H

#include "stdheader.h"
#include "SpaceShip/levelenemy.h"
#include "timer.h"

class SpaceShip;
class Level;

class CPU
{
public:
	static CPU &Instance();

	void action(vector<LevelEnemy> &spaceShips, float dy);
	void update();

	void setBoss(SpaceShip* boss);

	void setLevel(Level* level);

private:
	CPU();

	CPU(CPU const&) = delete;
	CPU& operator= (CPU const&) = delete;

	SpaceShip *boss;
	Level *level;

	bool actionSpaceShip(LevelEnemy &levelEnemy, ENEMY_ACTIONS actionType);

	bool isInstant(ENEMY_ACTIONS actionType) const;
};

#endif // CPU_H
