#include "gamepanel.h"
#include "level.h"
#include "camera.h"
#include "tower.h"
#include "lifebar.h"
#include "gamecursor.h"
#include "gameability.h"
#include "instructions.h"
#include "engine.h"
#include "enginedef.h"
#include "gamemanagers.h"
#include "managers.h"
#include "gameoptions.h"
#include "mainwindow.h"

GamePanel::GamePanel() :
	GameDrawable()
  ,waitBlink(false)
  ,currentCursorPos(0)
  ,m_drain(false)
  ,drainState(false)
{
	life = new LifeBar();
	progress = new LifeBar();

	const sf::Vector2f scaleFactor = Engine::Instance().settingsManager()->getScaleFactor();

	m_sprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::PANEL_TEXTURE));
	m_sprite.setScale(scaleFactor);

	info.setFont(Engine::Instance().fontManager()->font());
	info.setFillColor(GameOptions::primaryColor);
	info.setOutlineColor(GameOptions::secondaryColor);
	info.setOutlineThickness(2);
	info.setCharacterSize(Engine::Instance().fontManager()->getCharSize(25));
	info.setScale(scaleFactor);

	moneyCountText.setFont(Engine::Instance().fontManager()->font());
	moneyCountText.setFillColor(GameOptions::primaryColor);
	moneyCountText.setOutlineColor(GameOptions::secondaryColor);
	moneyCountText.setOutlineThickness(2);
	moneyCountText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(34));
	moneyCountText.setScale(scaleFactor);
	moneyCountText.setString("000");

	cursorSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::PANEL_CURSOR));
	cursorSprite.setScale(scaleFactor);

	currentIconRect.setSize(sf::Vector2f(GameOptions::CELL_SIZE, GameOptions::CELL_SIZE));
	currentIconRect.setFillColor(sf::Color(34,69,160,100));
	currentIconRect.setScale(scaleFactor);

	abilityBombSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::ABILITY_BOMB));
	abilityBombSprite.setScale(scaleFactor);

	abilityFreezeBombSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::ABILITY_FREEZE_BOMB));
	abilityFreezeBombSprite.setScale(scaleFactor);

	abilityAcidSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::ABILITY_ACID));
	abilityAcidSprite.setScale(scaleFactor);

	abilityIncreaseTowerDamageSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::ABILITY_INCREASE_TOWER_DAMAGE));
	abilityIncreaseTowerDamageSprite.setScale(scaleFactor);

	abilityIncreaseTowerAttackSpeedSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::ABILITY_INCREASE_TOWER_ATTACK_SPEED));
	abilityIncreaseTowerAttackSpeedSprite.setScale(scaleFactor);

	abilityStopSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::ABILITY_TIME_STOP));
	abilityStopSprite.setScale(scaleFactor);

	towerBaseSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::TOWER_BASE_ICON));
	towerBaseSprite.setScale(scaleFactor);

	towerLaserSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::TOWER_LASER_ICON));
	towerLaserSprite.setScale(scaleFactor);

	towerFreezeSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::TOWER_FREEZE_ICON));
	towerFreezeSprite.setScale(scaleFactor);

	towerRocketSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::TOWER_ROCKET_ICON));
	towerRocketSprite.setScale(scaleFactor);

	towerPowerSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::TOWER_POWER_ICON));
	towerPowerSprite.setScale(scaleFactor);

	towerImprovedSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::TOWER_IMPROVED_ICON));
	towerImprovedSprite.setScale(scaleFactor);

	readyText.setFont(Engine::Instance().fontManager()->font());
	readyText.setFillColor(GameOptions::primaryColor);
	readyText.setOutlineColor(GameOptions::secondaryColor);
	readyText.setOutlineThickness(2);
	readyText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(45));
	readyText.setScale(scaleFactor);
	readyText.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::START_GAME));

	actionsSprites.push_back(&towerBaseSprite);
	actionsSprites.push_back(&towerFreezeSprite);
	actionsSprites.push_back(&towerRocketSprite);
	actionsSprites.push_back(&towerPowerSprite);
	actionsSprites.push_back(&towerLaserSprite);
	actionsSprites.push_back(&towerImprovedSprite);
	actionsSprites.push_back(&abilityBombSprite);
	actionsSprites.push_back(&abilityFreezeBombSprite);
	actionsSprites.push_back(&abilityAcidSprite);
	actionsSprites.push_back(&abilityIncreaseTowerDamageSprite);
	actionsSprites.push_back(&abilityIncreaseTowerAttackSpeedSprite);
	actionsSprites.push_back(&abilityStopSprite);

	initCostText(&towerBaseCostText);
	initCostText(&towerFreezeCostText);
	initCostText(&towerRocketCostText);
	initCostText(&towerLaserCostText);
	initCostText(&towerPowerCostText);
	initCostText(&towerImprovedCostText);

	initDurationText(&abilityBombDurationText);
	initDurationText(&abilityFreezeBombDurationText);
	initDurationText(&abilityAcidDurationText);
	initDurationText(&abilityIncreaseTowerDamageDurationText);
	initDurationText(&abilityIncreaseTowerDamageDurationText);
	initDurationText(&abilityStopDurationText);

	towerBaseCostText.setString(GlobalVariables::to_string_with_precision(Balance::Instance().getTowerStats(BASE).cost, 0));
	towerFreezeCostText.setString(GlobalVariables::to_string_with_precision(Balance::Instance().getTowerStats(FREEZE).cost, 0));
	towerRocketCostText.setString(GlobalVariables::to_string_with_precision(Balance::Instance().getTowerStats(ROCKET).cost, 0));
	towerLaserCostText.setString(GlobalVariables::to_string_with_precision(Balance::Instance().getTowerStats(LASER).cost, 0));
	towerPowerCostText.setString(GlobalVariables::to_string_with_precision(Balance::Instance().getTowerStats(POWER).cost, 0));
	towerImprovedCostText.setString(GlobalVariables::to_string_with_precision(Balance::Instance().getTowerStats(IMPROVED).cost, 0));

	abilityBombDurationText.setString(GlobalVariables::to_string_with_precision(0, 0));
	abilityFreezeBombDurationText.setString(GlobalVariables::to_string_with_precision(0, 0));
	abilityAcidDurationText.setString(GlobalVariables::to_string_with_precision(0, 0));
	abilityIncreaseTowerDamageDurationText.setString(GlobalVariables::to_string_with_precision(0, 0));
	abilityIncreaseTowerAttackSpeedDurationText.setString(GlobalVariables::to_string_with_precision(0, 0));
	abilityStopDurationText.setString(GlobalVariables::to_string_with_precision(0, 0));

	initLimitText(&towerBaseLimitText);
	initLimitText(&towerFreezeLimitText);
	initLimitText(&towerRocketLimitText);
	initLimitText(&towerLaserLimitText);
	initLimitText(&towerPowerLimitText);
	initLimitText(&towerImprovedLimitText);

	moneyIcon.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::MONEY_ICON));
	moneyIcon.setScale(scaleFactor);

	lifeIcon.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::LIFE_ICON));
	lifeIcon.setScale(scaleFactor);

	waveText.setFont(Engine::Instance().fontManager()->font());
	waveText.setFillColor(GameOptions::primaryColor);
	waveText.setOutlineThickness(1);
	waveText.setOutlineColor(GameOptions::secondaryColor);
	waveText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(25));
	waveText.setScale(scaleFactor);

	m_bottomValue = 0;
	const int progressWidth = Engine::Instance().settingsManager()->getResolution().x * PROGRESS_WIDTH;
	progress->init(sf::Vector2i(progressWidth,
								LifeBar::LIFE_BAR_HEIGHT * Engine::Instance().settingsManager()->getScaleFactor().y), sf::Color::Red);
	life->init(sf::Vector2i(722 * scaleFactor.x, LIFE_BAR_HEIGHT * scaleFactor.y), GameOptions::secondaryColor);

	drainRect.setFillColor(sf::Color::Transparent);
	drainRect.setOutlineThickness(2);
	drainRect.setOutlineColor(sf::Color::Transparent);

	progress->setPos(sf::Vector2f(progressWidth, PROGRESS_OFFSET * scaleFactor.y));
	updatePos();
}

