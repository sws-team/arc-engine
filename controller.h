#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "stdheader.h"
#include "timer.h"

class Level;

class Controller
{
public:
	Controller();

	void eventFilter(sf::Event* event);

	void setPauseFunc(const std::function<void ()> &value);
	enum GAMEPAD_KEYS
	{
		KEY_START,
		KEY_CHOOSE,
		KEY_ESCAPE,
	};
	int currentJoystickId() const;

private:
	void joystickKeyEvent(const bool timeoutKey, const bool timeoutMove);
	std::function<void()> pauseFunc;
};

#endif // CONTROLLER_H
