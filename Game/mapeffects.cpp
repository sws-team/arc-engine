#include "mapeffects.h"
#include "camera.h"
#include "level.h"
#include "tower.h"
#include "gamecursor.h"
#include "gamepanel.h"
#include "managers.h"
#include "engine.h"
#include "gameoptions.h"
#include "gamemanagers.h"
#include "gameplatform.h"
#include "achievements.h"

const int Shake::MAX_SHAKE_COUNT = 9;
const int Shake::MAX_SHAKE_OFFSET = 10;
const int Shake::SHAKE_TIME = 50;

Shake::Shake()
{
	dangerRect.setSize(sf::Vector2f(Engine::Instance().settingsManager()->getInscribedResolution()));
	dangerRect.setFillColor(sf::Color(255,0,0,96));
	isActive = false;
	dangerRect.setPosition(0,0);
}

void Shake::draw(sf::RenderTarget * const target)
{
	if (isActive && state)
		target->draw(dangerRect);
}

void Shake::update()
{
	if (isActive)
	{
		if (dangerTimer.check(Shake::SHAKE_TIME))
		{
			if (state)
				offset = rand() % Shake::MAX_SHAKE_OFFSET * 2;
			else
				offset = -offset;

			Engine::Instance().options<GameOptions>()->camera()->getView()->setCenter(Engine::Instance().options<GameOptions>()->camera()->getView()->getCenter().x,
															  Engine::Instance().options<GameOptions>()->camera()->getView()->getCenter().y + offset);

			state = !state;
			count++;
			if (count > Shake::MAX_SHAKE_COUNT)
				isActive = false;
		}
	}
}

void Shake::start()
{
	dangerTimer.reset();
	isActive = true;
	state = true;
	count = 0;
}

void Shake::deactivate()
{
	isActive = false;
}


MapEffect::MapEffect()
	: GameDrawable()
	,m_interval(1000)
	,m_time(1000)
	,m_duration(1000)
	,m_count(1)
	,m_enabled(false)
	,m_state(READY)
{
	stepTimer.reset();
}

void MapEffect::clear()
{
	m_enabled = false;
	m_state = READY;
}

void MapEffect::update()
{
	if (!m_enabled)
		return;

	if (stepTimer.check(m_interval))
		step();
}

void MapEffect::setTime(float time)
{
	m_time = time;
}

void MapEffect::setDuration(float duration)
{
	m_duration = duration;
}

void MapEffect::setCount(unsigned int count)
{
	m_count = count;
}

void MapEffect::setEnabled(bool enabled)
{
	m_enabled = enabled;
}

void MapEffect::init()
{

}

void MapEffect::resetTimers()
{
	stepTimer.reset();
}

void MapEffect::step()
{
	switch (m_state)
	{
	case READY:
		setState(PREPARE_ACTIVE);
		break;
	case PREPARE_ACTIVE:
		setState(ACTIVE);
		break;
	case ACTIVE:
		setState(AFTER_ACTIVE);
		break;
	case AFTER_ACTIVE:
		setState(READY);
		break;
	default:
		break;
	}
}

void MapEffect::setState(MapEffect::STATES state)
{
	m_state = state;
	stateChanged();
	stepTimer.reset();
}

std::vector<Tower *> MapEffect::getRandomTowers(int count, const std::vector<Tower *> &towers)
{
	const int actualCount = ceil(count * 0.2f);
	const int interval = actualCount * 2 + 1;
	const int offset = rand() % interval - actualCount;
	const int resultCount = count + offset;

	const unsigned int towersCount = std::min(static_cast<size_t>(resultCount), towers.size());
	std::set<int> numbers;
	std::vector<Tower *> resultTowers;
	for (unsigned int i = 0; i < towersCount; ++i)
	{
		const int n = rand() % towers.size();
		if (towers.at(n)->isInvulnerable())
			continue;
		if (numbers.count(n) == 0)
		{
			numbers.insert(n);
			resultTowers.push_back(towers.at(n));
		}
		else
			i--;
	}
	return resultTowers;
}

MapExplosion::MapExplosion()
	: MapEffect()
{

}

void MapExplosion::draw(sf::RenderTarget * const target)
{
	if (m_state == ACTIVE || m_state == PREPARE_ACTIVE)
	{
		for(const sf::CircleShape &shape : targets)
		{
			target->draw(shape);
		}
	}
}

void MapExplosion::update()
{
	float opacity = 255 * stepTimer.getElapsedMilliseconds()/m_interval;

	if (m_state == PREPARE_ACTIVE)
		opacity = std::min(opacity, 255.f);
	else if (m_state == ACTIVE)
	{
		opacity = 255 - opacity;
		opacity = std::max(opacity, 1.f);
	}
	else
		opacity = 0;

	for(sf::CircleShape &shape : targets)
		shape.setFillColor(sf::Color(255, 15, 15, opacity));

	MapEffect::update();
}

