#include "level.h"
#include "Game/gameobject.h"
#include "settings.h"
#include "Game/animation.h"
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
#include "instructions.h"

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
const int Level::REGEN_ENERGY_TIMEOUT = 500;
const int Level::REGEN_ENERGY_VALUE = 5;
const int Level::VENOM_ABILITY_COST = 500;
const int Level::BOMB_ABILITY_COST = 350;
const int Level::FREEZE_BOMB_ABILITY_COST = 250;
const int Level::INC_TOWER_AS_ABILITY_COST = 200;
const int Level::INC_TOWER_DMG_ABILITY_COST = 200;
const float Level::BOMB_ABILITY_DAMAGE = 111;
const int Level::BOMB_ABILITIES_SIZE = 3;
const int Level::DIRECTION_LAYER = 3;

const int Level::Shake::MAX_SHAKE_COUNT = 9;
const int Level::Shake::MAX_SHAKE_OFFSET = 10;
const int Level::Shake::SHAKE_TIME = 50;


Level::Level() :
	gameMap(nullptr)
  ,m_actionState(READY)
  ,life(0.f)
  ,money(0.f)
  ,energy(0.f)
  ,m_state(WAIT_READY)
  ,m_powerTowersCount(0)
{
	venomAbility.isActive = false;
	venomAbility.object = new GameObject(RESOURCES::VENOM_EFFECT, Vector2f(0,0),
										 Vector2i(VenomAbility::VENOM_SIZE.x * GlobalVariables::Instance().tileSize().x,
												  VenomAbility::VENOM_SIZE.y * GlobalVariables::Instance().tileSize().y), 1);

	shake.dangerRect.setSize(Vector2f(Settings::Instance().getResolution()));
	shake.dangerRect.setFillColor(Color(255,0,0,96));
	shake.isActive = false;
	shake.dangerRect.setPosition(0,0);

	deadZone.setOutlineThickness(3.f);
	deadZone.setOutlineColor(Color::Red);
	deadZone.setFillColor(Color::Transparent);

	currentTowerRadius.setFillColor(Cursor::TOWER_AREA_COLOR);
	currentTowerRect.setFillColor(PowerTower::POWER_TOWER_AREA_COLOR);

	spawnRect.setFillColor(Color::Magenta);
	endRect.setFillColor(Color::Magenta);
}

Level::~Level()
{

}

void Level::draw(RenderTarget *const target)
{
	target->setView(*Engine::Instance().camera()->getView());
	drawLevel(target);
	target->draw(deadZone);
	target->draw(spawnRect);
	target->draw(endRect);
	Engine::Instance().panel()->draw(target);
	Engine::Instance().instructions()->draw(target);
}

