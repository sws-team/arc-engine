#include "level.h"
#include "gamepanel.h"
#include "camera.h"
#include "map.h"
#include "gamecursor.h"
#include "enemy.h"
#include "tower.h"
#include "projectile.h"
#include "instructions.h"
#include "gameability.h"
#include "mapeffects.h"
#include "levelobject.h"
#include "engine.h"
#include "gamemanagers.h"
#include "gameoptions.h"
#include "gameplatform.h"
#include "achievements.h"

Level::Level() :
	gameMap(nullptr)
  ,m_actionState(READY)
  ,life(0.f)
  ,money(0.f)
  ,currentWave(0)
  ,m_state(WAIT_READY)
  ,m_powerTowersCount(0)
  ,m_selectedTower(nullptr)
{
	shake = new Shake();
	abilities = new Abilities();
	mapExplosion = new MapExplosion();
	smoke = new Smoke();
	moneyDrain = new MoneyDrain();
	towersRegress = new TowersRegress();

	shadersFactory = new ShadersFactory();

	deadZone.setOutlineThickness(3.f);
	deadZone.setOutlineColor(sf::Color::Red);
	deadZone.setFillColor(sf::Color::Transparent);

	currentTowerRadius.setFillColor(GameCursor::TOWER_AREA_COLOR);
	currentTowerRect.setFillColor(PowerTower::POWER_TOWER_AREA_COLOR);

	spawnRect.setFillColor(sf::Color::Transparent);
	endRect.setFillColor(sf::Color::Transparent);

	sellSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::SELL_TEXTURE));
	sellSprite.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	upgradeSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::UPGRADE_TEXTURE));
	upgradeSprite.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	const sf::Color textColor = sf::Color(236,169,114);
	const sf::Color textOutlineColor = sf::Color(104,75,56);
	const float fieldIconsCharacterSize = 25;
	sellCostText.setFont(Engine::Instance().fontManager()->font());
	sellCostText.setFillColor(textColor);
	sellCostText.setOutlineColor(textOutlineColor);
	sellCostText.setOutlineThickness(1);
	sellCostText.setCharacterSize(fieldIconsCharacterSize);
	sellCostText.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	upgradeCostText.setFont(Engine::Instance().fontManager()->font());
	upgradeCostText.setFillColor(textColor);
	upgradeCostText.setStyle(sf::Text::Bold);
	upgradeCostText.setOutlineColor(textOutlineColor);
	upgradeCostText.setOutlineThickness(1);
	upgradeCostText.setCharacterSize(fieldIconsCharacterSize);
	upgradeCostText.setScale(Engine::Instance().settingsManager()->getScaleFactor());


	startObject = new GameObject(GAME_TEXTURE::DIRECTION_TEXTURE, sf::Vector2f(0,0), sf::Vector2i(64, 64), 3);
	endObject = new GameObject(GAME_TEXTURE::DIRECTION_TEXTURE, sf::Vector2f(0,0), sf::Vector2i(64, 64), 3);

	updateCurrentTower();
}

Level::~Level()
{
	delete shake;
	delete abilities;
	delete mapExplosion;
	delete smoke;
	delete moneyDrain;
	delete towersRegress;
}

void Level::draw(sf::RenderTarget *const target)
{
	target->setView(*Engine::Instance().options<GameOptions>()->camera()->getView());
	drawLevel(target);
	target->draw(deadZone);
	target->draw(spawnRect);
	target->draw(endRect);
	if(m_state == Level::WAIT_READY)
	{
		startObject->draw(target);
		endObject->draw(target);
	}
	Engine::Instance().window()->setView(Engine::Instance().window()->getDefaultView());
	Engine::Instance().options<GameOptions>()->panel()->draw(target);
	Engine::Instance().options<GameOptions>()->instructions()->draw(target);
}

void Level::update()
{
	const bool playing = m_state == PLAYING;
	if (timer.check(EngineDefs::FRAME_TIME))
	{
		for(Tower* tower : towers)
		{
			if (playing && tower->type() == POWER)
			{
				PowerTower *powerTower = static_cast<PowerTower*>(tower);
				if (powerTower->hasEnergy())
				{
					const float gain = powerTower->gain() * Engine::Instance().options<GameOptions>()->difficult();
					money += gain;
					powerTower->updateGain();
				}
			}
			else
				tower->action();
			tower->update();
		}
		updateUpgrade();
		shake->update();
		mapExplosion->update();
		moneyDrain->update();
		towersRegress->update();
		smoke->update();
		abilities->update();
		if (playing)
			checkRespawn();
		for(Enemy* enemy : enemies)
			enemy->useAbility();
		calculateCollisions();
		checkDeadZone();
		checkEnd();
		checkEnemyMove();
		for(Enemy* enemy : enemies)
			enemy->update();
		checkAlive();
		showAnimations();
	}
	if(m_state == Level::WAIT_READY)
	{
		startObject->update();
		endObject->update();
	}
	for(LevelObject *object : objects)
		object->update();
	Engine::Instance().options<GameOptions>()->panel()->updatePanel();

	shadersFactory->update();

}

