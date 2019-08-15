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

const float Level::FREEZE_ABILITY_K = 35.f;
const int Level::FREEZE_ABILITY_DURATION = 5000;
const int Level::INCREASE_DAMAGE_ABILITY_DURATION = 9000;
const int Level::INCREASE_DAMAGE_ABILITY_VALUE = 100;
const int Level::INCREASE_ATTACK_SPEED_ABILITY_DURATION = 9000;
const int Level::INCREASE_ATTACK_SPEED_ABILITY_VALUE = 100;

const float Level::VenomAbility::VENOM_DAMAGE = 10.f;
const Vector2i Level::VenomAbility::VENOM_SIZE = Vector2i(10, 3);
const int Level::VenomAbility::VENOM_DAMAGE_COUNT = 15;
const int Level::VenomAbility::VENOM_ATTACK_SPEED = 500;

Level::Level() :
  difficulty(1.f)
  ,gameMap(nullptr)
  ,m_actionState(READY)
  ,life(0.f)
  ,m_state(WAIT_READY)
{
	venomAbility.isActive = false;
	venomAbility.object =  new GameObject(RESOURCES::VENOM_EFFECT, Vector2f(0,0),
										  Vector2i(VenomAbility::VENOM_SIZE.x * GlobalVariables::Instance().tileSize().x,
												   VenomAbility::VENOM_SIZE.y * GlobalVariables::Instance().tileSize().y), 1);

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

	switch (m_state)
	{
	case WAIT_READY:

		break;
	default:
		break;
	}

	Engine::Instance().panel()->draw(target);
}

void Level::update()
{
	if (m_state != PLAYING)
		return;

	for(Tower* tower : towers)
	{
		if (tower->type() == POWER)
		{
			PowerTower *powerTower = static_cast<PowerTower*>(tower);
			if (powerTower->hasEnergy())
				energy += powerTower->gain();
		}
		else
			tower->action(enemies);
		tower->update();
	}
	if (venomAbility.isActive)
	{
		if (venomAbility.timer.check(VenomAbility::VENOM_ATTACK_SPEED))
		{
			for(Enemy *enemy : enemies)
			{
				if (enemy->gameRect().intersects(venomAbility.object->gameRect()))
					enemy->hit(VenomAbility::VENOM_DAMAGE);
			}
			venomAbility.count++;
			if (venomAbility.count > VenomAbility::VENOM_DAMAGE_COUNT)
				venomAbility.isActive = false;
		}
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

				const Vector2i cell = Engine::Instance().camera()->posToCellMap(enemy->enemyPos());
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

void Level::startMission(const unsigned int n)
{
	m_state = WAIT_READY;
	spawnEnemies = EnemiesFactory::generateEnemies(n);
	Engine::Instance().panel()->setProgressMax(spawnEnemies.size());
	life = 100.f + n * 10;
	energy = Engine::getStartEnergy(n);
	Engine::Instance().panel()->update();

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
	if (spawnEnemies.empty() && enemies.empty())
		changeState(WIN);
}

void Level::checkAlive()
{
	for (auto it = enemies.begin(); it != enemies.end();)
	{
		Enemy *enemy = *it;
		if (!enemy->isAlive())
		{			
			for (Tower *tower : towers)
				if (tower->type() == ROCKET)
					static_cast<RocketTower*>(tower)->checkEnemy(enemy);

			delete enemy;
			it = enemies.erase(it);
		}
		else
			++it;
	}
}

void Level::checkRespawn()
{
	const int timeOffset = rand() % 1000 - 300;
	if (spawnTimer.check(1000 + timeOffset))
	{
		const int n = rand() % spawnEnemies.size();
		ENEMY_TYPES type = spawnEnemies.at(n);
		spawnEnemies.erase(find(spawnEnemies.begin(), spawnEnemies.end(), type));
		spawn(type);
	}
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
				const Vector2i towerCell = Engine::Instance().camera()->posToCellMap(tower->pos());
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
		changeState(LOSE);
	}
}

void Level::changeState(Level::LEVEL_STATE state)
{
	m_state = state;
}

Level::LEVEL_STATE Level::getState() const
{
	return m_state;
}

void Level::ready()
{
	m_state = PLAYING;
}

int Level::currentProgress() const
{
	return spawnEnemies.size();
}

vector<Enemy *> Level::getAllEnemies() const
{
	return enemies;
}

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

	if (venomAbility.isActive)
		venomAbility.object->draw(target);

	//	Effects::Instance().draw(target);
}

