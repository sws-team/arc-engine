#include "gameability.h"
#include "Game/Level/level.h"
#include "Engine/engine.h"
#include "Game/Level/gamecursor.h"
#include "Game/Level/camera.h"
#include "Game/gamepanel.h"
#include "Game/Level/enemy.h"
#include "Game/Level/tower.h"
#include "globalvariables.h"
#include "Game/Audio/soundcontroller.h"

GameAbility::GameAbility(const Vector2i &areaSize,
						 const Vector2i &offset,
						 const float time) :
	m_isActive(false)
  ,m_isReady(true)
  ,m_areaSize(areaSize)
  ,m_offset(offset)
  ,m_time(time)
  ,m_rotated(false)
{

}

bool GameAbility::isReady()
{
	return m_isReady;
}

void GameAbility::setUp()
{
	m_rotated = Keyboard::isKeyPressed(Keyboard::LShift);
	if (m_rotated)
		Engine::Instance().cursor()->activateAbility(m_areaSize.y, m_areaSize.x, m_offset.y, m_offset.x);
	else
		Engine::Instance().cursor()->activateAbility(m_areaSize.x, m_areaSize.y, m_offset.x, m_offset.y);
	Engine::Instance().cursor()->swap();
}

void GameAbility::activate()
{
	m_isActive = true;
	m_isReady = false;
	cooldownTimer.reset();
}

void GameAbility::checkDuration()
{

}

void GameAbility::reset()
{
	m_isReady = true;
	cooldownTimer.reset();
}

void GameAbility::checkReady()
{
	if (cooldownTimer.check(m_time))
		m_isReady = true;
}

int GameAbility::timeLeft() const
{
	const float t = m_time/Engine::MSEC - cooldownTimer.getElapsedMilliseconds()/Engine::MSEC;
//	if (t < 0)
//		return 0;
	return static_cast<int>(ceil(t));
}

void GameAbility::finish()
{
	m_isActive = false;
}

bool GameAbility::isActive() const
{
	return m_isActive;
}

Abilities::Abilities()
	: GameDrawable()
{
	bombAbility = new BombAbility();
	venomAbility = new VenomAbility();
	freezeBombAbility = new FreezeBombAbility();
	increaseTowerAttackSpeedAbility = new IncreaseTowerAttackSpeedAbility();
	increaseTowerDamageAbility = new IncreaseTowerDamageAbility();
	stopAblity = new StopAbility();
}

void Abilities::draw(RenderTarget * const target)
{
	if (venomAbility->isActive())
		venomAbility->object->draw(target);
}

void Abilities::update()
{
	venomAbility->checkDuration();
	increaseTowerAttackSpeedAbility->checkDuration();
	increaseTowerDamageAbility->checkDuration();
	stopAblity->checkDuration();

	bombAbility->checkReady();
	freezeBombAbility->checkReady();
	venomAbility->checkReady();
	increaseTowerAttackSpeedAbility->checkReady();
	increaseTowerDamageAbility->checkReady();
	stopAblity->checkReady();

	Engine::Instance().panel()->updateAbilitiesDuration();
}

void Abilities::reset()
{
	bombAbility->reset();
	freezeBombAbility->reset();
	venomAbility->reset();
	increaseTowerAttackSpeedAbility->reset();
	increaseTowerDamageAbility->reset();
	stopAblity->reset();
}

BombAbility::BombAbility()
	: GameAbility(Vector2i(3, 3), Vector2i(2, 2), 15000)
{

}

void BombAbility::activate()
{
	SoundController::Instance().playOnce(BOMB_SOUND_FILE);
	GameAbility::activate();
	const FloatRect abilityRect = Engine::Instance().cursor()->getAbilityRect();
	Engine::Instance().level()->addAnimation(RESOURCES::BOMB_EXPLOSION,
				 Vector2f(abilityRect.left, abilityRect.top),
				 Vector2i(m_areaSize.x * GlobalVariables::CELL_SIZE,
						  m_areaSize.y * GlobalVariables::CELL_SIZE),
				 200, 12, 0);

//	Engine::Instance().panel()->updatePanel();

	for(Enemy *enemy : Engine::Instance().level()->getAllEnemies())
	{
		if (enemy->enemyRect().intersects(abilityRect))
			enemy->hit(BOMB_ABILITY_DAMAGE);
	}

	Engine::Instance().level()->checkAlive();
	GameAbility::finish();
}

FreezeBombAbility::FreezeBombAbility()
	: GameAbility(Vector2i(3, 3), Vector2i(2, 2), 10000)
{

}

