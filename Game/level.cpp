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
#include "mainwindow.h"

Level::Level() :
	gameMap(nullptr)
  ,m_actionState(READY)
  ,life(0.f)
  ,money(0.f)
  ,currentWave(0)
  ,m_state(WAIT_READY)
  ,m_selectedTower(nullptr)
  ,abilityActivated(false)
  ,attackTowerBuilded(false)
  ,showBuildRects(false)
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

	sellSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::SELL_TEXTURE));
	sellSprite.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	upgradeSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::UPGRADE_TEXTURE));
	upgradeSprite.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	fixSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::UPGRADE_TEXTURE));
	fixSprite.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	const sf::Color textColor = sf::Color(236,169,114);
	const sf::Color textOutlineColor = sf::Color(104,75,56);
	const float fieldIconsCharacterSize = 25;
	sellCostText.setFont(Engine::Instance().fontManager()->font());
	sellCostText.setFillColor(textColor);
	sellCostText.setOutlineColor(textOutlineColor);
	sellCostText.setOutlineThickness(1);
	sellCostText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(fieldIconsCharacterSize));
	sellCostText.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	upgradeCostText.setFont(Engine::Instance().fontManager()->font());
	upgradeCostText.setFillColor(textColor);
	upgradeCostText.setStyle(sf::Text::Bold);
	upgradeCostText.setOutlineColor(textOutlineColor);
	upgradeCostText.setOutlineThickness(1);
	upgradeCostText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(fieldIconsCharacterSize));
	upgradeCostText.setScale(Engine::Instance().settingsManager()->getScaleFactor());


	smokeRect.setPosition(0,0);
	smokeRect.setFillColor(sf::Color(50, 50, 50, 128));

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
	if(m_state == Level::WAIT_READY)
	{
		for(const ActionPoint& actionPoint : spawnPoints)
		{
#ifndef RELEASE_BUILD
			target->draw(actionPoint.rect);
#endif
			actionPoint.object->draw(target);
		}
		for(const ActionPoint& actionPoint : endPoints)
		{
#ifndef RELEASE_BUILD
			target->draw(actionPoint.rect);
#endif
			actionPoint.object->draw(target);
		}
	}
//	Engine::Instance().window()->setView(Engine::Instance().window()->getDefaultView());
	Engine::Instance().window()->setView(*Engine::Instance().window()->view());
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
					const float gain = powerTower->actualDamage(DEFAULT_ARMOR) * Engine::Instance().options<GameOptions>()->difficult();
					money += gain;
					powerTower->updateGain();

					if (money >= ECONOMIST_MONEY_VALUE)
						GamePlatform::Instance().unlock(ACHIEVEMENT_GAIN_MANY_MONEY);
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

		for(Enemy* enemy : enemies)
			enemy->useAbility();
		calculateCollisions();
		checkDeadZone();
		checkEnd();
		checkEnemyMove();
		if (playing)
			checkRespawn();
		for(Enemy* enemy : enemies)
			enemy->update();
		checkAlive();
		showAnimations();
	}
	if(m_state == Level::WAIT_READY)
	{
		for(const ActionPoint& actionPoint : spawnPoints)
			actionPoint.object->update();
		for(const ActionPoint& actionPoint : endPoints)
			actionPoint.object->update();
	}
	for(LevelObject *object : objects)
		object->update();
	Engine::Instance().options<GameOptions>()->panel()->updatePanel();

	shadersFactory->update();

}