GamePanel::~GamePanel()
{

}

void GamePanel::draw(sf::RenderTarget * const target)
{
	//draw minimap
	rTexture.clear(sf::Color::Transparent);
	Engine::Instance().options<GameOptions>()->level()->drawLevel(&rTexture);
	rTexture.display();
	miniMapSprite.setTexture(rTexture.getTexture());

	//draw
	if(Engine::Instance().options<GameOptions>()->level()->getState() == Level::WAIT_READY &&
			!Engine::Instance().options<GameOptions>()->instructions()->isActive())
		target->draw(readyText);		

	target->draw(miniMapSprite);
	target->draw(m_sprite);
	target->draw(moneyCountText);
	target->draw(info);

	target->draw(abilityBombSprite);
	target->draw(abilityFreezeBombSprite);
	target->draw(abilityAcidSprite);
	target->draw(abilityIncreaseTowerDamageSprite);
	target->draw(abilityIncreaseTowerAttackSpeedSprite);
	target->draw(abilityStopSprite);

	target->draw(towerBaseSprite);
	target->draw(towerLaserSprite);
	target->draw(towerFreezeSprite);
	target->draw(towerRocketSprite);
	target->draw(towerPowerSprite);
	target->draw(towerImprovedSprite);

	target->draw(towerBaseCostText);
	target->draw(towerFreezeCostText);
	target->draw(towerRocketCostText);
	target->draw(towerLaserCostText);
	target->draw(towerPowerCostText);
	target->draw(towerImprovedCostText);

	Abilities *abilities = Engine::Instance().options<GameOptions>()->level()->getAbilities();
	if (!abilities->bombAbility->isReady())
		target->draw(abilityBombDurationText);
	if (!abilities->freezeBombAbility->isReady())
		target->draw(abilityFreezeBombDurationText);
	if (!abilities->acidAbility->isReady())
		target->draw(abilityAcidDurationText);
	if (!abilities->increaseTowerDamageAbility->isReady())
		target->draw(abilityIncreaseTowerDamageDurationText);
	if (!abilities->increaseTowerAttackSpeedAbility->isReady())
		target->draw(abilityIncreaseTowerAttackSpeedDurationText);
	if (!abilities->stopAblity->isReady())
		target->draw(abilityStopDurationText);

	if (!TowersCounter::Instance().canBuildBaseTower())
		target->draw(towerBaseLimitText);
	if (!TowersCounter::Instance().canBuildFreezeTower())
		target->draw(towerFreezeLimitText);
	if (!TowersCounter::Instance().canBuildRocketTower())
		target->draw(towerRocketLimitText);
	if (!TowersCounter::Instance().canBuildLaserTower())
		target->draw(towerLaserLimitText);
	if (!TowersCounter::Instance().canBuildPowerTower())
		target->draw(towerPowerLimitText);
	if (!TowersCounter::Instance().canBuildImprovedTower())
		target->draw(towerImprovedLimitText);

	target->draw(moneyIcon);
	target->draw(lifeIcon);

	progress->draw(target);
	life->draw(target);

	target->draw(waveText);
	if (m_drain)
		target->draw(drainRect);

	if (Engine::Instance().options<GameOptions>()->cursor()->inPanel())
	{
		if (currentCursorPos != PANEL_POS_ID)
		{
			target->draw(currentIconRect);
			target->draw(cursorSprite);
		}
	}
}