void FreezeBombAbility::activate()
{
	GameAbility::activate();
	const FloatRect abilityRect = Engine::Instance().cursor()->getAbilityRect();
	Engine::Instance().level()->addAnimation(RESOURCES::FREEZE_BOMB_EXPLOSION,
				 Vector2f(abilityRect.left, abilityRect.top),
				 Vector2i(m_areaSize.x * GlobalVariables::CELL_SIZE,
						  m_areaSize.y * GlobalVariables::CELL_SIZE),
				 200, 6, 0);
	Engine::Instance().panel()->updatePanel();
	for(Enemy *enemy : Engine::Instance().level()->getAllEnemies())
		if (enemy->enemyRect().intersects(abilityRect))
			enemy->freeze(FREEZE_ABILITY_K, FREEZE_ABILITY_DURATION);
	GameAbility::finish();
}

VenomAbility::VenomAbility()
	: GameAbility(Vector2i(10, 3), Vector2i(7, 2), 30000)
	,object(nullptr)
	,count(0)
{

}

void VenomAbility::activate()
{
	const Vector2i size = m_rotated ? Vector2i(m_areaSize.y * GlobalVariables::CELL_SIZE,
											   m_areaSize.x * GlobalVariables::CELL_SIZE) :
									  Vector2i(m_areaSize.x * GlobalVariables::CELL_SIZE,
											   m_areaSize.y * GlobalVariables::CELL_SIZE);

	object = new GameObject(RESOURCES::VENOM_EFFECT, Vector2f(0, 0), size, 1);
	GameAbility::activate();
//	Engine::Instance().panel()->updatePanel();
	const Vector2f pos = Vector2f(Engine::Instance().cursor()->getAbilityRect().left,
								  Engine::Instance().cursor()->getAbilityRect().top);

	object->setPos(pos);
	abilityTimer.reset();
	count = 0;
}

void VenomAbility::checkDuration()
{
	if (!m_isActive)
		return;
	if (abilityTimer.check(VENOM_ATTACK_SPEED))
	{
		count++;
		for(Enemy *enemy : Engine::Instance().level()->getAllEnemies())
		{
			if (enemy->enemyRect().intersects(object->gameRect()))
				enemy->hit(VenomAbility::VENOM_DAMAGE);			
		}
	}
	if (count >= VENOM_DAMAGE_COUNT)
		finish();
}

IncreaseTowerDamageAbility::IncreaseTowerDamageAbility()
	: GameAbility(Vector2i(1, 1), Vector2i(0, 0), 35000)
	,target(nullptr)
{

}

void IncreaseTowerDamageAbility::activate()
{
	GameAbility::activate();
	Tower* tower = Engine::Instance().level()->getTowerAtPos(Engine::Instance().camera()->cellToPos(Engine::Instance().cursor()->cell()));
	if (tower == nullptr)
		return;
	target = tower;
	Engine::Instance().panel()->updatePanel();
	tower->increaseDamage(INCREASE_DAMAGE_ABILITY_VALUE);
	abilityTimer.reset();
}

void IncreaseTowerDamageAbility::checkDuration()
{
	if (target == nullptr)
		return;

	if (abilityTimer.check(INCREASE_DAMAGE_ABILITY_DURATION))
	{
		target->decreaseDamage(INCREASE_DAMAGE_ABILITY_VALUE);
		target = nullptr;
		finish();
	}
}

IncreaseTowerAttackSpeedAbility::IncreaseTowerAttackSpeedAbility()
	: GameAbility(Vector2i(1, 1), Vector2i(0, 0), 40000)
	,target(nullptr)
{

}

void IncreaseTowerAttackSpeedAbility::activate()
{
	GameAbility::activate();
	Tower* tower = Engine::Instance().level()->getTowerAtPos(Engine::Instance().camera()->cellToPos(Engine::Instance().cursor()->cell()));
	if (tower == nullptr)
		return;
	target = tower;
	Engine::Instance().panel()->updatePanel();
	tower->increaseAttackSpeed(INCREASE_ATTACK_SPEED_ABILITY_VALUE);
	abilityTimer.reset();
}

void IncreaseTowerAttackSpeedAbility::checkDuration()
{
	if (target == nullptr)
		return;

	if (abilityTimer.check(INCREASE_ATTACK_SPEED_ABILITY_DURATION))
	{
		target->decreaseAttackSpeed(INCREASE_ATTACK_SPEED_ABILITY_VALUE);
		target = nullptr;
		finish();
	}
}

StopAbility::StopAbility()
	: GameAbility(Vector2i(1, 1), Vector2i(0, 0), 60000)
{

}

void StopAbility::activate()
{
	SoundController::Instance().playOnce(STOP_SOUND_FILE);
	GameAbility::activate();
	abilityTimer.reset();
}

void StopAbility::checkDuration()
{
	if (abilityTimer.check(STOP_ABILITY_DURATION))
		finish();
}