void MapExplosion::stateChanged()
{
	switch (m_state)
	{
	case PREPARE_ACTIVE:
	{
		Engine::Instance().soundManager()->playLooped(GAME_SOUND::TARGET_LOCK);
		m_interval = m_duration;
		std::vector<Tower *> towers = getRandomTowers(m_count, Engine::Instance().options<GameOptions>()->level()->getAllTowers());
		for (unsigned int i = 0; i < towers.size(); ++i)
		{
			sf::CircleShape circle;
			circle.setFillColor(sf::Color(255, 15, 15, 0));
			circle.setRadius(Engine::Instance().options<GameOptions>()->mapTileSize().x/2);
			circle.setPosition(towers.at(i)->pos() +
							   sf::Vector2f(Engine::Instance().options<GameOptions>()->mapTileSize().x/2,
										Engine::Instance().options<GameOptions>()->mapTileSize().y/2));
			targets.push_back(circle);
		}
	}
		break;
	case ACTIVE:
	{
		m_interval = m_duration;
	}
		break;
	case AFTER_ACTIVE:
	{
		Engine::Instance().soundManager()->stop(GAME_SOUND::TARGET_LOCK);
		m_interval = 1000;
		for(const sf::CircleShape &shape : targets)
		{
			Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::EXPLOSION,
						 sf::Vector2f(shape.getGlobalBounds().left - Engine::Instance().options<GameOptions>()->mapTileSize().x/2,
								  shape.getGlobalBounds().top - Engine::Instance().options<GameOptions>()->mapTileSize().y/2),
						 sf::Vector2i(64, 64), 80, 12, 0);
			Engine::Instance().soundManager()->playOnce(GAME_SOUND::TOWER_EXPLOSION);
		}
	}
		break;
	case READY:
	{
		const std::vector<Tower *> towers = Engine::Instance().options<GameOptions>()->level()->getAllTowers();
		for(const sf::CircleShape &shape : targets)
		{
			for(Tower* tower : towers)
			{
				if (shape.getGlobalBounds().contains(tower->getCenter()))
				{
					Engine::Instance().options<GameOptions>()->level()->deleteTower(tower);
					continue;
				}
			}
		}
		targets.clear();
		m_interval = m_time;
	}
		break;
	default:
		break;
	}
}

TowersRegress::TowersRegress()
	: MapEffect()
{

}

void TowersRegress::draw(sf::RenderTarget * const target)
{
	if (m_state == ACTIVE)
	{
		for(GameObject *drain : objects)
			drain->draw(target);
	}
}

void TowersRegress::update()
{
	MapEffect::update();
	if (m_state == ACTIVE)
	{
		for(GameObject *object : objects)
			object->update();
	}
}

void TowersRegress::stateChanged()
{
	switch (m_state)
	{
	case PREPARE_ACTIVE:
	{
		m_interval = 800 * 4;
		const std::vector<Tower *> towers = getRandomTowers(m_count, Engine::Instance().options<GameOptions>()->level()->getAllTowers());
		for (unsigned int i = 0; i < towers.size(); ++i)
		{
			towers.at(i)->setRegressed(true);
			objects.push_back(new GameObject(GAME_TEXTURE::REGRESS, towers.at(i)->pos(), sf::Vector2i(64, 64), 4));
			if (objects.size() == m_count)
				break;
		}
		for(GameObject *object : objects)
		{
			Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::REGRESS, object->pos(),
													 sf::Vector2i(64, 64), 800, 4, 1);
		}
	}
		break;
	case ACTIVE:
	{
		m_interval = m_duration;
	}
		break;
	case AFTER_ACTIVE:
	{
		m_interval = 800 * 4;
		for(GameObject *object : objects)
		{
			Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::REGRESS, object->pos(),
													 sf::Vector2i(64, 64), 800, 4, 2);
		}
	}
		break;
	case READY:
	{
		m_interval = m_time;
		for(Tower* tower : Engine::Instance().options<GameOptions>()->level()->getAllTowers())
			tower->setRegressed(false);
		for(GameObject *object : objects)
			delete object;
		objects.clear();
	}
		break;
	default:
		break;
	}
}

Smoke::Smoke()
	: MapEffect()
{
	m_interval = m_time;
}

Smoke::~Smoke()
{
	this->clear();
}

void Smoke::draw(sf::RenderTarget * const target)
{
	if (m_state == ACTIVE)
	{
		for(GameObject *cloud : clouds)
			cloud->draw(target);
	}
}

void Smoke::update()
{
	MapEffect::update();
	for(GameObject *cloud : clouds)
		cloud->update();
}

void Smoke::init()
{
	for (unsigned int i = 0; i < m_count; ++i)
		clouds.push_back(new GameObject(GAME_TEXTURE::SMOKE, sf::Vector2f(0,0), sf::Vector2i(256, 256), 4));

	MapEffect::init();
}

