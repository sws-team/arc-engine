#ifndef ARCTIMER_H
#define ARCTIMER_H

#include "stdheader.h"

struct ArcTimer
{
	ArcTimer();
	~ArcTimer();

	void setInterval(int interval);
	void setRepeat(bool repeat);
	void setCallback(const std::function<void()>& callback);

	void start();
	void stop();
	void restart();

	bool isActive() const;
	int elapsed() const;

protected:
	void pause();
	void unpause();
	void update();

private:
	friend class TimersManager;
	sf::Clock clock;
	sf::Int32 lastTimer = 0;
	sf::Int32 pausedTime = 0;
	bool paused = false;
	bool active = false;
	bool repeat = true;

	int interval = 0;
	std::function<void()> callback = nullptr;
};

class TimersManager
{
public:
	static TimersManager &Instance();

	void addTimer(ArcTimer *timer);
	void removeTimer(ArcTimer *timer);

	void update();

	void setPaused(bool paused);

private:
	TimersManager() = default;
	TimersManager(const TimersManager& root);
	TimersManager& operator=(const TimersManager&);

	std::vector<ArcTimer*> timers;
	bool m_paused = false;
};

#endif // ARCTIMER_H
