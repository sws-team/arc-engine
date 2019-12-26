#include "enemy.h"

#define ADD_ENEMY(x, y) enemies.insert(make_pair<ENEMY_TYPES, int>(x, y));

Wave EnemiesFactory::createWave(float protection, float time, const map<ENEMY_TYPES, int> &enemies)
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

vector<Wave> EnemiesFactory::generateEnemies(unsigned int n)
{
	vector<Wave> waves;
	map<ENEMY_TYPES, int> enemies;
	switch (n)
	{
	case 0:
	{
		ADD_ENEMY(SCORPION, 10);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 20);
		ADD_ENEMY(SMALL_MEDIUM, 5);
		ADD_ENEMY(SMALL_FAST, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 30);
		ADD_ENEMY(SMALL_MEDIUM, 20);
		ADD_ENEMY(SMALL_FAST, 10);
		waves.push_back(createWave(0.1f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 45);
		ADD_ENEMY(SMALL_MEDIUM, 25);
		ADD_ENEMY(SMALL_FAST, 15);
		waves.push_back(createWave(0.2f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 100);
		ADD_ENEMY(SMALL_MEDIUM, 40);
		ADD_ENEMY(SMALL_FAST, 25);
		ADD_ENEMY(MID_SLOW, 2);
		waves.push_back(createWave(0.3f, 250, enemies));
		enemies.clear();
	}
		break;
	case 1:
	{
//		addEnemiesByType(SCORPION, 300, &spawnEnemies);
//		addEnemiesByType(SMALL_MEDIUM, 100, &spawnEnemies);
//		addEnemiesByType(SMALL_FAST, 50, &spawnEnemies);
//		addEnemiesByType(REPAIR_ENEMY, 5, &spawnEnemies);
//		addEnemiesByType(MID_SLOW, 10, &spawnEnemies);
//		addEnemiesByType(MID_FAST, 1, &spawnEnemies);
//		addEnemiesByType(SMALL_NEXT, 100, &spawnEnemies);
	}
		break;
	case 2:
	{
//		addEnemiesByType(SCORPION, 400, &spawnEnemies);
//		addEnemiesByType(SMALL_MEDIUM, 150, &spawnEnemies);
//		addEnemiesByType(SMALL_FAST, 75, &spawnEnemies);
//		addEnemiesByType(SELFHEAL_ENEMY, 2, &spawnEnemies);
//		addEnemiesByType(MID_SLOW, 50, &spawnEnemies);
//		addEnemiesByType(DOWN_TOWER_ENEMY, 5, &spawnEnemies);
//		addEnemiesByType(MID_FAST, 5, &spawnEnemies);
//		addEnemiesByType(SMALL_NEXT, 150, &spawnEnemies);
	}
		break;
	case 3:
	{
//		addEnemiesByType(SCORPION, 500, &spawnEnemies);
//		addEnemiesByType(SMALL_MEDIUM, 250, &spawnEnemies);
//		addEnemiesByType(SMALL_FAST, 100, &spawnEnemies);
//		addEnemiesByType(SELFHEAL_ENEMY, 5, &spawnEnemies);
//		addEnemiesByType(MID_SLOW, 75, &spawnEnemies);
//		addEnemiesByType(DOWN_TOWER_ENEMY, 15, &spawnEnemies);
//		addEnemiesByType(MID_FAST, 10, &spawnEnemies);
//		addEnemiesByType(SMALL_NEXT, 200, &spawnEnemies);
	}
		break;
	case 4:
	{
//		addEnemiesByType(SCORPION, 500, &spawnEnemies);
//		addEnemiesByType(SMALL_MEDIUM, 250, &spawnEnemies);
//		addEnemiesByType(SMALL_FAST, 100, &spawnEnemies);
//		addEnemiesByType(REPAIR_ENEMY, 5, &spawnEnemies);
//		addEnemiesByType(MID_SLOW, 75, &spawnEnemies);
//		addEnemiesByType(DOWN_TOWER_ENEMY, 15, &spawnEnemies);
//		addEnemiesByType(MID_FAST, 10, &spawnEnemies);
//		addEnemiesByType(SELFHEAL_ENEMY, 5, &spawnEnemies);
//		addEnemiesByType(SMALL_NEXT, 250, &spawnEnemies);
	}
		break;
	case 5:
	{
//		addEnemiesByType(SCORPION, 500, &spawnEnemies);
//		addEnemiesByType(SMALL_MEDIUM, 300, &spawnEnemies);
//		addEnemiesByType(SMALL_FAST, 100, &spawnEnemies);
//		addEnemiesByType(REPAIR_ENEMY, 10, &spawnEnemies);
//		addEnemiesByType(MID_SLOW, 75, &spawnEnemies);
//		addEnemiesByType(SPIDER, 25, &spawnEnemies);
//		addEnemiesByType(DOWN_TOWER_ENEMY, 15, &spawnEnemies);
//		addEnemiesByType(MID_FAST, 10, &spawnEnemies);
//		addEnemiesByType(BIG_SLOW, 1, &spawnEnemies);
//		addEnemiesByType(SELFHEAL_ENEMY, 10, &spawnEnemies);
//		addEnemiesByType(SMALL_NEXT, 300, &spawnEnemies);
	}
		break;
	case 6:
	{
//		addEnemiesByType(SCORPION, 500, &spawnEnemies);
//		addEnemiesByType(SMALL_MEDIUM, 300, &spawnEnemies);
//		addEnemiesByType(SMALL_FAST, 200, &spawnEnemies);
//		addEnemiesByType(REPAIR_ENEMY, 10, &spawnEnemies);
//		addEnemiesByType(MID_SLOW, 75, &spawnEnemies);
//		addEnemiesByType(SPIDER, 25, &spawnEnemies);
//		addEnemiesByType(SHELL_ENEMY, 15, &spawnEnemies);
//		addEnemiesByType(DOWN_TOWER_ENEMY, 15, &spawnEnemies);
//		addEnemiesByType(MID_FAST, 20, &spawnEnemies);
//		addEnemiesByType(BIG_SLOW, 2, &spawnEnemies);
//		addEnemiesByType(BIG_MEDIUM, 2, &spawnEnemies);
//		addEnemiesByType(SELFHEAL_ENEMY, 20, &spawnEnemies);
//		addEnemiesByType(SMALL_NEXT, 350, &spawnEnemies);
	}
		break;
	case 7:
	{
//		addEnemiesByType(SCORPION, 600, &spawnEnemies);
//		addEnemiesByType(SMALL_MEDIUM, 500, &spawnEnemies);
//		addEnemiesByType(SMALL_FAST, 200, &spawnEnemies);
//		addEnemiesByType(REPAIR_ENEMY, 20, &spawnEnemies);
//		addEnemiesByType(MID_SLOW, 100, &spawnEnemies);
//		addEnemiesByType(SPIDER, 45, &spawnEnemies);
//		addEnemiesByType(SHELL_ENEMY, 25, &spawnEnemies);
//		addEnemiesByType(DOWN_TOWER_ENEMY, 35, &spawnEnemies);
//		addEnemiesByType(MID_FAST, 25, &spawnEnemies);
//		addEnemiesByType(BIG_SLOW, 3, &spawnEnemies);
//		addEnemiesByType(BIG_MEDIUM, 2, &spawnEnemies);
//		addEnemiesByType(BIG_FAST, 2, &spawnEnemies);
//		addEnemiesByType(SELFHEAL_ENEMY, 20, &spawnEnemies);
//		addEnemiesByType(TELEPORT_ENEMY, 20, &spawnEnemies);
//		addEnemiesByType(SMALL_NEXT, 400, &spawnEnemies);
	}
		break;
	case 8:
	{
//		addEnemiesByType(SCORPION, 800, &spawnEnemies);
//		addEnemiesByType(SMALL_MEDIUM, 500, &spawnEnemies);
//		addEnemiesByType(SMALL_FAST, 250, &spawnEnemies);
//		addEnemiesByType(REPAIR_ENEMY, 40, &spawnEnemies);
//		addEnemiesByType(MID_SLOW, 150, &spawnEnemies);
//		addEnemiesByType(SPIDER, 50, &spawnEnemies);
//		addEnemiesByType(SHELL_ENEMY, 25, &spawnEnemies);
//		addEnemiesByType(DOWN_TOWER_ENEMY, 35, &spawnEnemies);
//		addEnemiesByType(MID_FAST, 25, &spawnEnemies);
//		addEnemiesByType(BIG_SLOW, 3, &spawnEnemies);
//		addEnemiesByType(BIG_MEDIUM, 2, &spawnEnemies);
//		addEnemiesByType(BIG_FAST, 2, &spawnEnemies);
//		addEnemiesByType(SELFHEAL_ENEMY, 40, &spawnEnemies);
//		addEnemiesByType(TELEPORT_ENEMY, 30, &spawnEnemies);
//		addEnemiesByType(SMALL_NEXT, 500, &spawnEnemies);
	}
		break;
	case 9:
	{
//		addEnemiesByType(SCORPION, 900, &spawnEnemies);
//		addEnemiesByType(SMALL_MEDIUM, 600, &spawnEnemies);
//		addEnemiesByType(SMALL_FAST, 300, &spawnEnemies);
//		addEnemiesByType(REPAIR_ENEMY, 50, &spawnEnemies);
//		addEnemiesByType(MID_SLOW, 100, &spawnEnemies);
//		addEnemiesByType(SPIDER, 150, &spawnEnemies);
//		addEnemiesByType(SHELL_ENEMY, 50, &spawnEnemies);
//		addEnemiesByType(DOWN_TOWER_ENEMY, 40, &spawnEnemies);
//		addEnemiesByType(MID_FAST, 30, &spawnEnemies);
//		addEnemiesByType(BIG_SLOW, 5, &spawnEnemies);
//		addEnemiesByType(BIG_MEDIUM, 3, &spawnEnemies);
//		addEnemiesByType(BIG_FAST, 3, &spawnEnemies);
//		addEnemiesByType(SELFHEAL_ENEMY, 50, &spawnEnemies);
//		addEnemiesByType(TELEPORT_ENEMY, 40, &spawnEnemies);
//		addEnemiesByType(SMALL_NEXT, 600, &spawnEnemies);
	}
		break;
	case 10:
	{
//		addEnemiesByType(SCORPION, 1000, &spawnEnemies);
//		addEnemiesByType(SMALL_MEDIUM, 600, &spawnEnemies);
//		addEnemiesByType(SMALL_FAST, 300, &spawnEnemies);
//		addEnemiesByType(REPAIR_ENEMY, 75, &spawnEnemies);
//		addEnemiesByType(MID_SLOW, 150, &spawnEnemies);
//		addEnemiesByType(SPIDER, 200, &spawnEnemies);
//		addEnemiesByType(SHELL_ENEMY, 50, &spawnEnemies);
//		addEnemiesByType(DOWN_TOWER_ENEMY, 50, &spawnEnemies);
//		addEnemiesByType(MID_FAST, 45, &spawnEnemies);
//		addEnemiesByType(BIG_SLOW, 7, &spawnEnemies);
//		addEnemiesByType(BIG_MEDIUM, 5, &spawnEnemies);
//		addEnemiesByType(BIG_FAST, 5, &spawnEnemies);
//		addEnemiesByType(SELFHEAL_ENEMY, 75, &spawnEnemies);
//		addEnemiesByType(TELEPORT_ENEMY, 60, &spawnEnemies);
//		addEnemiesByType(SMALL_NEXT, 700, &spawnEnemies);
	}
		break;
	case 11:
	{
//		addEnemiesByType(SCORPION, 1200, &spawnEnemies);
//		addEnemiesByType(SMALL_MEDIUM, 700, &spawnEnemies);
//		addEnemiesByType(SMALL_FAST, 300, &spawnEnemies);
//		addEnemiesByType(REPAIR_ENEMY, 100, &spawnEnemies);
//		addEnemiesByType(MID_SLOW, 200, &spawnEnemies);
//		addEnemiesByType(SPIDER, 250, &spawnEnemies);
//		addEnemiesByType(SHELL_ENEMY, 75, &spawnEnemies);
//		addEnemiesByType(DOWN_TOWER_ENEMY, 70, &spawnEnemies);
//		addEnemiesByType(MID_FAST, 50, &spawnEnemies);
//		addEnemiesByType(BIG_SLOW, 9, &spawnEnemies);
//		addEnemiesByType(BIG_MEDIUM, 7, &spawnEnemies);
//		addEnemiesByType(BIG_FAST, 7, &spawnEnemies);
//		addEnemiesByType(SELFHEAL_ENEMY, 90, &spawnEnemies);
//		addEnemiesByType(TELEPORT_ENEMY, 75, &spawnEnemies);
//		addEnemiesByType(SMALL_NEXT, 800, &spawnEnemies);
	}
		break;
	case 12:
	{
//		addEnemiesByType(SCORPION, 1400, &spawnEnemies);
//		addEnemiesByType(SMALL_MEDIUM, 800, &spawnEnemies);
//		addEnemiesByType(SMALL_FAST, 400, &spawnEnemies);
//		addEnemiesByType(REPAIR_ENEMY, 130, &spawnEnemies);
//		addEnemiesByType(MID_SLOW, 260, &spawnEnemies);
//		addEnemiesByType(SPIDER, 300, &spawnEnemies);
//		addEnemiesByType(SHELL_ENEMY, 80, &spawnEnemies);
//		addEnemiesByType(DOWN_TOWER_ENEMY, 90, &spawnEnemies);
//		addEnemiesByType(MID_FAST, 75, &spawnEnemies);
//		addEnemiesByType(BIG_SLOW, 10, &spawnEnemies);
//		addEnemiesByType(BIG_MEDIUM, 8, &spawnEnemies);
//		addEnemiesByType(BIG_FAST, 8, &spawnEnemies);
//		addEnemiesByType(SELFHEAL_ENEMY, 100, &spawnEnemies);
//		addEnemiesByType(TELEPORT_ENEMY, 100, &spawnEnemies);
//		addEnemiesByType(SMALL_NEXT, 900, &spawnEnemies);
	}
		break;
	case 13:
	{
//		addEnemiesByType(SCORPION, 1450, &spawnEnemies);
//		addEnemiesByType(SMALL_MEDIUM, 900, &spawnEnemies);
//		addEnemiesByType(SMALL_FAST, 450, &spawnEnemies);
//		addEnemiesByType(REPAIR_ENEMY, 150, &spawnEnemies);
//		addEnemiesByType(MID_SLOW, 280, &spawnEnemies);
//		addEnemiesByType(SPIDER, 350, &spawnEnemies);
//		addEnemiesByType(SHELL_ENEMY, 100, &spawnEnemies);
//		addEnemiesByType(DOWN_TOWER_ENEMY, 100, &spawnEnemies);
//		addEnemiesByType(MID_FAST, 100, &spawnEnemies);
//		addEnemiesByType(BIG_SLOW, 15, &spawnEnemies);
//		addEnemiesByType(BIG_MEDIUM, 10, &spawnEnemies);
//		addEnemiesByType(BIG_FAST, 10, &spawnEnemies);
//		addEnemiesByType(SELFHEAL_ENEMY, 150, &spawnEnemies);
//		addEnemiesByType(TELEPORT_ENEMY, 150, &spawnEnemies);
//		addEnemiesByType(SMALL_NEXT, 1000, &spawnEnemies);
	}
		break;
	case 14:
	{
//		addEnemiesByType(SCORPION, 1500, &spawnEnemies);
//		addEnemiesByType(SMALL_MEDIUM, 1000, &spawnEnemies);
//		addEnemiesByType(SMALL_FAST, 500, &spawnEnemies);
//		addEnemiesByType(REPAIR_ENEMY, 200, &spawnEnemies);
//		addEnemiesByType(MID_SLOW, 300, &spawnEnemies);
//		addEnemiesByType(SPIDER, 400, &spawnEnemies);
//		addEnemiesByType(SHELL_ENEMY, 150, &spawnEnemies);
//		addEnemiesByType(DOWN_TOWER_ENEMY, 150, &spawnEnemies);
//		addEnemiesByType(MID_FAST, 150, &spawnEnemies);
//		addEnemiesByType(BIG_SLOW, 50, &spawnEnemies);
//		addEnemiesByType(BIG_MEDIUM, 25, &spawnEnemies);
//		addEnemiesByType(BIG_FAST, 25, &spawnEnemies);
//		addEnemiesByType(SELFHEAL_ENEMY, 200, &spawnEnemies);
//		addEnemiesByType(TELEPORT_ENEMY, 200, &spawnEnemies);
//		addEnemiesByType(SMALL_NEXT, 1000, &spawnEnemies);
	}
		break;
	default:
		break;
	}
	return waves;
}

