#include "controller.h"
#include "Game/level.h"
#include "Game/camera.h"
#include "Game/gamecursor.h"
#include "Game/gamepanel.h"
#include "engine.h"
#include "Game/instructions.h"
#include "managers.h"
#include "gameoptions.h"

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
			if (Engine::Instance().options<GameOptions>()->instructions()->isActive())
			{
				Engine::Instance().options<GameOptions>()->instructions()->next();
				return;
			}

			if (Engine::Instance().options<GameOptions>()->panel()->clickOnMiniMap(Vector2f(event->mouseButton.x, event->mouseButton.y)))
				return;

			const Vector2i pixelPos = Vector2i(1, 1) + Vector2i(event->mouseButton.x, event->mouseButton.y);//Mouse::getPosition(*Engine::Instance().window());
			const Vector2f pos = Engine::Instance().window()->mapPixelToCoords(
						pixelPos, *Engine::Instance().options<GameOptions>()->camera()->getView());
			Engine::Instance().options<GameOptions>()->panel()->resetPanelIcon();
			Engine::Instance().options<GameOptions>()->level()->chooseByPos(pos);
		}
		else if (event->mouseButton.button == Mouse::Right)
		{
			if (Engine::Instance().options<GameOptions>()->instructions()->isActive())
			{
				Engine::Instance().options<GameOptions>()->instructions()->skip();
				return;
			}
			Engine::Instance().options<GameOptions>()->level()->clearCursor();
		}
	}
		break;
	case Event::KeyPressed:
	{
		switch (event->key.code)
		{
		case Keyboard::Space:
		{
			if (Engine::Instance().options<GameOptions>()->instructions()->isActive())
			{
				Engine::Instance().options<GameOptions>()->instructions()->skip();
				return;
			}
			Engine::Instance().options<GameOptions>()->level()->ready();
		}
			break;
		case Keyboard::Return:
		{
			if (Engine::Instance().options<GameOptions>()->instructions()->isActive())
			{
				Engine::Instance().options<GameOptions>()->instructions()->next();
				return;
			}
			Engine::Instance().options<GameOptions>()->level()->chooseCurrent();
		}
			break;
		case Keyboard::BackSpace:
			Engine::Instance().options<GameOptions>()->camera()->resetView();
			break;
		case Keyboard::Escape:
			pauseFunc();
			break;
		case Keyboard::F3:
			Engine::Instance().globalVariables()->switchFPS();
			break;
		case Keyboard::Q:
			Engine::Instance().options<GameOptions>()->cursor()->swap();
			break;
		case Keyboard::F:
			Engine::Instance().options<GameOptions>()->level()->upgradeTower(Engine::Instance().options<GameOptions>()->level()->selectedTower());
			break;
		case Keyboard::Add:
			Engine::Instance().options<GameOptions>()->camera()->zoomIn();
			break;
		case Keyboard::Subtract:
			Engine::Instance().options<GameOptions>()->camera()->zoomOut();
			break;
		case Keyboard::Left:
			Engine::Instance().options<GameOptions>()->cursor()->moveLeft();
			break;
		case Keyboard::Right:
			Engine::Instance().options<GameOptions>()->cursor()->moveRight();
			break;
		case Keyboard::Up:
			Engine::Instance().options<GameOptions>()->cursor()->moveUp();
			break;
		case Keyboard::Down:
			Engine::Instance().options<GameOptions>()->cursor()->moveDown();
			break;
		case Keyboard::A:
			Engine::Instance().options<GameOptions>()->camera()->moveLeftByCell();
			break;
		case Keyboard::D:
			Engine::Instance().options<GameOptions>()->camera()->moveRightByCell();
			break;
		case Keyboard::W:
			Engine::Instance().options<GameOptions>()->camera()->moveUpByCell();
			break;
		case Keyboard::S:
			Engine::Instance().options<GameOptions>()->camera()->moveDownByCell();
			break;
		case Keyboard::Z:
			Engine::Instance().options<GameOptions>()->level()->activateBombAbility();
			break;
		case Keyboard::X:
			Engine::Instance().options<GameOptions>()->level()->activateFreezeBombAbility();
			break;
		case Keyboard::C:
			Engine::Instance().options<GameOptions>()->level()->activateVenomAbility();
			break;
		case Keyboard::V:
			Engine::Instance().options<GameOptions>()->level()->activateIncreaseTowerDamageAbility();
			break;
		case Keyboard::B:
			Engine::Instance().options<GameOptions>()->level()->activateIncreaseTowerAttackSpeedAbility();
			break;
		case Keyboard::N:
			Engine::Instance().options<GameOptions>()->level()->activateStopAbility();
			break;
		case Keyboard::R:
			Engine::Instance().options<GameOptions>()->level()->test();
			break;
		case Keyboard::I:
			Engine::Instance().options<GameOptions>()->setNormalSpeed();
			break;
		case Keyboard::O:
			Engine::Instance().options<GameOptions>()->setFastSpeed();
			break;
		case Keyboard::P:
			Engine::Instance().options<GameOptions>()->setExtraFastSpeed();
			break;
		case Keyboard::Delete:
			Engine::Instance().options<GameOptions>()->level()->sellTower(Engine::Instance().options<GameOptions>()->level()->selectedTower());
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
			Engine::Instance().options<GameOptions>()->camera()->zoomOut();
		else
			Engine::Instance().options<GameOptions>()->camera()->zoomIn();
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
		if (Engine::Instance().options<GameOptions>()->instructions()->isActive())
		{
			if (Joystick::isButtonPressed(joystickId, KEY_START))
				Engine::Instance().options<GameOptions>()->instructions()->skip();
			else if (Joystick::isButtonPressed(joystickId, KEY_CHOOSE))
				Engine::Instance().options<GameOptions>()->instructions()->next();
			return;
		}
		if (Joystick::isButtonPressed(joystickId, KEY_START))
			Engine::Instance().options<GameOptions>()->level()->ready();
		if (Joystick::isButtonPressed(joystickId, KEY_CHOOSE))
			Engine::Instance().options<GameOptions>()->level()->chooseCurrent();
		if (Joystick::isButtonPressed(joystickId, KEY_ESCAPE))
			pauseFunc();
		if (Joystick::isButtonPressed(joystickId, 4))
			Engine::Instance().options<GameOptions>()->cursor()->swap();
		if (Joystick::isButtonPressed(joystickId, 5))
			Engine::Instance().options<GameOptions>()->level()->clearCursor();
	}
	if (timeoutMove)
	{
		const float joystick1X = Joystick::getAxisPosition(joystickId, Joystick::X);
		const float joystick1Y = Joystick::getAxisPosition(joystickId, Joystick::Y);
		if (joystick1X > 50)
			Engine::Instance().options<GameOptions>()->cursor()->moveRight();
		else if (joystick1X < -50)
			Engine::Instance().options<GameOptions>()->cursor()->moveLeft();
		if (joystick1Y > 50)
			Engine::Instance().options<GameOptions>()->cursor()->moveDown();
		else if (joystick1Y < -50)
			Engine::Instance().options<GameOptions>()->cursor()->moveUp();
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

