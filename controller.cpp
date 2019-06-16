#include "controller.h"
#include "Game/Level/level.h"
#include "globalvariables.h"
#include "Engine/engine.h"
#include "Game/Level/camera.h"
#include "Game/Level/cursor.h"

Controller::Controller()
{
	m_controls = GlobalVariables::Instance().controls();
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
		if (Keyboard::isKeyPressed(Keyboard::P))
			p_level->spawn();

		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(m_controls.action)))
			p_level->action();
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(m_controls.change)))
			p_level->change();
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(m_controls.start)))
			pauseFunc();
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			Engine::Instance().camera()->moveLeftByCell();
			Engine::Instance().camera()->detach();
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			Engine::Instance().camera()->moveRightByCell();
			Engine::Instance().camera()->detach();
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			Engine::Instance().camera()->moveUpByCell();
			Engine::Instance().camera()->detach();
		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			Engine::Instance().camera()->moveDownByCell();			
			Engine::Instance().camera()->detach();
		}
		if (Keyboard::isKeyPressed(Keyboard::BackSpace))
			Engine::Instance().camera()->resetZoom();
		if (Keyboard::isKeyPressed(Keyboard::Add))
			Engine::Instance().camera()->zoomIn();
		if (Keyboard::isKeyPressed(Keyboard::Subtract))
			Engine::Instance().camera()->zoomOut();

		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(m_controls.moveLeft)))
			Engine::Instance().cursor()->moveLeft();
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(m_controls.moveRight)))
			Engine::Instance().cursor()->moveRight();
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(m_controls.moveUp)))
			Engine::Instance().cursor()->moveUp();
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(m_controls.moveDown)))
			Engine::Instance().cursor()->moveDown();
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

void Controller::setLevel(Level *level)
{
	p_level = level;
}