void GamePanel::update()
{	
	if(Engine::Instance().options<GameOptions>()->level()->getState() == Level::WAIT_READY)
	{
		if (blinkTimer.check(BLINK_TIME))
		{
			waitBlink = !waitBlink;
			readyText.setFillColor(waitBlink ? GameOptions::alternativePrimaryColor : GameOptions::primaryColor);
		}
	}
	if (m_drain)
	{
		if (drainBlinkTimer.check(100))
		{
			drainState = !drainState;
			drainRect.setOutlineColor(drainState ? sf::Color::Red : sf::Color::Transparent);
		}
	}
	updateEnableTowers();
	updateEnableAbilities();
}

void GamePanel::updatePanel()
{
	const int money = static_cast<int>(Engine::Instance().options<GameOptions>()->level()->getMoneyCount());
	moneyCountText.setString(sf::String(std::to_string(money)));
	drainRect.setSize(sf::Vector2f(moneyCountText.getGlobalBounds().width,
							   moneyCountText.getGlobalBounds().height*2));

	const float progressValue = static_cast<float>(Engine::Instance().options<GameOptions>()->level()->currentProgress()) / m_progressMax;
	progress->setValue(progressValue);
	const float lifeValue = static_cast<float>(Engine::Instance().options<GameOptions>()->level()->getLifeCount()) / m_lifeMax;
	life->setValue(lifeValue);

	towerPowerCostText.setString(
				GlobalVariables::to_string_with_precision(
					Balance::Instance().getTowerStats(POWER).cost + TowersCounter::Instance().powerTowerCount * Balance::Instance().getPowerTowerCostOffset(), 0));

	updateCursor();
	updateInfo();
}

int GamePanel::cellsCount() const
{
	return static_cast<int>(-1 + m_sprite.getGlobalBounds().height / Engine::Instance().options<GameOptions>()->tileSize().y);
}

ACTION_STATE GamePanel::getCurrentIcon() const
{
	switch (currentCursorPos)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		return ACTION_STATE::ADD_TOWER;
	case 6:
		return ACTION_STATE::ABILITY_BOMB;
	case 7:
		return ACTION_STATE::ABILITY_FREEZE_BOMB;
	case 8:
		return ACTION_STATE::ABILITY_ACID;
	case 9:
		return ACTION_STATE::ABILITY_INCREASE_TOWER_DAMAGE;
	case 10:
		return ACTION_STATE::ABILITY_INCREASE_TOWER_ATTACK_SPEED;
	case 11:
		return ACTION_STATE::ABILITY_STOP;
	default:
		break;
	}
	return ACTION_STATE::READY;
}

TOWER_TYPES GamePanel::currentTower() const
{
	switch (currentCursorPos)
	{
	case 0:
		return BASE;
	case 1:
		return FREEZE;
	case 2:
		return ROCKET;
	case 3:
		return POWER;
	case 4:
		return LASER;
	case 5:
		return IMPROVED;
	default:
		break;
	}
	return POWER;
}

