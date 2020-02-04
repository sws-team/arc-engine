#ifndef MAPEFFECT_H
#define MAPEFFECT_H

#include "stdheader.h"
#include "timer.h"
#include "Game/gamedrawable.h"

class GameObject;
class Tower;

class Shake : public GameDrawable
{
public:
	Shake();
	void draw(RenderTarget *const target) override;
	void update() override;

	void start();
	void deactivate();

private:
	RectangleShape dangerRect;
	Timer dangerTimer;
	bool isActive;
	bool state;
	int count;
	float offset;

	static const int MAX_SHAKE_COUNT;
	static const int MAX_SHAKE_OFFSET;
	static const int SHAKE_TIME;
};


class MapEffect : public GameDrawable
{
public:
	MapEffect();
	void update() override;

	void setTime(float time);
	void setDuration(float duration);
	void setCount(unsigned int count);
	void setEnabled(bool enabled);

	virtual void init();

	void resetTimers();

protected:
	virtual void stateChanged() = 0;

	Timer stepTimer;
	float m_interval;
	float m_time;
	float m_duration;
	unsigned int m_count;
	bool m_enabled;

	enum STATES
	{
		READY,
		PREPARE_ACTIVE,
		ACTIVE,
		AFTER_ACTIVE,
	};
	STATES m_state;
	void setState(STATES state);

	static vector<Tower *> getRandomTowers(int count, const vector<Tower*>& towers);

private:
	void step();
};

class MapExplosion : public MapEffect
{
	//destroy tower
public:
	MapExplosion();

	void draw(RenderTarget *const target) override;
	void update() override;

	void init() override;
protected:
	void stateChanged() override;

private:
	vector<CircleShape> targets;
};

class TowersRegress : public MapEffect
{
	//down atk sp or dmg
public:
	TowersRegress();
	constexpr static float REGRESS_VALUE = 0.3f;

	void draw(RenderTarget *const target) override;
	void update() override;
protected:
	void stateChanged() override;
private:
	vector<GameObject *> objects;
};

class Smoke : public MapEffect
{
	//down radius
public:
	Smoke();
	~Smoke();

	void draw(RenderTarget *const target) override;
	void update() override;

	void init() override;
protected:
	void stateChanged() override;
private:
	vector<GameObject *> clouds;
	void smokeTowers(bool on);
};

class MoneyDrain : public MapEffect
{
public:
	MoneyDrain();
	~MoneyDrain();

	void draw(RenderTarget *const target) override;
	void update() override;

	void explosion(const FloatRect& rect);

protected:
	void stateChanged() override;

private:
	vector<GameObject *> drains;
	Timer drainTimer;
};

#endif // MAPEFFECT_H
