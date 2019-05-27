#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "stdheader.h"
#include "controlsettings.h"
#include "timer.h"

class Level;
class Cursor;

class Controller
{
public:
	Controller(Level* level, Cursor *cursor);
	void setPauseFunc(const function<void ()> &value);
	void keyEvent();
	void pausedEvents();

private:

	void keyboardKeyEvent(const bool timeout);
	void joystickKeyEvent(const bool timeout);

	Level *level;
	class Camera *camera;
	Cursor *cursor;
	Timer timerKey;
	Timer timerMove;

	constexpr static int CONTROLLER_TIME = 100;

	function<void()> pauseFunc;

	ControlSettings m_controls;
};

#endif // CONTROLLER_H