sf::Vector2f GamePanel::updatePos()
{
	const sf::Vector2f nullPos = sf::Vector2f(0, Engine::Instance().settingsManager()->getResolution().y -
											  m_sprite.getGlobalBounds().height);

	const sf::Vector2f scaleFactor = Engine::Instance().settingsManager()->getScaleFactor();
	const sf::Vector2f iconSize = sf::Vector2f(ICON_SIZE * scaleFactor.x, ICON_SIZE * scaleFactor.y);

	const float top_offset = 40 * scaleFactor.y;
	const float left_offset = 40 * scaleFactor.x;
	const float icon_offset = 24 * scaleFactor.x;
	const float text_offset = 338 * scaleFactor.x;
	const float panel_offset = PANEL_OFFSET * scaleFactor.y;

	m_bottomValue = nullPos.y + panel_offset;

	sf::Vector2f pos = sf::Vector2f(ceil(nullPos.x), ceil(nullPos.y));
	m_sprite.setPosition(pos);

	pos.y += panel_offset;
	pos.x += left_offset;
	pos.y += top_offset;

	sf::Vector2f secondRow = sf::Vector2f(pos.x, pos.y + iconSize.y + icon_offset);
	moneyIcon.setPosition(secondRow);
	secondRow.x += iconSize.x;
	moneyCountText.setPosition(secondRow.x,
							   secondRow.y + moneyIcon.getGlobalBounds().height/2 - moneyCountText.getGlobalBounds().height/2);
	secondRow.x += 178 * scaleFactor.x;
	secondRow.x += icon_offset;
	lifeIcon.setPosition(secondRow);
	secondRow.x += iconSize.x;
	secondRow.y += lifeIcon.getGlobalBounds().height/2 - LIFE_BAR_HEIGHT/2 * scaleFactor.y;
	life->setPos(secondRow);

	drainRect.setPosition(moneyCountText.getGlobalBounds().left,
						  moneyCountText.getGlobalBounds().top);

	//towers
	towerBaseSprite.setPosition(pos);
	pos.x += iconSize.x;
	pos.x += icon_offset;

	towerFreezeSprite.setPosition(pos);
	pos.x += iconSize.x;
	pos.x += icon_offset;

	towerRocketSprite.setPosition(pos);
	pos.x += iconSize.x;
	pos.x += icon_offset;

	towerPowerSprite.setPosition(pos);
	pos.x += iconSize.x;
	pos.x += icon_offset;

	towerLaserSprite.setPosition(pos);
	pos.x += iconSize.x;
	pos.x += icon_offset;

	towerImprovedSprite.setPosition(pos);
	pos.x += iconSize.x;
	pos.x += left_offset;


	//abilities
	abilityBombSprite.setPosition(pos);
	pos.x += iconSize.x;
	pos.x += icon_offset;

	abilityFreezeBombSprite.setPosition(pos);
	pos.x += iconSize.x;
	pos.x += icon_offset;

	abilityAcidSprite.setPosition(pos);
	pos.x += iconSize.x;
	pos.x += icon_offset;

	abilityIncreaseTowerDamageSprite.setPosition(pos);
	pos.x += iconSize.x;
	pos.x += icon_offset;

	abilityIncreaseTowerAttackSpeedSprite.setPosition(pos);
	pos.x += iconSize.x;
	pos.x += icon_offset;

	abilityStopSprite.setPosition(pos);
	pos.x += iconSize.x;	
	pos.x += left_offset;

	//text
	pos.y = nullPos.y + 24 * scaleFactor.y + panel_offset;
	info.setPosition(pos.x + 8 * scaleFactor.x, pos.y - 6 * scaleFactor.y);
	pos.x += text_offset;
	pos.x += left_offset;

	pos.x += icon_offset;
	pos.y = nullPos.y + 43 * scaleFactor.y;

	readyText.setPosition(sf::Vector2f(Engine::Instance().settingsManager()->getResolution().x/2 - readyText.getGlobalBounds().width/2,
								   Engine::Instance().settingsManager()->getResolution().y/2 + readyText.getGlobalBounds().height/2));
	waveText.setPosition(progress->pos() + sf::Vector2f(0, 16 * scaleFactor.y));

	towerBaseCostText.setPosition(towerBaseSprite.getPosition());
	towerFreezeCostText.setPosition(towerFreezeSprite.getPosition());
	towerRocketCostText.setPosition(towerRocketSprite.getPosition());
	towerLaserCostText.setPosition(towerLaserSprite.getPosition());
	towerPowerCostText.setPosition(towerPowerSprite.getPosition());
	towerImprovedCostText.setPosition(towerImprovedSprite.getPosition());

	abilityBombDurationText.setPosition(abilityBombSprite.getPosition());
	abilityFreezeBombDurationText.setPosition(abilityFreezeBombSprite.getPosition());
	abilityAcidDurationText.setPosition(abilityAcidSprite.getPosition());
	abilityIncreaseTowerDamageDurationText.setPosition(abilityIncreaseTowerDamageSprite.getPosition());
	abilityIncreaseTowerAttackSpeedDurationText.setPosition(abilityIncreaseTowerAttackSpeedSprite.getPosition());
	abilityStopDurationText.setPosition(abilityStopSprite.getPosition());

	const sf::Vector2f halfCellY = sf::Vector2f(Engine::Instance().options<GameOptions>()->tileSize().x/2 - towerBaseLimitText.getGlobalBounds().width/2,
												Engine::Instance().options<GameOptions>()->tileSize().y/2 - towerBaseLimitText.getGlobalBounds().height/2);
	towerBaseLimitText.setPosition(towerBaseSprite.getPosition() + halfCellY);
	towerFreezeLimitText.setPosition(towerFreezeSprite.getPosition() + halfCellY);
	towerRocketLimitText.setPosition(towerRocketSprite.getPosition() + halfCellY);
	towerLaserLimitText.setPosition(towerLaserSprite.getPosition() + halfCellY);
	towerPowerLimitText.setPosition(towerPowerSprite.getPosition() + halfCellY);
	towerImprovedLimitText.setPosition(towerImprovedSprite.getPosition() + halfCellY);

	miniMapSprite.setPosition(pos);

	return pos;
}

void GamePanel::setLifeMax(int lifeMax)
{
	m_lifeMax = lifeMax;
}

int GamePanel::getProgressMax() const
{
	return m_progressMax;
}

void GamePanel::updateWaveText()
{
	waveText.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::WAVE) +
				L" #" + std::to_wstring(Engine::Instance().options<GameOptions>()->level()->getCurrentWave() + 1));
}

float GamePanel::getTowerUpgradeCost(Tower *tower) const
{
	if (tower == nullptr)
		return 0.f;

	const TOWER_TYPES type = tower->type();
	const float cost = type == TOWER_TYPES::POWER ?
				Balance::Instance().getTowerStats(type).cost +
				TowersCounter::Instance().powerTowerCount * Balance::Instance().getPowerTowerCostOffset() :
				Balance::Instance().getTowerStats(type).cost;
	float result = 0;
	float price = cost;
	float lastPrice = cost;
	const float k = 1 + Balance::Instance().getTowerUpgradeGain() * Tower::UPGRADE_PRICE_MODIFIER;
	const int level = tower->level() + 1;
	for (int i = 1; i < level; ++i)
	{
		price *= k;
		result = price - lastPrice;
		lastPrice = price;
	}
	return result;
}

float GamePanel::getTowerSellCost(Tower *tower) const
{
	const TOWER_TYPES type = tower->type();
	float cost = type == TOWER_TYPES::POWER ?
				tower->data().cost + (TowersCounter::Instance().powerTowerCount - 1) * Balance::Instance().getPowerTowerCostOffset() :
				tower->data().cost;
	cost /= 2;
	return cost;
}

