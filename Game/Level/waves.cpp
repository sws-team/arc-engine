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
		//scorpoion, car, tricicle
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
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 45);
		ADD_ENEMY(SMALL_MEDIUM, 25);
		ADD_ENEMY(SMALL_FAST, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 100);
		ADD_ENEMY(SMALL_MEDIUM, 40);
		ADD_ENEMY(SMALL_FAST, 25);
		waves.push_back(createWave(0.1f, 250, enemies));
		enemies.clear();
	}
		break;
	case 1:
	{
		//mid slow, self heal
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(SCORPION, 10);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 30);
		ADD_ENEMY(SMALL_MEDIUM, 1);
		ADD_ENEMY(SMALL_FAST, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 5);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 30);
		ADD_ENEMY(SMALL_MEDIUM, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 5);
		ADD_ENEMY(MID_SLOW, 5);
		ADD_ENEMY(SMALL_FAST, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 40);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(MID_SLOW, 10);
		ADD_ENEMY(SMALL_MEDIUM, 10);
		ADD_ENEMY(SMALL_FAST, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(MID_SLOW, 15);
		ADD_ENEMY(SMALL_MEDIUM, 15);
		ADD_ENEMY(SMALL_FAST, 20);
		waves.push_back(createWave(0.1f, 250, enemies));
		enemies.clear();
	}
		break;
	case 2:
	{
		ADD_ENEMY(ANOTHER_ENEMY, 3);
		ADD_ENEMY(SCORPION, 15);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 1);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 30);
		ADD_ENEMY(SMALL_MEDIUM, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SMALL_FAST, 3);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 35);
		ADD_ENEMY(SMALL_MEDIUM, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 5);
		ADD_ENEMY(MID_SLOW, 5);
		ADD_ENEMY(SMALL_FAST, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(MID_SLOW, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 10);
		ADD_ENEMY(SMALL_MEDIUM, 10);
		ADD_ENEMY(SMALL_FAST, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 75);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(MID_SLOW, 15);
		ADD_ENEMY(SMALL_MEDIUM, 15);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 15);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(SMALL_FAST, 20);
		waves.push_back(createWave(0.1f, 250, enemies));
		enemies.clear();
	}
		break;
	case 3:
	{
		//small next,spider

		ADD_ENEMY(SCORPION, 5);
		ADD_ENEMY(SPIDER, 1);
		ADD_ENEMY(SMALL_NEXT, 10);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 20);
		ADD_ENEMY(SMALL_MEDIUM, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 30);
		ADD_ENEMY(SMALL_FAST, 3);
		ADD_ENEMY(SPIDER, 3);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 30);
		ADD_ENEMY(SMALL_MEDIUM, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(SPIDER, 5);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 50);
		ADD_ENEMY(MID_SLOW, 5);
		ADD_ENEMY(SMALL_FAST, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 35);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(MID_SLOW, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 10);
		ADD_ENEMY(SMALL_MEDIUM, 10);
		ADD_ENEMY(SMALL_NEXT, 100);
		ADD_ENEMY(SPIDER, 10);
		ADD_ENEMY(SMALL_FAST, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(MID_SLOW, 15);
		ADD_ENEMY(SMALL_MEDIUM, 15);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 15);
		ADD_ENEMY(SMALL_NEXT, 150);
		ADD_ENEMY(SPIDER, 15);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(SMALL_FAST, 20);
		waves.push_back(createWave(0.1f, 250, enemies));
		enemies.clear();
	}
		break;
	case 4:
	{
		//repair, mid fast
		ADD_ENEMY(SCORPION, 10);
		ADD_ENEMY(REPAIR_ENEMY, 1);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 20);
		ADD_ENEMY(SMALL_MEDIUM, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 30);
		ADD_ENEMY(SMALL_FAST, 3);
		ADD_ENEMY(SPIDER, 3);
		ADD_ENEMY(MID_FAST, 1);
		ADD_ENEMY(REPAIR_ENEMY, 3);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 30);
		ADD_ENEMY(SMALL_MEDIUM, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(SPIDER, 5);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 5);
		ADD_ENEMY(MID_FAST, 3);
		ADD_ENEMY(REPAIR_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 50);
		ADD_ENEMY(MID_SLOW, 5);
		ADD_ENEMY(SMALL_FAST, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 35);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(MID_SLOW, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 10);
		ADD_ENEMY(SMALL_MEDIUM, 10);
		ADD_ENEMY(SMALL_NEXT, 100);
		ADD_ENEMY(MID_FAST, 5);
		ADD_ENEMY(REPAIR_ENEMY, 10);
		ADD_ENEMY(SPIDER, 10);
		ADD_ENEMY(SMALL_FAST, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(MID_SLOW, 15);
		ADD_ENEMY(SMALL_MEDIUM, 15);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 15);
		ADD_ENEMY(SMALL_NEXT, 150);
		ADD_ENEMY(MID_FAST, 10);
		ADD_ENEMY(REPAIR_ENEMY, 15);
		ADD_ENEMY(SPIDER, 15);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(SMALL_FAST, 20);
		waves.push_back(createWave(0.1f, 250, enemies));
		enemies.clear();
	}
		break;
	case 5:
	{
		//shell
		ADD_ENEMY(SCORPION, 10);
		ADD_ENEMY(SHELL_ENEMY, 1);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 20);
		ADD_ENEMY(SMALL_MEDIUM, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 30);
		ADD_ENEMY(SMALL_FAST, 3);
		ADD_ENEMY(SPIDER, 3);
		ADD_ENEMY(SHELL_ENEMY, 3);
		ADD_ENEMY(MID_FAST, 1);
		ADD_ENEMY(REPAIR_ENEMY, 3);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 30);
		ADD_ENEMY(SMALL_MEDIUM, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(SPIDER, 5);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 5);
		ADD_ENEMY(SHELL_ENEMY, 5);
		ADD_ENEMY(MID_FAST, 3);
		ADD_ENEMY(REPAIR_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 50);
		ADD_ENEMY(MID_SLOW, 5);
		ADD_ENEMY(SMALL_FAST, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 35);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(MID_SLOW, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 10);
		ADD_ENEMY(SMALL_MEDIUM, 10);
		ADD_ENEMY(SMALL_NEXT, 100);
		ADD_ENEMY(SHELL_ENEMY, 10);
		ADD_ENEMY(MID_FAST, 5);
		ADD_ENEMY(REPAIR_ENEMY, 10);
		ADD_ENEMY(SPIDER, 10);
		ADD_ENEMY(SMALL_FAST, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(MID_SLOW, 15);
		ADD_ENEMY(SMALL_MEDIUM, 15);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 15);
		ADD_ENEMY(SMALL_NEXT, 150);
		ADD_ENEMY(MID_FAST, 10);
		ADD_ENEMY(SHELL_ENEMY, 15);
		ADD_ENEMY(REPAIR_ENEMY, 15);
		ADD_ENEMY(SPIDER, 15);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(SMALL_FAST, 20);
		waves.push_back(createWave(0.1f, 250, enemies));
		enemies.clear();
	}
		break;
	case 6:
	{
		//teleport
		ADD_ENEMY(SCORPION, 15);
		ADD_ENEMY(TELEPORT_ENEMY, 1);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 20);
		ADD_ENEMY(SMALL_MEDIUM, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 30);
		ADD_ENEMY(SMALL_FAST, 3);
		ADD_ENEMY(TELEPORT_ENEMY, 3);
		ADD_ENEMY(SPIDER, 3);
		ADD_ENEMY(SHELL_ENEMY, 3);
		ADD_ENEMY(MID_FAST, 1);
		ADD_ENEMY(REPAIR_ENEMY, 3);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 30);
		ADD_ENEMY(SMALL_MEDIUM, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(SPIDER, 5);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 5);
		ADD_ENEMY(SHELL_ENEMY, 5);
		ADD_ENEMY(TELEPORT_ENEMY, 5);
		ADD_ENEMY(MID_FAST, 3);
		ADD_ENEMY(REPAIR_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 50);
		ADD_ENEMY(MID_SLOW, 5);
		ADD_ENEMY(SMALL_FAST, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 35);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(MID_SLOW, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 10);
		ADD_ENEMY(SMALL_MEDIUM, 10);
		ADD_ENEMY(SMALL_NEXT, 100);
		ADD_ENEMY(SHELL_ENEMY, 10);
		ADD_ENEMY(MID_FAST, 10);
		ADD_ENEMY(REPAIR_ENEMY, 10);
		ADD_ENEMY(TELEPORT_ENEMY, 10);
		ADD_ENEMY(SPIDER, 10);
		ADD_ENEMY(SMALL_FAST, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(MID_SLOW, 15);
		ADD_ENEMY(SMALL_MEDIUM, 15);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 15);
		ADD_ENEMY(SMALL_NEXT, 150);
		ADD_ENEMY(MID_FAST, 15);
		ADD_ENEMY(SHELL_ENEMY, 15);
		ADD_ENEMY(TELEPORT_ENEMY, 15);
		ADD_ENEMY(REPAIR_ENEMY, 15);
		ADD_ENEMY(SPIDER, 15);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(SMALL_FAST, 20);
		waves.push_back(createWave(0.1f, 250, enemies));
		enemies.clear();
	}
		break;
	case 7:
	{
		//"big med tank
		ADD_ENEMY(SCORPION, 15);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 20);
		ADD_ENEMY(SMALL_MEDIUM, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 30);
		ADD_ENEMY(SMALL_FAST, 3);
		ADD_ENEMY(TELEPORT_ENEMY, 3);
		ADD_ENEMY(SPIDER, 3);
		ADD_ENEMY(SHELL_ENEMY, 3);
		ADD_ENEMY(MID_FAST, 1);
		ADD_ENEMY(REPAIR_ENEMY, 3);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 30);
		ADD_ENEMY(SMALL_MEDIUM, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(SPIDER, 5);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 5);
		ADD_ENEMY(SHELL_ENEMY, 5);
		ADD_ENEMY(TELEPORT_ENEMY, 5);
		ADD_ENEMY(MID_FAST, 3);
		ADD_ENEMY(REPAIR_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 50);
		ADD_ENEMY(MID_SLOW, 5);
		ADD_ENEMY(BIG_MEDIUM, 1);
		ADD_ENEMY(SMALL_FAST, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 35);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(MID_SLOW, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 10);
		ADD_ENEMY(SMALL_MEDIUM, 10);
		ADD_ENEMY(SMALL_NEXT, 100);
		ADD_ENEMY(SHELL_ENEMY, 10);
		ADD_ENEMY(MID_FAST, 10);
		ADD_ENEMY(REPAIR_ENEMY, 10);
		ADD_ENEMY(TELEPORT_ENEMY, 10);
		ADD_ENEMY(SPIDER, 10);
		ADD_ENEMY(BIG_MEDIUM, 5);
		ADD_ENEMY(SMALL_FAST, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(MID_SLOW, 15);
		ADD_ENEMY(SMALL_MEDIUM, 15);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 15);
		ADD_ENEMY(SMALL_NEXT, 150);
		ADD_ENEMY(MID_FAST, 15);
		ADD_ENEMY(SHELL_ENEMY, 15);
		ADD_ENEMY(TELEPORT_ENEMY, 15);
		ADD_ENEMY(REPAIR_ENEMY, 15);
		ADD_ENEMY(SPIDER, 15);
		ADD_ENEMY(BIG_MEDIUM, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(SMALL_FAST, 20);
		waves.push_back(createWave(0.1f, 250, enemies));
		enemies.clear();
	}
		break;
	case 8:
	{
		//spawn
		ADD_ENEMY(SCORPION, 15);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 20);
		ADD_ENEMY(SMALL_MEDIUM, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 30);
		ADD_ENEMY(SMALL_FAST, 3);
		ADD_ENEMY(TELEPORT_ENEMY, 3);
		ADD_ENEMY(SPIDER, 3);
		ADD_ENEMY(SHELL_ENEMY, 3);
		ADD_ENEMY(SPAWN_ENEMY, 1);
		ADD_ENEMY(MID_FAST, 1);
		ADD_ENEMY(REPAIR_ENEMY, 3);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 30);
		ADD_ENEMY(SMALL_MEDIUM, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(SPIDER, 5);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 5);
		ADD_ENEMY(SHELL_ENEMY, 5);
		ADD_ENEMY(TELEPORT_ENEMY, 5);
		ADD_ENEMY(SPAWN_ENEMY, 3);
		ADD_ENEMY(MID_FAST, 3);
		ADD_ENEMY(REPAIR_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 50);
		ADD_ENEMY(MID_SLOW, 5);
		ADD_ENEMY(BIG_MEDIUM, 1);
		ADD_ENEMY(SMALL_FAST, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 35);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(MID_SLOW, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 10);
		ADD_ENEMY(SMALL_MEDIUM, 10);
		ADD_ENEMY(SMALL_NEXT, 100);
		ADD_ENEMY(SHELL_ENEMY, 10);
		ADD_ENEMY(MID_FAST, 10);
		ADD_ENEMY(SPAWN_ENEMY, 5);
		ADD_ENEMY(REPAIR_ENEMY, 10);
		ADD_ENEMY(TELEPORT_ENEMY, 10);
		ADD_ENEMY(SPIDER, 10);
		ADD_ENEMY(BIG_MEDIUM, 5);
		ADD_ENEMY(SMALL_FAST, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(MID_SLOW, 15);
		ADD_ENEMY(SMALL_MEDIUM, 15);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 15);
		ADD_ENEMY(SMALL_NEXT, 150);
		ADD_ENEMY(MID_FAST, 15);
		ADD_ENEMY(SHELL_ENEMY, 15);
		ADD_ENEMY(SPAWN_ENEMY, 10);
		ADD_ENEMY(TELEPORT_ENEMY, 15);
		ADD_ENEMY(REPAIR_ENEMY, 15);
		ADD_ENEMY(SPIDER, 15);
		ADD_ENEMY(BIG_MEDIUM, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(SMALL_FAST, 20);
		waves.push_back(createWave(0.1f, 250, enemies));
		enemies.clear();
	}
		break;
	case 9:
	{
		//aircarrier
		ADD_ENEMY(SCORPION, 15);
		waves.push_back(createWave(0.f, 2000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 20);
		ADD_ENEMY(SMALL_MEDIUM, 1);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 30);
		ADD_ENEMY(SMALL_FAST, 3);
		ADD_ENEMY(TELEPORT_ENEMY, 3);
		ADD_ENEMY(SPIDER, 3);
		ADD_ENEMY(SHELL_ENEMY, 3);
		ADD_ENEMY(SPAWN_ENEMY, 1);
		ADD_ENEMY(MID_FAST, 1);
		ADD_ENEMY(BIG_SLOW, 1);
		ADD_ENEMY(REPAIR_ENEMY, 3);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 3);
		ADD_ENEMY(SELFHEAL_ENEMY, 1);
		waves.push_back(createWave(0.f, 1500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 30);
		ADD_ENEMY(SMALL_MEDIUM, 5);
		ADD_ENEMY(ANOTHER_ENEMY, 5);
		ADD_ENEMY(SELFHEAL_ENEMY, 3);
		ADD_ENEMY(SPIDER, 5);
		ADD_ENEMY(BIG_SLOW, 2);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 5);
		ADD_ENEMY(SHELL_ENEMY, 5);
		ADD_ENEMY(TELEPORT_ENEMY, 5);
		ADD_ENEMY(SPAWN_ENEMY, 3);
		ADD_ENEMY(MID_FAST, 3);
		ADD_ENEMY(REPAIR_ENEMY, 5);
		ADD_ENEMY(SMALL_NEXT, 50);
		ADD_ENEMY(MID_SLOW, 5);
		ADD_ENEMY(BIG_MEDIUM, 1);
		ADD_ENEMY(SMALL_FAST, 5);
		waves.push_back(createWave(0.0f, 1000, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 35);
		ADD_ENEMY(SELFHEAL_ENEMY, 10);
		ADD_ENEMY(MID_SLOW, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 10);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 10);
		ADD_ENEMY(SMALL_MEDIUM, 10);
		ADD_ENEMY(SMALL_NEXT, 100);
		ADD_ENEMY(SHELL_ENEMY, 10);
		ADD_ENEMY(MID_FAST, 10);
		ADD_ENEMY(BIG_SLOW, 3);
		ADD_ENEMY(SPAWN_ENEMY, 5);
		ADD_ENEMY(REPAIR_ENEMY, 10);
		ADD_ENEMY(TELEPORT_ENEMY, 10);
		ADD_ENEMY(SPIDER, 10);
		ADD_ENEMY(BIG_MEDIUM, 5);
		ADD_ENEMY(SMALL_FAST, 15);
		waves.push_back(createWave(0.1f, 500, enemies));
		enemies.clear();

		ADD_ENEMY(SCORPION, 50);
		ADD_ENEMY(SELFHEAL_ENEMY, 15);
		ADD_ENEMY(MID_SLOW, 15);
		ADD_ENEMY(SMALL_MEDIUM, 15);
		ADD_ENEMY(DOWN_TOWER_ENEMY, 15);
		ADD_ENEMY(SMALL_NEXT, 150);
		ADD_ENEMY(MID_FAST, 15);
		ADD_ENEMY(SHELL_ENEMY, 15);
		ADD_ENEMY(SPAWN_ENEMY, 10);
		ADD_ENEMY(TELEPORT_ENEMY, 15);
		ADD_ENEMY(BIG_SLOW, 5);
		ADD_ENEMY(REPAIR_ENEMY, 15);
		ADD_ENEMY(SPIDER, 15);
		ADD_ENEMY(BIG_MEDIUM, 10);
		ADD_ENEMY(ANOTHER_ENEMY, 15);
		ADD_ENEMY(SMALL_FAST, 20);
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

