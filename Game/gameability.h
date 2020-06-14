#ifndef GAMEABILITY_H
#define GAMEABILITY_H

#include "stdheader.h"
#include "timer.h"
#include "gameobject.h"

class Tower;
class GameAbility
{
public:
	GameAbility(const sf::Vector2i &areaSize,
				const sf::Vector2i &offset,
				const float time);

	bool isReady();
	void setUp();
	virtual void activate();
	virtual void checkDuration();
	void reset();

	void checkReady();

	int timeLeft() const;
	bool isActive() const;

	void rotate();

protected:
	void finish();

	bool m_isActive;
	bool m_isReady;
	sf::Vector2i m_areaSize;
	sf::Vector2i m_offset;
	float m_time;
	bool m_rotated;
private:
	void initCursor();
	Timer cooldownTimer;
};

class BombAbility : public GameAbility
{
public:
	BombAbility();
	void activate() override;
};

class FreezeBombAbility : public GameAbility
{
public:
	FreezeBombAbility();
	void activate() override;
};

class AcidAbility : public GameAbility
{
public:
	AcidAbility();

	void activate() override;
	void checkDuration() override;

	class GameObject *object;
	void clear();
private:
	Timer abilityTimer;
	int count;
};

class IncreaseTowerAttackSpeedAbility : public GameAbility
{
public:
	IncreaseTowerAttackSpeedAbility();

	void activate() override;
	void checkDuration() override;

private:
	Timer abilityTimer;
	Tower *target;
};

class IncreaseTowerDamageAbility : public GameAbility
{
public:
	IncreaseTowerDamageAbility();

	void activate() override;
	void checkDuration() override;

private:
	Timer abilityTimer;
	Tower *target;
};

class StopAbility : public GameAbility
{
public:
	StopAbility();

	void activate() override;
	void checkDuration() override;

private:
	Timer abilityTimer;
};

class Abilities : public GameDrawable
{
public:
	Abilities();

	void draw(sf::RenderTarget *const target) override;
	void update() override;
	void reset();
	void clear();

	AcidAbility *acidAbility;
	BombAbility *bombAbility;
	FreezeBombAbility *freezeBombAbility;
	IncreaseTowerAttackSpeedAbility *increaseTowerAttackSpeedAbility;
	IncreaseTowerDamageAbility *increaseTowerDamageAbility;
	StopAbility *stopAblity;
};

#endif // GAMEABILITY_H
