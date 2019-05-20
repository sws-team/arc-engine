#ifndef OBJECTSFACTORY_H
#define OBJECTSFACTORY_H

#include "stdheader.h"
#include "SpaceShip/Weapons/weapontypes.h"
#include "SpaceShip/spaceshiptypes.h"
#include "SpaceShip/spaceshipstats.h"
#include "SpaceShip/spaceship.h"
#include "SpaceShip/Weapons/weaponstats.h"
#include "SpaceShip/spaceshipstats.h"
#include "Game/Characters/character.h"
#include "Game/Characters/charactertypes.h"
#include "Game/Artifacts/artifacttypes.h"

class AbstractWeapon;

class ObjectsFactory
{
public:
	static ObjectsFactory &Instance();
	AbstractWeapon *createWeapon(WEAPON_TYPES type);

	WeaponStats weaponStats(WEAPON_TYPES type) const;

	SpaceShipStats getSpaceShipStats(SPACESHIP_TYPES type) const;

	SpaceShip *createSpaceShip(SPACESHIP_TYPES type, const Vector2f& pos);

	vector<Vector2f> getShootingPos(SHOOT_POSTITIONS type, SpaceShip *spaceShip) const;

	Character *createCharacter(CHARACTERS type);

	Artifact *createArtifact(ARTIFACT_TYPES type);

	void setWeaponsStats(const map<WEAPON_TYPES, WeaponStats> &weaponsStats);

	void setSpaceShipStats(const map<SPACESHIP_TYPES, SpaceShipStats> &spaceShipStats);

private:
	ObjectsFactory();
	map<WEAPON_TYPES, WeaponStats> m_weaponsStats;
	map<SPACESHIP_TYPES, SpaceShipStats> m_spaceShipStats;

//	void fillWeaponsStats();
};

#endif // OBJECTSFACTORY_H
