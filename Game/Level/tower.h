#ifndef TOWER_H
#define TOWER_H

#include "Game/gameobject.h"

struct TowerStats
{
	float damage;
	float attackSpeed;
	float damageOffset;
	float radius;
};

//class Tower : GameObject
//{
//public:
//	Tower();
//	void update() override;

//private:
//	TowerStats m_stats;
//};

#endif // TOWER_H