void Smoke::clear()
{
	for(GameObject *cloud : clouds)
		delete cloud;
	clouds.clear();
}

void Smoke::stateChanged()
{
	switch (m_state)
	{
	case PREPARE_ACTIVE:
	{
		m_interval = 800;
		const sf::Vector2i maxCells = Engine::Instance().options<GameOptions>()->cursor()->getMaxCell();
		for(GameObject *cloud : clouds)
		{
			const int x = rand() % maxCells.x;
			const int y = rand() % maxCells.y;
			cloud->setPos(sf::Vector2f(Engine::Instance().options<GameOptions>()->tileSize().x * x,
								   Engine::Instance().options<GameOptions>()->tileSize().y * y));

			Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::SMOKE, cloud->pos(),
													 sf::Vector2i(256, 256), 800, 4, 1);
		}
//		{
//			cloud->currentFrame = 0;
//			cloud->row = 1;
//		}
	}
		break;
	case ACTIVE:
	{
		m_interval = m_duration;
//		for(GameObject *cloud : clouds)
//		{
//			cloud->currentFrame = 0;
//			cloud->row = 0;
//		}
		smokeTowers(true);
	}
		break;
	case AFTER_ACTIVE:
	{
		m_interval = 800;
		for(GameObject *cloud : clouds)

			Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::SMOKE, cloud->pos(),
													 sf::Vector2i(256, 256), 800, 4, 2);
//		{
//			cloud->currentFrame = 0;
//			cloud->row = 2;
//		}
	}
		break;
	case READY:
		smokeTowers(false);
		m_interval = m_time;
		break;
	default:
		break;
	}
}

void Smoke::smokeTowers(bool on)
{
	const std::vector<Tower *> towers = Engine::Instance().options<GameOptions>()->level()->getAllTowers();
	for(Tower* tower : towers)
	{
		for(GameObject *cloud : clouds)
		{
			if(cloud->gameRect().contains(tower->getCenter()))
			{
				tower->setBlinded(on);
			}
		}
	}
}

MoneyDrain::MoneyDrain()
	: MapEffect()
{

}

MoneyDrain::~MoneyDrain()
{

}

void MoneyDrain::draw(sf::RenderTarget * const target)
{
	if (m_state == ACTIVE)
	{
		for(GameObject *drain : drains)
			drain->draw(target);
	}
}

void MoneyDrain::update()
{
	MapEffect::update();
	const bool hasDrain = m_state == ACTIVE && drainTimer.check(50);
	for(GameObject *drain : drains)
	{
		if (hasDrain)
			Engine::Instance().options<GameOptions>()->level()->drainMoney(Balance::Instance().getDrainValue());
		drain->update();
	}
}

void MoneyDrain::explosion(const sf::FloatRect &rect)
{
	for (auto it = drains.begin(); it != drains.end();)
	{
		GameObject *drain = *it;
		if (drain->sprite.getGlobalBounds().intersects(rect))
		{			
			GamePlatform::Instance().unlock(ACHIEVEMENT_DROP_DRAIN);
			delete drain;
			it = drains.erase(it);
		}
		else
			++it;
	}
	if (drains.size() == 0)
		setState(READY);
}

void MoneyDrain::stateChanged()
{
	switch (m_state)
	{
	case PREPARE_ACTIVE:
	{
		m_interval = 800 * 4;

		std::vector<Tower*> powerTowers;
		for(Tower* tower : Engine::Instance().options<GameOptions>()->level()->getAllTowers())
		{
			if (tower->type() == POWER)
				powerTowers.push_back(tower);
		}
		const std::vector<Tower *> towers = getRandomTowers(m_count, powerTowers);
		for (unsigned int i = 0; i < towers.size(); ++i)
		{
			if (towers.at(i)->type() == POWER)
				drains.push_back(new GameObject(GAME_TEXTURE::DRAIN, towers.at(i)->pos(), sf::Vector2i(64, 64), 4));
			if (drains.size() == m_count)
				break;
		}
		for(GameObject *drain : drains)
		{
			Engine::Instance().options<GameOptions>()->level()->addAnimation(GAME_TEXTURE::DRAIN, drain->pos(),
													 sf::Vector2i(64, 64), 800, 4, 1);
		}
	}
		break;
	case ACTIVE:
	{
		m_interval = m_duration;
		if (drains.size() != 0)
			Engine::Instance().options<GameOptions>()->panel()->setDrain(true);
	}
		break;
	case AFTER_ACTIVE:
	{
		setState(READY);
	}
		break;
	case READY:
	{
		m_interval = m_time;	
		Engine::Instance().options<GameOptions>()->panel()->setDrain(false);
		for(GameObject *drain : drains)
			delete drain;
		drains.clear();
	}
		break;
	default:
		break;
	}
}
