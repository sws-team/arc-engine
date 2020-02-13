#include "timer.h"
#include "Engine/engine.h"

Timer::Timer():
	lastTimer(0)
  ,pausedTime(0)
  ,isPaused(false)
{
	TimersManager::Instance().addTimer(this);
	reset();
}

Timer::~Timer()
{
	TimersManager::Instance().removeTimer(this);
}

void Timer::reset()
{
	isPaused = false;
	clock.restart();
	lastTimer = 0;
	pausedTime = 0;
}

void Timer::pause()
{
	isPaused = true;
	pausedTime = clock.getElapsedTime().asMilliseconds();
}

void Timer::unpause()
{
	lastTimer += clock.getElapsedTime().asMilliseconds() - pausedTime;
	isPaused = false;
}

bool Timer::check(float msec)
{
	if (isPaused)
		return false;
	const Int64 currentTime = clock.getElapsedTime().asMilliseconds();

	if (currentTime - lastTimer >= msec * Engine::Instance().gameSpeed())
	{
		reset();
		return true;
	}
	return false;
}

float Timer::getElapsedMilliseconds() const
{
	return clock.getElapsedTime().asMilliseconds() - lastTimer;
}

TimersManager &TimersManager::Instance()
{
	static TimersManager instance;
	return instance;
}

void TimersManager::addTimer(Timer *timer)
{
	timers.push_back(timer);
}

void TimersManager::removeTimer(Timer *timer)
{
	timers.erase(remove(timers.begin(), timers.end(), timer));
}

void TimersManager::setPaused(bool isPaused)
{
	for(Timer *timer : timers)
	{
		if (isPaused)
			timer->pause();
		else
			timer->unpause();
	}
}
TimersManager::TimersManager()
{

}
