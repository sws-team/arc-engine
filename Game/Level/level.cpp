#include "level.h"
#include "Game/gameobject.h"
#include "settings.h"
#include "Game/effects.h"
#include "Game/gamepanel.h"
#include "Game/Audio/soundcontroller.h"
#include "globalvariables.h"
#include "ResourcesManager/resourcesmanager.h"
#include "Game/Level/camera.h"
#include "Game/Level/map.h"
#include "Engine/engine.h"
#include "cursor.h"
#include "enemy.h"
#include "tower.h"
#include "projectile.h"

constexpr static int MAX_SHAKE_OFFSET = 5;
constexpr static int SHAKE_DURATION = 500;

Level::Level() :
	m_finished(false)
  ,m_isFailed(false)
  ,difficulty(1.f)
  ,gameMap(nullptr)
  ,m_state(READY)
  ,life(0.f)
{
	const float k = static_cast<float>(Settings::Instance().getInscribedResolution().y)/Settings::Instance().getResolution().y;
	resolutionOffsetX = Settings::Instance().getResolution().x - k * Settings::Instance().getResolution().x;
	resolutionOffsetX /= 2;
}

Level::~Level()
{

}

void Level::draw(RenderTarget *const target)
{
	target->draw(m_background);
	target->setView(*Engine::Instance().camera()->getView());
	drawLevel(target);
	target->draw(deadZone);

	Engine::Instance().panel()->draw(target);
}

void Level::update()
{
	for(Tower* tower : towers)
	{
		if (tower->type() == POWER)
		{
			if (static_cast<PowerTower*>(tower)->hasEnergy())
				energy += 10;
		}
		else
			tower->action(enemies);
		tower->update();
	}
	if (timer.check(GlobalVariables::FRAME_TIME))
	{
		checkRespawn();
		calculateCollisions();
		checkDeadZone();
		checkEnd();

		for(Enemy* enemy : enemies)
		{
			if (enemy->moveStep())
			{
				if (gameMap->endRect.contains(enemy->enemyPos()))
					continue;

				const Vector2i cell = Engine::Instance().camera()->posToCell(enemy->enemyPos());
				const int direction = getTileDirectionByCell(cell);
				enemy->moveNext(direction);
			}
		}
	}

	for(Enemy* enemy : enemies)
		enemy->update();

//	Effects::Instance().update();

	Engine::Instance().panel()->update();
}

Vector2f Level::getCenter() const
{
//	return view.getCenter();
}

Vector2f Level::getStartingPos() const
{
	return m_startPos;
}

void Level::startMission(const unsigned int n)
{
	life = 100.f;
	energy = 100;
	SoundController::Instance().startBackgroundSound("sounds/map1.ogg");
//	difficulty = 1.f + static_cast<float>(SavedGameLoader::Instance().getSavedGame().completedLevels.size()) / 10;

	gameMap = Engine::Instance().getMap(n);

	testTexture.loadFromFile("tiles.png", IntRect(0,0,32,32));
	testTexture.setRepeated(true);

	constexpr float deadZoneSize = 300;
	const Vector2f minPos = Vector2f(-deadZoneSize * Settings::Instance().getScaleFactor().x,
									 -deadZoneSize * Settings::Instance().getScaleFactor().y);

	deadZone.setPosition(minPos.x, minPos.y);
	deadZone.setOutlineThickness(3.f);
	deadZone.setSize(Vector2f(Settings::Instance().getResolution().x + fabs(minPos.x) * 2,
							  Settings::Instance().getResolution().y + fabs(minPos.y) * 2));
	deadZone.setOutlineColor(Color::Red);
	deadZone.setFillColor(Color::Transparent);
	m_background.setTexture(testTexture);
	m_background.setPosition(minPos.x, minPos.y);
	m_background.setTextureRect(IntRect(minPos.x, minPos.y, deadZone.getSize().x, deadZone.getSize().y));
}

void Level::calculateCollisions()
{
	for(Tower *tower : towers)
		tower->collide(enemies);
	checkAlive();
}

void Level::checkDeadZone()
{
	for(Tower *tower : towers)
	{
		const TOWER_TYPES type = tower->type();
		if (type == BASE ||
				type == IMPROVED ||
				type == FREEZE ||
				type == ROCKET)
		{
			ProjectilesTower *pTower = static_cast<ProjectilesTower*>(tower);
			for(Projectile *projectile : pTower->projectiles())
			{
				const Vector2f pos = projectile->pos();
				if (pos.x > deadZone.getGlobalBounds().left + deadZone.getGlobalBounds().width ||
						pos.x < deadZone.getGlobalBounds().left ||
						pos.y > deadZone.getGlobalBounds().top + deadZone.getGlobalBounds().height ||
						pos.y < deadZone.getGlobalBounds().top)
				{
					pTower->removeProjectile(projectile);
				}
			}
		}
	}
}