void Level::spawn(ENEMY_TYPES type)
{
	Enemy *enemy = EnemiesFactory::createEnemy(type, Engine::Instance().camera()->cellToPosMap(gameMap->spawnPos));
	enemy->moveNext(gameMap->spawnDirection);
	enemies.push_back(enemy);
}

void Level::test()
{	
	changeState(WIN);
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
	const Vector2i cell = Engine::Instance().camera()->posToCellMap(pos);
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

float Level::getEnergyCount() const
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
		m_actionState = Engine::Instance().panel()->getCurrentIcon(pos);
		switch (m_actionState)
		{
		case READY:
			break;
		case ADD_TOWER:
		{
			const TOWER_TYPES type = Engine::Instance().panel()->currentTower(pos);
			const float radius = TowersFactory::getTowerStats(type).radius * GlobalVariables::Instance().mapTileSize().x;
			Engine::Instance().cursor()->activateTower(radius, type);
			if (type != POWER)
				highlightPowerTowersRadius(true);
		}
			break;
		case ABILITY_INCREASE_TOWER_ATTACK_SPEED:
		case ABILITY_INCREASE_TOWER_DAMAGE:
			Engine::Instance().cursor()->activateAbility(1, 1, 0, 0);
			break;
		case ABILITY_VENOM:
			Engine::Instance().cursor()->activateAbility(VenomAbility::VENOM_SIZE.x, VenomAbility::VENOM_SIZE.y, 4, 1);
			break;
		case ABILITY_BOMB:
		case ABILITY_FREEZE_BOMB:
			Engine::Instance().cursor()->activateAbility(3, 3, 1, 1);
		break;
		case ABILITY_UNKNOWN:
			break;
		case SELL:
		{
			if (selectedTower == nullptr)
				return;

			const float cost = selectedTower->data().cost/2;
			energy += cost;
			towers.erase( remove( towers.begin(), towers.end(), selectedTower ), towers.end() );
			delete selectedTower;
			Engine::Instance().panel()->update();
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
		switch (m_actionState)
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

			if (!canAddTower(Engine::Instance().camera()->posToCellMap(Engine::Instance().camera()->cellToPos(cell)), type))
				return;

			if (type != POWER)
				highlightPowerTowersRadius(false);

			const float cost = TowersFactory::getTowerStats(type).cost;
			if (cost > energy)
				return;

			const Vector2f pos = Engine::Instance().camera()->cellToPos(cell);
			Tower *tower = TowersFactory::createTower(type, pos);
			if (tower == nullptr)
				return;
			towers.push_back(tower);
			energy -= cost;
			Engine::Instance().panel()->update();
		}
			break;
		case ABILITY_VENOM:
		{
			venomAbility.isActive = true;
			const Vector2f pos = Vector2f(Engine::Instance().cursor()->getAbilityRect().left, Engine::Instance().cursor()->getAbilityRect().top);
			venomAbility.object->setPos(pos);
			venomAbility.timer.clock.restart();
			venomAbility.count = 0;
		}
			break;
		case ABILITY_BOMB:
			for (auto it = enemies.begin(); it != enemies.end();)
			{
				Enemy *enemy = *it;
				if (enemy->gameRect().intersects(Engine::Instance().cursor()->getAbilityRect()))
				{
					enemy->hit(100);
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
		{
			for(Enemy *enemy : enemies)
				if (enemy->gameRect().intersects(Engine::Instance().cursor()->getAbilityRect()))
					enemy->freeze(FREEZE_ABILITY_K, FREEZE_ABILITY_DURATION);
		}
			break;
		case ABILITY_INCREASE_TOWER_ATTACK_SPEED:
		{
			Tower* tower = getTowerAtPos(Engine::Instance().camera()->cellToPos(cell));
			if (tower != nullptr)
				tower->increaseAttackSpeed(INCREASE_ATTACK_SPEED_ABILITY_DURATION, INCREASE_ATTACK_SPEED_ABILITY_VALUE);
		}
			break;
		case ABILITY_INCREASE_TOWER_DAMAGE:
		{
			Tower* tower = getTowerAtPos(Engine::Instance().camera()->cellToPos(cell));
			if (tower != nullptr)
				tower->increaseDamage(INCREASE_DAMAGE_ABILITY_DURATION, INCREASE_DAMAGE_ABILITY_VALUE);
		}
			break;
		default:
			break;
		}
		m_actionState = READY;
		Engine::Instance().cursor()->deactivate();
	}
}

void Level::action()
{

}

void Level::change()
{

}
