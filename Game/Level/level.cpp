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
#include "gamecursor.h"
#include "enemy.h"
#include "tower.h"
#include "projectile.h"
#include "instructions.h"
#include "gameability.h"
#include "mapeffects.h"
#include "levelobject.h"

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
	deadZone.setOutlineColor(Color::Red);
	deadZone.setFillColor(Color::Transparent);

	currentTowerRadius.setFillColor(GameCursor::TOWER_AREA_COLOR);
	currentTowerRect.setFillColor(PowerTower::POWER_TOWER_AREA_COLOR);

	spawnRect.setFillColor(Color::Transparent);
	endRect.setFillColor(Color::Transparent);

	sellSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::SELL_TEXTURE));
	sellSprite.setScale(Settings::Instance().getScaleFactor());

	upgradeSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::UPGRADE_TEXTURE));
	upgradeSprite.setScale(Settings::Instance().getScaleFactor());

	const Color textColor = Color(236,169,114);
	const Color textOutlineColor = Color(104,75,56);
	const float fieldIconsCharacterSize = 25;
	sellCostText.setFont(GlobalVariables::Instance().font());
	sellCostText.setFillColor(textColor);
	sellCostText.setOutlineColor(textOutlineColor);
	sellCostText.setOutlineThickness(1);
	sellCostText.setCharacterSize(fieldIconsCharacterSize);
	sellCostText.setScale(Settings::Instance().getScaleFactor());

	upgradeCostText.setFont(GlobalVariables::Instance().font());
	upgradeCostText.setFillColor(textColor);
	upgradeCostText.setStyle(Text::Bold);
	upgradeCostText.setOutlineColor(textOutlineColor);
	upgradeCostText.setOutlineThickness(1);
	upgradeCostText.setCharacterSize(fieldIconsCharacterSize);
	upgradeCostText.setScale(Settings::Instance().getScaleFactor());

	startSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::START_TEXTURE));
	startSprite.setScale(Settings::Instance().getScaleFactor());

	endSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::END_TEXTURE));
	endSprite.setScale(Settings::Instance().getScaleFactor());

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

void Level::draw(RenderTarget *const target)
{
	target->setView(*Engine::Instance().camera()->getView());
	drawLevel(target);
	target->draw(deadZone);
	target->draw(spawnRect);
	target->draw(endRect);
	if(m_state == Level::WAIT_READY)
	{
		target->draw(startSprite);
		target->draw(endSprite);
	}
	Engine::Instance().window()->setView(Engine::Instance().window()->getDefaultView());
	Engine::Instance().panel()->draw(target);
	Engine::Instance().instructions()->draw(target);
}