void Level::startMission(const unsigned int n)
{
	int mapNumber = n - 1;
	Engine::Instance().options<GameOptions>()->instructions()->init(n);
	spawnTimer.reset();
	currentWave = 0;
	Engine::Instance().options<GameOptions>()->panel()->updateWaveText();
	m_state = WAIT_READY;
	std::string showLevelText;
	if (n == 0)
	{
		const int _testWavesCount = 999;
		const int _testEnemiesCount = 999;
		const int _testInterval = 10000;

		Wave wave;
		wave.protection = 0.f;
		wave.respawnTime = _testInterval;
		for (int i = 0; i < _testEnemiesCount; ++i)
		{
#if 0
			for (int j = ENEMY_TYPES::INFANTRY; j < ENEMY_TYPES::BUGSAURUS; ++j)
				wave.spawnEnemies.push_back(static_cast<ENEMY_TYPES>(j));
#else
			wave.spawnEnemies.push_back(ENEMY_TYPES::TANK);
#endif
		}
		for (int i = 0; i < _testWavesCount; ++i)
			waves.push_back(wave);
		showLevelText = std::string("test");
	}
	else
	{
		waves = Balance::Instance().getWave(mapNumber);
		showLevelText = std::to_string(n);
	}

	const sf::Vector2i maxCells = Engine::Instance().options<GameOptions>()->cursor()->getMaxCell();
	const sf::Vector2f mapTileSize = Engine::Instance().options<GameOptions>()->mapTileSize();
	const sf::Vector2f tileSize = Engine::Instance().options<GameOptions>()->tileSize();
	gameMap = Engine::Instance().options<GameOptions>()->findMapByNumber(n);
	Engine::Instance().options<GameOptions>()->panel()->setMapSize(
				sf::Vector2f(gameMap->width * GameOptions::MAP_CELL_SIZE,
							 gameMap->height * GameOptions::MAP_CELL_SIZE));

	Engine::Instance().options<GameOptions>()->panel()->setProgressMax(currentProgress());
	life = gameMap->stats.life;
	Engine::Instance().options<GameOptions>()->panel()->setLifeMax(life);

	money = gameMap->stats.money * Engine::Instance().options<GameOptions>()->difficult();
	Engine::Instance().options<GameOptions>()->panel()->updatePanel();

	Engine::Instance().options<GameOptions>()->cursor()->setMaxCells(gameMap->width/2, gameMap->height/2);
	Engine::Instance().options<GameOptions>()->panel()->initMission(n);

	const int starsValue = Engine::Instance().options<GameOptions>()->missionStars(n);
	if (starsValue != -1)
		Engine::Instance().options<GameOptions>()->instructions()->skip();

	Engine::Instance().options<GameOptions>()->cursor()->initCell();

	for(const Map::SpawnInfo& spawnInfo : gameMap->spawnInfos)
	{
		ActionPoint actionPoint;
		actionPoint.direction = spawnInfo.direction;
		const sf::Vector2f pos = sf::Vector2f(spawnInfo.pos.x * Engine::Instance().settingsManager()->getScaleFactor().x,
											  spawnInfo.pos.y * Engine::Instance().settingsManager()->getScaleFactor().y);
		actionPoint.rect.setPosition(pos);
		actionPoint.rect.setSize(mapTileSize);
		actionPoint.rect.setFillColor(sf::Color::Transparent);
		actionPoint.rect.setOutlineColor(sf::Color::Magenta);
		actionPoint.rect.setOutlineThickness(1);
		actionPoint.object = new GameObject(GAME_TEXTURE::DIRECTION_TEXTURE,
											sf::Vector2f(0,0), sf::Vector2i(64, 64), 3);
		updateActionPoint(actionPoint.object, spawnInfo.pos, true);
		spawnPoints.push_back(actionPoint);
	}

	for(const sf::FloatRect& rect : gameMap->endRects)
	{
		ActionPoint actionPoint;
		const sf::Vector2f pos = sf::Vector2f(rect.left * Engine::Instance().settingsManager()->getScaleFactor().x,
											  rect.top * Engine::Instance().settingsManager()->getScaleFactor().y);
		actionPoint.rect.setPosition(pos);
		actionPoint.rect.setSize(sf::Vector2f(rect.width * Engine::Instance().settingsManager()->getScaleFactor().x,
											  rect.height* Engine::Instance().settingsManager()->getScaleFactor().y));
		actionPoint.rect.setFillColor(sf::Color::Transparent);
		actionPoint.rect.setOutlineColor(sf::Color::Magenta);
		actionPoint.rect.setOutlineThickness(1);
		actionPoint.direction = Map::NO_MOVE;
		actionPoint.object = new GameObject(GAME_TEXTURE::DIRECTION_TEXTURE,
											sf::Vector2f(0,0), sf::Vector2i(64, 64), 3);
		updateActionPoint(actionPoint.object, sf::Vector2f(rect.left, rect.top), false);

		endPoints.push_back(actionPoint);
	}

	const sf::Vector2f minPos = sf::Vector2f(-DEAD_ZONE_SIZE * Engine::Instance().settingsManager()->getScaleFactor().x,
											 -DEAD_ZONE_SIZE * Engine::Instance().settingsManager()->getScaleFactor().y);
	deadZone.setPosition(minPos);

	const sf::Vector2f mapPixelSize = sf::Vector2f(gameMap->width * mapTileSize.x,
												   gameMap->height * mapTileSize.y);
	deadZone.setSize(sf::Vector2f(mapPixelSize.x + fabs(minPos.x) * 2,
								  mapPixelSize.y + fabs(minPos.y) * 2));

	smoke->setTime(gameMap->stats.smoke.time);
	smoke->setDuration(gameMap->stats.smoke.duration);
	smoke->setCount(gameMap->stats.smoke.count);

	mapExplosion->setTime(gameMap->stats.explosions.time);
	mapExplosion->setDuration(gameMap->stats.explosions.duration);
	mapExplosion->setCount(gameMap->stats.explosions.count);

	moneyDrain->setTime(gameMap->stats.moneyDrain.time);
	moneyDrain->setDuration(gameMap->stats.moneyDrain.duration);
	moneyDrain->setCount(gameMap->stats.moneyDrain.count);

	towersRegress->setTime(gameMap->stats.regress.time);
	towersRegress->setDuration(gameMap->stats.regress.duration);
	towersRegress->setCount(gameMap->stats.regress.count);

	smoke->init();
	mapExplosion->init();
	moneyDrain->init();
	towersRegress->init();

	abilities->reset();

	smokeRect.setSize(sf::Vector2f(maxCells.x * tileSize.x,
								   maxCells.y * tileSize.y));

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

	GamePlatform::Instance().setPlatformState("lvl", showLevelText);
	GamePlatform::Instance().setPlatformState("steam_display", "#Status");

	for (int x = 0; x < maxCells.x; ++x)
	{
		for (int y = 0; y < maxCells.y; ++y)
		{
			sf::RectangleShape rect;
			rect.setSize(tileSize);
			rect.setPosition(x * tileSize.x, y * tileSize.y);
			buildCells.push_back(rect);
		}
	}
}

