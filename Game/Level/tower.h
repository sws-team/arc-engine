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

class Tower : public GameObject
{
public:
	Tower(const RESOURCES::TEXTURE_TYPE &texture_id, const Vector2f &pos);
//	void update() override;

private:
	TowerStats m_stats;
};

#endif // TOWER_H