void GamePanel::updateAbilitiesDuration()
{
	Abilities *abilities = Engine::Instance().options<GameOptions>()->level()->getAbilities();

	abilityBombDurationText.setString(std::to_string(abilities->bombAbility->timeLeft()));
	abilityFreezeBombDurationText.setString(std::to_string(abilities->freezeBombAbility->timeLeft()));
	abilityAcidDurationText.setString(std::to_string(abilities->acidAbility->timeLeft()));
	abilityIncreaseTowerDamageDurationText.setString(std::to_string(abilities->increaseTowerDamageAbility->timeLeft()));
	abilityIncreaseTowerAttackSpeedDurationText.setString(std::to_string(abilities->increaseTowerAttackSpeedAbility->timeLeft()));
	abilityStopDurationText.setString(std::to_string(abilities->stopAblity->timeLeft()));
}

void GamePanel::updateEnableTowers()
{
	const float money = Engine::Instance().options<GameOptions>()->level()->getMoneyCount();

	float cost = Balance::Instance().getTowerStats(TOWER_TYPES::BASE).cost;
	if (money < cost || !TowersCounter::Instance().canBuildBaseTower())
		towerBaseSprite.setColor(EngineDefs::GrayColor);
	else
		towerBaseSprite.setColor(sf::Color::White);

	cost = Balance::Instance().getTowerStats(TOWER_TYPES::LASER).cost;
	if (money < cost || !iconsAvaliable.isLaserEnabled || !TowersCounter::Instance().canBuildLaserTower())
		towerLaserSprite.setColor(EngineDefs::GrayColor);
	else
		towerLaserSprite.setColor(sf::Color::White);

	cost = Balance::Instance().getTowerStats(TOWER_TYPES::FREEZE).cost;
	if (money < cost || !iconsAvaliable.isFreezeEnabled || !TowersCounter::Instance().canBuildFreezeTower())
		towerFreezeSprite.setColor(EngineDefs::GrayColor);
	else
		towerFreezeSprite.setColor(sf::Color::White);

	cost = Balance::Instance().getTowerStats(TOWER_TYPES::ROCKET).cost;
	if (money < cost || !iconsAvaliable.isRocketEnabled || !TowersCounter::Instance().canBuildRocketTower())
		towerRocketSprite.setColor(EngineDefs::GrayColor);
	else
		towerRocketSprite.setColor(sf::Color::White);

	cost = Balance::Instance().getTowerStats(TOWER_TYPES::POWER).cost + TowersCounter::Instance().powerTowerCount * Balance::Instance().getPowerTowerCostOffset();
	if (money < cost || !TowersCounter::Instance().canBuildPowerTower())
		towerPowerSprite.setColor(EngineDefs::GrayColor);
	else
		towerPowerSprite.setColor(sf::Color::White);

	cost = Balance::Instance().getTowerStats(TOWER_TYPES::IMPROVED).cost;
	if (money < cost || !iconsAvaliable.isImprovedEnabled || !TowersCounter::Instance().canBuildImprovedTower())
		towerImprovedSprite.setColor(EngineDefs::GrayColor);
	else
		towerImprovedSprite.setColor(sf::Color::White);
}

void GamePanel::updateEnableAbilities()
{
	Abilities *abilities = Engine::Instance().options<GameOptions>()->level()->getAbilities();

	if (!abilities->bombAbility->isReady() || !iconsAvaliable.isAbilityBombEnabled)
		abilityBombSprite.setColor(EngineDefs::GrayColor);
	else
		abilityBombSprite.setColor(sf::Color::White);

	if (!abilities->freezeBombAbility->isReady() || !iconsAvaliable.isAbilityFreezeBombEnabled)
		abilityFreezeBombSprite.setColor(EngineDefs::GrayColor);
	else
		abilityFreezeBombSprite.setColor(sf::Color::White);

	if (!abilities->acidAbility->isReady() || !iconsAvaliable.isAbilityAcidEnabled)
		abilityAcidSprite.setColor(EngineDefs::GrayColor);
	else
		abilityAcidSprite.setColor(sf::Color::White);

	if (!abilities->increaseTowerDamageAbility->isReady() || !iconsAvaliable.isAbilityIncreaseTowerDamageEnabled)
		abilityIncreaseTowerDamageSprite.setColor(EngineDefs::GrayColor);
	else
		abilityIncreaseTowerDamageSprite.setColor(sf::Color::White);

	if (!abilities->increaseTowerAttackSpeedAbility->isReady() || !iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled)
		abilityIncreaseTowerAttackSpeedSprite.setColor(EngineDefs::GrayColor);
	else
		abilityIncreaseTowerAttackSpeedSprite.setColor(sf::Color::White);

	if (!abilities->stopAblity->isReady() || !iconsAvaliable.isAbilityStopEnabled)
		abilityStopSprite.setColor(EngineDefs::GrayColor);
	else
		abilityStopSprite.setColor(sf::Color::White);
}

sf::String GamePanel::towerInfo(TOWER_TYPES type, Tower *tower)
{
	sf::String str;
	switch (type)
	{
	case BASE:
		str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_BASE);
		break;
	case POWER:
		str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_POWER);
		break;
	case ROCKET:
		str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_ROCKET);
		break;
	case FREEZE:
		str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_FREEZE);
		break;
	case LASER:	
		str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_LASER);
		break;
	case IMPROVED:
		str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_IMPROVED);
		break;
	}
