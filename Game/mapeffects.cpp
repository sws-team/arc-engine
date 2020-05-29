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
	dangerRect.setSize(sf::Vector2f(Engine::Instance().settingsManager()->getResolution()));
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

void MapEffect::resetTimers()
{
	m_interval = m_time;
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
	std::vector<Tower*> avaliableTowers;
	for(Tower *tower : towers)
	{
		if (tower->isInvulnerable())
			continue;
		avaliableTowers.push_back(tower);
	}
	const unsigned int towersCount = std::min(static_cast<size_t>(count), avaliableTowers.size());
	while(avaliableTowers.size() != towersCount)
	{
		const int n = rand() % towers.size();
		avaliableTowers.erase(avaliableTowers.begin() + n);
	}
	return avaliableTowers;
}

std::vector<sf::Vector2f> MapEffect::getRandomPos(int count)
{
	std::vector<sf::Vector2f> result;
	const sf::Vector2i cells = Engine::Instance().options<GameOptions>()->cursor()->getMaxCell();
	for (int i = 0; i < count; ++i)
	{
		const int x = rand() % cells.x;
		const int y = rand() % cells.y;
		const sf::Vector2f pos = Engine::Instance().options<GameOptions>()->camera()->cellToPos(sf::Vector2i(x, y));
		result.push_back(pos);
	}
	return result;
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
			target->draw(shape);
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
		const std::vector<Tower *> towers = getRandomTowers(m_count,
															Engine::Instance().options<GameOptions>()->level()->getAllTowers());
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
		const std::vector<sf::Vector2f> points = getRandomPos(m_count);
		for(const sf::Vector2f& pos : points)
		{
			GameObject *cloud = new GameObject(GAME_TEXTURE::SMOKE, pos, sf::Vector2i(256, 256), 4);
			cloud->currentFrame = 0;
			cloud->row = 0;
			clouds.push_back(cloud);
		}

	}
		break;
	case ACTIVE:
	{
		m_interval = m_duration;
		for(GameObject *cloud : clouds)
		{
			cloud->currentFrame = 0;
			cloud->row = 1;
		}
		smokeTowers(true);
	}
		break;
	case AFTER_ACTIVE:
	{
		m_interval = 800;
		for(GameObject *cloud : clouds)
		{
			cloud->currentFrame = 0;
			cloud->row = 2;
		}
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
	for(GameObject *energyLeech : energyLeeches)
		energyLeech->draw(target);
}

void MoneyDrain::update()
{
	MapEffect::update();
	const bool hasDrain = m_state == ACTIVE && drainTimer.check(50);
	for(GameObject *energyLeech : energyLeeches)
	{
		if (hasDrain)
			Engine::Instance().options<GameOptions>()->level()->drainMoney(Balance::Instance().getDrainValue());
		energyLeech->update();
	}
}

void MoneyDrain::explosion(const sf::FloatRect &rect)
{
	for (auto it = energyLeeches.begin(); it != energyLeeches.end();)
	{
		GameObject *energyLeech = *it;
		if (energyLeech->sprite.getGlobalBounds().intersects(rect))
		{			
			GamePlatform::Instance().unlock(ACHIEVEMENT_DROP_DRAIN);
			delete energyLeech;
			it = energyLeeches.erase(it);
		}
		else
			++it;
	}
	if (energyLeeches.size() == 0)
		setState(READY);
}

void MoneyDrain::stateChanged()
{
	switch (m_state)
	{
	case PREPARE_ACTIVE:
	{
		std::vector<Tower*> powerTowers;
		for(Tower* tower : Engine::Instance().options<GameOptions>()->level()->getAllTowers())
		{
			if (tower->type() == POWER)
				powerTowers.push_back(tower);
		}
		const std::vector<Tower *> towers = getRandomTowers(m_count, powerTowers);
		for(Tower *tower : towers)
		{
			const sf::Vector2i frameSize = sf::Vector2i(384, 384);
			GameObject *energyLeech = new GameObject(GAME_TEXTURE::ENERGY_LEECH,
													 tower->pos() - Engine::Instance().options<GameOptions>()->mapTileSize(),
													 frameSize, ENERGY_LEECH_FRAME_COUNT);
			energyLeech->sprite.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);
			energyLeech->row = 0;
			energyLeech->animationSpeed = ENERGY_LEECH_ANIMATION_SPEED;
			energyLeeches.push_back(energyLeech);
		}
		m_interval = ENERGY_LEECH_FRAME_COUNT * ENERGY_LEECH_ANIMATION_SPEED;
	}
		break;
	case ACTIVE:
	{
		m_interval = m_duration;
		if (energyLeeches.size() != 0)
		{
			for(GameObject *energyLeech : energyLeeches)
			{
				energyLeech->currentFrame = 0;
				energyLeech->row = 1;
				energyLeech->updateTextureRect();
//				energyLeech->animationSpeed = ANIMATION_SPEED;
			}
			Engine::Instance().options<GameOptions>()->panel()->setDrain(true);
		}
	}
		break;
	case AFTER_ACTIVE:
	{
		for(GameObject *energyLeech : energyLeeches)
		{
			energyLeech->row = 2;
			energyLeech->currentFrame = 0;
			energyLeech->updateTextureRect();
//			energyLeech->animationSpeed = ANIMATION_SPEED;
		}
		m_interval = ENERGY_LEECH_FRAME_COUNT * ENERGY_LEECH_ANIMATION_SPEED;
	}
		break;
	case READY:
	{
		m_interval = m_time;	
		Engine::Instance().options<GameOptions>()->panel()->setDrain(false);
		for(GameObject *drain : energyLeeches)
			delete drain;
		energyLeeches.clear();
	}
		break;
	default:
		break;
	}
}

Lava::Lava()
	: MapEffect()
{

}

void Lava::draw(sf::RenderTarget * const target)
{
	for(GameObject* lava : lavas)
		lava->draw(target);
}

void Lava::update()
{
	for(GameObject* lava : lavas)
		lava->update();
	MapEffect::update();
}

void Lava::stateChanged()
{
	switch (m_state)
	{
	case PREPARE_ACTIVE:
	{
		const std::vector<sf::Vector2f> points = getRandomPos(m_count);
		for(const sf::Vector2f& pos : points)
		{
			GameObject *lava = new GameObject(GAME_TEXTURE::LAVA, pos,
											  sf::Vector2i(640, 640), LAVA_FRAME_COUNT);
			lava->animationSpeed = LAVA_ANIMATION_SPEED;
			lava->sprite.scale(0.5f, 0.5f);
			lavas.push_back(lava);
		}
		m_interval = LAVA_FRAME_COUNT * LAVA_ANIMATION_SPEED - 50;
	}
		break;
	case ACTIVE:
	{
		for(GameObject* lava : lavas)
		{
			lava->row = 1;
			lava->frameCount = LAVA_FRAME_COUNT - 1;
			lava->currentFrame = 0;
			lava->updateTextureRect();
		}
		m_interval = m_duration;
	}
		break;
	case AFTER_ACTIVE:
	{
		for(GameObject* lava : lavas)
		{
			lava->row = 2;
			lava->currentFrame = 0;
			lava->frameCount = LAVA_FRAME_COUNT;
			lava->updateTextureRect();
		}
		m_interval = (LAVA_FRAME_COUNT - 1) * LAVA_ANIMATION_SPEED;
	}
		break;
	case READY:
	{
		m_interval = m_time;
		for(GameObject* lava : lavas)
			delete lava;
		lavas.clear();
	}
		break;
	}
}
