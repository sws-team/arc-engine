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

	float getElapsedMilliseconds() const;

private:
	Clock clock;
	Int64 lastTimer;
	Int64 pausedTime;
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

	vector<Timer*> timers;
};

#endif // TIMER_H