void Level::checkEnd()
{
	for (auto it = enemies.begin(); it != enemies.end();)
	{
		Enemy *enemy = *it;
		if (gameMap->endRect.contains(enemy->enemyPos()))
		{
			hitPlayer(enemy->getData().damage);
			delete enemy;
			it = enemies.erase(it);
		}
		else
			++it;
	}
}

void Level::checkAlive()
{
	for (auto it = enemies.begin(); it != enemies.end();)
	{
		Enemy *enemy = *it;
		if (!enemy->isAlive())
		{
			delete enemy;
			it = enemies.erase(it);
		}
		else
			++it;
	}
}

void Level::checkRespawn()
{
//	spawn();
}

Tower *Level::getTowerAtPos(const Vector2f &pos) const
{
	for(Tower* tower : towers)
	{
		if (tower->pos() == pos)
			return tower;
	}
	return nullptr;
}

bool Level::canAddTower(const Vector2i &cell, TOWER_TYPES towerType) const
{
	bool canCreate = true;
	const int direction = Engine::Instance().level()->getTileDirectionByCell(cell);
	canCreate = canCreate && direction == 0;
	Tower *tower = this->getTowerAtPos(Engine::Instance().camera()->cellToPos(cell));
	canCreate = canCreate && tower == nullptr;
	if (towerType != POWER)
	{
		bool finded = false;
		for(Tower *tower : towers)
		{
			if (tower->type() == POWER)
			{				
				const int radius = tower->data().radius;
				const Vector2i towerCell = Engine::Instance().camera()->posToCell(tower->pos());
				if (abs(towerCell.x - cell.x) < radius &&
						abs(towerCell.y - cell.y) < radius)
				{
					finded = true;
					break;
				}
			}
		}
		canCreate = canCreate && finded;
	}
	return canCreate;
}

void Level::highlightPowerTowersRadius(bool active)
{
	for(Tower *tower : towers)
		if (tower->type() == POWER)
			static_cast<PowerTower*>(tower)->setHighlighted(active);
}

void Level::hitPlayer(float damage)
{
	life -= damage;
	if (life <= 0.f)
	{
		//game over
		m_isFailed = true;
		m_finished = true;
	}
}

//void Level::fillLevel()
//{
//	m_startPos.x = static_cast<float>(Settings::Instance().getResolution().x)/2;
//	m_startPos.y = static_cast<float>(Settings::Instance().getResolution().y)/2;

//	m_topBorder = view.getCenter().y - view.getSize().y/2;
//	m_bottomBorder = view.getCenter().y + view.getSize().y/2;

//	panel->setLeftBorder(leftBorder());

	//	SoundController::Instance().startBackgroundSound(mission.backgroundSound);
//}

void Level::drawLevel(RenderTarget * const target)
{
	for (size_t layer = 0; layer < gameMap->layers.size(); layer++)
		for (size_t tile = 0; tile < gameMap->layers[layer].tiles.size(); tile++)
			target->draw(gameMap->layers[layer].tiles[tile].sprite);

	for(Enemy* enemy : enemies)
		enemy->draw(target);

	for(Tower *tower : towers)
		tower->draw(target);

	Engine::Instance().cursor()->draw(target);
	//	Effects::Instance().draw(target);
}

void Level::spawn(ENEMY_TYPES type)
{
	Enemy *enemy = EnemiesFactory::createEnemy(type, Engine::Instance().camera()->cellToPos(gameMap->spawnPos));
	enemy->moveNext(gameMap->spawnDirection);
	enemies.push_back(enemy);
}

void Level::test()
{	
	for(Enemy* enemy : enemies)
		enemy->moveStep();
}

void Level::left()
{
	for(Enemy* enemy : enemies)
		enemy->moveNext(Map::LEFT);
}

void Level::right()
{
	for(Enemy* enemy : enemies)
		enemy->moveNext(Map::RIGHT);
}

void Level::down()
{
	for(Enemy* enemy : enemies)
		enemy->moveNext(Map::DOWN);
}

void Level::up()
{
	for(Enemy* enemy : enemies)
		enemy->moveNext(Map::UP);
}

Tile Level::getTileByPos(const Vector2f &pos)
{
	const Vector2i cell = Engine::Instance().camera()->posToCell(pos);
	return getTileByCell(cell);
}

Tile Level::getTileByCell(const Vector2i &cell) const
{
	const int layer = 0;
	for (unsigned int i = 0; i < gameMap->layers[layer].tiles.size(); i++)
	{
		const Tile tile = gameMap->layers[layer].tiles[i];
		if (tile.cell == cell)
			return tile;

	}
	return Tile();
}

void Level::chooseCurrent()
{
	choose(Engine::Instance().cursor()->cell(), Engine::Instance().cursor()->inPanel());
}

int Level::getTileDirectionByCell(const Vector2i& cell) const
{
	const int id = getTileByCell(cell).id;

	if(gameMap->tileProperties.find(id) != gameMap->tileProperties.end())
	{
		const Tile::TileProperties properties = gameMap->tileProperties.at(id);
		return properties.direction;
	}
	return 0;
}

int Level::getEnergyCount() const
{
	return energy;
}

