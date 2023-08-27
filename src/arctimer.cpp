#include "arctimer.h"

ArcTimer::ArcTimer():
	lastTimer(0)
	,pausedTime(0)
	,isPaused(false)
{
	TimersManager::Instance().addTimer(this);
	restart();
}

ArcTimer::~ArcTimer()
{
	TimersManager::Instance().removeTimer(this);
}

void ArcTimer::restart()
{
	isPaused = false;
	clock.restart();
	lastTimer = 0;
	pausedTime = 0;
}

void ArcTimer::pause()
{
	isPaused = true;
	pausedTime = clock.getElapsedTime().asMilliseconds();
}

void ArcTimer::unpause()
{
	lastTimer += clock.getElapsedTime().asMilliseconds() - pausedTime;
	isPaused = false;
}

bool ArcTimer::isTimeout(float msec)
{
	if (isPaused)
		return false;
	if (elapsed() >= msec) {
		restart();
		return true;
	}
	return false;
}

sf::Int32 ArcTimer::elapsed() const
{
	return clock.getElapsedTime().asMilliseconds() - lastTimer;
}

TimersManager &TimersManager::Instance()
{
	static TimersManager instance;
	return instance;
}

void TimersManager::addTimer(ArcTimer *timer)
{
	timers.push_back(timer);
}

void TimersManager::removeTimer(ArcTimer *timer)
{
	timers.erase(remove(timers.begin(), timers.end(), timer));
}

void TimersManager::setPaused(bool paused)
{
	if (m_paused == paused)
		return;

	m_paused = paused;

	for(ArcTimer *timer : timers) {
		if (paused)
			timer->pause();
		else
			timer->unpause();
	}
}
