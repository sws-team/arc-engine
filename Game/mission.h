#ifndef MISSION_H
#define MISSION_H

#include "stdheader.h"
#include "SpaceShip/spaceshiptypes.h"
#include "SpaceShip/levelenemy.h"
#include "SpaceShip/Weapons/weapontypes.h"
#include "Characters/charactertypes.h"
#include "Artifacts/artifacttypes.h"
#include "Characters/characteristics.h"

struct Mission
{
	struct Enemy
	{
		Vector2f pos;
		EnemyMove move;
		SPACESHIP_TYPES spaceShipType;
		ARTIFACT_TYPES artifact;
		vector<WEAPON_TYPES> weapons;
		vector<CHARACTERS> characters;
		Characteristics stats;
	};

	struct Boss
	{
		SPACESHIP_TYPES type;
		float x;
	};

	string name;
	vector<Enemy> enemies;
	Boss boss;

//	string loop;
//	vector<string> sprites;
	int maps;

	float speed;
	string backgroundSound;

	vector<string> speeches_start;
	vector<string> speeches_end;

	int id;
};

class Campaign
{
public:
	static Campaign& Instance();

	vector<Mission> missions;
	bool loadMissions(const string& fileName);
	bool loadSpaceShips(const string& fileName);
	bool loadWeapons(const string& fileName);

private:
	Campaign();
	Campaign(Campaign const&) = delete;
	Campaign& operator= (Campaign const&) = delete;

};

#endif // MISSION_H
