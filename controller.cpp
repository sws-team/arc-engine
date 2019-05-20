#include "controller.h"
#include "Game/level.h"
#include "globalvariables.h"
#include "Game/gamecontroller.h"
#include "globalvariables.h"

Controller::Controller(Level *level)
	: level(level)
{

}

void Controller::update()
{
	keyEvent();
}

void Controller::keyEvent()
{
	const bool timeout = timerKey.check(CONTROLLER_TIME);
	const ControlSettings controls = GlobalVariables::Instance().controls();
	if (controls.joystickId == 0)
		keyboardKeyEvent(spaceShip, controls, timeout);
	else
		joystickKeyEvent(spaceShip, controls, timeout);
}

bool Controller::canMove(Controller::MOVE_DIRECTIONS direction, SpaceShip *spaceShip)
{
	switch (direction)
	{
	case MOVE_LEFT:
	{
		if (spaceShip->pos().x - Level::LEVEL_BORDER < level->leftBorder())
			return false;
	}
		break;
	case MOVE_RIGHT:
	{
		if (spaceShip->getSize().x + spaceShip->pos().x + Level::LEVEL_BORDER > level->rightBorder())
			return false;
	}
		break;
	case MOVE_UP:
	{
		if (spaceShip->pos().y - Level::LEVEL_BORDER < level->topBorder())
			return false;
	}
		break;
	case MOVE_DOWN:
	{
		if (spaceShip->getSize().y + spaceShip->pos().y + Level::LEVEL_BORDER > level->bottomBorder())
			return false;
	}
		break;
	}
	return true;
}

void Controller::keyboardKeyEvent(SpaceShip *player, const ControlSettings &controls, const bool timeout)
{
	if (Keyboard::isKeyPressed(Keyboard::RBracket))
	{
		player->rotate(45.f);
		cout << 45<<endl;
	}
	if (Keyboard::isKeyPressed(Keyboard::LBracket))
	{
		player->rotate(-45.f);
		cout << -45<<endl;
	}
	if (Keyboard::isKeyPressed(Keyboard::Space))
	{
		level->smokeEnabled = !level->smokeEnabled;
	}

	player->shoot(Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.shoot)));
	player->shootWithDefaultWeapon(Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.shootWithDefaultWeapon)));
	if (timeout)
	{
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.nextWeapon)))
			player->nextWeapon();
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.previousWeapon)))
			player->previousWeapon();

		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.nextAbility)))
			player->nextCharacter();
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.previousAbility)))
			player->previousCharacter();
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.useAbility)))
			player->use();

		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.start)))
			pauseFunc();
	}
	if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.moveLeft)))
	{
		if (!canMove(MOVE_LEFT, player))
			return;
		player->moveLeft();
	}
	if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.moveRight)))
	{
		if (!canMove(MOVE_RIGHT, player))
			return;
		player->moveRight();
	}
	if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.moveUp)))
	{
		if (!canMove(MOVE_UP, player))
			return;
		player->moveUp();
	}
	if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.moveDown)))
	{
		if (!canMove(MOVE_DOWN, player))
			return;
		player->moveDown();
	}
}

void Controller::joystickKeyEvent(SpaceShip *player, const ControlSettings &controls, const bool timeout)
{
	player->shoot(Joystick::isButtonPressed(controls.joystickId, controls.shoot));
	player->shootWithDefaultWeapon(Joystick::isButtonPressed(controls.joystickId, controls.shootWithDefaultWeapon));

	const float joystick1X = Joystick::getAxisPosition(controls.joystickId, static_cast<Joystick::Axis>(controls.moveLeft));
	const float joystick1Y = Joystick::getAxisPosition(controls.joystickId,  static_cast<Joystick::Axis>(controls.moveUp));

	if (timeout)
	{
		if (Joystick::isButtonPressed(controls.joystickId, controls.nextWeapon))
			player->nextWeapon();
		if (Joystick::isButtonPressed(controls.joystickId, controls.previousWeapon))
			player->previousWeapon();

		if (Joystick::isButtonPressed(controls.joystickId, controls.nextAbility))
			player->nextCharacter();
		if (Joystick::isButtonPressed(controls.joystickId, controls.previousAbility))
			player->previousCharacter();
		if (Joystick::isButtonPressed(controls.joystickId, controls.useAbility))
			player->use();

		if (Joystick::isButtonPressed(controls.joystickId, controls.start))
			pauseFunc();
	}
	if (joystick1X > 50)
	{
		if (!canMove(MOVE_RIGHT, player))
			return;
		player->moveRight();
	}
	else if (joystick1X < -50)
	{
		if (!canMove(MOVE_LEFT, player))
			return;
		player->moveLeft();
	}
	if (joystick1Y > 50)
	{
		if (!canMove(MOVE_DOWN, player))
			return;
		player->moveDown();
	}
	else if (joystick1Y < -50)
	{
		if (!canMove(MOVE_UP, player))
			return;
		player->moveUp();
	}
}

void Controller::setPauseFunc(const function<void ()> &value)
{
	pauseFunc = value;
}

void Controller::pausedEvents()
{
	const bool timeout = timerKey.check(CONTROLLER_TIME);
	if (!timeout)
		return;
	for (unsigned int i = 0; i < GameController::Instance().players().size(); ++i)
	{
		const ControlSettings controls = GlobalVariables::Instance().controls().at(i);
		if (controls.joystickId == 0)
		{
			if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.start)))
				pauseFunc();
		}
		else
		{
			if (Joystick::isButtonPressed(controls.joystickId, controls.start))
				pauseFunc();
		}
	}
}