void Level::update()
{
	if (m_state != PLAYING)
		return;

	if (timer.check(GlobalVariables::FRAME_TIME))
	{
		for(Tower* tower : towers)
		{
			if (!tower->isActive())
				continue;

			if (tower->type() == POWER)
			{
				PowerTower *powerTower = static_cast<PowerTower*>(tower);
				if (powerTower->hasEnergy())
					money += powerTower->gain();
			}
			else
				tower->action(enemies);
			tower->update();
		}
		if (shake.isActive)
		{
			if (shake.dangerTimer.check(Shake::SHAKE_TIME))
			{
				if (shake.state)
					shake.offset = rand() % Shake::MAX_SHAKE_OFFSET * 2;
				else
					shake.offset = -shake.offset;

				Engine::Instance().camera()->getView()->setCenter(Engine::Instance().camera()->getView()->getCenter().x,
																  Engine::Instance().camera()->getView()->getCenter().y + shake.offset);

				shake.state = !shake.state;
				shake.count++;
				if (shake.count > Shake::MAX_SHAKE_COUNT)
					shake.isActive = false;
			}
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

		if (timerRegenEnergy.check(REGEN_ENERGY_TIMEOUT))
			energy += REGEN_ENERGY_VALUE;

		checkRespawn();
		for(Enemy* enemy : enemies)
			enemy->useAbility();
		calculateCollisions();
		checkDeadZone();
		checkEnd();
		checkEnemyMove();

		for(Enemy* enemy : enemies)
			enemy->update();
		for(Animation *effect : effects)
			effect->update();
	}

	Engine::Instance().panel()->updatePanel();
}

void Level::startMission(const unsigned int n)
{
	m_state = WAIT_READY;
	spawnEnemies = EnemiesFactory::generateEnemies(n);
	Engine::Instance().panel()->setProgressMax(spawnEnemies.size());
	life = Engine::getStartHealth(n);
	money = Engine::getStartMoney(n);
	energy = Engine::getStartEnergy(n);
	Engine::Instance().panel()->updatePanel();

	SoundController::Instance().startBackgroundSound("sounds/map1.ogg");
	gameMap = Engine::Instance().getMap(n);
	Engine::Instance().cursor()->setMaxCells(gameMap->width/2, gameMap->height/2);
	Engine::Instance().panel()->initMission(n);
	Engine::Instance().panel()->updateStartEndPos(gameMap->spawnPos,
												  Vector2f(gameMap->endRect.left, gameMap->endRect.top));
	if (n != 0)
		Engine::Instance().instructions()->skip();
	Engine::Instance().cursor()->initCell();

	spawnRect.setPosition(gameMap->spawnPos.x * Settings::Instance().getScaleFactor().x,
						  gameMap->spawnPos.y * Settings::Instance().getScaleFactor().y);
	spawnRect.setSize(GlobalVariables::Instance().mapTileSize());

	endRect.setPosition(gameMap->endRect.left * Settings::Instance().getScaleFactor().x,
						gameMap->endRect.top * Settings::Instance().getScaleFactor().y);
	endRect.setSize(Vector2f(gameMap->endRect.width * Settings::Instance().getScaleFactor().x,
							 gameMap->endRect.height* Settings::Instance().getScaleFactor().y));

	const Vector2f deadZoneSize = Vector2f(gameMap->width * GlobalVariables::Instance().mapTileSize().x,
										   gameMap->height * GlobalVariables::Instance().mapTileSize().y);
	const Vector2f minPos = Vector2f(-DEAD_ZONE_SIZE * Settings::Instance().getScaleFactor().x,
									 -DEAD_ZONE_SIZE * Settings::Instance().getScaleFactor().y);
	deadZone.setPosition(minPos.x, minPos.y);
	deadZone.setSize(Vector2f(deadZoneSize.x + fabs(minPos.x) * 2,
							  deadZoneSize.y + fabs(minPos.y) * 2));
}

void Level::clear()
{
	m_powerTowersCount = 0;
	shake.isActive = false;
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
				pTower->removeProjectile(projectile);
		}
		delete tower;
	}
	towers.clear();

	for(Enemy *enemy : enemies)
		delete enemy;
	enemies.clear();

	for(Animation *animation : effects)
		delete animation;
	effects.clear();
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
	const FloatRect endFRect = getEndRect();
	for (auto it = enemies.begin(); it != enemies.end();)
	{
		Enemy *enemy = *it;
		if (endFRect.contains(enemy->enemyPos()))
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
			energy++;
			Engine::Instance().panel()->updatePanel();
			it = enemies.erase(it);
			enemy = nullptr;
		}
		else
			++it;
	}
}

void Level::checkRespawn()
{
	if (spawnEnemies.empty())
		return;

	const int time = RESPAWN_TIME - Engine::Instance().getMission() * 100;
	const int timeOffset = rand() % (RESPAWN_OFFSET + 1) - RESPAWN_OFFSET/2;
	int spawnCount = 1;
	const float k = 1 - static_cast<float>(spawnEnemies.size()) / Engine::Instance().panel()->getProgressMax();
	if (k > 0.9f)	
		spawnCount = 6;	
	else if(k > 0.8f)	
		spawnCount = 5;	
	else if(k > 0.6f)	
		spawnCount = 4;	
	else if(k > 0.4f)
		spawnCount = 3;	
	else if(k > 0.2f)	
		spawnCount = 2;	

	const int resultTime = time + timeOffset + RESPAWN_TIME * (1 - k);
	if (spawnTimer.check(resultTime))
	{
		for (int i = 0; i < spawnCount; ++i)
		{
			if (spawnEnemies.empty())
				break;
			spawnEnemy();
		}
	}
}

