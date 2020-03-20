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

protected:
	void finish();

	bool m_isActive;
	bool m_isReady;
	sf::Vector2i m_areaSize;
	sf::Vector2i m_offset;
	float m_time;
	bool m_rotated;
private:
	Timer cooldownTimer;
};

class BombAbility : public GameAbility
{
public:
	BombAbility();

	void activate() override;
	static constexpr float BOMB_ABILITY_DAMAGE = 200;
	static constexpr float BOMB_ABILITY_COOLDOWN = 15000;
};

class FreezeBombAbility : public GameAbility
{
public:
	FreezeBombAbility();

	void activate() override;

	static constexpr float FREEZE_ABILITY_K = 0.3f;
	static constexpr int FREEZE_ABILITY_DURATION = 5000;
	static constexpr int FREEZE_ABILITY_COOLDOWN = 10000;
};

class AcidAbility : public GameAbility
{
public:
	AcidAbility();

	void activate() override;
	void checkDuration() override;

	static constexpr float VENOM_DAMAGE = 15.f;
	static constexpr int VENOM_DAMAGE_COUNT = 15;
	static constexpr int VENOM_ATTACK_SPEED = 500;
	static constexpr int VENOM_ABLITY_COOLDOWN = 30000;

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

	static constexpr int INCREASE_ATTACK_SPEED_ABILITY_DURATION = 9000;
	static constexpr int INCREASE_ATTACK_SPEED_ABILITY_VALUE = 5;
	static constexpr int INCREASE_ATTACK_SPEED_ABILITY_COOLDOWN = 40000;
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

	static constexpr int INCREASE_DAMAGE_ABILITY_DURATION = 9000;
	static constexpr int INCREASE_DAMAGE_ABILITY_VALUE = 5;
	static constexpr int INCREASE_DAMAGE_ABILITY_COOLDOWN = 35000;

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
	static constexpr int STOP_ABILITY_DURATION = 5000;
	static constexpr int STOP_ABILITY_COOLDOWN = 60000;
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
