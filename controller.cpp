#include "controller.h"
#include "Game/Level/level.h"
#include "Game/Level/camera.h"
#include "Game/Level/gamecursor.h"
#include "Game/gamepanel.h"
#include "globalvariables.h"
#include "Engine/engine.h"
#include "Game/Level/instructions.h"

Controller::Controller()
{
#ifdef STEAM_API
	p_screenShoots = SteamScreenshots();
	p_screenShoots->HookScreenshots(true);
#endif
}

void Controller::eventFilter(Event *event)
{
	switch (event->type)
	{
	case Event::MouseButtonPressed:
	{
		if (event->mouseButton.button == Mouse::Left)
		{
			if (Engine::Instance().instructions()->isActive())
			{
				Engine::Instance().instructions()->next();
				return;
			}

			if (Engine::Instance().panel()->clickOnMiniMap(Vector2f(event->mouseButton.x, event->mouseButton.y)))
				return;

			const Vector2i pixelPos = Vector2i(1, 1) + Vector2i(event->mouseButton.x, event->mouseButton.y);//Mouse::getPosition(*Engine::Instance().window());
			const Vector2f pos = Engine::Instance().window()->mapPixelToCoords(
						pixelPos, *Engine::Instance().camera()->getView());
			Engine::Instance().panel()->resetPanelIcon();
			Engine::Instance().level()->chooseByPos(pos);
		}
		else if (event->mouseButton.button == Mouse::Right)
		{
			if (Engine::Instance().instructions()->isActive())
			{
				Engine::Instance().instructions()->skip();
				return;
			}
			Engine::Instance().level()->clearCursor();
		}
	}
		break;
	case Event::KeyPressed:
	{
		switch (event->key.code)
		{
		case Keyboard::Space:
		{
			if (Engine::Instance().instructions()->isActive())
			{
				Engine::Instance().instructions()->skip();
				return;
			}
			Engine::Instance().level()->ready();
		}
			break;
		case Keyboard::Return:
		{
			if (Engine::Instance().instructions()->isActive())
			{
				Engine::Instance().instructions()->next();
				return;
			}
			Engine::Instance().level()->chooseCurrent();
		}
			break;
		case Keyboard::BackSpace:
			Engine::Instance().camera()->resetView();
			break;
		case Keyboard::Escape:
			pauseFunc();
			break;
		case Keyboard::Q:
			Engine::Instance().cursor()->swap();
			break;
		case Keyboard::F:
			Engine::Instance().level()->upgradeTower(Engine::Instance().level()->selectedTower());
			break;
		case Keyboard::Add:
			Engine::Instance().camera()->zoomIn();
			break;
		case Keyboard::Subtract:
			Engine::Instance().camera()->zoomOut();
			break;
		case Keyboard::Left:
			Engine::Instance().cursor()->moveLeft();
			break;
		case Keyboard::Right:
			Engine::Instance().cursor()->moveRight();
			break;
		case Keyboard::Up:
			Engine::Instance().cursor()->moveUp();
			break;
		case Keyboard::Down:
			Engine::Instance().cursor()->moveDown();
			break;
		case Keyboard::A:
			Engine::Instance().camera()->moveLeftByCell();
			break;
		case Keyboard::D:
			Engine::Instance().camera()->moveRightByCell();
			break;
		case Keyboard::W:
			Engine::Instance().camera()->moveUpByCell();
			break;
		case Keyboard::S:
			Engine::Instance().camera()->moveDownByCell();
			break;
		case Keyboard::Z:
			Engine::Instance().level()->activateBombAbility();
			break;
		case Keyboard::X:
			Engine::Instance().level()->activateFreezeBombAbility();
			break;
		case Keyboard::C:
			Engine::Instance().level()->activateVenomAbility();
			break;
		case Keyboard::V:
			Engine::Instance().level()->activateIncreaseTowerDamageAbility();
			break;
		case Keyboard::B:
			Engine::Instance().level()->activateIncreaseTowerAttackSpeedAbility();
			break;
		case Keyboard::N:
			Engine::Instance().level()->activateStopAbility();
			break;
		case Keyboard::R:
			Engine::Instance().level()->test();
			break;
		case Keyboard::P:
			Engine::Instance().level()->plus();
			break;
		case Keyboard::O:
			Engine::Instance().level()->minus();
			break;
		case Keyboard::Delete:
			Engine::Instance().level()->sellTower(Engine::Instance().level()->selectedTower());
			break;
#ifdef STEAM_API
		case Keyboard::F12:
			p_screenShoots->TriggerScreenshot();
			break;
#endif
		default:
			break;
		}
	}
		break;
	case Event::MouseWheelScrolled:
	{
		if (event->mouseWheelScroll.delta < 0)
			Engine::Instance().camera()->zoomOut();
		else
			Engine::Instance().camera()->zoomIn();
	}
		break;
	default:
		break;
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
		if (Joystick::isButtonPressed(joystickId, 5))
			Engine::Instance().level()->clearCursor();
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

int Controller::currentJoystickId() const
{
	return 1;
}