void Level::startMission(const unsigned int n)
{
	currentWave = 0;
	Engine::Instance().options<GameOptions>()->panel()->updateWaveText();
	m_state = WAIT_READY;
	waves = EnemiesFactory::generateEnemies(n);
	gameMap = Engine::Instance().options<GameOptions>()->getMap(n);
	Engine::Instance().options<GameOptions>()->panel()->setMapSize(sf::Vector2f(gameMap->width * GameOptions::MAP_CELL_SIZE,
													gameMap->height * GameOptions::MAP_CELL_SIZE));

	Engine::Instance().options<GameOptions>()->panel()->setProgressMax(currentProgress());
	life = gameMap->life;
	Engine::Instance().options<GameOptions>()->panel()->setLifeMax(life);

	money = gameMap->money * Engine::Instance().options<GameOptions>()->difficult();
	Engine::Instance().options<GameOptions>()->panel()->updatePanel();

	Engine::Instance().options<GameOptions>()->cursor()->setMaxCells(gameMap->width/2, gameMap->height/2);
	Engine::Instance().options<GameOptions>()->panel()->initMission(n);
	updateStartEndPos(gameMap->spawnPos, sf::Vector2f(gameMap->endRect.left, gameMap->endRect.top));

	if (n != 0)
		Engine::Instance().options<GameOptions>()->instructions()->skip();
	Engine::Instance().options<GameOptions>()->cursor()->initCell();

	spawnRect.setPosition(gameMap->spawnPos.x * Engine::Instance().settingsManager()->getScaleFactor().x,
						  gameMap->spawnPos.y * Engine::Instance().settingsManager()->getScaleFactor().y);
	spawnRect.setSize(Engine::Instance().options<GameOptions>()->mapTileSize());

	endRect.setPosition(gameMap->endRect.left * Engine::Instance().settingsManager()->getScaleFactor().x,
						gameMap->endRect.top * Engine::Instance().settingsManager()->getScaleFactor().y);
	endRect.setSize(sf::Vector2f(gameMap->endRect.width * Engine::Instance().settingsManager()->getScaleFactor().x,
							 gameMap->endRect.height* Engine::Instance().settingsManager()->getScaleFactor().y));

	const sf::Vector2f minPos = sf::Vector2f(-DEAD_ZONE_SIZE * Engine::Instance().settingsManager()->getScaleFactor().x,
											 -DEAD_ZONE_SIZE * Engine::Instance().settingsManager()->getScaleFactor().y);
	deadZone.setPosition(minPos);

	const sf::Vector2f mapPixelSize = sf::Vector2f(gameMap->width * Engine::Instance().options<GameOptions>()->mapTileSize().x,
												   gameMap->height * Engine::Instance().options<GameOptions>()->mapTileSize().y);
	deadZone.setSize(sf::Vector2f(mapPixelSize.x + fabs(minPos.x) * 2,
								  mapPixelSize.y + fabs(minPos.y) * 2));

	smoke->setTime(gameMap->smoke.time);
	smoke->setDuration(gameMap->smoke.duration);
	smoke->setCount(gameMap->smoke.count);

	mapExplosion->setTime(gameMap->explosions.time);
	mapExplosion->setDuration(gameMap->explosions.duration);
	mapExplosion->setCount(gameMap->explosions.count);

	moneyDrain->setTime(gameMap->moneyDrain.time);
	moneyDrain->setDuration(gameMap->moneyDrain.duration);
	moneyDrain->setCount(gameMap->moneyDrain.count);

	towersRegress->setTime(gameMap->regress.time);
	towersRegress->setDuration(gameMap->regress.duration);
	towersRegress->setCount(gameMap->regress.count);

	smoke->init();
	mapExplosion->init();
	moneyDrain->init();
	towersRegress->init();

	abilities->reset();

	for(const Map::MapObject& mapObject : gameMap->objects)
	{
		sf::Vector2f startPos = mapObject.pos;
		startPos.x *= Engine::Instance().settingsManager()->getScaleFactor().x;
		startPos.y *= Engine::Instance().settingsManager()->getScaleFactor().y;
		LevelObject *object = Engine::Instance().options<GameOptions>()->createObject(mapObject.type, startPos);
		if (object != nullptr)
		{
			object->setLayer(mapObject.layer);
			sf::Shader *shader = shadersFactory->getShader(mapObject.shader_type);
			if (shader != nullptr)
				object->setShader(shader);
			objects.push_back(object);
		}
	}
}

