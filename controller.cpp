#include "controller.h"
#include "Game/level.h"
#include "Game/camera.h"
#include "Game/gamecursor.h"
#include "Game/gamepanel.h"
#include "engine.h"
#include "Game/instructions.h"
#include "managers.h"
#include "gameoptions.h"

#ifdef STEAM_API
#include "steam_api.h"
#endif

Controller::Controller()
{
#ifdef STEAM_API
	p_screenShoots = SteamScreenshots();
	p_screenShoots->HookScreenshots(true);
#endif
}

void Controller::eventFilter(sf::Event *event)
{
	switch (event->type)
	{
	case sf::Event::MouseButtonPressed:
	{
		if (event->mouseButton.button == sf::Mouse::Left)
		{
			if (Engine::Instance().options<GameOptions>()->instructions()->isActive())
			{
				Engine::Instance().options<GameOptions>()->instructions()->next();
				return;
			}

			if (Engine::Instance().options<GameOptions>()->panel()->clickOnMiniMap(sf::Vector2f(event->mouseButton.x,
																								event->mouseButton.y)))
				return;

			const sf::Vector2i pixelPos = sf::Vector2i(1, 1) +
					sf::Vector2i(event->mouseButton.x, event->mouseButton.y);//Mouse::getPosition(*Engine::Instance().window());
			const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(
						pixelPos, *Engine::Instance().options<GameOptions>()->camera()->getView());
			Engine::Instance().options<GameOptions>()->panel()->resetPanelIcon();
			Engine::Instance().options<GameOptions>()->level()->chooseByPos(pos);
		}
		else if (event->mouseButton.button == sf::Mouse::Right)
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
	case sf::Event::KeyPressed:
	{
		switch (event->key.code)
		{
		case sf::Keyboard::Space:
		{
			if (Engine::Instance().options<GameOptions>()->instructions()->isActive())
			{
				Engine::Instance().options<GameOptions>()->instructions()->skip();
				return;
			}
			Engine::Instance().options<GameOptions>()->level()->ready();
		}
			break;
		case sf::Keyboard::Return:
		{
			if (Engine::Instance().options<GameOptions>()->instructions()->isActive())
			{
				Engine::Instance().options<GameOptions>()->instructions()->next();
				return;
			}
			Engine::Instance().options<GameOptions>()->level()->chooseCurrent();
		}
			break;
		case sf::Keyboard::F2:
			Engine::Instance().options<GameOptions>()->level()->test();
			break;
		case sf::Keyboard::BackSpace:
			Engine::Instance().options<GameOptions>()->camera()->resetView();
			break;
		case sf::Keyboard::Escape:
			pauseFunc();
			break;
		case sf::Keyboard::F3:
			Engine::Instance().globalVariables()->switchFPS();
			break;
		case sf::Keyboard::Q:
			Engine::Instance().options<GameOptions>()->cursor()->swap();
			break;
		case sf::Keyboard::F:
			Engine::Instance().options<GameOptions>()->level()->upgradeTower(Engine::Instance().options<GameOptions>()->level()->selectedTower());
			Engine::Instance().options<GameOptions>()->level()->updateCurrentTower();
			Engine::Instance().options<GameOptions>()->level()->updateRadius();
			break;
		case sf::Keyboard::Add:
			Engine::Instance().options<GameOptions>()->camera()->zoomIn();
			break;
		case sf::Keyboard::Subtract:
			Engine::Instance().options<GameOptions>()->camera()->zoomOut();
			break;
		case sf::Keyboard::Left:
			Engine::Instance().options<GameOptions>()->cursor()->moveLeft();
			break;
		case sf::Keyboard::Right:
			Engine::Instance().options<GameOptions>()->cursor()->moveRight();
			break;
		case sf::Keyboard::Up:
			Engine::Instance().options<GameOptions>()->cursor()->moveUp();
			break;
		case sf::Keyboard::Down:
			Engine::Instance().options<GameOptions>()->cursor()->moveDown();
			break;
		case sf::Keyboard::A:
			Engine::Instance().options<GameOptions>()->camera()->moveLeftByCell();
			break;
		case sf::Keyboard::D:
			Engine::Instance().options<GameOptions>()->camera()->moveRightByCell();
			break;
		case sf::Keyboard::W:
			Engine::Instance().options<GameOptions>()->camera()->moveUpByCell();
			break;
		case sf::Keyboard::S:
			Engine::Instance().options<GameOptions>()->camera()->moveDownByCell();
			break;
		case sf::Keyboard::Z:
			Engine::Instance().options<GameOptions>()->level()->activateBombAbility();
			break;
		case sf::Keyboard::X:
			Engine::Instance().options<GameOptions>()->level()->activateFreezeBombAbility();
			break;
		case sf::Keyboard::C:
			Engine::Instance().options<GameOptions>()->level()->activateVenomAbility();
			break;
		case sf::Keyboard::V:
			Engine::Instance().options<GameOptions>()->level()->activateIncreaseTowerDamageAbility();
			break;
		case sf::Keyboard::B:
			Engine::Instance().options<GameOptions>()->level()->activateIncreaseTowerAttackSpeedAbility();
			break;
		case sf::Keyboard::N:
			Engine::Instance().options<GameOptions>()->level()->activateStopAbility();
			break;
		case sf::Keyboard::R:
			Engine::Instance().options<GameOptions>()->level()->test();
			break;
		case sf::Keyboard::I:
			Engine::Instance().options<GameOptions>()->setNormalSpeed();
			break;
		case sf::Keyboard::O:
			Engine::Instance().options<GameOptions>()->setFastSpeed();
			break;
		case sf::Keyboard::P:
			Engine::Instance().options<GameOptions>()->setExtraFastSpeed();
			break;
		case sf::Keyboard::Delete:
			Engine::Instance().options<GameOptions>()->level()->sellTower(Engine::Instance().options<GameOptions>()->level()->selectedTower());
			break;
#ifdef STEAM_API
		case sf::Keyboard::F12:
			p_screenShoots->TriggerScreenshot();
			break;
#endif
		default:
			break;
		}
	}
		break;
	case sf::Event::MouseWheelScrolled:
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
	if (!sf::Joystick::isConnected(joystickId))
		return;
	if (timeoutKey)
	{
		if (Engine::Instance().options<GameOptions>()->instructions()->isActive())
		{
			if (sf::Joystick::isButtonPressed(joystickId, KEY_START))
				Engine::Instance().options<GameOptions>()->instructions()->skip();
			else if (sf::Joystick::isButtonPressed(joystickId, KEY_CHOOSE))
				Engine::Instance().options<GameOptions>()->instructions()->next();
			return;
		}
		if (sf::Joystick::isButtonPressed(joystickId, KEY_START))
			Engine::Instance().options<GameOptions>()->level()->ready();
		if (sf::Joystick::isButtonPressed(joystickId, KEY_CHOOSE))
			Engine::Instance().options<GameOptions>()->level()->chooseCurrent();
		if (sf::Joystick::isButtonPressed(joystickId, KEY_ESCAPE))
			pauseFunc();
		if (sf::Joystick::isButtonPressed(joystickId, 4))
			Engine::Instance().options<GameOptions>()->cursor()->swap();
		if (sf::Joystick::isButtonPressed(joystickId, 5))
			Engine::Instance().options<GameOptions>()->level()->clearCursor();
	}
	if (timeoutMove)
	{
		const float joystick1X = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::X);
		const float joystick1Y = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::Y);
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

void Controller::setPauseFunc(const std::function<void ()> &value)
{
	pauseFunc = value;
}

int Controller::currentJoystickId() const
{
	return 1;
}

