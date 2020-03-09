#include "enemy.h"

#define ADD_ENEMY(x, y) enemies.insert(std::make_pair<ENEMY_TYPES, int>(x, y));

Wave EnemiesFactory::createWave(float protection, float time, const std::map<ENEMY_TYPES, int> &enemies)
{
	Wave wave;
	wave.protection = protection;
	wave.respawnTime = time;
	for(auto enemy : enemies)
	{
		for (int i = 0; i < enemy.second; ++i)
			wave.spawnEnemies.push_back(enemy.first);
	}
	return wave;
}

std::vector<Wave> EnemiesFactory::generateEnemies(unsigned int n)
{
	std::vector<Wave> waves;
	std::map<ENEMY_TYPES, int> enemies;
	switch (n)
	{
	case 0:
	{
		//scorpion, car, tricicle
		ADD_ENEMY(INFANTRY, 10);
		waves.push_back(createWave(0.f, 2750, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 20);
		ADD_ENEMY(CAR, 5);
		ADD_ENEMY(TRICYCLE, 1);
		waves.push_back(createWave(0.f, 2250, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 30);
		ADD_ENEMY(CAR, 20);
		ADD_ENEMY(TRICYCLE, 10);
		waves.push_back(createWave(0.0f, 1250, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 45);
		ADD_ENEMY(CAR, 25);
		ADD_ENEMY(TRICYCLE, 15);
		waves.push_back(createWave(0.1f, 750, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 100);
		ADD_ENEMY(CAR, 40);
		ADD_ENEMY(TRICYCLE, 25);
		waves.push_back(createWave(0.1f, 350, enemies));
		enemies.clear();
	}
		break;
	case 1:
	{
		ADD_ENEMY(SHIELD_ENEMY, 5);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(TRACTOR, 5);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 2);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(CAR, 2);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

//		ADD_ENEMY(SPIDER, 2);
//		waves.push_back(createWave(0.f, 2000, enemies));
//		enemies.clear();

		break;



		//mid slow, self heal
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(INFANTRY, 10);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 30);
		ADD_ENEMY(CAR, 1);
		ADD_ENEMY(TRICYCLE, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 5);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 30);
		ADD_ENEMY(CAR, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 5);
		ADD_ENEMY(TANK, 1);
		ADD_ENEMY(TRICYCLE, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 40);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(TANK, 5);
		ADD_ENEMY(CAR, 10);
		ADD_ENEMY(TRICYCLE, 15);
		waves.push_back(createWave(0.1f, 750, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(TANK, 2);
		ADD_ENEMY(CAR, 15);
		ADD_ENEMY(TRICYCLE, 20);
		waves.push_back(createWave(0.1f, 350, enemies));
		enemies.clear();
	}
		break;
	case 2:
	{
		ADD_ENEMY(ANOTHER_ENEMY, 3);
		ADD_ENEMY(INFANTRY, 15);
		ADD_ENEMY(TRACTOR, 1);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 30);
		ADD_ENEMY(CAR, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(TRICYCLE, 3);
		ADD_ENEMY(TRACTOR, 2);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 50);
		ADD_ENEMY(CAR, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(TRACTOR, 3);
		ADD_ENEMY(TANK, 3);
		ADD_ENEMY(TRICYCLE, 5);
		waves.push_back(createWave(0.0f, 900, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 75);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(TANK, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(TRACTOR, 10);
		ADD_ENEMY(CAR, 10);
		ADD_ENEMY(TRICYCLE, 15);
		waves.push_back(createWave(0.2f, 550, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 75);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(TANK, 3);
		ADD_ENEMY(CAR, 15);
		ADD_ENEMY(TRACTOR, 15);
		ADD_ENEMY(ANOTHER_ENEMY, 20);
		ADD_ENEMY(TRICYCLE, 20);
		waves.push_back(createWave(0.2f, 300, enemies));
		enemies.clear();
	}
		break;
	case 3:
	{
		//small next,spider
		ADD_ENEMY(INFANTRY, 5);
		ADD_ENEMY(SPIDER, 1);
		ADD_ENEMY(SMALL_NEXT, 10);
		waves.push_back(createWave(0.f, 1750, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 20);
		ADD_ENEMY(CAR, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 30);
		ADD_ENEMY(TRICYCLE, 3);
		ADD_ENEMY(SPIDER, 3);
		ADD_ENEMY(TRACTOR, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 30);
		ADD_ENEMY(CAR, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(SPIDER, 5);
		ADD_ENEMY(TRACTOR, 5);
		ADD_ENEMY(SMALL_NEXT, 50);
		ADD_ENEMY(TANK, 5);
		ADD_ENEMY(TRICYCLE, 5);
		waves.push_back(createWave(0.0f, 1250, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 35);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(TANK, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(TRACTOR, 10);
		ADD_ENEMY(CAR, 10);
		ADD_ENEMY(SMALL_NEXT, 100);
		ADD_ENEMY(SPIDER, 10);
		ADD_ENEMY(TRICYCLE, 15);
		waves.push_back(createWave(0.1f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(TANK, 5);
		ADD_ENEMY(CAR, 15);
		ADD_ENEMY(TRACTOR, 15);
		ADD_ENEMY(SMALL_NEXT, 150);
		ADD_ENEMY(SPIDER, 15);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(TRICYCLE, 20);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();
	}
		break;
	case 4:
	{
		//repair, mid fast
		ADD_ENEMY(INFANTRY, 10);
		ADD_ENEMY(REPAIR_ENEMY, 1);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 20);
		ADD_ENEMY(CAR, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 30);
		ADD_ENEMY(TRICYCLE, 3);
		ADD_ENEMY(SPIDER, 3);
		ADD_ENEMY(MID_FAST, 1);
		ADD_ENEMY(REPAIR_ENEMY, 3);
		ADD_ENEMY(TRACTOR, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 30);
		ADD_ENEMY(CAR, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(SPIDER, 5);
		ADD_ENEMY(TRACTOR, 5);
		ADD_ENEMY(MID_FAST, 3);
		ADD_ENEMY(REPAIR_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 50);
		ADD_ENEMY(TANK, 5);
		ADD_ENEMY(TRICYCLE, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 35);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(TANK, 15);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(TRACTOR, 10);
		ADD_ENEMY(CAR, 10);
		ADD_ENEMY(SMALL_NEXT, 100);
		ADD_ENEMY(MID_FAST, 5);
		ADD_ENEMY(REPAIR_ENEMY, 10);
		ADD_ENEMY(SPIDER, 10);
		ADD_ENEMY(TRICYCLE, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(TANK, 5);
		ADD_ENEMY(CAR, 15);
		ADD_ENEMY(TRACTOR, 15);
		ADD_ENEMY(SMALL_NEXT, 150);
		ADD_ENEMY(MID_FAST, 10);
		ADD_ENEMY(REPAIR_ENEMY, 15);
		ADD_ENEMY(SPIDER, 15);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(TRICYCLE, 20);
		waves.push_back(createWave(0.2f, 350, enemies));
		enemies.clear();
	}
		break;
	case 5:
	{
		//shell
		ADD_ENEMY(INFANTRY, 10);
		ADD_ENEMY(SHIELD_ENEMY, 1);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 20);
		ADD_ENEMY(CAR, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 30);
		ADD_ENEMY(TRICYCLE, 3);
		ADD_ENEMY(SPIDER, 3);
		ADD_ENEMY(SHIELD_ENEMY, 3);
		ADD_ENEMY(MID_FAST, 1);
		ADD_ENEMY(REPAIR_ENEMY, 3);
		ADD_ENEMY(TRACTOR, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 30);
		ADD_ENEMY(CAR, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(SPIDER, 5);
		ADD_ENEMY(TRACTOR, 5);
		ADD_ENEMY(SHIELD_ENEMY, 5);
		ADD_ENEMY(MID_FAST, 3);
		ADD_ENEMY(REPAIR_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 50);
		ADD_ENEMY(TANK, 5);
		ADD_ENEMY(TRICYCLE, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 35);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(TANK, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(TRACTOR, 10);
		ADD_ENEMY(CAR, 10);
		ADD_ENEMY(SMALL_NEXT, 100);
		ADD_ENEMY(SHIELD_ENEMY, 10);
		ADD_ENEMY(MID_FAST, 5);
		ADD_ENEMY(REPAIR_ENEMY, 10);
		ADD_ENEMY(SPIDER, 10);
		ADD_ENEMY(TRICYCLE, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(TANK, 15);
		ADD_ENEMY(CAR, 15);
		ADD_ENEMY(TRACTOR, 15);
		ADD_ENEMY(SMALL_NEXT, 150);
		ADD_ENEMY(MID_FAST, 10);
		ADD_ENEMY(SHIELD_ENEMY, 15);
		ADD_ENEMY(REPAIR_ENEMY, 15);
		ADD_ENEMY(SPIDER, 15);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(TRICYCLE, 20);
		waves.push_back(createWave(0.1f, 250, enemies));
		enemies.clear();
	}
		break;
	case 6:
	{
		//teleport
		ADD_ENEMY(INFANTRY, 15);
		ADD_ENEMY(TELEPORT_ENEMY, 1);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 20);
		ADD_ENEMY(CAR, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 30);
		ADD_ENEMY(TRICYCLE, 3);
		ADD_ENEMY(TELEPORT_ENEMY, 3);
		ADD_ENEMY(SPIDER, 3);
		ADD_ENEMY(SHIELD_ENEMY, 3);
		ADD_ENEMY(MID_FAST, 1);
		ADD_ENEMY(REPAIR_ENEMY, 3);
		ADD_ENEMY(TRACTOR, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 30);
		ADD_ENEMY(CAR, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(SPIDER, 5);
		ADD_ENEMY(TRACTOR, 5);
		ADD_ENEMY(SHIELD_ENEMY, 5);
		ADD_ENEMY(TELEPORT_ENEMY, 5);
		ADD_ENEMY(MID_FAST, 3);
		ADD_ENEMY(REPAIR_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 50);
		ADD_ENEMY(TANK, 5);
		ADD_ENEMY(TRICYCLE, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 35);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(TANK, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(TRACTOR, 10);
		ADD_ENEMY(CAR, 10);
		ADD_ENEMY(SMALL_NEXT, 100);
		ADD_ENEMY(SHIELD_ENEMY, 10);
		ADD_ENEMY(MID_FAST, 10);
		ADD_ENEMY(REPAIR_ENEMY, 10);
		ADD_ENEMY(TELEPORT_ENEMY, 10);
		ADD_ENEMY(SPIDER, 10);
		ADD_ENEMY(TRICYCLE, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(TANK, 15);
		ADD_ENEMY(CAR, 15);
		ADD_ENEMY(TRACTOR, 15);
		ADD_ENEMY(SMALL_NEXT, 150);
		ADD_ENEMY(MID_FAST, 15);
		ADD_ENEMY(SHIELD_ENEMY, 15);
		ADD_ENEMY(TELEPORT_ENEMY, 15);
		ADD_ENEMY(REPAIR_ENEMY, 15);
		ADD_ENEMY(SPIDER, 15);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(TRICYCLE, 20);
		waves.push_back(createWave(0.1f, 250, enemies));
		enemies.clear();
	}
		break;
	case 7:
	{
		//"big med tank
		ADD_ENEMY(INFANTRY, 15);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 20);
		ADD_ENEMY(CAR, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 30);
		ADD_ENEMY(TRICYCLE, 3);
		ADD_ENEMY(TELEPORT_ENEMY, 3);
		ADD_ENEMY(SPIDER, 3);
		ADD_ENEMY(SHIELD_ENEMY, 3);
		ADD_ENEMY(MID_FAST, 1);
		ADD_ENEMY(REPAIR_ENEMY, 3);
		ADD_ENEMY(TRACTOR, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 30);
		ADD_ENEMY(CAR, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(SPIDER, 5);
		ADD_ENEMY(TRACTOR, 5);
		ADD_ENEMY(SHIELD_ENEMY, 5);
		ADD_ENEMY(TELEPORT_ENEMY, 5);
		ADD_ENEMY(MID_FAST, 3);
		ADD_ENEMY(REPAIR_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 50);
		ADD_ENEMY(TANK, 5);
		ADD_ENEMY(BIG_MEDIUM, 1);
		ADD_ENEMY(TRICYCLE, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 35);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(TANK, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(TRACTOR, 10);
		ADD_ENEMY(CAR, 10);
		ADD_ENEMY(SMALL_NEXT, 100);
		ADD_ENEMY(SHIELD_ENEMY, 10);
		ADD_ENEMY(MID_FAST, 10);
		ADD_ENEMY(REPAIR_ENEMY, 10);
		ADD_ENEMY(TELEPORT_ENEMY, 10);
		ADD_ENEMY(SPIDER, 10);
		ADD_ENEMY(BIG_MEDIUM, 5);
		ADD_ENEMY(TRICYCLE, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(TANK, 15);
		ADD_ENEMY(CAR, 15);
		ADD_ENEMY(TRACTOR, 15);
		ADD_ENEMY(SMALL_NEXT, 150);
		ADD_ENEMY(MID_FAST, 15);
		ADD_ENEMY(SHIELD_ENEMY, 15);
		ADD_ENEMY(TELEPORT_ENEMY, 15);
		ADD_ENEMY(REPAIR_ENEMY, 15);
		ADD_ENEMY(SPIDER, 15);
		ADD_ENEMY(BIG_MEDIUM, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(TRICYCLE, 20);
		waves.push_back(createWave(0.1f, 250, enemies));
		enemies.clear();
	}
		break;
	case 8:
	{
		//spawn
		ADD_ENEMY(INFANTRY, 15);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 20);
		ADD_ENEMY(CAR, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 30);
		ADD_ENEMY(TRICYCLE, 3);
		ADD_ENEMY(TELEPORT_ENEMY, 3);
		ADD_ENEMY(SPIDER, 3);
		ADD_ENEMY(SHIELD_ENEMY, 3);
		ADD_ENEMY(SPAWN_ENEMY, 1);
		ADD_ENEMY(MID_FAST, 1);
		ADD_ENEMY(REPAIR_ENEMY, 3);
		ADD_ENEMY(TRACTOR, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 30);
		ADD_ENEMY(CAR, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(SPIDER, 5);
		ADD_ENEMY(TRACTOR, 5);
		ADD_ENEMY(SHIELD_ENEMY, 5);
		ADD_ENEMY(TELEPORT_ENEMY, 5);
		ADD_ENEMY(SPAWN_ENEMY, 3);
		ADD_ENEMY(MID_FAST, 3);
		ADD_ENEMY(REPAIR_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 50);
		ADD_ENEMY(TANK, 5);
		ADD_ENEMY(BIG_MEDIUM, 1);
		ADD_ENEMY(TRICYCLE, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 35);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(TANK, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(TRACTOR, 10);
		ADD_ENEMY(CAR, 10);
		ADD_ENEMY(SMALL_NEXT, 100);
		ADD_ENEMY(SHIELD_ENEMY, 10);
		ADD_ENEMY(MID_FAST, 10);
		ADD_ENEMY(SPAWN_ENEMY, 5);
		ADD_ENEMY(REPAIR_ENEMY, 10);
		ADD_ENEMY(TELEPORT_ENEMY, 10);
		ADD_ENEMY(SPIDER, 10);
		ADD_ENEMY(BIG_MEDIUM, 5);
		ADD_ENEMY(TRICYCLE, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(TANK, 15);
		ADD_ENEMY(CAR, 15);
		ADD_ENEMY(TRACTOR, 15);
		ADD_ENEMY(SMALL_NEXT, 150);
		ADD_ENEMY(MID_FAST, 15);
		ADD_ENEMY(SHIELD_ENEMY, 15);
		ADD_ENEMY(SPAWN_ENEMY, 10);
		ADD_ENEMY(TELEPORT_ENEMY, 15);
		ADD_ENEMY(REPAIR_ENEMY, 15);
		ADD_ENEMY(SPIDER, 15);
		ADD_ENEMY(BIG_MEDIUM, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(TRICYCLE, 20);
		waves.push_back(createWave(0.1f, 250, enemies));
		enemies.clear();
	}
		break;
	case 9:
	{
		//aircarrier
		ADD_ENEMY(INFANTRY, 15);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 20);
		ADD_ENEMY(CAR, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 30);
		ADD_ENEMY(TRICYCLE, 3);
		ADD_ENEMY(TELEPORT_ENEMY, 3);
		ADD_ENEMY(SPIDER, 3);
		ADD_ENEMY(SHIELD_ENEMY, 3);
		ADD_ENEMY(SPAWN_ENEMY, 1);
		ADD_ENEMY(MID_FAST, 1);
		ADD_ENEMY(BIG_SLOW, 1);
		ADD_ENEMY(REPAIR_ENEMY, 3);
		ADD_ENEMY(TRACTOR, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 30);
		ADD_ENEMY(CAR, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(SPIDER, 5);
		ADD_ENEMY(BIG_SLOW, 2);
		ADD_ENEMY(TRACTOR, 5);
		ADD_ENEMY(SHIELD_ENEMY, 5);
		ADD_ENEMY(TELEPORT_ENEMY, 5);
		ADD_ENEMY(SPAWN_ENEMY, 3);
		ADD_ENEMY(MID_FAST, 3);
		ADD_ENEMY(REPAIR_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 50);
		ADD_ENEMY(TANK, 5);
		ADD_ENEMY(BIG_MEDIUM, 1);
		ADD_ENEMY(TRICYCLE, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 35);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(TANK, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(TRACTOR, 10);
		ADD_ENEMY(CAR, 10);
		ADD_ENEMY(SMALL_NEXT, 100);
		ADD_ENEMY(SHIELD_ENEMY, 10);
		ADD_ENEMY(MID_FAST, 10);
		ADD_ENEMY(BIG_SLOW, 3);
		ADD_ENEMY(SPAWN_ENEMY, 5);
		ADD_ENEMY(REPAIR_ENEMY, 10);
		ADD_ENEMY(TELEPORT_ENEMY, 10);
		ADD_ENEMY(SPIDER, 10);
		ADD_ENEMY(BIG_MEDIUM, 5);
		ADD_ENEMY(TRICYCLE, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(INFANTRY, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(TANK, 15);
		ADD_ENEMY(CAR, 15);
		ADD_ENEMY(TRACTOR, 15);
		ADD_ENEMY(SMALL_NEXT, 150);
		ADD_ENEMY(MID_FAST, 15);
		ADD_ENEMY(SHIELD_ENEMY, 15);
		ADD_ENEMY(SPAWN_ENEMY, 10);
		ADD_ENEMY(TELEPORT_ENEMY, 15);
		ADD_ENEMY(BIG_SLOW, 5);
		ADD_ENEMY(REPAIR_ENEMY, 15);
		ADD_ENEMY(SPIDER, 15);
		ADD_ENEMY(BIG_MEDIUM, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(TRICYCLE, 20);
		waves.push_back(createWave(0.1f, 250, enemies));
		enemies.clear();
	}
		break;
	case 10:
	{

	}
		break;
	case 11:
	{

	}
		break;
	case 12:
	{

	}
		break;
	case 13:
	{

	}
		break;
	case 14:
	{

	}
		break;
	default:
		break;
	}
	return waves;
}