void Level::clear()
{
	clearCursor();
	m_powerTowersCount = 0;
	shake->deactivate();
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

	for(LevelObject *object : objects)
		delete object;
	objects.clear();

	for(Animation *animation : effects)
		delete animation;
	effects.clear();

	smoke->clear();
	mapExplosion->clear();
	moneyDrain->clear();
	towersRegress->clear();
}

void Level::calculateCollisions()
{
	for(Tower *tower : towers)
		tower->collide(enemies);	
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
				const sf::Vector2f pos = projectile->pos();
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
	const sf::FloatRect endFRect = getEndRect();
	for (auto it = enemies.begin(); it != enemies.end();)
	{
		Enemy *enemy = *it;
		if (endFRect.contains(enemy->enemyPos()))
		{
			float damage = enemy->getData().damage / Engine::Instance().options<GameOptions>()->difficult();
			hitPlayer(damage);
			delete enemy;
			it = enemies.erase(it);
		}
		else
			++it;
	}
	if (isFinalWave() && enemies.empty())
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


			switch (enemy->type())
			{
			case INFANTRY:
				GamePlatform::Instance().incrementValue(STAT_INFANTRY_KILLS);
				break;
			case CAR:
				GamePlatform::Instance().incrementValue(STAT_CARS_KILLS);
				break;
			case TRICYCLE:
				GamePlatform::Instance().incrementValue(STAT_TRICYCLE_KILLS);
				break;
			case SMALL_NEXT:
				GamePlatform::Instance().incrementValue(STAT_SMALL_KILLS);
				break;
			case SELFHEAL_ENEMY:
				GamePlatform::Instance().incrementValue(STAT_SELFHEAL_KILLS2);
				break;
			case TRACTOR:
				GamePlatform::Instance().incrementValue(STAT_TRACTOR_KILLS);
				break;
			case ANOTHER_ENEMY:
				GamePlatform::Instance().incrementValue(STAT_ANOTHER_KILLS);
				break;
			case TANK:
				GamePlatform::Instance().incrementValue(STAT_TANK_KILLS);
				break;
			case SPIDER:
				GamePlatform::Instance().incrementValue(STAT_SPIDER_KILLS);
				break;
			case MID_FAST:
				GamePlatform::Instance().incrementValue(STAT_MID_KILLS);
				break;
			case REPAIR_ENEMY:
				GamePlatform::Instance().incrementValue(STAT_REPAIR_KILLS);
				break;
			case SHIELD_ENEMY:
				GamePlatform::Instance().incrementValue(STAT_SHIELD_KILLS);
				break;
			case TELEPORT_ENEMY:
				GamePlatform::Instance().incrementValue(STAT_TELEPORT_KILLS);
				break;
			case BIG_SLOW:
				GamePlatform::Instance().incrementValue(STAT_BIG_KILLS);
				break;
			case BIG_TANK:
				GamePlatform::Instance().incrementValue(STAT_BIG_TANK_KILLS);
				break;
			case SPAWN_ENEMY:
				GamePlatform::Instance().incrementValue(STAT_SPAWN_KILLS);
				break;
			default:
				break;
			}

			delete enemy;
			money++;
			Engine::Instance().options<GameOptions>()->panel()->updatePanel();
			it = enemies.erase(it);
			enemy = nullptr;
		}
		else
			++it;
	}
}

void Level::deleteTower(Tower *tower)
{
	if (tower == m_selectedTower)
		clearCursor();

	towers.erase( remove( towers.begin(), towers.end(), tower ), towers.end() );
	delete tower;
	tower = nullptr;
}

void Level::drainMoney(float m)
{
	money -= m;
	if (money < 0)
		money = 0;
}

float Level::getStartLife() const
{
	return gameMap->life;
}

void Level::updateCurrentTower()
{
	if (m_selectedTower == nullptr)
		return;

	sf::Vector2f optionsPos = m_selectedTower->pos();
	optionsPos.x -= Engine::Instance().options<GameOptions>()->tileSize().x;
	sellSprite.setPosition(optionsPos);
	const sf::Vector2i towerCell = Engine::Instance().options<GameOptions>()->camera()->posToCell(m_selectedTower->pos());
	if (towerCell.x == 0)
	{
		if (towerCell.y == 0)
			sellSprite.setPosition(optionsPos.x + Engine::Instance().options<GameOptions>()->tileSize().x,
								   optionsPos.y + Engine::Instance().options<GameOptions>()->tileSize().x);
		else
			sellSprite.setPosition(optionsPos.x + Engine::Instance().options<GameOptions>()->tileSize().x,
								   optionsPos.y - Engine::Instance().options<GameOptions>()->tileSize().x);
	}
	optionsPos.x += Engine::Instance().options<GameOptions>()->tileSize().x * 2;
	upgradeSprite.setPosition(optionsPos);
	if (towerCell.x == Engine::Instance().options<GameOptions>()->cursor()->getMaxCell().x - 1)
	{
		if (towerCell.y == 0)
			upgradeSprite.setPosition(optionsPos.x - Engine::Instance().options<GameOptions>()->tileSize().x,
									  optionsPos.y + Engine::Instance().options<GameOptions>()->tileSize().x);
		else
			upgradeSprite.setPosition(optionsPos.x - Engine::Instance().options<GameOptions>()->tileSize().x,
									  optionsPos.y - Engine::Instance().options<GameOptions>()->tileSize().x);
	}
	const float sellCost = Engine::Instance().options<GameOptions>()->panel()->getTowerSellCost(m_selectedTower);
	const float upgradeCost = Engine::Instance().options<GameOptions>()->panel()->getTowerUpgradeCost(m_selectedTower);

	sellCostText.setString(GlobalVariables::to_string_with_precision(sellCost, 1));
	upgradeCostText.setString(GlobalVariables::to_string_with_precision(upgradeCost, 0));

	sellCostText.setPosition(sellSprite.getPosition());
	upgradeCostText.setPosition(upgradeSprite.getPosition());
	updateUpgrade();
}

