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
#include "gameplatform.h"
#include "achievements.h"

GameAbility::GameAbility(const sf::Vector2i &areaSize,
						 const sf::Vector2i &offset,
						 const float time) :
	m_isActive(false)
  ,m_isReady(true)
  ,m_areaSize(areaSize)
  ,m_offset(offset)
  ,m_time(time * EngineDefs::MSEC)
  ,m_rotated(false)
{

}

bool GameAbility::isReady()
{
	return m_isReady;
}

void GameAbility::setUp()
{
	Engine::Instance().soundManager()->playOnce(GAME_SOUND::ACTIVATE_ABILITY);
	m_rotated = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
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
	acidAbility = new AcidAbility();
	freezeBombAbility = new FreezeBombAbility();
	increaseTowerAttackSpeedAbility = new IncreaseTowerAttackSpeedAbility();
	increaseTowerDamageAbility = new IncreaseTowerDamageAbility();
	stopAblity = new StopAbility();
}

void Abilities::draw(sf::RenderTarget * const target)
{
	if (acidAbility->isActive())
		acidAbility->object->draw(target);
}

void Abilities::update()
{
	if (acidAbility->isActive())
		acidAbility->object->update();

	acidAbility->checkDuration();
	increaseTowerAttackSpeedAbility->checkDuration();
	increaseTowerDamageAbility->checkDuration();
	stopAblity->checkDuration();

	bombAbility->checkReady();
	freezeBombAbility->checkReady();
	acidAbility->checkReady();
	increaseTowerAttackSpeedAbility->checkReady();
	increaseTowerDamageAbility->checkReady();
	stopAblity->checkReady();

	Engine::Instance().options<GameOptions>()->panel()->updateAbilitiesDuration();
}

void Abilities::reset()
{
	bombAbility->reset();
	freezeBombAbility->reset();
	acidAbility->reset();
	increaseTowerAttackSpeedAbility->reset();
	increaseTowerDamageAbility->reset();
	stopAblity->reset();
}

void Abilities::clear()
{
	acidAbility->clear();
}

BombAbility::BombAbility()
	: GameAbility(sf::Vector2i(3, 3), sf::Vector2i(2, 2), Balance::Instance().getBombCooldown())
{

}

void BombAbility::activate()
{
	Engine::Instance().soundManager()->playOnce(GAME_SOUND::BOMB);
	GameAbility::activate();
	const sf::FloatRect abilityRect = Engine::Instance().options<GameOptions>()->cursor()->getAbilityRect();
	Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::BOMB_EXPLOSION,
																	 sf::Vector2f(abilityRect.left, abilityRect.top),
																	 sf::Vector2i(m_areaSize.x * GameOptions::CELL_SIZE,
																				  m_areaSize.y * GameOptions::CELL_SIZE),
																	 25, 8, 0);

//	Engine::Instance().options<GameOptions>()->panel()->updatePanel();
	unsigned int count = 0;
	for(Enemy *enemy : Engine::Instance().options<GameOptions>()->level()->getAllEnemies())
		if (enemy->enemyRect().intersects(abilityRect))
		{
			enemy->hit(Balance::Instance().getBombDamage());
			if (!enemy->isAlive())
				count++;
			else
				enemy->protect(-0.1f);
		}

	if (count >= 5)
		GamePlatform::Instance().unlock(ACHIEVEMENT_KILL_5_BY_SINGLE_BOMB);

	Engine::Instance().options<GameOptions>()->level()->checkAlive();
	GameAbility::finish();
}

FreezeBombAbility::FreezeBombAbility()
	: GameAbility(sf::Vector2i(3, 3), sf::Vector2i(2, 2), Balance::Instance().getFreezeCooldown())
{

}

void FreezeBombAbility::activate()
{	
	Engine::Instance().soundManager()->playOnce(GAME_SOUND::FREEZE_BOMB);
	GameAbility::activate();
	const sf::FloatRect abilityRect = Engine::Instance().options<GameOptions>()->cursor()->getAbilityRect();
	Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::FREEZE_BOMB_EXPLOSION,
				 sf::Vector2f(abilityRect.left, abilityRect.top),
				 sf::Vector2i(m_areaSize.x * GameOptions::CELL_SIZE,
						  m_areaSize.y * GameOptions::CELL_SIZE),
				 25, 8, 0);
	Engine::Instance().options<GameOptions>()->panel()->updatePanel();
	unsigned int count = 0;
	for(Enemy *enemy : Engine::Instance().options<GameOptions>()->level()->getAllEnemies())
		if (enemy->enemyRect().intersects(abilityRect))
		{
			enemy->protect(-0.2f);
			enemy->freeze(Balance::Instance().getFreezeValue(), Balance::Instance().getFreezeDuration() * EngineDefs::MSEC);
			count++;
		}
	if (count >= 5)
		GamePlatform::Instance().unlock(ACHIEVEMENT_FREEZE_5_BY_SINGLE_BOMB);

	GameAbility::finish();
}

