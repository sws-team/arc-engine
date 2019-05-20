#ifndef TIMER_H
#define TIMER_H

#include "stdheader.h"

struct Timer
{
	Timer()
	{
		clock.restart();
		lastTimer = 0;
	}

	Clock clock;
	Int64 lastTimer;

	bool check(float msec)
	{
		const Int64 currentTime = clock.getElapsedTime().asMilliseconds();
		const Int64 cur = _abs64(currentTime - lastTimer);// Время прошедшее с предыдущего раза

		if (cur > msec)
		{
			lastTimer = currentTime;
			return true;
		}

		return false;
	}
};

#endif // TIMER_H