Tower *Level::selectedTower() const
{
	return m_selectedTower;
}

void Level::activateBombAbility()
{
	Engine::Instance().options<GameOptions>()->panel()->setCurrentIcon(ACTION_STATE::ABILITY_BOMB);
	choose(sf::Vector2i(0,0), true);
}

void Level::activateFreezeBombAbility()
{
	Engine::Instance().options<GameOptions>()->panel()->setCurrentIcon(ACTION_STATE::ABILITY_FREEZE_BOMB);
	choose(sf::Vector2i(0,0), true);
}

void Level::activateVenomAbility()
{
	Engine::Instance().options<GameOptions>()->panel()->setCurrentIcon(ACTION_STATE::ABILITY_VENOM);
	choose(sf::Vector2i(0,0), true);
}

void Level::activateIncreaseTowerDamageAbility()
{
	Engine::Instance().options<GameOptions>()->panel()->setCurrentIcon(ACTION_STATE::ABILITY_INCREASE_TOWER_DAMAGE);
	choose(sf::Vector2i(0,0), true);
}

void Level::activateIncreaseTowerAttackSpeedAbility()
{
	Engine::Instance().options<GameOptions>()->panel()->setCurrentIcon(ACTION_STATE::ABILITY_INCREASE_TOWER_ATTACK_SPEED);
	choose(sf::Vector2i(0,0), true);
}

void Level::activateStopAbility()
{
	Engine::Instance().options<GameOptions>()->panel()->setCurrentIcon(ACTION_STATE::ABILITY_STOP);
	choose(sf::Vector2i(0,0), true);
}

void Level::upgradeTower(Tower *tower)
{
	if (tower == nullptr)
		return;

	const float cost = Engine::Instance().options<GameOptions>()->panel()->getTowerUpgradeCost(tower);
	if (money < cost)
		return;

	if (tower->level() < Tower::ABILITY_LEVEL)
	{
		money -= cost;
		tower->upgrade();
	}

	if (tower->level() == Tower::ABILITY_LEVEL)
	{
		fullyUpgradedTowers.insert(tower->type());
		if (fullyUpgradedTowers.size() == 6)//towers count = 6
			GamePlatform::Instance().unlock(ACHIEVEMENT_FULL_UPGRADE_ALL_TOWERS);
	}

	Engine::Instance().options<GameOptions>()->panel()->updatePanel();
}

void Level::sellTower(Tower *tower)
{
	if (tower == nullptr)
		return;

	const float cost = Engine::Instance().options<GameOptions>()->panel()->getTowerSellCost(tower);
	money += cost;
	towers.erase( remove( towers.begin(), towers.end(), tower ), towers.end() );
	if (tower->type() == POWER)
		m_powerTowersCount--;
	delete tower;
	Engine::Instance().soundManager()->playOnce(GAME_SOUND::SELL);
	Engine::Instance().options<GameOptions>()->panel()->updatePanel();
}

bool Level::isFinalWave() const
{
	return waves.size() - 1 == currentWave;
}

void Level::checkRespawn()
{
	if (abilities->stopAblity->isActive())
		return;
	Wave wave = waves.at(currentWave);
	if (wave.spawnEnemies.empty())
		return;
	const int respawnOffset = static_cast<int>(wave.respawnTime * 0.15);
	const int timeOffset = rand() % (respawnOffset + 1) - respawnOffset/2;
	const int resultTime = timeOffset + wave.respawnTime;
	if (spawnTimer.check(resultTime))
	{
		const int n = rand() % wave.spawnEnemies.size();
		ENEMY_TYPES type = wave.spawnEnemies.at(n);
		wave.spawnEnemies.erase(std::find(wave.spawnEnemies.begin(), wave.spawnEnemies.end(), type));
		waves[currentWave] = wave;

		spawn(spawnRect.getPosition(), type, wave.protection, gameMap->spawnDirection);
	}
	if (wave.spawnEnemies.empty())
	{
		if(!isFinalWave())
			currentWave++;
		Engine::Instance().options<GameOptions>()->panel()->updateWaveText();
	}
}