void Level::clear()
{
	buildCells.clear();
	clearCursor();
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

	for(const ActionPoint& actionPoint : spawnPoints)
		delete actionPoint.object;
	spawnPoints.clear();
	for(const ActionPoint& actionPoint : endPoints)
		delete actionPoint.object;
	endPoints.clear();

	for(Enemy *enemy : enemies)
		delete enemy;
	enemies.clear();

	for(LevelObject *object : objects)
		delete object;
	objects.clear();

	for(Animation *animation : effects)
		delete animation;
	effects.clear();

	abilities->clear();
	smoke->clear();
	mapExplosion->clear();
	moneyDrain->clear();
	towersRegress->clear();
	TowersCounter::Instance().reset();
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
	for(const ActionPoint& actionPoint : endPoints)
	{
		const sf::FloatRect endRect = sf::FloatRect(actionPoint.rect.getPosition(),
													 actionPoint.rect.getSize());
		for (auto it = enemies.begin(); it != enemies.end();)
		{
			Enemy *enemy = *it;
			if (endRect.contains(enemy->enemyPos()))
			{
				float damage = enemy->getData().damage / Engine::Instance().options<GameOptions>()->difficult();
				hitPlayer(damage);
				delete enemy;
				it = enemies.erase(it);
			}
			else
				++it;
		}
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
			case WAR_VEHICLE:
			case TRICYCLE:
			case WORM:
			case SELFHEAL_ENEMY:
			case TRACTOR:
			case SLUGGY:
				GamePlatform::Instance().incrementValue(STAT_SMALL_KILLS);
				break;				
			case TANK:
			case MECHSPIDER:
			case TUATARA:
			case REPAIR_ENEMY:
			case SHIELD_ENEMY:
			case WALKER:
				GamePlatform::Instance().incrementValue(STAT_MID_KILLS);
				break;
			case GIANT_SLUG:
			case HEAVY_TANK:
			case BUGSAURUS:
				GamePlatform::Instance().incrementValue(STAT_BIG_KILLS);
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
	return gameMap->stats.life;
}

void Level::updateCurrentTower()
{
	if (m_selectedTower == nullptr)
		return;

	const sf::Vector2f tileSize = Engine::Instance().options<GameOptions>()->tileSize();
	sf::Vector2f optionsPos = m_selectedTower->pos();
	const sf::Vector2i towerCell = Engine::Instance().options<GameOptions>()->camera()->posToCell(optionsPos);

	sf::Vector2f fixPos = sf::Vector2f(optionsPos.x,
									   optionsPos.y - tileSize.y);
	sf::Vector2f sellPos = sf::Vector2f(optionsPos.x - tileSize.x,
										optionsPos.y);
	sf::Vector2f upgradePos = sf::Vector2f(optionsPos.x + tileSize.x,
										   optionsPos.y);

	if (towerCell.y == 0)
		fixPos.y += tileSize.y * 2;

	if (towerCell.x == 0)
	{
		fixPos.x += tileSize.x;
		sellPos.x += tileSize.x;
		sellPos.y += tileSize.y;
		if (towerCell.y == Engine::Instance().options<GameOptions>()->cursor()->getMaxCell().y - 1)
		{
//TODO fix
		}
	}
	if (towerCell.x == Engine::Instance().options<GameOptions>()->cursor()->getMaxCell().x - 1)
	{
		fixPos.x -= tileSize.x;
		upgradePos.x -=  tileSize.x;
		upgradePos.y += tileSize.y;
		if (towerCell.y == Engine::Instance().options<GameOptions>()->cursor()->getMaxCell().y - 1)
		{

		}
	}
	const float sellCost = Engine::Instance().options<GameOptions>()->panel()->getTowerSellCost(m_selectedTower);
	const float upgradeCost = Engine::Instance().options<GameOptions>()->panel()->getTowerUpgradeCost(m_selectedTower);

	sellCostText.setString(GlobalVariables::to_string_with_precision(sellCost, 1));
	upgradeCostText.setString(GlobalVariables::to_string_with_precision(upgradeCost, 1));

	sellSprite.setPosition(sellPos);
	upgradeSprite.setPosition(upgradePos);
	fixSprite.setPosition(fixPos);
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
	Engine::Instance().options<GameOptions>()->panel()->setCurrentIcon(ACTION_STATE::ABILITY_ACID);
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

	if (!tower->isActive())
		GamePlatform::Instance().unlock(ACHIEVEMENT_SELL_INACTIVE_TOWER);

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
	if (waves.empty())
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

		const int spawnNumber = rand() % spawnPoints.size();
		const ActionPoint actionPoint = spawnPoints.at(spawnNumber);
		spawn(actionPoint.rect.getPosition(), type, wave.protection, actionPoint.direction);
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

	for(Enemy* enemy : enemies)
	{
		if (enemy->isStopped())
			continue;
		enemyMove(enemy);
	}
}

void Level::enemyMove(Enemy *enemy)
{
	enemy->moveEnemy();

	for(const ActionPoint& actionPoint : endPoints)
	{
		const sf::FloatRect endRect = sf::FloatRect(actionPoint.rect.getPosition(),
													 actionPoint.rect.getSize());
		if (endRect.contains(enemy->enemyPos()))
			return;
	}

	const sf::Vector2i cell = Engine::Instance().options<GameOptions>()->camera()->posToCellMap(enemy->enemyPos());

	if (enemy->getLastUp())
	{
		sf::Vector2f nextPos = enemy->enemyPos();
		nextPos.y -= enemy->actualMoveStep().y;
		const sf::Vector2i newCell = Engine::Instance().options<GameOptions>()->camera()->posToCellMap(nextPos);
		const int newDirection = getTileDirectionByCell(newCell);
		if (newDirection == Map::RIGHT || newDirection == Map::LEFT)
			return;
	}

	if (cell == enemy->getLastCell())
		return;

	enemy->setLastCell(cell);
	const int direction = getTileDirectionByCell(cell);
	enemy->moveNext(direction);
}

Tower *Level::getTowerAtPos(const sf::Vector2f &pos) const
{
	for(Tower* tower : towers)
		if (tower->pos() == pos)
			return tower;
	return nullptr;
}

bool Level::canAddTower(const sf::Vector2i &cell, bool isEnergy) const
{
	const int direction = getTileDirectionByCell(cell);
	if (direction != Map::STAY)
		return false;
	Tower *tower = this->getTowerAtPos(Engine::Instance().options<GameOptions>()->camera()->cellToPosMap(cell));
	if (tower != nullptr)
		return false;
	bool canCreate = true;
	if (!isEnergy)
	{
		const sf::Vector2f targetPos = Engine::Instance().options<GameOptions>()->camera()->cellToPosMap(
					cell + sf::Vector2i(1, 1));
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

void Level::updateBuildCells(TOWER_TYPES type)
{
	for(sf::RectangleShape &rect : buildCells)
	{
		const sf::Vector2f pos = sf::Vector2f(rect.getGlobalBounds().left,
											  rect.getGlobalBounds().top) +
				sf::Vector2f(rect.getGlobalBounds().width/2,
							 rect.getGlobalBounds().height/2);
		const sf::Vector2i cell = Engine::Instance().options<GameOptions>()->camera()->posToCell(pos);
		const bool canAdd = canAddTower(sf::Vector2i(cell.x * 2, cell.y * 2), type == POWER);
		rect.setFillColor(canAdd ? PowerTower::POWER_TOWER_AREA_COLOR : GameCursor::INACTIVE_TOWER_AREA_COLOR);
	}
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
	{
		Engine::Instance().soundManager()->endBackgroundSound();
		Engine::Instance().soundManager()->playOnce(GAME_SOUND::WIN);

		if (!attackTowerBuilded)
			GamePlatform::Instance().unlock(ACHIEVEMENT_COMPLETE_LEVEL_WITHOUT_TOWERS);
		if (!abilityActivated)
			GamePlatform::Instance().unlock(ACHIEVEMENT_COMPLETE_LEVEL_WITHOUT_ABILITIES);
	}
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
		currentTowerRadius.setRadius(m_selectedTower->actualRadius() * GameOptions::MAP_CELL_SIZE);
		currentTowerRadius.setScale(Engine::Instance().settingsManager()->getScaleFactor());
		currentTowerRadius.setPosition(m_selectedTower->pos());
		currentTowerRadius.setOrigin(currentTowerRadius.getRadius() - GameOptions::MAP_CELL_SIZE,
									 currentTowerRadius.getRadius() - GameOptions::MAP_CELL_SIZE);
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
	if (fixSprite.getGlobalBounds().contains(gPos))
		return FIX;
	return READY;
}

void Level::setSelectedTower(Tower *tower)
{
	m_selectedTower = tower;
	updateCurrentTower();
}

void Level::updateActionPoint(GameObject *object, const sf::Vector2f& pos, bool isSpawn)
{
	const sf::Vector2f mapSize = sf::Vector2f(gameMap->width * Engine::Instance().options<GameOptions>()->mapTileSize().x,
											  gameMap->height * Engine::Instance().options<GameOptions>()->mapTileSize().y);
	const int centerX = mapSize.x/2;
	const int centerY = mapSize.y/2;

	const float x0 = 0;
	const float x1 = mapSize.x;
	const float y0 = 0;
	const float y1 = mapSize.y;
	object->sprite.setRotation(0);

	sf::Vector2f resultPos = sf::Vector2f(pos.x * Engine::Instance().settingsManager()->getScaleFactor().x,
										  pos.y * Engine::Instance().settingsManager()->getScaleFactor().y);
	if (isSpawn)
	{
		if (pos.x <= x0 || pos.x >= x1)
		{
			//x
			if (pos.x > centerX)
			{
				//-x
				resultPos.x -= Engine::Instance().options<GameOptions>()->tileSize().x + Engine::Instance().options<GameOptions>()->mapTileSize().x;
			}
			else
			{
				//+x
				resultPos.x += Engine::Instance().options<GameOptions>()->tileSize().x;
			}
			resultPos.y += Engine::Instance().options<GameOptions>()->mapTileSize().y;

			object->sprite.setRotation(-90);
			resultPos.y += Engine::Instance().options<GameOptions>()->tileSize().y;
		}
		else if (pos.y <= y0 || pos.y >= y1)
		{
			//y
			if (pos.y > centerY)
			{
				//-y
				resultPos.y -= Engine::Instance().options<GameOptions>()->tileSize().y + Engine::Instance().options<GameOptions>()->mapTileSize().y;
			}
			else
			{
				//+y
				resultPos.y += Engine::Instance().options<GameOptions>()->tileSize().y;
			}
			resultPos.x += Engine::Instance().options<GameOptions>()->mapTileSize().x;
		}
	}
	else
	{
		if (pos.x <= x0 || pos.x >= x1)
		{
			//x
			if (pos.x < centerX)
			{
				//-x
				resultPos.x += Engine::Instance().options<GameOptions>()->mapTileSize().x + Engine::Instance().options<GameOptions>()->tileSize().x;
			}
			else
			{
				//+x
				resultPos.x -= Engine::Instance().options<GameOptions>()->mapTileSize().x + Engine::Instance().options<GameOptions>()->tileSize().x;
			}
			resultPos.y += Engine::Instance().options<GameOptions>()->mapTileSize().y;

			object->sprite.setRotation(-90);
			resultPos.y += Engine::Instance().options<GameOptions>()->tileSize().y;
		}
		else if (pos.y <= y0 || pos.y >= y1)
		{
			//y
			if (pos.y > centerY)
			{
				//-y
				resultPos.y -= Engine::Instance().options<GameOptions>()->mapTileSize().y + Engine::Instance().options<GameOptions>()->mapTileSize().y;
			}
			else
			{
				//+y
				resultPos.y += Engine::Instance().options<GameOptions>()->tileSize().y;
			}
			resultPos.x += Engine::Instance().options<GameOptions>()->mapTileSize().x;
		}
	}
	object->setPos(resultPos);
}
/*
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
*/
void Level::updateUpgrade()
{
	if (m_selectedTower == nullptr)
		return;

	upgradeSprite.setColor(sf::Color::White);
	const float cost = Engine::Instance().options<GameOptions>()->panel()->getTowerUpgradeCost(m_selectedTower);
	if (money < cost)
		upgradeSprite.setColor(EngineDefs::GrayColor);
}

void Level::clearCursor()
{
	setSelectedTower(nullptr);
	m_actionState = READY;
	Engine::Instance().options<GameOptions>()->cursor()->deactivate();
	showBuildRects = false;
}

std::vector<Tower *> Level::getAllTowers() const
{
	return towers;
}

unsigned int Level::getCurrentWave() const
{
	return currentWave;
}
#ifndef RELEASE_BUILD
void Level::test()
{
	changeState(LEVEL_STATE::WIN);
}

void Level::giveMeMoney()
{
	money += 1000;
}
#endif
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

	towersRegress->setEnabled(gameMap->stats.regress.enabled);
	smoke->setEnabled(gameMap->stats.smoke.enabled);
	moneyDrain->setEnabled(gameMap->stats.moneyDrain.enabled);
	mapExplosion->setEnabled(gameMap->stats.explosions.enabled);

	towersRegress->resetTimers();
	smoke->resetTimers();
	moneyDrain->resetTimers();
	mapExplosion->resetTimers();

	for(Tower* tower : towers)
		tower->resume();
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

Animation* Level::addAnimation(const TextureType& texture_id,
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
	return animation;
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

//		for (unsigned int tile = 0; tile < gameMap->layers[layer].tiles.size(); tile++)
//			target->draw(gameMap->layers[layer].tiles[tile].sprite, shader);

		sf::RenderStates states;
		states.texture = &Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::TILES);
		states.shader = shader;
		target->draw(gameMap->layers[layer].vertices, states);

		for(LevelObject *object : objects)
		{
			if (object->layer() == layer)
				object->draw(target);
		}
	}
	if (gameMap->stats.smoke.enabled)
		target->draw(smokeRect, shadersFactory->getShader(OBJECTS::SMOKE));

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

	if (showBuildRects)
		for(const sf::RectangleShape& rect : buildCells)
		{
			if (rect.getFillColor() == GameCursor::INACTIVE_TOWER_AREA_COLOR)
				continue;
			target->draw(rect);
		}

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
//		if (m_selectedTower->isDowngraded())
			target->draw(fixSprite);
		if (m_selectedTower->level() < Tower::ABILITY_LEVEL)
		{
			target->draw(upgradeSprite);
			target->draw(upgradeCostText);
		}
	}
}

