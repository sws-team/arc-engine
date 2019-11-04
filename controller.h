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

	void keyboardKeyEvent(const bool timeout);
	void joystickKeyEvent(const bool timeout);

	Timer timerKey;
	Timer timerMove;

	constexpr static int CONTROLLER_TIME = 30;

	function<void()> pauseFunc;

	ControlSettings m_controls;
#ifdef STEAM_API
	class ISteamScreenshots *p_screenShoots;
#endif
};

#endif // CONTROLLER_H