void Level::checkEnemyMove()
{
	if (abilities->stopAblity->isActive())
		return;

	const sf::FloatRect endFRect = getEndRect();
	for(Enemy* enemy : enemies)
	{
		if (enemy->isStopped())
			continue;
		enemy->moveEnemy();

		if (endFRect.contains(enemy->enemyPos()))
			continue;
		const sf::Vector2i cell = Engine::Instance().options<GameOptions>()->camera()->posToCellMap(enemy->enemyPos());
		const int direction = getTileDirectionByCell(cell);
		if (direction != 0)
			enemy->moveNext(direction);
	}
}

Tower *Level::getTowerAtPos(const sf::Vector2f &pos) const
{
	for(Tower* tower : towers)
		if (tower->pos() == pos)
			return tower;
	return nullptr;
}

bool Level::canAddTower(const sf::Vector2i &cell, TOWER_TYPES towerType) const
{
	const int direction = getTileDirectionByCell(cell);
	if (direction != Map::STAY)
		return false;
	Tower *tower = this->getTowerAtPos(Engine::Instance().options<GameOptions>()->camera()->cellToPosMap(cell));
	if (tower != nullptr)
		return false;
	bool canCreate = true;
	if (towerType != POWER)
	{
		const sf::Vector2f targetPos = Engine::Instance().options<GameOptions>()->camera()->cellToPosMap(cell + sf::Vector2i(1, 1));
		bool finded = false;
		for(Tower *tower : towers)
		{
			if (tower->type() == POWER)
			{
				const sf::FloatRect rect = static_cast<PowerTower*>(tower)->getValidArea();
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
	shake->start();
	if (life <= 0.f)
	{
		life = 0.f;
		//game over
		changeState(LOSE);
	}
	Engine::Instance().options<GameOptions>()->panel()->updatePanel();
}

void Level::changeState(Level::LEVEL_STATE state)
{
	m_state = state;
	switch (m_state)
	{
	case WIN:
		Engine::Instance().soundManager()->endBackgroundSound();
		Engine::Instance().soundManager()->playOnce(GAME_SOUND::WIN);
		break;
	case LOSE:
		Engine::Instance().soundManager()->endBackgroundSound();

		break;
	default:
		break;
	}
}

void Level::updateRadius()
{
	if (m_selectedTower == nullptr)
		return;

	if (m_selectedTower->type() == POWER)
	{
		const sf::FloatRect rect = static_cast<PowerTower*>(m_selectedTower)->getValidArea();
		currentTowerRect.setPosition(rect.left, rect.top);
		currentTowerRect.setSize(sf::Vector2f(rect.width, rect.height));
	}
	else
	{
		currentTowerRadius.setRadius(m_selectedTower->actualRadius() * Engine::Instance().options<GameOptions>()->mapTileSize().x);
		currentTowerRadius.setPosition(m_selectedTower->pos());
		currentTowerRadius.setOrigin(currentTowerRadius.getRadius() - Engine::Instance().options<GameOptions>()->mapTileSize().x,
						 currentTowerRadius.getRadius() - Engine::Instance().options<GameOptions>()->mapTileSize().y);
	}
}

void Level::showAnimations()
{
	for(Animation *effect : effects)
		effect->update();
}

ACTION_STATE Level::isFieldButtons(const sf::Vector2f &pos) const
{
	const sf::Vector2f gPos = pos + sf::Vector2f(1, 1);
	if (sellSprite.getGlobalBounds().contains(gPos))
		return SELL;
	if (upgradeSprite.getGlobalBounds().contains(gPos))
		return UPGRADE;
	return READY;
}

void Level::setSelectedTower(Tower *tower)
{
	m_selectedTower = tower;
	updateCurrentTower();
}

void Level::updateStartEndPos(const sf::Vector2f &startPos, const sf::Vector2f &endPos)
{
	const sf::Vector2f mapSize = sf::Vector2f(gameMap->width * Engine::Instance().options<GameOptions>()->mapTileSize().x,
									  gameMap->height * Engine::Instance().options<GameOptions>()->mapTileSize().y);
	const int centerX = mapSize.x/2;
	const int centerY = mapSize.y/2;

	const float x0 = 0;
	const float x1 = mapSize.x;
	const float y0 = 0;
	const float y1 = mapSize.y;
	startObject->sprite.setRotation(0);
	endObject->sprite.setRotation(0);

	sf::Vector2f resultStartPos = sf::Vector2f(startPos.x * Engine::Instance().settingsManager()->getScaleFactor().x,
									   startPos.y * Engine::Instance().settingsManager()->getScaleFactor().y);
	sf::Vector2f resultEndPos = sf::Vector2f(endPos.x * Engine::Instance().settingsManager()->getScaleFactor().x,
									 endPos.y * Engine::Instance().settingsManager()->getScaleFactor().y);
	//start
	if (startPos.x <= x0 || startPos.x >= x1)
	{
		//x
		if (startPos.x > centerX)
		{
			//-x
			resultStartPos.x -= Engine::Instance().options<GameOptions>()->tileSize().x + Engine::Instance().options<GameOptions>()->mapTileSize().x;
		}
		else
		{
			//+x
			resultStartPos.x += Engine::Instance().options<GameOptions>()->tileSize().x;
		}
		resultStartPos.y += Engine::Instance().options<GameOptions>()->mapTileSize().y;

		startObject->sprite.setRotation(-90);
		resultStartPos.y += Engine::Instance().options<GameOptions>()->tileSize().y;
	}
	else if (startPos.y <= y0 || startPos.y >= y1)
	{
		//y
		if (startPos.y > centerY)
		{
			//-y
			resultStartPos.y -= Engine::Instance().options<GameOptions>()->tileSize().y + Engine::Instance().options<GameOptions>()->mapTileSize().y;
		}
		else
		{
			//+y
			resultStartPos.y += Engine::Instance().options<GameOptions>()->tileSize().y;
		}
		resultStartPos.x += Engine::Instance().options<GameOptions>()->mapTileSize().x;
	}

	//end
	if (endPos.x <= x0 || endPos.x >= x1)
	{
		//x
		if (endPos.x < centerX)
		{
			//-x
			resultEndPos.x += Engine::Instance().options<GameOptions>()->mapTileSize().x + Engine::Instance().options<GameOptions>()->tileSize().x;
		}
		else
		{
			//+x
			resultEndPos.x -= Engine::Instance().options<GameOptions>()->mapTileSize().x + Engine::Instance().options<GameOptions>()->tileSize().x;
		}
		resultEndPos.y += Engine::Instance().options<GameOptions>()->mapTileSize().y;

		endObject->sprite.setRotation(-90);
		resultEndPos.y += Engine::Instance().options<GameOptions>()->tileSize().y;
	}
	else if (endPos.y <= y0 || endPos.y >= y1)
	{
		//y
		if (endPos.y > centerY)
		{
			//-y
			resultEndPos.y -= Engine::Instance().options<GameOptions>()->mapTileSize().y + Engine::Instance().options<GameOptions>()->mapTileSize().y;
		}
		else
		{
			//+y
			resultEndPos.y += Engine::Instance().options<GameOptions>()->tileSize().y;
		}
		resultEndPos.x += Engine::Instance().options<GameOptions>()->mapTileSize().x;
	}
	startObject->setPos(resultStartPos);
	endObject->setPos(resultEndPos);
}

void Level::updateUpgrade()
{
	if (m_selectedTower == nullptr)
		return;

	upgradeSprite.setColor(sf::Color::White);
	const float cost = Engine::Instance().options<GameOptions>()->panel()->getTowerUpgradeCost(m_selectedTower);
	if (money < cost)
		upgradeSprite.setColor(EngineDefs::GrayColor);
}

unsigned int Level::getPowerTowersCount() const
{
	return m_powerTowersCount;
}

void Level::clearCursor()
{
	setSelectedTower(nullptr);
	m_actionState = READY;
	Engine::Instance().options<GameOptions>()->cursor()->deactivate();
	highlightPowerTowersRadius(false);
}

std::vector<Tower *> Level::getAllTowers() const
{
	return towers;
}

sf::FloatRect Level::getEndRect() const
{
	return sf::FloatRect(endRect.getPosition(), endRect.getSize());
}

unsigned int Level::getCurrentWave() const
{
	return currentWave;
}

void Level::test()
{
	GamePlatform::Instance().unlock(ACHIEVEMENT_COMPLETE_ALL_LEVELS);
}

Abilities *Level::getAbilities()
{
	return abilities;
}

Level::LEVEL_STATE Level::getState() const
{
	return m_state;
}

void Level::ready()
{
	m_state = PLAYING;

	towersRegress->setEnabled(gameMap->regress.enabled);
	smoke->setEnabled(gameMap->smoke.enabled);
	moneyDrain->setEnabled(gameMap->moneyDrain.enabled);
	mapExplosion->setEnabled(gameMap->explosions.enabled);

	towersRegress->resetTimers();
	smoke->resetTimers();
	moneyDrain->resetTimers();
	mapExplosion->resetTimers();
}

int Level::currentProgress() const
{
	int progress = 0;
	for(const Wave& wave : waves)
		progress += wave.spawnEnemies.size();
	return progress;
}

std::vector<Enemy *> Level::getAllEnemies() const
{
	return enemies;
}

void Level::addAnimation(const TextureType& texture_id,
						 const sf::Vector2f &pos,
						 const sf::Vector2i &size,
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

void Level::drawLevel(sf::RenderTarget * const target)
{
	for (size_t layer = 0; layer < gameMap->layers.size(); layer++)
	{
		if (!gameMap->layers[layer].visibility)
			continue;
		sf::Shader *shader = nullptr;
		if (layer == gameMap->movingLayer)
			shader = shadersFactory->getShader(OBJECTS::MOVING);
		else if (layer == gameMap->waterLayer)
			shader = shadersFactory->getShader(OBJECTS::WAVE);

		for (unsigned int tile = 0; tile < gameMap->layers[layer].tiles.size(); tile++)
			target->draw(gameMap->layers[layer].tiles[tile].sprite, shader);

		for(LevelObject *object : objects)
		{
			if (object->layer() == layer)
				object->draw(target);
		}
	}

	for(LevelObject *object : objects)
	{
		if (object->layer() == -1)
			object->draw(target);
	}

	abilities->draw(target);

	for(Enemy* enemy : enemies)
		enemy->draw(target);

	for(Tower *tower : towers)
		tower->draw(target);

	Engine::Instance().options<GameOptions>()->cursor()->draw(target);

	for(Animation *effect : effects)
		effect->draw(target);

	shake->draw(target);
	mapExplosion->draw(target);
	smoke->draw(target);
	moneyDrain->draw(target);
	towersRegress->draw(target);

	if (m_selectedTower != nullptr)
	{
		if (m_selectedTower->type() == POWER)
			target->draw(currentTowerRect);
		else
			target->draw(currentTowerRadius);

		target->draw(sellSprite);
		target->draw(sellCostText);
		if (m_selectedTower->level() < Tower::ABILITY_LEVEL)
		{
			target->draw(upgradeSprite);
			target->draw(upgradeCostText);
		}
	}
}

void Level::spawn(const sf::Vector2f& pos, ENEMY_TYPES type, float protection, int moveDirection)
{
	Enemy *enemy = EnemiesFactory::createEnemy(type, pos);
	enemy->protect(protection, false);
	enemy->moveNext(moveDirection);
	enemies.push_back(enemy);
}

Tile Level::getTileByPos(const sf::Vector2f &pos, unsigned int layer)
{
	const sf::Vector2i cell = Engine::Instance().options<GameOptions>()->camera()->posToCellMap(pos);
	return getTileByCell(cell, layer);
}

Tile Level::getTileByCell(const sf::Vector2i &cell, unsigned int layer) const
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
	choose(Engine::Instance().options<GameOptions>()->cursor()->cell(), Engine::Instance().options<GameOptions>()->cursor()->inPanel());
}

int Level::getTileDirectionByCell(const sf::Vector2i& cell) const
{
	const int id = getTileByCell(cell, gameMap->directionsLayer).id;
	if (id == Map::NO_MOVE)
		return id;
	const std::map<int, Tile::TileProperties> tileProperties = Engine::Instance().options<GameOptions>()->getTileProperties();
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

void Level::chooseByPos(const sf::Vector2f &pos)
{
	const bool inPanel = pos.y > Engine::Instance().options<GameOptions>()->panel()->getBottomValue();
	choose(Engine::Instance().options<GameOptions>()->camera()->posToCell(pos), inPanel);
}

void Level::choose(const sf::Vector2i &cell, bool inPanel)
{
	Tower *selectedTower = m_selectedTower;
	if (m_selectedTower != nullptr)
		setSelectedTower(nullptr);
	if (inPanel)
	{
		const ACTION_STATE currentState = Engine::Instance().options<GameOptions>()->panel()->getCurrentIcon();
		switch (currentState)
		{
		case READY:
			break;
		case ADD_TOWER:
		{
			const TOWER_TYPES type = Engine::Instance().options<GameOptions>()->panel()->currentTower();
			const float cost = type == TOWER_TYPES::POWER ?
						TowersFactory::getTowerStats(type).cost + m_powerTowersCount * PowerTower::COST_OFFSET :
						TowersFactory::getTowerStats(type).cost;
			if (money < cost)
				return;
			if (!Engine::Instance().options<GameOptions>()->panel()->isTowerIconActive(type))
				return;

			const float radius = TowersFactory::getTowerStats(type).radius * Engine::Instance().options<GameOptions>()->mapTileSize().x;
			Engine::Instance().options<GameOptions>()->cursor()->activateTower(radius, type);
			highlightPowerTowersRadius(type != POWER);
			Engine::Instance().options<GameOptions>()->cursor()->swap();
		}
			break;
		case ABILITY_INCREASE_TOWER_ATTACK_SPEED:
		{
			if (!Engine::Instance().options<GameOptions>()->panel()->isAbilityIconActive(ABILITY_INCREASE_TOWER_ATTACK_SPEED))
				return;

			if (!abilities->increaseTowerAttackSpeedAbility->isReady())
				return;

			abilities->increaseTowerAttackSpeedAbility->setUp();
		}
			break;
		case ABILITY_INCREASE_TOWER_DAMAGE:
		{
			if (!Engine::Instance().options<GameOptions>()->panel()->isAbilityIconActive(ABILITY_INCREASE_TOWER_DAMAGE))
				return;

			if (!abilities->increaseTowerDamageAbility->isReady())
				return;

			abilities->increaseTowerDamageAbility->setUp();
		}
			break;
		case ABILITY_VENOM:
		{
			if (!Engine::Instance().options<GameOptions>()->panel()->isAbilityIconActive(ABILITY_VENOM))
				return;
			if (!abilities->venomAbility->isReady())
				return;

			abilities->venomAbility->setUp();
		}
			break;
		case ABILITY_BOMB:
		{
			if (!Engine::Instance().options<GameOptions>()->panel()->isAbilityIconActive(ABILITY_BOMB))
				return;

			if (!abilities->bombAbility->isReady())
				return;

			abilities->bombAbility->setUp();
		}
			break;
		case ABILITY_FREEZE_BOMB:
		{
			if (!Engine::Instance().options<GameOptions>()->panel()->isAbilityIconActive(ABILITY_FREEZE_BOMB))
				return;

			if (!abilities->freezeBombAbility->isReady())
				return;

			abilities->freezeBombAbility->setUp();
		}
			break;
		case ABILITY_STOP:
		{
			if (!Engine::Instance().options<GameOptions>()->panel()->isAbilityIconActive(ABILITY_STOP))
				return;

			if (!abilities->stopAblity->isReady())
				return;

			Engine::Instance().options<GameOptions>()->cursor()->swap();
			abilities->stopAblity->activate();
		}
			break;
		default:
			break;
		}
		m_actionState = currentState;
	}
	else
	{
		const sf::Vector2f pos = Engine::Instance().options<GameOptions>()->camera()->cellToPos(cell);
		if (m_actionState == READY && selectedTower != nullptr)
		{
			const ACTION_STATE fieldState = isFieldButtons(pos);
			if (fieldState != READY)
			{
				m_actionState = fieldState;
				if (m_actionState == UPGRADE && selectedTower->level() >= Tower::ABILITY_LEVEL)
					m_actionState = READY;
			}
		}
		switch (m_actionState)
		{
		case READY:
		{			
			Tower* tower = getTowerAtPos(pos);
			if (tower != nullptr)
			{
				setSelectedTower(tower);
				updateRadius();
			}
		}
			break;
		case ADD_TOWER:
		{
			const TOWER_TYPES type = Engine::Instance().options<GameOptions>()->cursor()->getTowerType();

			float cost = type == TOWER_TYPES::POWER ?
						TowersFactory::getTowerStats(type).cost + m_powerTowersCount * 10 :
						TowersFactory::getTowerStats(type).cost;

			if (cost > money)
				return;

			if (!canAddTower(sf::Vector2i(cell.x * 2, cell.y * 2), type))
				return;

			if (type != POWER)
				highlightPowerTowersRadius(false);

			Tower *tower = TowersFactory::createTower(type, pos);
			if (tower == nullptr)
				return;
			GamePlatform::Instance().incrementValue(STAT_TOWERS_BUILDED);
			Engine::Instance().soundManager()->playOnce(GAME_SOUND::SETUP);
			towers.push_back(tower);
			money -= cost;
			if (type == POWER)
				m_powerTowersCount++;
			Engine::Instance().options<GameOptions>()->panel()->updatePanel();
		}
			break;
		case ABILITY_VENOM:		
			abilities->venomAbility->activate();		
			break;
		case ABILITY_BOMB:
		{
			abilities->bombAbility->activate();
			moneyDrain->explosion(Engine::Instance().options<GameOptions>()->cursor()->getAbilityRect());
		}
			break;
		case ABILITY_FREEZE_BOMB:		
			abilities->freezeBombAbility->activate();		
			break;
		case ABILITY_INCREASE_TOWER_ATTACK_SPEED:		
			abilities->increaseTowerAttackSpeedAbility->activate();		
			break;
		case ABILITY_INCREASE_TOWER_DAMAGE:
			abilities->increaseTowerDamageAbility->activate();
			break;
		case ABILITY_STOP:
			break;
		case SELL:		
			sellTower(selectedTower);
			break;
		case UPGRADE:
			upgradeTower(selectedTower);
			break;
		default:
			break;
		}
		m_actionState = READY;
		Engine::Instance().options<GameOptions>()->cursor()->deactivate();
	}
}