Enemy* Level::spawn(const sf::Vector2f& pos, ENEMY_TYPES type, float protection, int moveDirection)
{
	Enemy *enemy = EnemiesFactory::createEnemy(type, pos);
	enemy->protect(protection);
	enemy->moveNext(moveDirection);
	enemies.push_back(enemy);
	return enemy;
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
	return Tile(Map::STAY);
}

void Level::chooseCurrent()
{
	choose(Engine::Instance().options<GameOptions>()->cursor()->cell(),
		   Engine::Instance().options<GameOptions>()->cursor()->inPanel());
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
	choose(Engine::Instance().options<GameOptions>()->camera()->posToCell(pos),
		   Engine::Instance().options<GameOptions>()->cursor()->inPanel());
}

void Level::choose(const sf::Vector2i &cell, bool inPanel)
{
//	for(Enemy *enemy : enemies)
//	{
//		if (enemy->type() == ENEMY_TYPES::MECHSPIDER)
//		{
//			ShutdownTowerAbility *web = static_cast<ShutdownTowerAbility*>(enemy->getAbility());
//			if (web == nullptr || web->getTarget() == nullptr)
//				continue;
//			const sf::Vector2i towerCell = Engine::Instance().options<GameOptions>()->camera()->posToCell(
//						web->getTarget()->getCenter());
//			if (towerCell == cell)
//			{
//				web->stop();
//				return;
//			}
//		}
//	}

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
						Balance::Instance().getTowerStats(type).cost + TowersCounter::Instance().powerTowerCount * Balance::Instance().getPowerTowerCostOffset() :
						Balance::Instance().getTowerStats(type).cost;
			if (money < cost)
				return;
			if (!Engine::Instance().options<GameOptions>()->panel()->isTowerIconActive(type))
				return;

			const float radius = Balance::Instance().getTowerStats(type).radius * GameOptions::MAP_CELL_SIZE;
			Engine::Instance().options<GameOptions>()->cursor()->activateTower(radius, type);
			updateBuildCells(type);
			showBuildRects = true;
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
		case ABILITY_ACID:
		{
			if (!Engine::Instance().options<GameOptions>()->panel()->isAbilityIconActive(ABILITY_ACID))
				return;
			if (!abilities->acidAbility->isReady())
				return;

			abilities->acidAbility->setUp();
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
			abilityActivated = true;
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
						Balance::Instance().getTowerStats(type).cost + TowersCounter::Instance().powerTowerCount * 10 :
						Balance::Instance().getTowerStats(type).cost;

			if (cost > money)
				return;

			if (!canAddTower(sf::Vector2i(cell.x * 2, cell.y * 2), type == TOWER_TYPES::POWER))
				return;

			showBuildRects = false;
			Tower *tower = TowersFactory::createTower(type, pos);
			if (tower == nullptr)
				return;
			if (m_state == WAIT_READY)
				tower->pause();
			GamePlatform::Instance().incrementValue(STAT_TOWERS_BUILDED);
			Engine::Instance().soundManager()->playOnce(GAME_SOUND::SETUP);
			towers.push_back(tower);
			money -= cost;
			if (type != POWER)
				attackTowerBuilded = true;
			Engine::Instance().options<GameOptions>()->panel()->updatePanel();
		}
			break;
		case ABILITY_ACID:
			abilityActivated = true;
			abilities->acidAbility->activate();
			break;
		case ABILITY_BOMB:
		{
			abilities->bombAbility->activate();
			abilityActivated = true;
			moneyDrain->explosion(Engine::Instance().options<GameOptions>()->cursor()->getAbilityRect());
		}
			break;
		case ABILITY_FREEZE_BOMB:		
			abilityActivated = true;
			abilities->freezeBombAbility->activate();		
			break;
		case ABILITY_INCREASE_TOWER_ATTACK_SPEED:
			abilityActivated = true;
			abilities->increaseTowerAttackSpeedAbility->activate();		
			break;
		case ABILITY_INCREASE_TOWER_DAMAGE:
			abilityActivated = true;
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
		case FIX:
		{
			if (selectedTower->isDowngraded())
				selectedTower->setDowngrade(false);
		}
			break;
		default:
			break;
		}
		m_actionState = READY;
		Engine::Instance().options<GameOptions>()->cursor()->deactivate();
	}
}