//	if (tower == nullptr)
//	{
//		str += EngineDefs::endline;
//		switch (type)
//		{
//		case BASE:
//			str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION);
//			break;
//		case POWER:
//			str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION);
//			break;
//		case ROCKET:
//			str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION);
//			break;
//		case FREEZE:
//			str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION);
//			break;
//		case LASER:
//			str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION);
//			break;
//		case IMPROVED:
//			str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION);
//			break;
//		}
//	}
	str += EngineDefs::endline;
	str += towerStatsStr(type, tower);
	return str;
}

void GamePanel::updateCursor()
{
//	m_sprite.setColor(Engine::Instance().options<GameOptions>()->cursor()->inPanel()?sf::Color::Red:sf::Color::Green);
	if (!Engine::Instance().options<GameOptions>()->cursor()->inPanel())
		return;
	const sf::Vector2f pos = Engine::Instance().options<GameOptions>()->cursor()->windowScreenPos();

	for (unsigned int i = 0; i < actionsSprites.size(); ++i)
	{
		sf::Sprite *sprite = actionsSprites.at(i);
		if (sprite->getGlobalBounds().contains(pos))
		{
			currentCursorPos = i;
			break;
		}
	}
	updateCurrentCursor();
}

void GamePanel::moveCursorLeft()
{
	currentCursorPos--;
	updateCurrentCursor();
}

void GamePanel::moveCursorRight()
{
	currentCursorPos++;
	updateCurrentCursor();
}

bool GamePanel::isTowerIconActive(TOWER_TYPES type) const
{
	switch (type)
	{
	case BASE:
		return TowersCounter::Instance().canBuildBaseTower();
	case POWER:
		return TowersCounter::Instance().canBuildPowerTower();
	case ROCKET:
		return iconsAvaliable.isRocketEnabled && TowersCounter::Instance().canBuildRocketTower();
	case FREEZE:
		return iconsAvaliable.isFreezeEnabled && TowersCounter::Instance().canBuildFreezeTower();
	case LASER:
		return iconsAvaliable.isLaserEnabled && TowersCounter::Instance().canBuildLaserTower();
	case IMPROVED:
		return iconsAvaliable.isImprovedEnabled && TowersCounter::Instance().canBuildImprovedTower();
	}
	return true;
}

bool GamePanel::isAbilityIconActive(ACTION_STATE type) const
{
	switch (type)
	{
	case ABILITY_ACID:
		return iconsAvaliable.isAbilityAcidEnabled;
	case ABILITY_BOMB:
		return iconsAvaliable.isAbilityBombEnabled;
	case ABILITY_FREEZE_BOMB:
		return iconsAvaliable.isAbilityFreezeBombEnabled;
	case ABILITY_INCREASE_TOWER_ATTACK_SPEED:
		return iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled;
	case ABILITY_INCREASE_TOWER_DAMAGE:
		return iconsAvaliable.isAbilityIncreaseTowerDamageEnabled;
	case ABILITY_STOP:
		return iconsAvaliable.isAbilityStopEnabled;
	default:
		break;
	}
	return false;
}

void GamePanel::updateCurrentCursor()
{
	if (currentCursorPos == PANEL_POS_ID)
		return;

	if(currentCursorPos >= actionsSprites.size())
		currentCursorPos = actionsSprites.size() - 1;
	if (currentCursorPos < 0)
		currentCursorPos = 0;

	cursorSprite.setPosition(actionsSprites.at(currentCursorPos)->getPosition());
	currentIconRect.setPosition(cursorSprite.getPosition());
	updateInfo();
}

sf::String GamePanel::towerStatsStr(TOWER_TYPES type, Tower *tower) const
{
	sf::String str;

	if (tower == nullptr)
	{
		const sf::String level_separator = "/";		
		sf::String radiusStr;
		sf::String costStr;
		sf::String dpsStr;
		const std::vector<TowerStats> towersStats = levelTowerStats(type);
		for (size_t i = 0; i < towersStats.size(); ++i)
		{
			const TowerStats stats = towersStats.at(i);
			const bool nolast = i != towersStats.size() - 1;
			const float dps = stats.damage / (stats.attackSpeed * 1 / EngineDefs::MSEC);
			dpsStr += GlobalVariables::to_string_with_precision(dps, 1);
			if (nolast)
				dpsStr += level_separator;
			radiusStr += GlobalVariables::to_string_with_precision(stats.radius, 1);
			if (nolast)
				radiusStr += level_separator;
			costStr += GlobalVariables::to_string_with_precision(stats.cost, 2);
			if (nolast)
				costStr += level_separator;
		}
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DPS);
		str += EngineDefs::separator;
		str += dpsStr;
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::RADIUS);
		str += EngineDefs::separator;
		str += radiusStr;
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COST);
		str += EngineDefs::separator;
		str += costStr;
	}
	else
	{
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::LEVEL)
				+ EngineDefs::separator + std::to_string(tower->level()) + EngineDefs::endline;
		const TowerStats towerStats = tower->data();
		const float dps = towerStats.damage / (tower->actualAttackSpeed() * 1 / EngineDefs::MSEC);
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DPS) + EngineDefs::separator + GlobalVariables::to_string_with_precision(dps, 2);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::RADIUS) + EngineDefs::separator +
				GlobalVariables::to_string_with_precision(tower->actualRadius(), 1);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::KILLS) + EngineDefs::separator +
				std::to_string(tower->kills());
		str += EngineDefs::endline;
		const float cost = getTowerSellCost(tower);
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::SELL_COST);
		str += EngineDefs::separator;
		str += GlobalVariables::to_string_with_precision(cost, 2);
	}
	return str;
}

