#ifndef ARCTIMER_H
#define ARCTIMER_H

#include "stdheader.h"

struct ArcTimer
{
	ArcTimer();
	~ArcTimer();

	void restart();
	bool isTimeout(float msec);

	sf::Int32 elapsed() const;

protected:
	void pause();
	void unpause();

private:
	friend class TimersManager;
	sf::Clock clock;
	sf::Int32 lastTimer = 0;
	sf::Int32 pausedTime = 0;
	bool isPaused = false;
	int m_interval = 0;
	std::function<void()> m_callback = nullptr;
};

class TimersManager
{
public:
	static TimersManager &Instance();

	void addTimer(ArcTimer *timer);
	void removeTimer(ArcTimer *timer);

	void setPaused(bool paused);

private:
	TimersManager() = default;
	TimersManager(const TimersManager& root);
	TimersManager& operator=(const TimersManager&);

	std::vector<ArcTimer*> timers;
	bool m_paused = false;
};

#endif // ARCTIMER_H
