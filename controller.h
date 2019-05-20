#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "stdheader.h"
#include "controlsettings.h"
#include "timer.h"

class SpaceShip;
class Level;

class Controller
{
public:
	Controller(Level* level);
	void update();
	void setPauseFunc(const function<void ()> &value);

	void pausedEvents();

private:
	void keyEvent();

	enum MOVE_DIRECTIONS
	{
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_UP,
		MOVE_DOWN
	};

	bool canMove(MOVE_DIRECTIONS direction, SpaceShip *spaceShip);

	void keyboardKeyEvent(SpaceShip* player, const ControlSettings& contols, const bool timeout);
	void joystickKeyEvent(SpaceShip* player, const ControlSettings& contols, const bool timeout);

	Level *level;
	Timer timerKey;
	Timer timerMove;

	constexpr static int CONTROLLER_TIME = 100;

	function<void()> pauseFunc;
};

#endif // CONTROLLER_H
