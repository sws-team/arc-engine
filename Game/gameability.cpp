#include "gameability.h"
#include "level.h"
#include "gamecursor.h"
#include "camera.h"
#include "gamepanel.h"
#include "enemy.h"
#include "tower.h"
#include "engine.h"
#include "enginedef.h"
#include "gameoptions.h"
#include "gamemanagers.h"

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
		Engine::Instance().options<GameOptions>()->cursor()->activateAbility(m_areaSize.y, m_areaSize.x, m_offset.y, m_offset.x);
	else
		Engine::Instance().options<GameOptions>()->cursor()->activateAbility(m_areaSize.x, m_areaSize.y, m_offset.x, m_offset.y);
	Engine::Instance().options<GameOptions>()->cursor()->swap();
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
	const float t = m_time/EngineDefs::MSEC - cooldownTimer.getElapsedMilliseconds()/EngineDefs::MSEC;
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

	Engine::Instance().options<GameOptions>()->panel()->updateAbilitiesDuration();
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
	Engine::Instance().soundManager()->playOnce(GAME_SOUND::BOMB);
	GameAbility::activate();
	const FloatRect abilityRect = Engine::Instance().options<GameOptions>()->cursor()->getAbilityRect();
	Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::BOMB_EXPLOSION,
				 Vector2f(abilityRect.left, abilityRect.top),
				 Vector2i(m_areaSize.x * GameOptions::CELL_SIZE,
						  m_areaSize.y * GameOptions::CELL_SIZE),
				 200, 12, 0);

//	Engine::Instance().options<GameOptions>()->panel()->updatePanel();

	for(Enemy *enemy : Engine::Instance().options<GameOptions>()->level()->getAllEnemies())
	{
		if (enemy->enemyRect().intersects(abilityRect))
			enemy->hit(BOMB_ABILITY_DAMAGE);
	}

	Engine::Instance().options<GameOptions>()->level()->checkAlive();
	GameAbility::finish();
}

FreezeBombAbility::FreezeBombAbility()
	: GameAbility(Vector2i(3, 3), Vector2i(2, 2), 10000)
{

}

void FreezeBombAbility::activate()
{	
	GameAbility::activate();
	const FloatRect abilityRect = Engine::Instance().options<GameOptions>()->cursor()->getAbilityRect();
	Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::FREEZE_BOMB_EXPLOSION,
				 Vector2f(abilityRect.left, abilityRect.top),
				 Vector2i(m_areaSize.x * GameOptions::CELL_SIZE,
						  m_areaSize.y * GameOptions::CELL_SIZE),
				 200, 6, 0);
	Engine::Instance().options<GameOptions>()->panel()->updatePanel();
	for(Enemy *enemy : Engine::Instance().options<GameOptions>()->level()->getAllEnemies())
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
	const Vector2i size = m_rotated ? Vector2i(m_areaSize.y * GameOptions::CELL_SIZE,
											   m_areaSize.x * GameOptions::CELL_SIZE) :
									  Vector2i(m_areaSize.x * GameOptions::CELL_SIZE,
											   m_areaSize.y * GameOptions::CELL_SIZE);

	object = new GameObject(GAME_TEXTURE::VENOM_EFFECT, Vector2f(0, 0), size, 1);
	GameAbility::activate();
//	Engine::Instance().options<GameOptions>()->panel()->updatePanel();
	const Vector2f pos = Vector2f(Engine::Instance().options<GameOptions>()->cursor()->getAbilityRect().left,
								  Engine::Instance().options<GameOptions>()->cursor()->getAbilityRect().top);

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
		for(Enemy *enemy : Engine::Instance().options<GameOptions>()->level()->getAllEnemies())
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
	Tower* tower = Engine::Instance().options<GameOptions>()->level()->getTowerAtPos(Engine::Instance().options<GameOptions>()->camera()->cellToPos(Engine::Instance().options<GameOptions>()->cursor()->cell()));
	if (tower == nullptr)
		return;
	target = tower;
	Engine::Instance().options<GameOptions>()->panel()->updatePanel();
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
	Tower* tower = Engine::Instance().options<GameOptions>()->level()->getTowerAtPos(Engine::Instance().options<GameOptions>()->camera()->cellToPos(Engine::Instance().options<GameOptions>()->cursor()->cell()));
	if (tower == nullptr)
		return;
	target = tower;
	Engine::Instance().options<GameOptions>()->panel()->updatePanel();
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
	Engine::Instance().soundManager()->playOnce(GAME_SOUND::STOP);
	GameAbility::activate();
	abilityTimer.reset();
}

void StopAbility::checkDuration()
{
	if (abilityTimer.check(STOP_ABILITY_DURATION))
		finish();
}
