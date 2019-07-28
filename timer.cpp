#include "timer.h"
#include "Engine/engine.h"

Timer::Timer()
{
	clock.restart();
	lastTimer = 0;
}

bool Timer::check(float msec)
{
	const Int64 currentTime = clock.getElapsedTime().asMilliseconds();
	const Int64 cur = _abs64(currentTime - lastTimer);// Время прошедшее с предыдущего раза

	if (cur > msec * Engine::Instance().gameSpeed())
	{
		lastTimer = currentTime;
		return true;
	}

	return false;
}