void Level::spawnEnemy()
{
	const int n = rand() % spawnEnemies.size();
	ENEMY_TYPES type = spawnEnemies.at(n);
	spawnEnemies.erase(find(spawnEnemies.begin(), spawnEnemies.end(), type));
	spawn(type);
}

void Level::checkEnemyMove()
{
	const FloatRect endFRect = getEndRect();
	for(Enemy* enemy : enemies)
	{
		if (enemy->isStopped())
			continue;
		if (enemy->moveStep())
		{
			if (endFRect.contains(enemy->enemyPos()))
				continue;

			const Vector2i cell = Engine::Instance().camera()->posToCellMap(enemy->enemyPos());
			const int direction = getTileDirectionByCell(cell);
			enemy->moveNext(direction);
		}
	}
}

Tower *Level::getTowerAtPos(const Vector2f &pos) const
{
	for(Tower* tower : towers)
		if (tower->pos() == pos)
			return tower;
	return nullptr;
}

bool Level::canAddTower(const Vector2i &cell, TOWER_TYPES towerType) const
{
	const int direction = getTileDirectionByCell(cell);
	if (direction != Map::STAY)
		return false;
	Tower *tower = this->getTowerAtPos(Engine::Instance().camera()->cellToPosMap(cell));
	if (tower != nullptr)
		return false;
	bool canCreate = true;
	if (towerType != POWER)
	{
		const Vector2f targetPos = Engine::Instance().camera()->cellToPosMap(cell + Vector2i(1, 1));
		bool finded = false;
		for(Tower *tower : towers)
		{
			if (tower->type() == POWER)
			{
				const FloatRect rect = static_cast<PowerTower*>(tower)->getValidArea();
				if (rect.contains(targetPos))
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
	shake.startShake();
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

void Level::updateRadius()
{
	Tower *tower = Engine::Instance().panel()->selectedTower();
	if (tower == nullptr)
		return;

	if (tower->type() == POWER)
	{
		const FloatRect rect = static_cast<PowerTower*>(tower)->getValidArea();
		currentTowerRect.setPosition(rect.left, rect.top);
		currentTowerRect.setSize(Vector2f(rect.width, rect.height));
	}
	else
	{
		currentTowerRadius.setRadius(tower->data().radius * GlobalVariables::Instance().mapTileSize().x);
		currentTowerRadius.setPosition(tower->pos());
		currentTowerRadius.setOrigin(currentTowerRadius.getRadius() - GlobalVariables::Instance().mapTileSize().x,
						 currentTowerRadius.getRadius() - GlobalVariables::Instance().mapTileSize().y);
	}
}

unsigned int Level::getPowerTowersCount() const
{
	return m_powerTowersCount;
}

void Level::clearCursor()
{
	m_actionState = READY;
	Engine::Instance().cursor()->deactivate();
	highlightPowerTowersRadius(false);
}

vector<Tower *> Level::getAllTowers() const
{
	return towers;
}

FloatRect Level::getEndRect() const
{
	return FloatRect(endRect.getPosition(), endRect.getSize());
}

void Level::test()
{
//	spawn(DOWN_TOWER_ENEMY);
//	spawn(MID_MEDIUM);
//	spawn(TELEPORT_ENEMY);
//	spawn(REPAIR_ENEMY);
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

void Level::addAnimation(const RESOURCES::TEXTURE_TYPE& texture_id,
						 const Vector2f &pos,
						 const Vector2i &size,
						 int duration,
						 int frameCount,
						 int row)
{
	Animation *animation = new Animation();
	animation->row = row;
	animation->animationSpeed = duration;
	animation->size = size;
	animation->callback = std::bind(&Level::removeAnimation, this, animation);
	animation->frameCount = frameCount;
	animation->setTextureId(texture_id);
	animation->sprite.setPosition(pos);

	effects.push_back(animation);
}

void Level::removeAnimation(Animation *animation)
{
	effects.erase( remove( effects.begin(), effects.end(), animation ), effects.end() );
}

void Level::drawLevel(RenderTarget * const target)
{
	for (size_t layer = 0; layer < gameMap->layers.size(); layer++)
	{
		if (!gameMap->layers[layer].visibility)
			continue;
		for (size_t tile = 0; tile < gameMap->layers[layer].tiles.size(); tile++)
			target->draw(gameMap->layers[layer].tiles[tile].sprite);
	}

	for(Enemy* enemy : enemies)
		enemy->draw(target);

	for(Tower *tower : towers)
		tower->draw(target);

	Engine::Instance().cursor()->draw(target);

	if (venomAbility.isActive)
		venomAbility.object->draw(target);

	for(Animation *effect : effects)
		effect->draw(target);

	if (shake.isActive && shake.state)
		target->draw(shake.dangerRect);	

	if (Engine::Instance().panel()->selectedTower() != nullptr)
	{
		if (Engine::Instance().panel()->selectedTower()->type() == POWER)
			target->draw(currentTowerRect);
		else
			target->draw(currentTowerRadius);
	}
}

void Level::spawn(ENEMY_TYPES type)
{
	Enemy *enemy = EnemiesFactory::createEnemy(type, spawnRect.getPosition());
	enemy->moveNext(gameMap->spawnDirection);
	enemies.push_back(enemy);
}

Tile Level::getTileByPos(const Vector2f &pos, unsigned int layer)
{
	const Vector2i cell = Engine::Instance().camera()->posToCellMap(pos);
	return getTileByCell(cell, layer);
}

Tile Level::getTileByCell(const Vector2i &cell, unsigned int layer) const
{
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
	const int id = getTileByCell(cell, DIRECTION_LAYER).id;
	if (id == Map::NO_MOVE)
		return id;
	const map<int, Tile::TileProperties> tileProperties = Engine::Instance().getTileProperties();
	if(tileProperties.find(id) != tileProperties.end())
	{
		const Tile::TileProperties properties = tileProperties.at(id);
		if (properties.alternate_direction1 == -1 && properties.alternate_direction2 == -1)
			return properties.direction;
		int num = 1;
		if (properties.alternate_direction1 != -1)
			num++;
		if (properties.alternate_direction2 != -1)
			num++;

		const int result = rand() % num;
		switch (result)
		{
		case 0:
			return properties.direction;
		case 1:
			return properties.alternate_direction1;
		case 2:
			return properties.alternate_direction2;
		default:
			break;
		}
	}
	return 0;
}

float Level::getMoneyCount() const
{
	return money;
}

float Level::getLifeCount() const
{
	return life;
}

float Level::getEnergyCount() const
{
	return energy;
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
		Engine::Instance().panel()->setSelectedTower(nullptr);	
	if (inPanel)
	{
		m_actionState = Engine::Instance().panel()->getCurrentIcon();
		switch (m_actionState)
		{
		case READY:
			break;
		case ADD_TOWER:
		{
			const TOWER_TYPES type = Engine::Instance().panel()->currentTower();
			const float cost = type == TOWER_TYPES::POWER ?
						TowersFactory::getTowerStats(type).cost + m_powerTowersCount * PowerTower::COST_OFFSET :
						TowersFactory::getTowerStats(type).cost;
			if (money < cost)
				return;
			if (!Engine::Instance().panel()->isTowerIconActive(type))
				return;

			const float radius = TowersFactory::getTowerStats(type).radius * GlobalVariables::Instance().mapTileSize().x;
			Engine::Instance().cursor()->activateTower(radius, type);			
			highlightPowerTowersRadius(type != POWER);
			Engine::Instance().cursor()->swap();
		}
			break;
		case ABILITY_INCREASE_TOWER_ATTACK_SPEED:
		{
			if (m_state != PLAYING)
				return;
			if (!Engine::Instance().panel()->isAbilityIconActive(ABILITY_INCREASE_TOWER_ATTACK_SPEED))
				return;
			if (energy < INC_TOWER_AS_ABILITY_COST)
				return;
			Engine::Instance().cursor()->activateAbility(1, 1, 0, 0);
			Engine::Instance().cursor()->swap();
		}
			break;
		case ABILITY_INCREASE_TOWER_DAMAGE:
		{
			if (m_state != PLAYING)
				return;
			if (!Engine::Instance().panel()->isAbilityIconActive(ABILITY_INCREASE_TOWER_DAMAGE))
				return;
			if (energy < INC_TOWER_DMG_ABILITY_COST)
				return;
			Engine::Instance().cursor()->activateAbility(1, 1, 0, 0);
			Engine::Instance().cursor()->swap();
		}
			break;
		case ABILITY_VENOM:
		{
			if (m_state != PLAYING)
				return;
			if (!Engine::Instance().panel()->isAbilityIconActive(ABILITY_VENOM))
				return;
			if (energy < VENOM_ABILITY_COST)
				return;
			Engine::Instance().cursor()->activateAbility(VenomAbility::VENOM_SIZE.x,
														 VenomAbility::VENOM_SIZE.y, 4, 1);
			Engine::Instance().cursor()->swap();
		}
			break;
		case ABILITY_BOMB:
		{
			if (m_state != PLAYING)
				return;
			if (!Engine::Instance().panel()->isAbilityIconActive(ABILITY_BOMB))
				return;
			if (energy < BOMB_ABILITY_COST)
				return;
			Engine::Instance().cursor()->activateAbility(BOMB_ABILITIES_SIZE, BOMB_ABILITIES_SIZE, 1, 1);
			Engine::Instance().cursor()->swap();
		}
			break;
		case ABILITY_FREEZE_BOMB:
		{
			if (m_state != PLAYING)
				return;
			if (!Engine::Instance().panel()->isAbilityIconActive(ABILITY_FREEZE_BOMB))
				return;
			if (energy < FREEZE_BOMB_ABILITY_COST)
				return;
			Engine::Instance().cursor()->activateAbility(BOMB_ABILITIES_SIZE, BOMB_ABILITIES_SIZE, 1, 1);
			Engine::Instance().cursor()->swap();
		}
			break;
		case ABILITY_UNKNOWN:
		{
			if (m_state != PLAYING)
				return;
			if (!Engine::Instance().panel()->isAbilityIconActive(ABILITY_UNKNOWN))
				return;
			break;
		}
		case SELL:
		{
			if (selectedTower == nullptr)
				return;

			const TOWER_TYPES type = selectedTower->type();
			float cost = type == TOWER_TYPES::POWER ?
						selectedTower->data().cost + (m_powerTowersCount - 1) * PowerTower::COST_OFFSET :
						selectedTower->data().cost;
			cost /= 2;
			money += cost;
			towers.erase( remove( towers.begin(), towers.end(), selectedTower ), towers.end() );
			delete selectedTower;			
			if (type == POWER)
				m_powerTowersCount--;
			Engine::Instance().panel()->updatePanel();
			Engine::Instance().cursor()->swap();
		}
			break;
		case UPGRADE:
		{
			const TOWER_TYPES type = Engine::Instance().panel()->currentTower();
			float cost = type == TOWER_TYPES::POWER ?
						TowersFactory::getTowerStats(type).cost + m_powerTowersCount * PowerTower::COST_OFFSET :
						TowersFactory::getTowerStats(type).cost;
			cost *= TowersFactory::UPGRADE_COST_MODIFIER;
			if (money < cost)
				return;

			if (selectedTower == nullptr)
				return;
			if (selectedTower->level() < 3)
			{
				money -= cost;
				selectedTower->upgrade();
			}
			Engine::Instance().panel()->updatePanel();
			Engine::Instance().cursor()->swap();
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
				Engine::Instance().panel()->setSelectedTower(tower);
				updateRadius();
			}
		}
			break;
		case ADD_TOWER:
		{
			const TOWER_TYPES type = Engine::Instance().cursor()->getTowerType();

			const float cost = TowersFactory::getTowerStats(type).cost + m_powerTowersCount * 10;
			if (cost > money)
				return;

			if (!canAddTower(Vector2i(cell.x * 2, cell.y * 2), type))
				return;

			if (type != POWER)
				highlightPowerTowersRadius(false);

			const Vector2f pos = Engine::Instance().camera()->cellToPos(cell);
			Tower *tower = TowersFactory::createTower(type, pos);
			if (tower == nullptr)
				return;
			towers.push_back(tower);
			money -= cost;
			if (type == POWER)
				m_powerTowersCount++;
			Engine::Instance().panel()->updatePanel();
		}
			break;
		case ABILITY_VENOM:
		{
			if (energy < VENOM_ABILITY_COST)
				return;
			energy -= VENOM_ABILITY_COST;
			Engine::Instance().panel()->updatePanel();
			venomAbility.isActive = true;
			const Vector2f pos = Vector2f(Engine::Instance().cursor()->getAbilityRect().left, Engine::Instance().cursor()->getAbilityRect().top);
			venomAbility.object->setPos(pos);
			venomAbility.timer.clock.restart();
			venomAbility.count = 0;
		}
			break;
		case ABILITY_BOMB:
		{
			if (energy < BOMB_ABILITY_COST)
				return;
			const FloatRect abilityRect = Engine::Instance().cursor()->getAbilityRect();
			addAnimation(RESOURCES::BOMB_EXPLOSION,
						 Vector2f(abilityRect.left, abilityRect.top),
						 Vector2i(BOMB_ABILITIES_SIZE * GlobalVariables::CELL_SIZE, BOMB_ABILITIES_SIZE * GlobalVariables::CELL_SIZE),
						 200, 12, 0);
			energy -= BOMB_ABILITY_COST;
			Engine::Instance().panel()->updatePanel();
			for (auto it = enemies.begin(); it != enemies.end();)
			{
				Enemy *enemy = *it;
				if (enemy->gameRect().intersects(abilityRect))
				{
					enemy->hit(BOMB_ABILITY_DAMAGE);
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
		}
			break;
		case ABILITY_FREEZE_BOMB:
		{
			if (energy < FREEZE_BOMB_ABILITY_COST)
				return;
			const FloatRect abilityRect = Engine::Instance().cursor()->getAbilityRect();
			addAnimation(RESOURCES::FREEZE_BOMB_EXPLOSION,
						 Vector2f(abilityRect.left, abilityRect.top),
						 Vector2i(BOMB_ABILITIES_SIZE * GlobalVariables::CELL_SIZE, BOMB_ABILITIES_SIZE * GlobalVariables::CELL_SIZE),
						 200, 6, 0);
			energy -= FREEZE_BOMB_ABILITY_COST;
			Engine::Instance().panel()->updatePanel();
			for(Enemy *enemy : enemies)
				if (enemy->gameRect().intersects(abilityRect))
					enemy->freeze(FREEZE_ABILITY_K, FREEZE_ABILITY_DURATION);
		}
			break;
		case ABILITY_INCREASE_TOWER_ATTACK_SPEED:
		{			
			if (energy < INC_TOWER_AS_ABILITY_COST)
				return;
			Tower* tower = getTowerAtPos(Engine::Instance().camera()->cellToPos(cell));
			if (tower != nullptr)
			{
				energy -= INC_TOWER_AS_ABILITY_COST;
				Engine::Instance().panel()->updatePanel();
				tower->increaseAttackSpeed(INCREASE_ATTACK_SPEED_ABILITY_DURATION, INCREASE_ATTACK_SPEED_ABILITY_VALUE);
			}
		}
			break;
		case ABILITY_INCREASE_TOWER_DAMAGE:
		{
			if (energy < INC_TOWER_DMG_ABILITY_COST)
				return;
			Tower* tower = getTowerAtPos(Engine::Instance().camera()->cellToPos(cell));
			if (tower != nullptr)
			{
				energy -= INC_TOWER_DMG_ABILITY_COST;
				Engine::Instance().panel()->updatePanel();
				tower->increaseDamage(INCREASE_DAMAGE_ABILITY_DURATION, INCREASE_DAMAGE_ABILITY_VALUE);
			}
		}
			break;
		default:
			break;
		}
		m_actionState = READY;
		Engine::Instance().cursor()->deactivate();
	}
}

void Level::Shake::startShake()
{
	dangerTimer.clock.restart();
	isActive = true;
	state = true;
	count = 0;
}
