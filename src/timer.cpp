#include "gametimer.h"
#include "engine.h"

GameTimer::GameTimer():
	lastTimer(0)
  ,pausedTime(0)
  ,isPaused(false)
{
	TimersManager::Instance().addTimer(this);
	reset();
}

GameTimer::~GameTimer()
{
	TimersManager::Instance().removeTimer(this);
}

void GameTimer::reset()
{
	isPaused = false;
	clock.restart();
	lastTimer = 0;
	pausedTime = 0;
}

void GameTimer::pause()
{
	isPaused = true;
	pausedTime = clock.getElapsedTime().asMilliseconds();
}

void GameTimer::unpause()
{
	lastTimer += clock.getElapsedTime().asMilliseconds() - pausedTime;
	isPaused = false;
}

bool GameTimer::check(float msec)
{
	if (isPaused)
		return false;
	const sf::Int64 currentTime = clock.getElapsedTime().asMilliseconds();

	if (currentTime - lastTimer >= msec)
	{
		reset();
		return true;
	}
	return false;
}

float GameTimer::getElapsedMilliseconds() const
{
	return clock.getElapsedTime().asMilliseconds() - lastTimer;
}

TimersManager &TimersManager::Instance()
{
	static TimersManager instance;
	return instance;
}

void TimersManager::addTimer(GameTimer *timer)
{
	timers.push_back(timer);
}

void TimersManager::removeTimer(GameTimer *timer)
{
	timers.erase(remove(timers.begin(), timers.end(), timer));
}

void TimersManager::setPaused(bool isPaused)
{
	for(GameTimer *timer : timers)
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
