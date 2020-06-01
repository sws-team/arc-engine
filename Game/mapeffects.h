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
	virtual ~MapEffect();

	virtual void clear();

	void update() override;

	void setTime(float time);
	void setDuration(float duration);
	void setCount(unsigned int count);
	void setEnabled(bool enabled);

	void resetTimers();

	bool isEnabled() const;

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
	static std::vector<sf::Vector2f> getRandomPos(int count);
	static std::vector<sf::Vector2f> getRandomEnemyPos(int count);

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
	void clear() override;

	static constexpr int REGRESS_FRAME_COUNT = 9;
	static constexpr int REGRESS_ANIMATION_SPEED = 100;
	static constexpr int REGRESS_SIZE = 64;
	static constexpr float REGRESS_SCALE = 1.f;
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

	void draw(sf::RenderTarget *const target) override;
	void update() override;
	void clear() override;

	static constexpr int SMOKE_ANIMATION_SPEED = 150;
	static constexpr int SMOKE_FRAME_COUNT = 24;
	static constexpr int SMOKE_ACTIVE_FRAME_COUNT = 2;
	static constexpr int SMOKE_SIZE = 512;
	static constexpr float SMOKE_SCALE = 0.5f;
protected:
	void stateChanged() override;
private:
	std::vector<GameObject *> clouds;
	void checkBlinded();
};

class MoneyDrain : public MapEffect
{
public:
	MoneyDrain();

	void draw(sf::RenderTarget *const target) override;
	void update() override;
	void clear() override;

	void explosion(const sf::FloatRect& rect);

	static constexpr int ENERGY_LEECH_FRAME_COUNT = 9;
	static constexpr int ENERGY_LEECH_ANIMATION_SPEED = 100;
	static constexpr int ENERGY_LEECH_SIZE = 384;
protected:
	void stateChanged() override;

private:
	std::vector<GameObject *> energyLeeches;
	Timer drainTimer;
};

class Lava : public MapEffect
{
public:
	Lava();

	void draw(sf::RenderTarget *const target) override;
	void update() override;
	void clear() override;

	bool isIntersects(const sf::Vector2f &pos) const;

	static constexpr int LAVA_FRAME_COUNT = 5;
	static constexpr int LAVA_ANIMATION_SPEED = 350;
	static constexpr int LAVA_SIZE = 640;
	static constexpr float LAVA_SCALE = 0.5f;
protected:
	void stateChanged() override;

private:
	std::vector<GameObject*> lavas;
};

class InvilibilityEffect : public MapEffect
{
public:
	InvilibilityEffect();

	void draw(sf::RenderTarget *const target) override;
	void update() override;
	void clear() override;

	static constexpr int INVISIBILITY_SIZE = 640;
	static constexpr int INVISIBILITY_FRAME_COUNT = 5;
	static constexpr float INVISIBILITY_SCALE = 0.5f;
	static constexpr int INVISIBILITY_ANIMATION_SPEED = 350;
protected:
	void stateChanged() override;

private:
	std::vector<GameObject*> areas;
	void checkVisibility();
};

#endif // MAPEFFECT_H