std::vector<TowerStats> GamePanel::levelTowerStats(TOWER_TYPES type) const
{
	std::vector<TowerStats> results;
	TowerStats towerStats = Balance::Instance().getTowerStats(type);;
	sf::String radiusStr;
	sf::String costStr;
	sf::String dpsStr;
	if (type == POWER)
		towerStats.cost += TowersCounter::Instance().powerTowerCount * Balance::Instance().getPowerTowerCostOffset();
	const float k = 1 + Balance::Instance().getTowerUpgradeGain();
	for (int level = 1; level <= Tower::ABILITY_LEVEL; ++level)
	{
		results.push_back(towerStats);
		towerStats.radius *= k;
		towerStats.damage *= k;
		towerStats.cost *= 1 + Balance::Instance().getTowerUpgradeGain() * Tower::UPGRADE_PRICE_MODIFIER;
		towerStats.attackSpeed *= 1 - Balance::Instance().getTowerUpgradeGain();
		if (type == POWER)
		{
			towerStats.radius = PowerTower::getRadius(level);
			towerStats.attackSpeed = PowerTower::getAttackSpeed(level);
		}
	}
	return results;
}

void GamePanel::setDrain(bool drain)
{
	m_drain = drain;
	drainBlinkTimer.reset();
	if (drain)
		Engine::Instance().soundManager()->playLooped(GAME_SOUND::WARNING);
	else
		Engine::Instance().soundManager()->stop(GAME_SOUND::WARNING);
}

void GamePanel::setCurrentIcon(const ACTION_STATE &state)
{
	switch (state)
	{
	case ACTION_STATE::ABILITY_BOMB:
		currentCursorPos = 6;
		break;
	case ACTION_STATE::ABILITY_FREEZE_BOMB:
		currentCursorPos = 7;
		break;
	case ACTION_STATE::ABILITY_ACID:
		currentCursorPos = 8;
		break;
	case ACTION_STATE::ABILITY_INCREASE_TOWER_DAMAGE:
		currentCursorPos = 9;
		break;
	case ACTION_STATE::ABILITY_INCREASE_TOWER_ATTACK_SPEED:
		currentCursorPos = 10;
		break;
	case ACTION_STATE::ABILITY_STOP:
		currentCursorPos = 11;
		break;
	default:
		break;
	}
}

void GamePanel::resetPanelIcon()
{
	currentCursorPos = 128;
	updateCursor();
}

bool GamePanel::clickOnMiniMap(const sf::Vector2f &pos)
{
	if (miniMapSprite.getGlobalBounds().contains(pos))
	{
		sf::Vector2f rectPos;
		rectPos.x = pos.x - miniMapSprite.getGlobalBounds().left;
		rectPos.y = pos.y - miniMapSprite.getGlobalBounds().top;
		sf::Vector2f k;
		k.x = rectPos.x/miniMapSprite.getGlobalBounds().width;
		k.y = rectPos.y/miniMapSprite.getGlobalBounds().height;
		sf::Vector2f center;
		center.x = Engine::Instance().options<GameOptions>()->cursor()->getMaxCell().x * Engine::Instance().options<GameOptions>()->tileSize().x;
		center.y = Engine::Instance().options<GameOptions>()->cursor()->getMaxCell().y * Engine::Instance().options<GameOptions>()->tileSize().y;
		center.x *= k.x;
		center.y *= k.y;
		Engine::Instance().options<GameOptions>()->camera()->setCenter(center);
		return true;
	}
	return false;
}

void GamePanel::initText(sf::Text *text, const unsigned int size,
						 const sf::Color &fillColor,
						 const sf::Color &outlineColor)
{
	text->setFont(Engine::Instance().fontManager()->font());
	text->setFillColor(fillColor);
	text->setOutlineColor(outlineColor);
	text->setOutlineThickness(1);
	text->setCharacterSize(Engine::Instance().fontManager()->getCharSize(size));
	text->setScale(Engine::Instance().settingsManager()->getScaleFactor());
}

void GamePanel::initCostText(sf::Text *costText)
{
	const unsigned int costTextCharacterSize = 20;
	initText(costText, costTextCharacterSize, sf::Color::White, sf::Color::Black);
}

void GamePanel::initDurationText(sf::Text *costText)
{
	const unsigned int durationTextCharacterSize = 40;
	initText(costText, durationTextCharacterSize, sf::Color::White, sf::Color::Black);
}

void GamePanel::initLimitText(sf::Text *limitText)
{
	constexpr int limitTextCharacterSize = 20;
	initText(limitText, limitTextCharacterSize, sf::Color::Red, sf::Color::Black);
	limitText->setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::LIMIT));
}

void GamePanel::updateInfo()
{
	sf::String str;
	if (Engine::Instance().options<GameOptions>()->cursor()->inPanel())
	{
		const ACTION_STATE state = getCurrentIcon();
		if (state == ADD_TOWER)
		{
			const TOWER_TYPES type = currentTower();
			str = towerInfo(type, nullptr);
		}
		else
		{
			switch (state)
			{
			case ABILITY_BOMB:
				str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_BOMB);
				break;
			case ABILITY_FREEZE_BOMB:
				str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_FREEZE_BOMB);
				break;
			case ABILITY_ACID:
				str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_ACID);
				break;
			case ABILITY_INCREASE_TOWER_ATTACK_SPEED:
				str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED);
				break;
			case ABILITY_INCREASE_TOWER_DAMAGE:
				str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE);
				break;
			case ABILITY_STOP:
				str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_STOP);
				break;
			default:
				break;
			}
			str += EngineDefs::endline;
			str += Instructions::abilityInfoText(state);
		}
	}
	else
	{
		Tower* tower = Engine::Instance().options<GameOptions>()->level()->getTowerAtPos(Engine::Instance().options<GameOptions>()->cursor()->pos());
		if (tower != nullptr)
			str = towerInfo(tower->type(), tower);
		Engine::Instance().options<GameOptions>()->cursor()->setHighlight(
					Engine::Instance().options<GameOptions>()->level()->selectedTower() != tower && tower != nullptr);
	}
	info.setString(str);
}

