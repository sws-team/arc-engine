#include "controller.h"
#include "Game/level.h"
#include "globalvariables.h"
#include "Game/Level/Cursor/cursor.h"
#include "Game/Level/camera.h"

Controller::Controller(Level *level, Cursor *cursor)
	: level(level)
	,cursor(cursor)
{
	m_controls = GlobalVariables::Instance().controls();

	camera = new Camera();
	level->setCamera(camera);
}

void Controller::keyEvent()
{
	const bool timeout = timerKey.check(CONTROLLER_TIME);
	keyboardKeyEvent(timeout);
	joystickKeyEvent(timeout);
}

void Controller::keyboardKeyEvent(const bool timeout)
{
	if (timeout)
	{
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(m_controls.action)))
			level->action();
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(m_controls.change)))
			level->change();
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(m_controls.start)))
			pauseFunc();
		if (Keyboard::isKeyPressed(Keyboard::Left))
			camera->moveLeft();
		if (Keyboard::isKeyPressed(Keyboard::Right))
			camera->moveRight();
		if (Keyboard::isKeyPressed(Keyboard::Up))
			camera->moveUp();
		if (Keyboard::isKeyPressed(Keyboard::Down))
			camera->moveDown();
		if (Keyboard::isKeyPressed(Keyboard::BackSpace))
			camera->resetZoom();
		if (Keyboard::isKeyPressed(Keyboard::Add))
			camera->zoomIn();
		if (Keyboard::isKeyPressed(Keyboard::Subtract))
			camera->zoomOut();

		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(m_controls.moveLeft)))
		{
			if (!cursor->canMove(Cursor::MOVE_LEFT))
				return;
			cursor->moveLeft();
		}
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(m_controls.moveRight)))
		{
			if (!cursor->canMove(Cursor::MOVE_RIGHT))
				return;
			cursor->moveRight();
		}
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(m_controls.moveUp)))
		{
			if (!cursor->canMove(Cursor::MOVE_UP))
				return;
			cursor->moveUp();
		}
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(m_controls.moveDown)))
		{
			if (!cursor->canMove(Cursor::MOVE_DOWN))
				return;
			cursor->moveDown();
		}
	}

}

void Controller::joystickKeyEvent(const bool timeout)
{
	/*
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
	}*/
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

	if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(GlobalVariables::Instance().controls().start)))
		pauseFunc();
//	if (Joystick::isButtonPressed(controls.joystickId, controls.start))
//		pauseFunc();
}
