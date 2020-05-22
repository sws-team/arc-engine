#ifndef MAPEFFECT_H
#define MAPEFFECT_H

#include "stdheader.h"
#include "timer.h"
#include "gameobject.h"

class Tower;

class Shake : public GameDrawable
{
public:
	Shake();
	void draw(sf::RenderTarget *const target) override;
	void update() override;

	void start();
	void deactivate();

private:
	sf::RectangleShape dangerRect;
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

	virtual void clear();

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

	static std::vector<Tower *> getRandomTowers(int count, const std::vector<Tower*>& towers);

private:
	void step();
};

class MapExplosion : public MapEffect
{
	//destroy tower
public:
	MapExplosion();

	void draw(sf::RenderTarget *const target) override;
	void update() override;
protected:
	void stateChanged() override;

private:
	std::vector<sf::CircleShape> targets;
};

class TowersRegress : public MapEffect
{
	//down atk sp or dmg
public:
	TowersRegress();

	void draw(sf::RenderTarget *const target) override;
	void update() override;
protected:
	void stateChanged() override;
private:
	std::vector<GameObject *> objects;
};

class Smoke : public MapEffect
{
	//down radius
public:
	Smoke();
	~Smoke();

	void draw(sf::RenderTarget *const target) override;
	void update() override;

	void init() override;
	void clear() override;
protected:
	void stateChanged() override;
private:
	std::vector<GameObject *> clouds;
	void smokeTowers(bool on);
};

class MoneyDrain : public MapEffect
{
public:
	MoneyDrain();
	~MoneyDrain();

	void draw(sf::RenderTarget *const target) override;
	void update() override;

	void explosion(const sf::FloatRect& rect);

protected:
	void stateChanged() override;

private:
	std::vector<GameObject *> energyLeeches;
	Timer drainTimer;
	static constexpr int ENERGY_LEECH_FRAME_COUNT = 9;
	static constexpr float ENERGY_LEECH_ANIMATION_SPEED = 100;
};

class Lava : public MapEffect
{
public:
	Lava();
	void draw(sf::RenderTarget *const target) override;
	void update() override;

protected:
	void stateChanged() override;

private:
	std::vector<GameObject*> lavas;
	static constexpr int LAVA_FRAME_COUNT = 4;
	static constexpr int LAVA_ANIMATION_SPEED = 200;
};

#endif // MAPEFFECT_H