void Level::update()
{
	const bool playing = m_state == PLAYING;

	if (timer.check(GlobalVariables::FRAME_TIME))
	{
		for(Tower* tower : towers)
		{
			if (playing && tower->type() == POWER)
			{
				PowerTower *powerTower = static_cast<PowerTower*>(tower);
				if (powerTower->hasEnergy())
					money += powerTower->gain();
			}
			else
				tower->action(enemies);
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
		showAnimations();
	}
	for(LevelObject *object : objects)
		object->update();
	Engine::Instance().panel()->updatePanel();

	shadersFactory->update();
}

void Level::startMission(const unsigned int n)
{
	currentWave = 0;
	Engine::Instance().panel()->updateWaveText();
	m_state = WAIT_READY;
	waves = EnemiesFactory::generateEnemies(n);
	gameMap = Engine::Instance().getMap(n);
	Engine::Instance().panel()->setProgressMax(currentProgress());
	life = gameMap->life;
	Engine::Instance().panel()->setLifeMax(life);

	money = gameMap->money;
	Engine::Instance().panel()->updatePanel();

	SoundController::Instance().startBackgroundSound("sounds/map1.ogg");

	Engine::Instance().cursor()->setMaxCells(gameMap->width/2, gameMap->height/2);
	Engine::Instance().panel()->initMission(n);
	updateStartEndPos(gameMap->spawnPos, Vector2f(gameMap->endRect.left, gameMap->endRect.top));

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
	deadZone.setPosition(minPos);
	deadZone.setSize(Vector2f(deadZoneSize.x + fabs(minPos.x) * 2,
							  deadZoneSize.y + fabs(minPos.y) * 2));


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

	for(const Map::MapObject& mapObject : gameMap->objects)
	{
		Vector2f startPos = mapObject.pos;
		startPos.x *= Settings::Instance().getScaleFactor().x;
		startPos.y *= Settings::Instance().getScaleFactor().y;
		LevelObject *object = Engine::Instance().createObject(mapObject.type, startPos);
		if (object != nullptr)
		{
			object->setLayer(mapObject.layer);
			Shader *shader = shadersFactory->getShader(mapObject.shader_type);
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
	if (currentWave == waves.size() && enemies.empty())
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
			money++;
			Engine::Instance().panel()->updatePanel();
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

	Vector2f optionsPos = m_selectedTower->pos();
	optionsPos.x -= GlobalVariables::Instance().tileSize().x;
	sellSprite.setPosition(optionsPos);
	const Vector2i towerCell = Engine::Instance().camera()->posToCell(m_selectedTower->pos());
	if (towerCell.x == 0)
	{
		if (towerCell.y == 0)
			sellSprite.setPosition(optionsPos.x + GlobalVariables::Instance().tileSize().x,
								   optionsPos.y + GlobalVariables::Instance().tileSize().x);
		else
			sellSprite.setPosition(optionsPos.x + GlobalVariables::Instance().tileSize().x,
								   optionsPos.y - GlobalVariables::Instance().tileSize().x);
	}
	optionsPos.x += GlobalVariables::Instance().tileSize().x * 2;
	upgradeSprite.setPosition(optionsPos);
	if (towerCell.x == Engine::Instance().cursor()->getMaxCell().x - 1)
	{
		if (towerCell.y == 0)
			upgradeSprite.setPosition(optionsPos.x - GlobalVariables::Instance().tileSize().x,
									  optionsPos.y + GlobalVariables::Instance().tileSize().x);
		else
			upgradeSprite.setPosition(optionsPos.x - GlobalVariables::Instance().tileSize().x,
									  optionsPos.y - GlobalVariables::Instance().tileSize().x);
	}
	const float sellCost = Engine::Instance().panel()->getTowerSellCost(m_selectedTower);
	const float upgradeCost = Engine::Instance().panel()->getTowerUpgradeCost(m_selectedTower);

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

void Level::blinkStartEnd(bool state)
{
	startSprite.setColor(state?Color::Yellow:Color::Red);
	endSprite.setColor(state?Color::Yellow:Color::Red);
}

void Level::activateBombAbility()
{
	Engine::Instance().panel()->setCurrentIcon(ACTION_STATE::ABILITY_BOMB);
	choose(Vector2i(0,0), true);
}

void Level::activateFreezeBombAbility()
{
	Engine::Instance().panel()->setCurrentIcon(ACTION_STATE::ABILITY_FREEZE_BOMB);
	choose(Vector2i(0,0), true);
}

void Level::activateVenomAbility()
{
	Engine::Instance().panel()->setCurrentIcon(ACTION_STATE::ABILITY_VENOM);
	choose(Vector2i(0,0), true);
}

void Level::activateIncreaseTowerDamageAbility()
{
	Engine::Instance().panel()->setCurrentIcon(ACTION_STATE::ABILITY_INCREASE_TOWER_DAMAGE);
	choose(Vector2i(0,0), true);
}

void Level::activateIncreaseTowerAttackSpeedAbility()
{
	Engine::Instance().panel()->setCurrentIcon(ACTION_STATE::ABILITY_INCREASE_TOWER_ATTACK_SPEED);
	choose(Vector2i(0,0), true);
}

void Level::activateStopAbility()
{
	Engine::Instance().panel()->setCurrentIcon(ACTION_STATE::ABILITY_STOP);
	choose(Vector2i(0,0), true);
}

void Level::checkRespawn()
{
	if (abilities->stopAblity->isActive())
		return;
	if (currentWave == waves.size())
		return;

	Wave wave = waves.at(currentWave);
	const int respawnOffset = static_cast<int>(wave.respawnTime * 0.15);
	const int timeOffset = rand() % (respawnOffset + 1) - respawnOffset/2;
	const int resultTime = timeOffset + wave.respawnTime;
	if (spawnTimer.check(resultTime))
	{
		const int n = rand() % wave.spawnEnemies.size();
		ENEMY_TYPES type = wave.spawnEnemies.at(n);
		wave.spawnEnemies.erase(find(wave.spawnEnemies.begin(), wave.spawnEnemies.end(), type));
		waves[currentWave] = wave;

		spawn(spawnRect.getPosition(), type, wave.protection, gameMap->spawnDirection);
	}
	if (wave.spawnEnemies.empty())
	{
		currentWave++;
		Engine::Instance().panel()->updateWaveText();
	}
}

void Level::checkEnemyMove()
{
	if (abilities->stopAblity->isActive())
		return;

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
	shake->start();
	if (life <= 0.f)
	{
		life = 0.f;
		//game over
		changeState(LOSE);
	}
	Engine::Instance().panel()->updatePanel();
}

void Level::changeState(Level::LEVEL_STATE state)
{
	m_state = state;
}

void Level::updateRadius()
{
	if (m_selectedTower == nullptr)
		return;

	if (m_selectedTower->type() == POWER)
	{
		const FloatRect rect = static_cast<PowerTower*>(m_selectedTower)->getValidArea();
		currentTowerRect.setPosition(rect.left, rect.top);
		currentTowerRect.setSize(Vector2f(rect.width, rect.height));
	}
	else
	{
		currentTowerRadius.setRadius(m_selectedTower->actualRadius() * GlobalVariables::Instance().mapTileSize().x);
		currentTowerRadius.setPosition(m_selectedTower->pos());
		currentTowerRadius.setOrigin(currentTowerRadius.getRadius() - GlobalVariables::Instance().mapTileSize().x,
						 currentTowerRadius.getRadius() - GlobalVariables::Instance().mapTileSize().y);
	}
}

void Level::showAnimations()
{
	for(Animation *effect : effects)
		effect->update();
}

ACTION_STATE Level::isFieldButtons(const Vector2f &pos) const
{
	const Vector2f gPos = pos + Vector2f(1, 1);

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

void Level::updateStartEndPos(const Vector2f &startPos, const Vector2f &endPos)
{
	const int centerX = Settings::Instance().getResolution().x/2;
	const int centerY = Settings::Instance().getResolution().y/2;

	const float x0 = 0;
	const float x1 = Settings::Instance().getResolution().x;
	const float y0 = 0;
	const float y1 = Settings::Instance().getResolution().y;

	Vector2f resultStartPos = Vector2f(startPos.x * Settings::Instance().getScaleFactor().x,
									   startPos.y * Settings::Instance().getScaleFactor().y);
	Vector2f resultEndPos = Vector2f(endPos.x * Settings::Instance().getScaleFactor().x,
									 endPos.y * Settings::Instance().getScaleFactor().y);
	//start
	if (startPos.x <= x0 || startPos.x >= x1)
	{
		//x
		if (startPos.x > centerX)
		{
			//-x
			resultStartPos.x -= GlobalVariables::Instance().tileSize().x + GlobalVariables::Instance().mapTileSize().x;
		}
		else
		{
			//+x
			resultStartPos.x += GlobalVariables::Instance().tileSize().x;
		}
		resultStartPos.y += GlobalVariables::Instance().mapTileSize().y;
	}
	else if (startPos.y <= y0 || startPos.y >= y1)
	{
		//y
		if (startPos.y > centerY)
		{
			//-y
			resultStartPos.y -= GlobalVariables::Instance().tileSize().y + GlobalVariables::Instance().mapTileSize().y;
		}
		else
		{
			//+y
			resultStartPos.y += GlobalVariables::Instance().tileSize().y;
		}
		resultStartPos.x += GlobalVariables::Instance().mapTileSize().x;
	}
	//end
	if (endPos.x <= x0 || endPos.x >= x1)
	{
		//x
		if (endPos.x < centerX)
		{
			//-x
			resultEndPos.x += GlobalVariables::Instance().mapTileSize().x + GlobalVariables::Instance().tileSize().x;
		}
		else
		{
			//+x
			resultEndPos.x -= GlobalVariables::Instance().mapTileSize().x + GlobalVariables::Instance().tileSize().x;
		}
		resultEndPos.y += GlobalVariables::Instance().mapTileSize().y;
	}
	else if (endPos.y <= y0 || endPos.y >= y1)
	{
		//y
		if (endPos.y > centerY)
		{
			//-y
			resultEndPos.y -= GlobalVariables::Instance().mapTileSize().y + GlobalVariables::Instance().mapTileSize().y;
		}
		else
		{
			//+y
			resultEndPos.y += GlobalVariables::Instance().tileSize().y;
		}
		resultEndPos.x += GlobalVariables::Instance().mapTileSize().x;
	}
	startSprite.setPosition(resultStartPos);
	endSprite.setPosition(resultEndPos);
}

void Level::updateUpgrade()
{
	if (m_selectedTower == nullptr)
		return;

	upgradeSprite.setColor(Color::White);
	const float cost = Engine::Instance().panel()->getTowerUpgradeCost(m_selectedTower);
	if (money < cost)
		upgradeSprite.setColor(GlobalVariables::GrayColor);
}

unsigned int Level::getPowerTowersCount() const
{
	return m_powerTowersCount;
}

void Level::clearCursor()
{
	setSelectedTower(nullptr);
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

unsigned int Level::getCurrentWave() const
{
	return currentWave;
}

void Level::test()
{
	changeState(LEVEL_STATE::LOSE);
//	spawn(DOWN_TOWER_ENEMY);
//	spawn(MID_MEDIUM);
//	spawn(TELEPORT_ENEMY);
	//	spawn(REPAIR_ENEMY);
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
}

int Level::currentProgress() const
{
	int progress = 0;
	for(const Wave& wave : waves)
		progress += wave.spawnEnemies.size();
	return progress;
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

	Engine::Instance().cursor()->draw(target);

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
		if (m_selectedTower->level() <= 2)
		{
			target->draw(upgradeSprite);
			target->draw(upgradeCostText);
		}
	}
}

void Level::spawn(const Vector2f& pos, ENEMY_TYPES type, float protection, int moveDirection)
{
	Enemy *enemy = EnemiesFactory::createEnemy(type, pos);
	enemy->protect(protection, false);
	enemy->moveNext(moveDirection);
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
	const int id = getTileByCell(cell, gameMap->directionsLayer).id;
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

void Level::chooseByPos(const Vector2f &pos)
{
	const bool inPanel = pos.y > Engine::Instance().panel()->getBottomValue();
	choose(Engine::Instance().camera()->posToCell(pos), inPanel);
}

void Level::choose(const Vector2i &cell, bool inPanel)
{
	Tower *selectedTower = m_selectedTower;
	if (m_selectedTower != nullptr)
		setSelectedTower(nullptr);
	if (inPanel)
	{
		const ACTION_STATE currentState = Engine::Instance().panel()->getCurrentIcon();
		switch (currentState)
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
			if (!Engine::Instance().panel()->isAbilityIconActive(ABILITY_INCREASE_TOWER_ATTACK_SPEED))
				return;

			if (!abilities->increaseTowerAttackSpeedAbility->isReady())
				return;

			abilities->increaseTowerAttackSpeedAbility->setUp();
		}
			break;
		case ABILITY_INCREASE_TOWER_DAMAGE:
		{
			if (!Engine::Instance().panel()->isAbilityIconActive(ABILITY_INCREASE_TOWER_DAMAGE))
				return;

			if (!abilities->increaseTowerDamageAbility->isReady())
				return;

			abilities->increaseTowerDamageAbility->setUp();
		}
			break;
		case ABILITY_VENOM:
		{
			if (!Engine::Instance().panel()->isAbilityIconActive(ABILITY_VENOM))
				return;
			if (!abilities->venomAbility->isReady())
				return;

			abilities->venomAbility->setUp();
		}
			break;
		case ABILITY_BOMB:
		{
			if (!Engine::Instance().panel()->isAbilityIconActive(ABILITY_BOMB))
				return;

			if (!abilities->bombAbility->isReady())
				return;

			abilities->bombAbility->setUp();
		}
			break;
		case ABILITY_FREEZE_BOMB:
		{
			if (!Engine::Instance().panel()->isAbilityIconActive(ABILITY_FREEZE_BOMB))
				return;

			if (!abilities->freezeBombAbility->isReady())
				return;

			abilities->freezeBombAbility->setUp();
		}
			break;
		case ABILITY_STOP:
		{
			if (!Engine::Instance().panel()->isAbilityIconActive(ABILITY_STOP))
				return;

			if (!abilities->stopAblity->isReady())
				return;

			Engine::Instance().cursor()->swap();
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
		const Vector2f pos = Engine::Instance().camera()->cellToPos(cell);
		if (m_actionState == READY && selectedTower != nullptr)
		{
			const ACTION_STATE fieldState = isFieldButtons(pos);
			if (fieldState != READY)
			{
				m_actionState = fieldState;
				if (m_actionState == UPGRADE && selectedTower->level() > 2)
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
			const TOWER_TYPES type = Engine::Instance().cursor()->getTowerType();

			float cost = type == TOWER_TYPES::POWER ?
						TowersFactory::getTowerStats(type).cost + m_powerTowersCount * 10 :
						TowersFactory::getTowerStats(type).cost;

			if (cost > money)
				return;

			if (!canAddTower(Vector2i(cell.x * 2, cell.y * 2), type))
				return;

			if (type != POWER)
				highlightPowerTowersRadius(false);

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
			abilities->venomAbility->activate();		
			break;
		case ABILITY_BOMB:
		{
			abilities->bombAbility->activate();
			moneyDrain->explosion(Engine::Instance().cursor()->getAbilityRect());
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
		{
			if (selectedTower == nullptr)
				return;

			const float cost = Engine::Instance().panel()->getTowerSellCost(selectedTower);
			money += cost;
			towers.erase( remove( towers.begin(), towers.end(), selectedTower ), towers.end() );
			delete selectedTower;
			if (selectedTower->type() == POWER)
				m_powerTowersCount--;
			Engine::Instance().panel()->updatePanel();
			Engine::Instance().cursor()->swap();
		}
			break;
		case UPGRADE:
		{
			if (selectedTower == nullptr)
				break;

			const float cost = Engine::Instance().panel()->getTowerUpgradeCost(selectedTower);
			if (money < cost)
				break;

			if (selectedTower->level() < 3)
			{
				money -= cost;
				selectedTower->upgrade();
			}
			Engine::Instance().panel()->updatePanel();
			Engine::Instance().cursor()->swap();
		}
			break;
		default:
			break;
		}
		m_actionState = READY;
		Engine::Instance().cursor()->deactivate();
	}
}