void GamePanel::initMission(unsigned int n)
{
	iconsAvaliable.isRocketEnabled = true;
	iconsAvaliable.isFreezeEnabled = true;
	iconsAvaliable.isLaserEnabled = true;
	iconsAvaliable.isImprovedEnabled = true;
	iconsAvaliable.isAbilityBombEnabled = true;
	iconsAvaliable.isAbilityFreezeBombEnabled = true;
	iconsAvaliable.isAbilityAcidEnabled = true;
	iconsAvaliable.isAbilityIncreaseTowerDamageEnabled = true;
	iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled = true;
	iconsAvaliable.isAbilityStopEnabled = true;

	switch (n)
	{
	case 1:
		iconsAvaliable.isRocketEnabled = false;
		iconsAvaliable.isFreezeEnabled = false;
		iconsAvaliable.isLaserEnabled = false;
		iconsAvaliable.isImprovedEnabled = false;
		iconsAvaliable.isAbilityFreezeBombEnabled = false;
		iconsAvaliable.isAbilityAcidEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerDamageEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled = false;
		iconsAvaliable.isAbilityStopEnabled = false;
		break;
	case 2:
		iconsAvaliable.isRocketEnabled = false;
		iconsAvaliable.isLaserEnabled = false;
		iconsAvaliable.isImprovedEnabled = false;
		iconsAvaliable.isAbilityAcidEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerDamageEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled = false;
		iconsAvaliable.isAbilityStopEnabled = false;
		break;
	case 3:
		iconsAvaliable.isLaserEnabled = false;
		iconsAvaliable.isImprovedEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerDamageEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled = false;
		iconsAvaliable.isAbilityStopEnabled = false;
		break;
	case 4:
		iconsAvaliable.isImprovedEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerDamageEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled = false;
		iconsAvaliable.isAbilityStopEnabled = false;
		break;
	case 5:
		iconsAvaliable.isImprovedEnabled = false;
		iconsAvaliable.isAbilityStopEnabled = false;
		break;
	default:
		break;
	}
}

sf::FloatRect GamePanel::getTowerRect(const int n) const
{
	const float icons_space = ICONS_SPACE * Engine::Instance().settingsManager()->getScaleFactor().x;
	sf::Vector2f pos = sf::Vector2f(towerBaseSprite.getGlobalBounds().left,
										  towerBaseSprite.getGlobalBounds().top);
	pos.x += Engine::Instance().options<GameOptions>()->tileSize().x * n;
	pos.x += icons_space * n;
	return sf::FloatRect(pos, Engine::Instance().options<GameOptions>()->tileSize());
}

sf::FloatRect GamePanel::getAbilityRect(const int n) const
{
	const float icons_space = ICONS_SPACE * Engine::Instance().settingsManager()->getScaleFactor().x;
	sf::Vector2f pos = sf::Vector2f(abilityBombSprite.getGlobalBounds().left,
									abilityBombSprite.getGlobalBounds().top);
	pos.x += Engine::Instance().options<GameOptions>()->tileSize().x * n;
	pos.x += icons_space * n;
	return sf::FloatRect(pos, Engine::Instance().options<GameOptions>()->tileSize());
}

sf::FloatRect GamePanel::getMoneyRect() const
{
	return moneyCountText.getGlobalBounds();
}

sf::FloatRect GamePanel::getHealthRect() const
{
	return life->fullValue.getGlobalBounds();
}

sf::FloatRect GamePanel::getProgressRect() const
{
	return sf::FloatRect(progress->pos().x, progress->pos().y,
					 Engine::Instance().settingsManager()->getResolution().x * PROGRESS_WIDTH,
						 PROGRESS_OFFSET/2 * Engine::Instance().settingsManager()->getScaleFactor().y);
}

sf::FloatRect GamePanel::getTowersRect() const
{
	const float icons_space = ICONS_SPACE * Engine::Instance().settingsManager()->getScaleFactor().x;

	const int towersCount = 6;
	const sf::Vector2f pos = sf::Vector2f(towerBaseSprite.getGlobalBounds().left,
										  towerBaseSprite.getGlobalBounds().top);
	sf::Vector2f size;
	size.y = Engine::Instance().options<GameOptions>()->tileSize().y;
	size.x = Engine::Instance().options<GameOptions>()->tileSize().x * towersCount
			+ icons_space * (towersCount - 1);
	return sf::FloatRect(pos, size);
}

void GamePanel::setProgressMax(int progressMax)
{
	m_progressMax = progressMax;
}

void GamePanel::init()
{
	rTexture.setView(*Engine::Instance().options<GameOptions>()->camera()->getMiniMapView());
}

void GamePanel::setMapSize(const sf::Vector2f& size)
{
	rTexture.create(static_cast<unsigned int>(size.x),
					static_cast<unsigned int>(size.y));
	const float minimap_scale_x = Engine::Instance().settingsManager()->getScaleFactor().x * 344.f / Engine::Instance().settingsManager()->getResolution().x;
	const float minimap_scale_y = Engine::Instance().settingsManager()->getScaleFactor().y * 213.f / Engine::Instance().settingsManager()->getResolution().y;
	miniMapSprite.setScale(minimap_scale_x, minimap_scale_y);
}

float GamePanel::getBottomValue() const
{
	return m_bottomValue;
}
