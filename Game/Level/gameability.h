#ifndef GAMEABILITY_H
#define GAMEABILITY_H

#include "stdheader.h"
#include "timer.h"
#include "Game/gamedrawable.h"

class Tower;
class GameAbility
{
public:
	GameAbility(const Vector2i &areaSize,
				const Vector2i &offset,
				const float time);

	bool isReady();
	void setUp();
	virtual void activate();
	virtual void checkDuration();

	void checkReady();

	int timeLeft() const;
	bool isActive() const;

protected:
	void finish();

	bool m_isActive;
	bool m_isReady;
	Vector2i m_areaSize;
	Vector2i m_offset;
	float m_time;
private:
	Timer cooldownTimer;
};

class BombAbility : public GameAbility
{
public:
	BombAbility();

	void activate() override;
	static constexpr float BOMB_ABILITY_DAMAGE = 111;
};

class FreezeBombAbility : public GameAbility
{
public:
	FreezeBombAbility();

	void activate() override;

	static constexpr float FREEZE_ABILITY_K = 35.f;
	static constexpr int FREEZE_ABILITY_DURATION = 5000;

};

class VenomAbility : public GameAbility
{
public:
	VenomAbility();

	void activate() override;
	void checkDuration() override;

	static constexpr float VENOM_DAMAGE = 10.f;
	static constexpr int VENOM_DAMAGE_COUNT = 15;
	static constexpr int VENOM_ATTACK_SPEED = 500;

	class GameObject *object;
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
	static constexpr int INCREASE_ATTACK_SPEED_ABILITY_VALUE = 100;
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
	static constexpr int INCREASE_DAMAGE_ABILITY_VALUE = 100;

private:
	Timer abilityTimer;
	Tower *target;
};

class UnknownAbility : public GameAbility
{
public:
	UnknownAbility();

	void activate() override;
};

class Abilities : public GameDrawable
{
public:
	Abilities();

	void draw(RenderTarget *const target) override;
	void update() override;

	VenomAbility *venomAbility;
	BombAbility *bombAbility;
	FreezeBombAbility *freezeBombAbility;
	IncreaseTowerAttackSpeedAbility *increaseTowerAttackSpeedAbility;
	IncreaseTowerDamageAbility *increaseTowerDamageAbility;
	UnknownAbility *unknownAblity;
};

#endif // GAMEABILITY_H