float Level::getLifeCount() const
{
	return life;
}

void Level::chooseByPos(const Vector2f &pos)
{
	const bool inPanel = pos.y > Engine::Instance().panel()->getBottomValue();
	choose(Engine::Instance().camera()->posToCell(pos), inPanel);
}

void Level::choose(const Vector2i &cell, bool inPanel)
{
	Tower *selectedTower = Engine::Instance().panel()->selectedTower();
	if (Engine::Instance().panel()->selectedTower() != nullptr)
	{
		Engine::Instance().panel()->selectedTower()->deselect();
		Engine::Instance().panel()->setSelectedTower(nullptr);
	}

	if (inPanel)
	{
		const Vector2f pos = Engine::Instance().camera()->cellToPos(cell);
		m_state = Engine::Instance().panel()->getCurrentIcon(pos);
		switch (m_state)
		{
		case READY:
			break;
		case ADD_TOWER:
		{
			const TOWER_TYPES type = Engine::Instance().panel()->currentTower(pos);
			const float radius = TowersFactory::getTowerStats(type).radius * GlobalVariables::Instance().tileSize().x;
			Engine::Instance().cursor()->activateTower(radius, type);
			if (type != POWER)
				highlightPowerTowersRadius(true);
		}
			break;
		case ABILITY_INCREASE_TOWER_ATTACK_SPEED:
		case ABILITY_INCREASE_TOWER_DAMAGE:
			Engine::Instance().cursor()->activateAbility(1, 1, 0, 0);
			break;
		case ABILITY_CARPET_BOMBING:
			Engine::Instance().cursor()->activateAbility(10, 3, 5, 2);
			break;
		case ABILITY_BOMB:
		case ABILITY_FREEZE_BOMB:
			Engine::Instance().cursor()->activateAbility(3, 3, 1, 1);
		break;
		case SELL:
		{
			if (selectedTower == nullptr)
				return;

			const int cost = selectedTower->data().cost/2;
			energy += cost;
			towers.erase( remove( towers.begin(), towers.end(), selectedTower ), towers.end() );
			delete selectedTower;
		}
			break;
		case UPGRADE:
		{
			if (selectedTower == nullptr)
				return;
			selectedTower->upgrade();
		}
			break;
		}
	}
	else
	{
		switch (m_state)
		{
		case READY:
		{
			Tower* tower = getTowerAtPos(Engine::Instance().camera()->cellToPos(cell));
			if (tower != nullptr)
			{
				tower->select();
				Engine::Instance().panel()->setSelectedTower(tower);
			}
		}
			break;
		case ADD_TOWER:
		{
			const TOWER_TYPES type = Engine::Instance().cursor()->getTowerType();
			if (!canAddTower(cell, type))
				return;

			if (type != POWER)
				highlightPowerTowersRadius(false);

			const int cost = TowersFactory::getTowerStats(type).cost;
			if (cost > energy)
				return;

			const Vector2f pos = Engine::Instance().camera()->cellToPos(cell);
			Tower *tower = TowersFactory::createTower(type, pos);
			if (tower == nullptr)
				return;
			towers.push_back(tower);
			energy -= cost;
		}
			break;
		case ABILITY_CARPET_BOMBING:

			break;
		case ABILITY_BOMB:
			for (auto it = enemies.begin(); it != enemies.end();)
			{
				Enemy *enemy = *it;
				if (enemy->gameRect().intersects(Engine::Instance().cursor()->getAbilityRect()))
				{
					enemy->hit(15);
					if (!enemy->isAlive())
					{
						delete enemy;
						it = enemies.erase(it);
					}
					else
						++it;
				}
				else
					++it;
			}
			break;
		case ABILITY_FREEZE_BOMB:
			for(Enemy *enemy : enemies)
			{
				if (enemy->gameRect().intersects(Engine::Instance().cursor()->getAbilityRect()))
				{
					enemy->freeze(25.f, 5000);
				}
			}
			break;
		case ABILITY_INCREASE_TOWER_ATTACK_SPEED:
		{
			Tower* tower = getTowerAtPos(Engine::Instance().camera()->cellToPos(cell));
		}
			break;
		case ABILITY_INCREASE_TOWER_DAMAGE:
		{
			Tower* tower = getTowerAtPos(Engine::Instance().camera()->cellToPos(cell));
		}
			break;
		default:
			break;
		}
		m_state = READY;
		Engine::Instance().cursor()->deactivate();
	}
}

bool Level::isFinished() const
{
	return m_finished;
}

bool Level::isFailed() const
{
	return m_isFailed;
}

void Level::action()
{

}

void Level::change()
{

}
void Level::setDoubleSpeed()
{
	Engine::Instance().setGameSpeed(2.f);
}

void Level::setNormalSpeed()
{
	Engine::Instance().setGameSpeed(1.f);
}

void Level::setFastSpeed()
{
	Engine::Instance().setGameSpeed(8.f);
}

void Level::setNullSpeed()
{
	Engine::Instance().setGameSpeed(1/INT_MAX);
}
