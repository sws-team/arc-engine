#include "mapeffects.h"
#include "settings.h"
#include "Engine/engine.h"
#include "camera.h"
#include "Game/gameobject.h"
#include "level.h"
#include "tower.h"
#include "gamecursor.h"
#include "globalvariables.h"
#include "Game/gamepanel.h"

const int Shake::MAX_SHAKE_COUNT = 9;
const int Shake::MAX_SHAKE_OFFSET = 10;
const int Shake::SHAKE_TIME = 50;

Shake::Shake()
{
	dangerRect.setSize(Vector2f(Settings::Instance().getResolution()));
	dangerRect.setFillColor(Color(255,0,0,96));
	isActive = false;
	dangerRect.setPosition(0,0);
}

void Shake::draw(RenderTarget * const target)
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

			Engine::Instance().camera()->getView()->setCenter(Engine::Instance().camera()->getView()->getCenter().x,
															  Engine::Instance().camera()->getView()->getCenter().y + offset);

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

vector<Tower *> MapEffect::getRandomTowers(int count, const vector<Tower *> &towers)
{
	const int actualCount = ceil(count * 0.2f);
	const int interval = actualCount * 2 + 1;
	const int offset = rand() % interval - actualCount;
	const int resultCount = count + offset;

	const unsigned int towersCount = min(static_cast<size_t>(resultCount), towers.size());
	set<int> numbers;
	vector<Tower *> resultTowers;
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

void MapExplosion::draw(RenderTarget * const target)
{
	if (m_state == ACTIVE || m_state == PREPARE_ACTIVE)
	{
		for(const CircleShape &shape : targets)
		{
			target->draw(shape);
		}
	}
}

void MapExplosion::update()
{
	float opacity = 255 * stepTimer.getElapsedMilliseconds()/m_interval;

	if (m_state == PREPARE_ACTIVE)
		opacity = min(opacity, 255.f);
	else if (m_state == ACTIVE)
	{
		opacity = 255 - opacity;
		opacity = max(opacity, 1.f);
	}
	else
		opacity = 0;

	for(CircleShape &shape : targets)
		shape.setFillColor(Color(255, 15, 15, opacity));

	MapEffect::update();
}

void MapExplosion::init()
{
	MapEffect::init();
}

void MapExplosion::stateChanged()
{
	switch (m_state)
	{
	case PREPARE_ACTIVE:
	{
		m_interval = m_duration;
		vector<Tower *> towers = getRandomTowers(m_count, Engine::Instance().level()->getAllTowers());
		for (unsigned int i = 0; i < towers.size(); ++i)
		{
			CircleShape circle;
			circle.setFillColor(Color(255, 15, 15, 0));
			circle.setRadius(GlobalVariables::Instance().mapTileSize().x/2);
			circle.setPosition(towers.at(i)->pos() +
							   Vector2f(GlobalVariables::Instance().mapTileSize().x/2,
										GlobalVariables::Instance().mapTileSize().y/2));
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
		m_interval = 1000;
		for(const CircleShape &shape : targets)
		{
			Engine::Instance().level()->addAnimation(RESOURCES::EXPLOSION,
						 Vector2f(shape.getGlobalBounds().left - GlobalVariables::Instance().mapTileSize().x/2,
								  shape.getGlobalBounds().top - GlobalVariables::Instance().mapTileSize().y/2),
						 Vector2i(64, 64), 80, 12, 0);
		}
	}
		break;
	case READY:
	{
		const vector<Tower *> towers = Engine::Instance().level()->getAllTowers();
		for(const CircleShape &shape : targets)
		{
			for(Tower* tower : towers)
			{
				if (shape.getGlobalBounds().contains(tower->getCenter()))
				{
					Engine::Instance().level()->deleteTower(tower);
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

void TowersRegress::draw(RenderTarget * const target)
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
		const vector<Tower *> towers = getRandomTowers(m_count, Engine::Instance().level()->getAllTowers());
		for (unsigned int i = 0; i < towers.size(); ++i)
		{
			towers.at(i)->setRegressed(true);
			objects.push_back(new GameObject(RESOURCES::REGRESS, towers.at(i)->pos(), Vector2i(64, 64), 4));
			if (objects.size() == m_count)
				break;
		}
		for(GameObject *object : objects)
		{
			Engine::Instance().level()->addAnimation(RESOURCES::REGRESS, object->pos(),
													 Vector2i(64, 64), 800, 4, 1);
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
			Engine::Instance().level()->addAnimation(RESOURCES::REGRESS, object->pos(),
													 Vector2i(64, 64), 800, 4, 2);
		}
	}
		break;
	case READY:
	{
		m_interval = m_time;
		for(Tower* tower : Engine::Instance().level()->getAllTowers())
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

void Smoke::draw(RenderTarget * const target)
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
		clouds.push_back(new GameObject(RESOURCES::SMOKE, Vector2f(0,0), Vector2i(256, 256), 4));

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
		const Vector2i maxCells = Engine::Instance().cursor()->getMaxCell();
		for(GameObject *cloud : clouds)
		{
			const int x = rand() % maxCells.x;
			const int y = rand() % maxCells.y;
			cloud->setPos(Vector2f(GlobalVariables::Instance().tileSize().x * x,
								   GlobalVariables::Instance().tileSize().y * y));

			Engine::Instance().level()->addAnimation(RESOURCES::SMOKE, cloud->pos(),
													 Vector2i(256, 256), 800, 4, 1);
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

			Engine::Instance().level()->addAnimation(RESOURCES::SMOKE, cloud->pos(),
													 Vector2i(256, 256), 800, 4, 2);
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
	const vector<Tower *> towers = Engine::Instance().level()->getAllTowers();
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

void MoneyDrain::draw(RenderTarget * const target)
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
			Engine::Instance().level()->drainMoney(1);
		drain->update();
	}
}

void MoneyDrain::explosion(const FloatRect &rect)
{
	for (auto it = drains.begin(); it != drains.end();)
	{
		GameObject *drain = *it;
		if (drain->sprite.getGlobalBounds().intersects(rect))
		{
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

		vector<Tower*> powerTowers;
		for(Tower* tower : Engine::Instance().level()->getAllTowers())
		{
			if (tower->type() == POWER)
				powerTowers.push_back(tower);
		}
		const vector<Tower *> towers = getRandomTowers(m_count, powerTowers);
		for (unsigned int i = 0; i < towers.size(); ++i)
		{
			if (towers.at(i)->type() == POWER)
				drains.push_back(new GameObject(RESOURCES::DRAIN, towers.at(i)->pos(), Vector2i(64, 64), 4));
			if (drains.size() == m_count)
				break;
		}
		for(GameObject *drain : drains)
		{
			Engine::Instance().level()->addAnimation(RESOURCES::DRAIN, drain->pos(),
													 Vector2i(64, 64), 800, 4, 1);
		}
	}
		break;
	case ACTIVE:
	{
		m_interval = m_duration;
		if (drains.size() != 0)
			Engine::Instance().panel()->setDrain(true);
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
		Engine::Instance().panel()->setDrain(false);
		for(GameObject *drain : drains)
			delete drain;
		drains.clear();
	}
		break;
	default:
		break;
	}
}
