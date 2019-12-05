#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "stdheader.h"
#include "controlsettings.h"
#include "timer.h"

class Level;

class Controller
{
public:
	Controller();

	void setPauseFunc(const function<void ()> &value);
	void keyEvent();
	void pausedEvents();
private:

	void keyboardKeyEvent(const bool timeoutKey, const bool timeoutMove);
	void joystickKeyEvent(const bool timeoutKey, const bool timeoutMove);

	Timer timerKey;
	Timer timerMove;

	constexpr static int CONTROLLER_TIME = 20;
	constexpr static int CONTROLLER_MOVE_TIME = 50;

	function<void()> pauseFunc;

	ControlSettings m_controls;
#ifdef STEAM_API
	class ISteamScreenshots *p_screenShoots;
#endif
};

#endif // CONTROLLER_H
