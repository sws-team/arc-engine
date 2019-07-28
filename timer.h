#ifndef TIMER_H
#define TIMER_H

#include "stdheader.h"

struct Timer
{
	Timer();

	Clock clock;
	Int64 lastTimer;

	bool check(float msec);
};

#endif // TIMER_H
