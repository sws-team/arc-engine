#include "controller.h"
#include "Game/Level/level.h"
#include "Game/Level/camera.h"
#include "Game/Level/cursor.h"
#include "Game/gamepanel.h"
#include "globalvariables.h"
#include "Engine/engine.h"
#include "Game/Level/instructions.h"

Controller::Controller()
{
//	m_controls = GlobalVariables::Instance().controls();
#ifdef STEAM_API
	p_screenShoots = SteamScreenshots();
	p_screenShoots->HookScreenshots(true);
#endif
}

void Controller::keyEvent()
{
	const bool timeoutKey = timerKey.check(CONTROLLER_TIME);
	const bool timeoutMove = timerMove.check(CONTROLLER_MOVE_TIME);
	keyboardKeyEvent(timeoutKey, timeoutMove);
	joystickKeyEvent(timeoutKey, timeoutMove);
}

void Controller::keyboardKeyEvent(const bool timeoutKey, const bool timeoutMove)
{
	if (timeoutKey)
	{
		if (Engine::Instance().instructions()->isActive())
		{
			if (Keyboard::isKeyPressed(Keyboard::Space))
				Engine::Instance().instructions()->skip();
			else if (Keyboard::isKeyPressed(Keyboard::Return))
				Engine::Instance().instructions()->next();

			if (Mouse::isButtonPressed(Mouse::Left))
				Engine::Instance().instructions()->next();
			else if (Mouse::isButtonPressed(Mouse::Right))
				Engine::Instance().instructions()->skip();

			return;
		}
#ifdef STEAM_API
		if (Keyboard::isKeyPressed(Keyboard::F12))
			p_screenShoots->TriggerScreenshot();
#endif
		if (Keyboard::isKeyPressed(Keyboard::Space))
			Engine::Instance().level()->ready();

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			const Vector2i pixelPos = Mouse::getPosition(*Engine::Instance().window());
			const Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos, *Engine::Instance().camera()->getView());
			Engine::Instance().level()->chooseByPos(pos);
		}
		if (Keyboard::isKeyPressed(Keyboard::Return))
			Engine::Instance().level()->chooseCurrent();
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			pauseFunc();
		if (Keyboard::isKeyPressed(Keyboard::Q))
			Engine::Instance().cursor()->swap();


		if (Keyboard::isKeyPressed(Keyboard::Add))
			Engine::Instance().camera()->zoomIn();
		if (Keyboard::isKeyPressed(Keyboard::Subtract))
			Engine::Instance().camera()->zoomOut();
	}
	if (timeoutMove)
	{
		if (Keyboard::isKeyPressed(Keyboard::Left/*static_cast<Keyboard::Key>(m_controls.moveLeft)*/))
			Engine::Instance().cursor()->moveLeft();
		if (Keyboard::isKeyPressed(Keyboard::Right/*static_cast<Keyboard::Key>(m_controls.moveRight)*/))
			Engine::Instance().cursor()->moveRight();
		if (Keyboard::isKeyPressed(Keyboard::Up/*static_cast<Keyboard::Key>(m_controls.moveUp)*/))
			Engine::Instance().cursor()->moveUp();
		if (Keyboard::isKeyPressed(Keyboard::Down/*static_cast<Keyboard::Key>(m_controls.moveDown)*/))
			Engine::Instance().cursor()->moveDown();

		if (Keyboard::isKeyPressed(Keyboard::A))
			Engine::Instance().camera()->moveLeftByCell();
		if (Keyboard::isKeyPressed(Keyboard::D))
			Engine::Instance().camera()->moveRightByCell();
		if (Keyboard::isKeyPressed(Keyboard::W))
			Engine::Instance().camera()->moveUpByCell();
		if (Keyboard::isKeyPressed(Keyboard::S))
			Engine::Instance().camera()->moveDownByCell();
	}
}

void Controller::joystickKeyEvent(const bool timeoutKey, const bool timeoutMove)
{
	const int joystickId = currentJoystickId();
	if (timeoutKey)
	{
		if (Engine::Instance().instructions()->isActive())
		{
			if (Joystick::isButtonPressed(joystickId, KEY_START))
				Engine::Instance().instructions()->skip();
			else if (Joystick::isButtonPressed(joystickId, KEY_CHOOSE))
				Engine::Instance().instructions()->next();
			return;
		}
		if (Joystick::isButtonPressed(joystickId, KEY_START))
			Engine::Instance().level()->ready();
		if (Joystick::isButtonPressed(joystickId, KEY_CHOOSE))
			Engine::Instance().level()->chooseCurrent();
		if (Joystick::isButtonPressed(joystickId, KEY_ESCAPE))
			pauseFunc();
		if (Joystick::isButtonPressed(joystickId, 4))
			Engine::Instance().cursor()->swap();
	}
	if (timeoutMove)
	{
		const float joystick1X = Joystick::getAxisPosition(joystickId, Joystick::X);
		const float joystick1Y = Joystick::getAxisPosition(joystickId, Joystick::Y);
		if (joystick1X > 50)
			Engine::Instance().cursor()->moveRight();
		else if (joystick1X < -50)
			Engine::Instance().cursor()->moveLeft();
		if (joystick1Y > 50)
			Engine::Instance().cursor()->moveDown();
		else if (joystick1Y < -50)
			Engine::Instance().cursor()->moveUp();
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

	if (Keyboard::isKeyPressed(Keyboard::Escape))
		pauseFunc();
	if (Joystick::isButtonPressed(currentJoystickId(), KEY_ESCAPE))
			pauseFunc();
}

int Controller::currentJoystickId() const
{
	return 1;
}

