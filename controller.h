#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "stdheader.h"
#include "timer.h"

class Level;

class Controller
{
public:
	Controller();

	void eventFilter(Event* event);

	void setPauseFunc(const function<void ()> &value);
	enum GAMEPAD_KEYS
	{
		KEY_START,
		KEY_CHOOSE,
		KEY_ESCAPE,
	};
	int currentJoystickId() const;

private:
	void joystickKeyEvent(const bool timeoutKey, const bool timeoutMove);
	function<void()> pauseFunc;
#ifdef STEAM_API
	class ISteamScreenshots *p_screenShoots;
#endif
};

#endif // CONTROLLER_H
