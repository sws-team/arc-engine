#include "timer.h"
#include "Engine/engine.h"

Timer::Timer()
{
	reset();
}

void Timer::reset()
{
	clock.restart();
	lastTimer = 0;
}

bool Timer::check(float msec)
{
	const Int64 currentTime = clock.getElapsedTime().asMilliseconds();
	const Int64 cur = std::abs(currentTime - lastTimer);// Время прошедшее с предыдущего раза

	if (cur > msec * Engine::Instance().gameSpeed())
	{
		lastTimer = currentTime;
		return true;
	}

	return false;
}