AcidAbility::AcidAbility()
	: GameAbility(sf::Vector2i(12, 3), sf::Vector2i(7, 2), Balance::Instance().getAcidCooldown())
	,object(nullptr)
	,count(0)
{

}

void AcidAbility::activate()
{
	Engine::Instance().soundManager()->playOnce(GAME_SOUND::ACID);
	const sf::Vector2i size = m_rotated ? sf::Vector2i(m_areaSize.y * GameOptions::CELL_SIZE,
											   m_areaSize.x * GameOptions::CELL_SIZE) :
									  sf::Vector2i(m_areaSize.x * GameOptions::CELL_SIZE,
											   m_areaSize.y * GameOptions::CELL_SIZE);

	object = new GameObject(GAME_TEXTURE::VENOM_EFFECT, sf::Vector2f(0, 0), size, 4);
	object->animationSpeed = 450;
//	if (m_rotated)
//		object->sprite.rotate(45);
	GameAbility::activate();
//	Engine::Instance().options<GameOptions>()->panel()->updatePanel();
	const sf::Vector2f pos = sf::Vector2f(Engine::Instance().options<GameOptions>()->cursor()->getAbilityRect().left,
								  Engine::Instance().options<GameOptions>()->cursor()->getAbilityRect().top);

	object->setPos(pos);
	abilityTimer.reset();
	count = 0;
}

void AcidAbility::checkDuration()
{
	if (!m_isActive)
		return;
	if (abilityTimer.check(Balance::Instance().getAcidAttackSpeed() * EngineDefs::MSEC))
	{
		count++;
		for(Enemy *enemy : Engine::Instance().options<GameOptions>()->level()->getAllEnemies())
		{
			if (enemy->sprite.getGlobalBounds().intersects(object->sprite.getGlobalBounds()))
			{
				enemy->hit(Balance::Instance().getAcidDamage());

				if(!enemy->isAlive())
					GamePlatform::Instance().incrementValue(STAT_KILLS_BY_ACID);
			}
		}
	}
	if (count >= Balance::Instance().getAcidCount())
		clear();
}

void AcidAbility::clear()
{
	finish();
	delete object;
	object = nullptr;
}

IncreaseTowerDamageAbility::IncreaseTowerDamageAbility()
	: GameAbility(sf::Vector2i(1, 1), sf::Vector2i(0, 0), Balance::Instance().getIncreaseDamageCooldown())
	,target(nullptr)
{

}

void IncreaseTowerDamageAbility::activate()
{
	Tower* tower = Engine::Instance().options<GameOptions>()->level()->getTowerAtPos(Engine::Instance().options<GameOptions>()->camera()->cellToPos(Engine::Instance().options<GameOptions>()->cursor()->cell()));
	if (tower == nullptr)
		return;
	Engine::Instance().soundManager()->playOnce(GAME_SOUND::TOWER_ABILITY);
	GameAbility::activate();
	target = tower;
	Engine::Instance().options<GameOptions>()->panel()->updatePanel();
	tower->increaseDamage(Balance::Instance().getIncreaseDamageValue());
	abilityTimer.reset();
}

void IncreaseTowerDamageAbility::checkDuration()
{
	if (target == nullptr)
		return;

	if (abilityTimer.check(Balance::Instance().getIncreaseDamageDuration() * EngineDefs::MSEC))
	{
		target->decreaseDamage(Balance::Instance().getIncreaseDamageValue());
		target = nullptr;
		finish();
	}
}

IncreaseTowerAttackSpeedAbility::IncreaseTowerAttackSpeedAbility()
	: GameAbility(sf::Vector2i(1, 1), sf::Vector2i(0, 0), Balance::Instance().getIncreaseAttackSpeedCooldown())
	,target(nullptr)
{

}

void IncreaseTowerAttackSpeedAbility::activate()
{
	Tower* tower = Engine::Instance().options<GameOptions>()->level()->getTowerAtPos(Engine::Instance().options<GameOptions>()->camera()->cellToPos(Engine::Instance().options<GameOptions>()->cursor()->cell()));
	if (tower == nullptr)
		return;
	Engine::Instance().soundManager()->playOnce(GAME_SOUND::TOWER_ABILITY);
	GameAbility::activate();
	target = tower;
	Engine::Instance().options<GameOptions>()->panel()->updatePanel();
	tower->increaseAttackSpeed(Balance::Instance().getIncreaseAttackSpeedValue());
	abilityTimer.reset();
}

void IncreaseTowerAttackSpeedAbility::checkDuration()
{
	if (target == nullptr)
		return;

	if (abilityTimer.check(Balance::Instance().getIncreaseAttackSpeedDuration() * EngineDefs::MSEC))
	{
		target->decreaseAttackSpeed(Balance::Instance().getIncreaseAttackSpeedValue());
		target = nullptr;
		finish();
	}
}

StopAbility::StopAbility()
	: GameAbility(sf::Vector2i(1, 1), sf::Vector2i(0, 0), Balance::Instance().getStopCooldown())
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
	if (abilityTimer.check(Balance::Instance().getStopDuration() * EngineDefs::MSEC))
		finish();
}
