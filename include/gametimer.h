#ifndef GAMETIMER_H
#define GAMETIMER_H

#include "stdheader.h"

struct GameTimer
{
	GameTimer();
	~GameTimer();

	void reset();
	void pause();
	void unpause();

	bool check(float msec);

	float getElapsedMilliseconds() const;

private:
	sf::Clock clock;
	sf::Int64 lastTimer;
	sf::Int64 pausedTime;
	bool isPaused;
};

class TimersManager
{
public:
	static TimersManager &Instance();

	void addTimer(GameTimer *timer);
	void removeTimer(GameTimer *timer);

	void setPaused(bool isPaused);

private:
	TimersManager();
	TimersManager(const TimersManager& root);
	TimersManager& operator=(const TimersManager&);

	std::vector<GameTimer*> timers;
};

#endif // GAMETIMER_H
