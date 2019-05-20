#include "cpu.h"
#include "SpaceShip/spaceship.h"
#include "gamecontroller.h"
#include "globalvariables.h"
#include "Game/level.h"

CPU::CPU()
{
	boss = nullptr;
	level = nullptr;
}

bool CPU::actionSpaceShip(LevelEnemy& levelEnemy, ENEMY_ACTIONS actionType)
{
	float moveX = 0;
	float moveY = 0;

	switch (actionType)
	{
	case STAY:
		break;
	case LEFT_DOWN:
		levelEnemy.spaceShip->moveLeft();
		levelEnemy.spaceShip->moveDown();
		break;
	case DOWN:
		levelEnemy.spaceShip->moveDown();
		break;
	case RIGHT_DOWN:
		levelEnemy.spaceShip->moveDown();
		levelEnemy.spaceShip->moveRight();
		break;
	case LEFT:
		levelEnemy.spaceShip->moveLeft();
		break;
	case RIGHT:
		levelEnemy.spaceShip->moveRight();
		break;
	case LEFT_UP:
		levelEnemy.spaceShip->moveLeft();
		levelEnemy.spaceShip->moveUp();
		break;
	case UP:
		levelEnemy.spaceShip->moveUp();
		break;
	case RIGHT_UP:
		levelEnemy.spaceShip->moveRight();
		levelEnemy.spaceShip->moveUp();
		break;
	case SHOOT_DEFAULT_WEAPON:
		levelEnemy.spaceShip->shootWithDefaultWeapon(true);
		break;
	case SHOOT:
		levelEnemy.spaceShip->shoot(true);
		break;
	case USE_ABILITY:
		levelEnemy.spaceShip->use();
		break;
	case NEXT_WEAPON:
		levelEnemy.spaceShip->nextWeapon();
		break;
	case PREVIOUS_WEAPON:
		levelEnemy.spaceShip->previousWeapon();
		break;
	case NEXT_CHARACTER:
		levelEnemy.spaceShip->nextCharacter();
		break;
	case PREVIOUS_CHARACTER:
		levelEnemy.spaceShip->previousCharacter();
		break;
	case ROTATE_1_DEGREES:
		levelEnemy.spaceShip->rotateWeapon(1);
		break;
	case ROTATE_5_DEGREES:
		levelEnemy.spaceShip->rotateWeapon(5);
		break;
	case ROTATE_10_DEGREES:
		levelEnemy.spaceShip->rotateWeapon(10);
		break;
	case ROTATE_45_DEGREES:
		levelEnemy.spaceShip->rotateWeapon(45);
		break;
	case ROTATE_1_NEGATIVE_DEGREES:
		levelEnemy.spaceShip->rotateWeapon(-1);
		break;
	case ROTATE_5_NEGATIVE_DEGREES:
		levelEnemy.spaceShip->rotateWeapon(-5);
		break;
	case ROTATE_10_NEGATIVE_DEGREES:
		levelEnemy.spaceShip->rotateWeapon(-10);
		break;
	case ROTATE_45_NEGATIVE_DEGREES:
		levelEnemy.spaceShip->rotateWeapon(-45);
		break;
	}

	levelEnemy.spaceShip->moveSpaceShip(moveX, moveY);
	levelEnemy.currentMove++;
	if (levelEnemy.currentMove >= levelEnemy.move.count)
	{
		levelEnemy.currentDirection++;
		levelEnemy.currentMove = 0;
		if (levelEnemy.currentDirection >= levelEnemy.move.directions.size())
		{
			if (levelEnemy.move.looped)
				levelEnemy.currentDirection = 0;
			else
			{
				level->removeEnemy(levelEnemy.spaceShip);
				return false;
			}
		}
	}
	return true;
}

bool CPU::isInstant(ENEMY_ACTIONS actionType) const
{
	if (actionType == SHOOT_DEFAULT_WEAPON ||
			actionType == USE_ABILITY ||
			actionType == NEXT_WEAPON ||
			actionType == PREVIOUS_WEAPON ||
			actionType == NEXT_CHARACTER ||
			actionType == PREVIOUS_CHARACTER ||
			actionType == SHOOT ||
			actionType == ROTATE_1_DEGREES ||
			actionType == ROTATE_5_DEGREES ||
			actionType == ROTATE_10_DEGREES ||
			actionType == ROTATE_45_DEGREES ||
			actionType == ROTATE_1_NEGATIVE_DEGREES ||
			actionType == ROTATE_5_NEGATIVE_DEGREES ||
			actionType == ROTATE_10_NEGATIVE_DEGREES ||
			actionType == ROTATE_45_NEGATIVE_DEGREES)
		return true;
	return false;
}

CPU &CPU::Instance()
{
	static CPU instance;
	return instance;
}

void CPU::action(vector<LevelEnemy> &spaceShips, float dy)
{
	for(LevelEnemy& levelEnemy : spaceShips)
	{
		if (!levelEnemy.spaceShip->getSpaceShipStats().isStatic)
			levelEnemy.spaceShip->move(0, dy);

		bool instant = false;
		do
		{
			const ENEMY_ACTIONS actionType = levelEnemy.move.directions.at(levelEnemy.currentDirection);
			instant = isInstant(actionType);
			const bool isOk = actionSpaceShip(levelEnemy, actionType);
			if (!isOk)
				break;
		}
		while(instant);
	}
	if (boss != nullptr)
		boss->move(0, dy);
}

void CPU::update()
{

}

void CPU::setBoss(SpaceShip *boss)
{
	this->boss = boss;
}

void CPU::setLevel(Level *level)
{
	this->level = level;
}

