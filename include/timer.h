#ifndef TIMER_H
#define TIMER_H

#include "stdheader.h"

struct Timer
{
	Timer();
	~Timer();

	void reset();
	void pause();
	void unpause();

	bool check(float msec);

	sf::Int32 getElapsedMilliseconds() const;

private:
	sf::Clock clock;
	sf::Int32 lastTimer;
	sf::Int32 pausedTime;
	bool isPaused;
};

class TimersManager
{
public:
	static TimersManager &Instance();

	void addTimer(Timer *timer);
	void removeTimer(Timer *timer);

	void setPaused(bool isPaused);

private:
	TimersManager();
	TimersManager(const TimersManager& root);
	TimersManager& operator=(const TimersManager&);

	std::vector<Timer*> timers;
};

#endif // TIMER_H
