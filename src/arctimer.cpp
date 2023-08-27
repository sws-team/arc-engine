#include "arctimer.h"

ArcTimer::ArcTimer()
{
	TimersManager::Instance().addTimer(this);
}

ArcTimer::~ArcTimer()
{
	TimersManager::Instance().removeTimer(this);
}

void ArcTimer::setInterval(int interval)
{
	this->interval = interval;
}

void ArcTimer::setRepeat(bool repeat)
{
	this->repeat = repeat;
}

void ArcTimer::start()
{
	restart();
}

void ArcTimer::stop()
{
	active = false;
}

void ArcTimer::restart()
{
	active = true;
	paused = false;
	clock.restart();
	lastTimer = 0;
	pausedTime = 0;
}

bool ArcTimer::isActive() const
{
	return active;
}

void ArcTimer::pause()
{
	paused = true;
	pausedTime = clock.getElapsedTime().asMilliseconds();
}

void ArcTimer::unpause()
{
	lastTimer += clock.getElapsedTime().asMilliseconds() - pausedTime;
	paused = false;
}

void ArcTimer::update()
{
	if (!active)
		return;
	if (paused)
		return;
	if (elapsed() >= interval) {
		if (callback != nullptr)
			callback();
		if (repeat)
			restart();
		else {
			stop();
		}
	}
}

int ArcTimer::elapsed() const
{
	return static_cast<int>(clock.getElapsedTime().asMilliseconds() - lastTimer);
}

void ArcTimer::setCallback(const std::function<void ()> &callback)
{
	this->callback = callback;
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

void TimersManager::update()
{
	for(ArcTimer *timer : timers) {
		timer->update();
	}
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
