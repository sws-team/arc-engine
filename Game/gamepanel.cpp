#include "gamepanel.h"
#include "globalvariables.h"
#include "settings.h"
#include "Level/level.h"
#include "ResourcesManager/resourcesmanager.h"
#include "Engine/engine.h"
#include "Level/camera.h"
#include "Level/tower.h"
#include "Level/lifebar.h"
#include "Level/gamecursor.h"
#include "Translations/language.h"
#include "Game/Level/gameability.h"
#include "Level/instructions.h"

#include <stdlib.h>

const String GamePanel::endline = "\n";

GamePanel::GamePanel() :
	GameDrawable()
  ,waitBlink(false)
  ,currentCursorPos(0)
  ,m_drain(false)
  ,drainState(false)
{
	life = new LifeBar();
	progress = new LifeBar();

	const Vector2f scaleFactor = Settings::Instance().getScaleFactor();

	m_sprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::PANEL_TEXTURE));
	m_sprite.setScale(scaleFactor);

	info.setFont(GlobalVariables::Instance().font());
	info.setFillColor(Color::Black);
	info.setOutlineColor(Color::Yellow);
	info.setOutlineThickness(2);
	info.setCharacterSize(25);
	info.setScale(scaleFactor);

	moneyCountText.setFont(GlobalVariables::Instance().font());
	moneyCountText.setFillColor(Color::Black);
	moneyCountText.setOutlineColor(Color::Yellow);
	moneyCountText.setOutlineThickness(2);
	moneyCountText.setCharacterSize(34);
	moneyCountText.setScale(scaleFactor);

	cursorSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::PANEL_CURSOR));
	cursorSprite.setScale(scaleFactor);

	currentIconRect.setSize(Vector2f(GlobalVariables::CELL_SIZE, GlobalVariables::CELL_SIZE));
	currentIconRect.setFillColor(Color(34,69,160,100));
	currentIconRect.setScale(scaleFactor);

	abilityBombSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_BOMB));
	abilityBombSprite.setScale(scaleFactor);

	abilityFreezeBombSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_FREEZE_BOMB));
	abilityFreezeBombSprite.setScale(scaleFactor);

	abilityVenomSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_CARPET_BOMBING));
	abilityVenomSprite.setScale(scaleFactor);

	abilityIncreaseTowerDamageSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_INCREASE_TOWER_DAMAGE));
	abilityIncreaseTowerDamageSprite.setScale(scaleFactor);

	abilityIncreaseTowerAttackSpeedSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_INCREASE_TOWER_ATTACK_SPEED));
	abilityIncreaseTowerAttackSpeedSprite.setScale(scaleFactor);

	abilityStopSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_TIME_STOP));
	abilityStopSprite.setScale(scaleFactor);

	towerBaseSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_BASE));
	towerBaseSprite.setScale(scaleFactor);
	towerBaseSprite.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);

	towerLaserSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_LASER));
	towerLaserSprite.setScale(scaleFactor);
	towerLaserSprite.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);

	towerFreezeSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_FREEZE));
	towerFreezeSprite.setScale(scaleFactor);
	towerFreezeSprite.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);

	towerRocketSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_ROCKET));
	towerRocketSprite.setScale(scaleFactor);
	towerRocketSprite.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);

	towerPowerSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_POWER));
	towerPowerSprite.setScale(scaleFactor);
	towerPowerSprite.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);

	towerImprovedSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_IMPROVED));
	towerImprovedSprite.setScale(scaleFactor);
	towerImprovedSprite.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);


	readyText.setFont(GlobalVariables::Instance().font());
	readyText.setFillColor(Color::Black);
	readyText.setOutlineColor(Color::Yellow);
	readyText.setOutlineThickness(2);
	readyText.setCharacterSize(45);
	readyText.setScale(scaleFactor);
	readyText.setString(Language::Instance().translate(Language::START_GAME));

	actionsSprites.push_back(&towerBaseSprite);
	actionsSprites.push_back(&towerFreezeSprite);
	actionsSprites.push_back(&towerRocketSprite);
	actionsSprites.push_back(&towerPowerSprite);
	actionsSprites.push_back(&towerLaserSprite);
	actionsSprites.push_back(&towerImprovedSprite);
	actionsSprites.push_back(&abilityBombSprite);
	actionsSprites.push_back(&abilityFreezeBombSprite);
	actionsSprites.push_back(&abilityVenomSprite);
	actionsSprites.push_back(&abilityIncreaseTowerDamageSprite);
	actionsSprites.push_back(&abilityIncreaseTowerAttackSpeedSprite);
	actionsSprites.push_back(&abilityStopSprite);

	const float costTextCharacterSize = 20;
	const Color costTextFillColor = Color::White;
	const Color costTextOutlineColor = Color::Black;

	towerBaseCostText.setFont(GlobalVariables::Instance().font());
	towerBaseCostText.setFillColor(costTextFillColor);
	towerBaseCostText.setOutlineColor(costTextOutlineColor);
	towerBaseCostText.setOutlineThickness(1);
	towerBaseCostText.setCharacterSize(costTextCharacterSize);
	towerBaseCostText.setScale(scaleFactor);

	towerFreezeCostText.setFont(GlobalVariables::Instance().font());
	towerFreezeCostText.setFillColor(costTextFillColor);
	towerFreezeCostText.setOutlineColor(costTextOutlineColor);
	towerFreezeCostText.setOutlineThickness(1);
	towerFreezeCostText.setCharacterSize(costTextCharacterSize);
	towerFreezeCostText.setScale(scaleFactor);

	towerRocketCostText.setFont(GlobalVariables::Instance().font());
	towerRocketCostText.setFillColor(costTextFillColor);
	towerRocketCostText.setOutlineColor(costTextOutlineColor);
	towerRocketCostText.setOutlineThickness(1);
	towerRocketCostText.setCharacterSize(costTextCharacterSize);
	towerRocketCostText.setScale(scaleFactor);

	towerLaserCostText.setFont(GlobalVariables::Instance().font());
	towerLaserCostText.setFillColor(costTextFillColor);
	towerLaserCostText.setOutlineColor(costTextOutlineColor);
	towerLaserCostText.setOutlineThickness(1);
	towerLaserCostText.setCharacterSize(costTextCharacterSize);
	towerLaserCostText.setScale(scaleFactor);

	towerPowerCostText.setFont(GlobalVariables::Instance().font());
	towerPowerCostText.setFillColor(costTextFillColor);
	towerPowerCostText.setOutlineColor(costTextOutlineColor);
	towerPowerCostText.setOutlineThickness(1);
	towerPowerCostText.setCharacterSize(costTextCharacterSize);
	towerPowerCostText.setScale(scaleFactor);

	towerImprovedCostText.setFont(GlobalVariables::Instance().font());
	towerImprovedCostText.setFillColor(costTextFillColor);
	towerImprovedCostText.setOutlineColor(costTextOutlineColor);
	towerImprovedCostText.setOutlineThickness(1);
	towerImprovedCostText.setCharacterSize(costTextCharacterSize);
	towerImprovedCostText.setScale(scaleFactor);

	const float durationTextCharacterSize = 40;
	abilityBombDurationText.setFont(GlobalVariables::Instance().font());
	abilityBombDurationText.setFillColor(costTextFillColor);
	abilityBombDurationText.setOutlineColor(costTextOutlineColor);
	abilityBombDurationText.setOutlineThickness(1);
	abilityBombDurationText.setCharacterSize(durationTextCharacterSize);
	abilityBombDurationText.setScale(scaleFactor);

	abilityFreezeBombDurationText.setFont(GlobalVariables::Instance().font());
	abilityFreezeBombDurationText.setFillColor(costTextFillColor);
	abilityFreezeBombDurationText.setOutlineColor(costTextOutlineColor);
	abilityFreezeBombDurationText.setOutlineThickness(1);
	abilityFreezeBombDurationText.setCharacterSize(durationTextCharacterSize);
	abilityFreezeBombDurationText.setScale(scaleFactor);

	abilityVenomDurationText.setFont(GlobalVariables::Instance().font());
	abilityVenomDurationText.setFillColor(costTextFillColor);
	abilityVenomDurationText.setOutlineColor(costTextOutlineColor);
	abilityVenomDurationText.setOutlineThickness(1);
	abilityVenomDurationText.setCharacterSize(durationTextCharacterSize);
	abilityVenomDurationText.setScale(scaleFactor);

	abilityIncreaseTowerDamageDurationText.setFont(GlobalVariables::Instance().font());
	abilityIncreaseTowerDamageDurationText.setFillColor(costTextFillColor);
	abilityIncreaseTowerDamageDurationText.setOutlineColor(costTextOutlineColor);
	abilityIncreaseTowerDamageDurationText.setOutlineThickness(1);
	abilityIncreaseTowerDamageDurationText.setCharacterSize(durationTextCharacterSize);
	abilityIncreaseTowerDamageDurationText.setScale(scaleFactor);

	abilityIncreaseTowerAttackSpeedDurationText.setFont(GlobalVariables::Instance().font());
	abilityIncreaseTowerAttackSpeedDurationText.setFillColor(costTextFillColor);
	abilityIncreaseTowerAttackSpeedDurationText.setOutlineColor(costTextOutlineColor);
	abilityIncreaseTowerAttackSpeedDurationText.setOutlineThickness(1);
	abilityIncreaseTowerAttackSpeedDurationText.setCharacterSize(durationTextCharacterSize);
	abilityIncreaseTowerAttackSpeedDurationText.setScale(scaleFactor);

	abilityStopDurationText.setFont(GlobalVariables::Instance().font());
	abilityStopDurationText.setFillColor(costTextFillColor);
	abilityStopDurationText.setOutlineColor(costTextOutlineColor);
	abilityStopDurationText.setOutlineThickness(1);
	abilityStopDurationText.setCharacterSize(durationTextCharacterSize);
	abilityStopDurationText.setScale(scaleFactor);


	towerBaseCostText.setString(GlobalVariables::to_string_with_precision(BaseTower::STATS.cost, 0));
	towerFreezeCostText.setString(GlobalVariables::to_string_with_precision(FreezeTower::STATS.cost, 0));
	towerRocketCostText.setString(GlobalVariables::to_string_with_precision(RocketTower::STATS.cost, 0));
	towerLaserCostText.setString(GlobalVariables::to_string_with_precision(LaserTower::STATS.cost, 0));
	towerPowerCostText.setString(GlobalVariables::to_string_with_precision(PowerTower::STATS.cost, 0));
	towerImprovedCostText.setString(GlobalVariables::to_string_with_precision(ImprovedTower::STATS.cost, 0));

	abilityBombDurationText.setString(GlobalVariables::to_string_with_precision(0, 0));
	abilityFreezeBombDurationText.setString(GlobalVariables::to_string_with_precision(0, 0));
	abilityVenomDurationText.setString(GlobalVariables::to_string_with_precision(0, 0));
	abilityIncreaseTowerDamageDurationText.setString(GlobalVariables::to_string_with_precision(0, 0));
	abilityIncreaseTowerAttackSpeedDurationText.setString(GlobalVariables::to_string_with_precision(0, 0));
	abilityStopDurationText.setString(GlobalVariables::to_string_with_precision(0, 0));

	moneyIcon.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::MONEY_ICON));
	moneyIcon.setScale(scaleFactor);

	lifeIcon.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::LIFE_ICON));
	lifeIcon.setScale(scaleFactor);

	waveText.setFont(GlobalVariables::Instance().font());
	waveText.setFillColor(Color::Magenta);
	waveText.setOutlineThickness(2);
	waveText.setOutlineColor(Color::Yellow);
	waveText.setCharacterSize(25);
	waveText.setScale(scaleFactor);

	m_bottomValue = 0;
	const int progressWidth = Settings::Instance().getResolution().x * PROGRESS_WIDTH;
	progress->init(Vector2i(progressWidth, LifeBar::LIFE_BAR_HEIGHT * Settings::Instance().getScaleFactor().y), Color::Red);
	life->init(Vector2i(722 * scaleFactor.x, 37 * scaleFactor.y), Color::Cyan);

	drainRect.setFillColor(Color::Transparent);
	drainRect.setOutlineThickness(2);
	drainRect.setOutlineColor(Color::Transparent);

	//set pos
	//	pos.x = target->getView().getCenter().x - target->getView().getSize().x/2;
	//	pos.y = target->getView().getCenter().y + target->getView().getSize().y/2;

	progress->setPos(Vector2f(progressWidth, PROGRESS_OFFSET * scaleFactor.y));
	updatePos();
}

GamePanel::~GamePanel()
{

}

void GamePanel::draw(RenderTarget * const target)
{
	//draw minimap
	rTexture.clear(Color::Transparent);
	Engine::Instance().level()->drawLevel(&rTexture);
	rTexture.display();
	miniMapSprite.setTexture(rTexture.getTexture());

	//draw
	if(Engine::Instance().level()->getState() == Level::WAIT_READY &&
			!Engine::Instance().instructions()->isActive())
		target->draw(readyText);		

	target->draw(miniMapSprite);
	target->draw(m_sprite);
	target->draw(moneyCountText);
	target->draw(info);

	target->draw(abilityBombSprite);
	target->draw(abilityFreezeBombSprite);
	target->draw(abilityVenomSprite);
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

	Abilities *abilities = Engine::Instance().level()->getAbilities();
	if (!abilities->bombAbility->isReady())
		target->draw(abilityBombDurationText);
	if (!abilities->freezeBombAbility->isReady())
		target->draw(abilityFreezeBombDurationText);
	if (!abilities->venomAbility->isReady())
		target->draw(abilityVenomDurationText);
	if (!abilities->increaseTowerDamageAbility->isReady())
		target->draw(abilityIncreaseTowerDamageDurationText);
	if (!abilities->increaseTowerAttackSpeedAbility->isReady())
		target->draw(abilityIncreaseTowerAttackSpeedDurationText);
	if (!abilities->stopAblity->isReady())
		target->draw(abilityStopDurationText);

	target->draw(moneyIcon);
	target->draw(lifeIcon);

	progress->draw(target);
	life->draw(target);

	target->draw(waveText);
	if (m_drain)
		target->draw(drainRect);

	if (Engine::Instance().cursor()->inPanel())
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
	if(Engine::Instance().level()->getState() == Level::WAIT_READY)
	{
		if (blinkTimer.check(BLINK_TIME))
		{
			waitBlink = !waitBlink;
			readyText.setFillColor(waitBlink?Color::Black:Color::Red);
		}
	}
	if (m_drain)
	{
		if (drainBlinkTimer.check(100))
		{
			drainState = !drainState;
			drainRect.setOutlineColor(drainState ? Color::Red : Color::Transparent);
		}
	}
	updateEnableTowers();
	updateEnableAbilities();
}

void GamePanel::updatePanel()
{
	const int money = static_cast<int>(Engine::Instance().level()->getMoneyCount());
	moneyCountText.setString(String(to_string(money)));

	const float progressValue = static_cast<float>(Engine::Instance().level()->currentProgress()) / m_progressMax;
	progress->setValue(progressValue);
	const float lifeValue = static_cast<float>(Engine::Instance().level()->getLifeCount()) / m_lifeMax;
	life->setValue(lifeValue);

	towerPowerCostText.setString(
				GlobalVariables::to_string_with_precision(
					PowerTower::STATS.cost + Engine::Instance().level()->getPowerTowersCount() * PowerTower::COST_OFFSET, 0));

	updateCursor();
	updateInfo();
}

int GamePanel::cellsCount() const
{
	return static_cast<int>(-1 + m_sprite.getGlobalBounds().height / GlobalVariables::Instance().tileSize().y);
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
		return ACTION_STATE::ABILITY_VENOM;
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

Vector2f GamePanel::updatePos()
{
	const Vector2f nullPos = Vector2f(0, Settings::Instance().getResolution().y - m_sprite.getGlobalBounds().height);
	const Vector2f scaleFactor = Settings::Instance().getScaleFactor();
	const Vector2f iconSize = Vector2f(ICON_SIZE * scaleFactor.x, ICON_SIZE * scaleFactor.y);

	const float top_offset = 40 * scaleFactor.x;
	const float left_offset = 40 * scaleFactor.x;
	const float icon_offset = 24 * scaleFactor.x;
	const float text_offset = 338 * scaleFactor.x;
	const float panel_offset = PANEL_OFFSET * scaleFactor.x;

	m_bottomValue = nullPos.y + PANEL_OFFSET * scaleFactor.y;

	Vector2f pos = Vector2f(ceil(nullPos.x), ceil(nullPos.y));
	m_sprite.setPosition(pos);

	pos.y += panel_offset;

	pos.x += left_offset;
	pos.y += top_offset;

	Vector2f secondRow = Vector2f(pos.x, pos.y + iconSize.y + icon_offset);
	moneyIcon.setPosition(secondRow);
	secondRow.x += iconSize.x;
	moneyCountText.setPosition(secondRow);
	secondRow.x += 178 * scaleFactor.x;
	secondRow.x += icon_offset;
	lifeIcon.setPosition(secondRow);
	secondRow.x += iconSize.x;
	life->setPos(secondRow);

	drainRect.setPosition(moneyCountText.getGlobalBounds().left, moneyCountText.getGlobalBounds().top);
	drainRect.setSize(Vector2f(moneyCountText.getGlobalBounds().width, moneyCountText.getGlobalBounds().height));

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

	abilityVenomSprite.setPosition(pos);
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

	readyText.setPosition(Vector2f(Settings::Instance().getResolution().x/2 - readyText.getGlobalBounds().width/2,
								   Settings::Instance().getResolution().y/2 + readyText.getGlobalBounds().height/2));
	waveText.setPosition(progress->pos());

	towerBaseCostText.setPosition(towerBaseSprite.getPosition());
	towerFreezeCostText.setPosition(towerFreezeSprite.getPosition());
	towerRocketCostText.setPosition(towerRocketSprite.getPosition());
	towerLaserCostText.setPosition(towerLaserSprite.getPosition());
	towerPowerCostText.setPosition(towerPowerSprite.getPosition());
	towerImprovedCostText.setPosition(towerImprovedSprite.getPosition());

	abilityBombDurationText.setPosition(abilityBombSprite.getPosition());
	abilityFreezeBombDurationText.setPosition(abilityFreezeBombSprite.getPosition());
	abilityVenomDurationText.setPosition(abilityVenomSprite.getPosition());
	abilityIncreaseTowerDamageDurationText.setPosition(abilityIncreaseTowerDamageSprite.getPosition());
	abilityIncreaseTowerAttackSpeedDurationText.setPosition(abilityIncreaseTowerAttackSpeedSprite.getPosition());
	abilityStopDurationText.setPosition(abilityStopSprite.getPosition());

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
	waveText.setString(Language::Instance().translate(Language::WAVE) +
				" #" + to_string(Engine::Instance().level()->getCurrentWave() + 1));
}

float GamePanel::getTowerUpgradeCost(Tower *tower) const
{
	if (tower == nullptr)
		return 0.f;

	const TOWER_TYPES type = tower->type();
	float cost = type == TOWER_TYPES::POWER ?
				TowersFactory::getTowerStats(type).cost + Engine::Instance().level()->getPowerTowersCount() * PowerTower::COST_OFFSET :
				TowersFactory::getTowerStats(type).cost;

	switch (tower->level())
	{
	case 1:
		cost *= TowersFactory::UPGRADE_TO_2_COST_MODIFIER;
		break;
	case 2:
		cost *= TowersFactory::UPGRADE_TO_3_COST_MODIFIER;
		break;
	default:
		break;
	}
	return cost;
}

float GamePanel::getTowerSellCost(Tower *tower) const
{
	const TOWER_TYPES type = tower->type();
	float cost = type == TOWER_TYPES::POWER ?
				tower->data().cost + (Engine::Instance().level()->getPowerTowersCount() - 1) * PowerTower::COST_OFFSET :
				tower->data().cost;
	cost /= 2;
	return cost;
}

void GamePanel::updateAbilitiesDuration()
{
	Abilities *abilities = Engine::Instance().level()->getAbilities();

	abilityBombDurationText.setString(to_string(abilities->bombAbility->timeLeft()));
	abilityFreezeBombDurationText.setString(to_string(abilities->freezeBombAbility->timeLeft()));
	abilityVenomDurationText.setString(to_string(abilities->venomAbility->timeLeft()));
	abilityIncreaseTowerDamageDurationText.setString(to_string(abilities->increaseTowerDamageAbility->timeLeft()));
	abilityIncreaseTowerAttackSpeedDurationText.setString(to_string(abilities->increaseTowerAttackSpeedAbility->timeLeft()));
	abilityStopDurationText.setString(to_string(abilities->stopAblity->timeLeft()));
}

void GamePanel::updateEnableTowers()
{
	const float money = Engine::Instance().level()->getMoneyCount();

	float cost = TowersFactory::getTowerStats(TOWER_TYPES::BASE).cost;
	if (money < cost)
		towerBaseSprite.setColor(GlobalVariables::GrayColor);
	else
		towerBaseSprite.setColor(Color::White);

	cost = TowersFactory::getTowerStats(TOWER_TYPES::LASER).cost;
	if (money < cost || !iconsAvaliable.isLaserEnabled)
		towerLaserSprite.setColor(GlobalVariables::GrayColor);
	else
		towerLaserSprite.setColor(Color::White);

	cost = TowersFactory::getTowerStats(TOWER_TYPES::FREEZE).cost;
	if (money < cost || !iconsAvaliable.isFreezeEnabled)
		towerFreezeSprite.setColor(GlobalVariables::GrayColor);
	else
		towerFreezeSprite.setColor(Color::White);

	cost = TowersFactory::getTowerStats(TOWER_TYPES::ROCKET).cost;
	if (money < cost || !iconsAvaliable.isRocketEnabled)
		towerRocketSprite.setColor(GlobalVariables::GrayColor);
	else
		towerRocketSprite.setColor(Color::White);

	cost = TowersFactory::getTowerStats(TOWER_TYPES::POWER).cost + Engine::Instance().level()->getPowerTowersCount() * PowerTower::COST_OFFSET;
	if (money < cost)
		towerPowerSprite.setColor(GlobalVariables::GrayColor);
	else
		towerPowerSprite.setColor(Color::White);

	cost = TowersFactory::getTowerStats(TOWER_TYPES::IMPROVED).cost;
	if (money < cost || !iconsAvaliable.isImprovedEnabled)
		towerImprovedSprite.setColor(GlobalVariables::GrayColor);
	else
		towerImprovedSprite.setColor(Color::White);
}

void GamePanel::updateEnableAbilities()
{
	Abilities *abilities = Engine::Instance().level()->getAbilities();

	if (!abilities->bombAbility->isReady() || !iconsAvaliable.isAbilityBombEnabled)
		abilityBombSprite.setColor(GlobalVariables::GrayColor);
	else
		abilityBombSprite.setColor(Color::White);

	if (!abilities->freezeBombAbility->isReady() || !iconsAvaliable.isAbilityFreezeBombEnabled)
		abilityFreezeBombSprite.setColor(GlobalVariables::GrayColor);
	else
		abilityFreezeBombSprite.setColor(Color::White);

	if (!abilities->venomAbility->isReady() || !iconsAvaliable.isAbilityVenomEnabled)
		abilityVenomSprite.setColor(GlobalVariables::GrayColor);
	else
		abilityVenomSprite.setColor(Color::White);

	if (!abilities->increaseTowerDamageAbility->isReady() || !iconsAvaliable.isAbilityIncreaseTowerDamageEnabled)
		abilityIncreaseTowerDamageSprite.setColor(GlobalVariables::GrayColor);
	else
		abilityIncreaseTowerDamageSprite.setColor(Color::White);

	if (!abilities->increaseTowerAttackSpeedAbility->isReady() || !iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled)
		abilityIncreaseTowerAttackSpeedSprite.setColor(GlobalVariables::GrayColor);
	else
		abilityIncreaseTowerAttackSpeedSprite.setColor(Color::White);

	if (!abilities->stopAblity->isReady() || !iconsAvaliable.isAbilityStopEnabled)
		abilityStopSprite.setColor(GlobalVariables::GrayColor);
	else
		abilityStopSprite.setColor(Color::White);
}

String GamePanel::towerInfo(TOWER_TYPES type, Tower *tower)
{
	String str;
	switch (type)
	{
	case BASE:
	{
		str = Language::Instance().translate(Language::TOWER_BASE);
		str += endline;
		str += Language::Instance().translate(Language::BASE_TOWER_DESCRIPTION);
	}
		break;
	case POWER:
	{
		str = Language::Instance().translate(Language::TOWER_POWER);
		str += endline;
		str += Language::Instance().translate(Language::POWER_TOWER_DESCRIPTION);
	}
		break;
	case ROCKET:
	{
		str = Language::Instance().translate(Language::TOWER_ROCKET);
		str += endline;
		str += Language::Instance().translate(Language::ROCKET_TOWER_DESCRIPTION);
	}
		break;
	case FREEZE:
	{
		str = Language::Instance().translate(Language::TOWER_FREEZE);
		str += endline;
		str += Language::Instance().translate(Language::FREEZE_TOWER_DESCRIPTION);
	}
		break;
	case LASER:
	{
		str = Language::Instance().translate(Language::TOWER_LASER);
		str += endline;
		str += Language::Instance().translate(Language::LASER_TOWER_DESCRIPTION);
	}
		break;
	case IMPROVED:
	{
		str = Language::Instance().translate(Language::TOWER_IMPROVED);
		str += endline;
		str += Language::Instance().translate(Language::IMPROVED_TOWER_DESCRIPTION);
	}
		break;
	}

	str += endline;

	const String separator = ": ";
	if (tower != nullptr)
		str += Language::Instance().translate(Language::LEVEL) + separator + to_string(tower->level()) + endline;

	const TowerStats towerStats = tower == nullptr ? TowersFactory::getTowerStats(type) : tower->data();
	const float dps = towerStats.damage / ((tower == nullptr ? towerStats.attackSpeed : tower->actualAttackSpeed()) * 0.001f);

	str += Language::Instance().translate(Language::DAMAGE_PER_SECOND) + separator + GlobalVariables::to_string_with_precision(dps, 2);
	str += endline;

	str += Language::Instance().translate(Language::RADIUS) + separator +
			GlobalVariables::to_string_with_precision(tower == nullptr ? towerStats.radius : tower->actualRadius(), 1);
	str += endline;

	if (tower == nullptr)
	{
		const float cost = type == TOWER_TYPES::POWER ?
					towerStats.cost + Engine::Instance().level()->getPowerTowersCount() * PowerTower::COST_OFFSET :
					towerStats.cost;

		str += Language::Instance().translate(Language::COST);
		str += separator;
		str += GlobalVariables::to_string_with_precision(cost, 2);
	}
	else
	{
		str += Language::Instance().translate(Language::KILLS) + separator + to_string(tower->kills());
		str += endline;
		const float cost = getTowerSellCost(tower);
		str += Language::Instance().translate(Language::SELL_COST);
		str += separator;
		str += GlobalVariables::to_string_with_precision(cost, 2);
	}
	return str;
}

void GamePanel::updateCursor()
{
//	m_sprite.setColor(Engine::Instance().cursor()->inPanel()?Color::Red:Color::Green);

	if (!Engine::Instance().cursor()->inPanel())
		return;
	const Vector2f pos = Engine::Instance().cursor()->windowScreenPos();

	for (unsigned int i = 0; i < actionsSprites.size(); ++i)
	{
		Sprite *sprite = actionsSprites.at(i);
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
	case POWER:
		return true;
	case ROCKET:
		return iconsAvaliable.isRocketEnabled;
	case FREEZE:
		return iconsAvaliable.isFreezeEnabled;
	case LASER:
		return iconsAvaliable.isLaserEnabled;
	case IMPROVED:
		return iconsAvaliable.isImprovedEnabled;
	}
	return true;
}

bool GamePanel::isAbilityIconActive(ACTION_STATE type) const
{
	switch (type)
	{
	case ABILITY_VENOM:
		return iconsAvaliable.isAbilityVenomEnabled;
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

void GamePanel::setDrain(bool drain)
{
	m_drain = drain;
	drainBlinkTimer.reset();
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
	case ACTION_STATE::ABILITY_VENOM:
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

bool GamePanel::clickOnMiniMap(const Vector2f &pos)
{
	if (miniMapSprite.getGlobalBounds().contains(pos))
	{
		Vector2f rectPos;
		rectPos.x = pos.x - miniMapSprite.getGlobalBounds().left;
		rectPos.y = pos.y - miniMapSprite.getGlobalBounds().top;
		Vector2f k;
		k.x = rectPos.x/miniMapSprite.getGlobalBounds().width;
		k.y = rectPos.y/miniMapSprite.getGlobalBounds().height;
		Vector2f center;
		center.x = Engine::Instance().cursor()->getMaxCell().x * GlobalVariables::Instance().tileSize().x;
		center.y = Engine::Instance().cursor()->getMaxCell().y * GlobalVariables::Instance().tileSize().y;
		center.x *= k.x;
		center.y *= k.y;
		Engine::Instance().camera()->setCenter(center);
		return true;
	}
	return false;
}

void GamePanel::updateInfo()
{
	String str;
	if (Engine::Instance().cursor()->inPanel())
	{
		const ACTION_STATE state = getCurrentIcon();
		switch (state)
		{
		case ADD_TOWER:
		{
			const TOWER_TYPES type = currentTower();
			str = towerInfo(type, nullptr);
		}
			break;
		case ABILITY_BOMB:
		{
			str = Language::Instance().translate(Language::ABILITY_BOMB);
			str += endline;
			str += Language::Instance().translate(Language::BOMB_ABILITY_DESCRIPTION);
		}
			break;
		case ABILITY_FREEZE_BOMB:
		{
			str = Language::Instance().translate(Language::ABILITY_FREEZE_BOMB);
			str += endline;
			str += Language::Instance().translate(Language::FREEZE_BOMB_ABILITY_DESCRIPTION);
		}
			break;
		case ABILITY_VENOM:
		{
			str = Language::Instance().translate(Language::ABILITY_VENOM);
			str += endline;
			str += Language::Instance().translate(Language::VENOM_ABILITY_DESCRIPTION);
		}
			break;
		case ABILITY_INCREASE_TOWER_ATTACK_SPEED:
		{
			str = Language::Instance().translate(Language::ABILITY_INCREASE_TOWER_ATTACK_SPEED);
			str += endline;
			str += Language::Instance().translate(Language::INC_AS_ABILITY_DESCRIPTION);
		}
			break;
		case ABILITY_INCREASE_TOWER_DAMAGE:
		{
			str = Language::Instance().translate(Language::ABILITY_INCREASE_TOWER_DAMAGE);
			str += endline;
			str += Language::Instance().translate(Language::INC_DMG_ABILITY_DESCRIPTION);
		}
			break;
		case ABILITY_STOP:
		{
			str = Language::Instance().translate(Language::ABILITY_STOP);
			str += endline;
			str += Language::Instance().translate(Language::STOP_ABILITY_DESCRIPTION);
		}
			break;
		default:
			break;
		}
	}
	else
	{
		Tower* tower = Engine::Instance().level()->getTowerAtPos(Engine::Instance().cursor()->pos());
		if (tower != nullptr)
			str = towerInfo(tower->type(), tower);
		Engine::Instance().cursor()->setHighlight(
					Engine::Instance().level()->selectedTower() != tower && tower != nullptr);
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
	iconsAvaliable.isAbilityVenomEnabled = true;
	iconsAvaliable.isAbilityIncreaseTowerDamageEnabled = true;
	iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled = true;
	iconsAvaliable.isAbilityStopEnabled = true;

	switch (n)
	{
	case 0:
		iconsAvaliable.isRocketEnabled = false;
		iconsAvaliable.isFreezeEnabled = false;
		iconsAvaliable.isLaserEnabled = false;
		iconsAvaliable.isImprovedEnabled = false;
		iconsAvaliable.isAbilityFreezeBombEnabled = false;
		iconsAvaliable.isAbilityVenomEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerDamageEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled = false;
		iconsAvaliable.isAbilityStopEnabled = false;
		break;
	case 1:
		iconsAvaliable.isRocketEnabled = false;
		iconsAvaliable.isLaserEnabled = false;
		iconsAvaliable.isImprovedEnabled = false;
		iconsAvaliable.isAbilityVenomEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerDamageEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled = false;
		iconsAvaliable.isAbilityStopEnabled = false;
		break;
	case 2:
		iconsAvaliable.isLaserEnabled = false;
		iconsAvaliable.isImprovedEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerDamageEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled = false;
		iconsAvaliable.isAbilityStopEnabled = false;
		break;
	case 3:
		iconsAvaliable.isImprovedEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerDamageEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled = false;
		iconsAvaliable.isAbilityStopEnabled = false;
		break;
	case 4:
		iconsAvaliable.isImprovedEnabled = false;
		iconsAvaliable.isAbilityStopEnabled = false;
		break;
	default:
		break;
	}
}

FloatRect GamePanel::getTowersRect() const
{
	const Vector2f pos = Vector2f(towerBaseSprite.getGlobalBounds().left, towerBaseSprite.getGlobalBounds().top);
	const float icons_space = ICONS_SPACE * Settings::Instance().getScaleFactor().x;
	Vector2f size;
	size.x = towerBaseSprite.getGlobalBounds().width * 6;
	size.x += icons_space * 5;
	size.y = towerBaseSprite.getGlobalBounds().height;
	return FloatRect(pos, size);
}

FloatRect GamePanel::getAbilitiesRect() const
{
	const Vector2f pos = Vector2f(abilityBombSprite.getGlobalBounds().left, abilityBombSprite.getGlobalBounds().top);
	const float icons_space = ICONS_SPACE * Settings::Instance().getScaleFactor().x;
	Vector2f size;
	size.x = abilityBombSprite.getGlobalBounds().width * 6;
	size.x += icons_space * 5;
	size.y = abilityBombSprite.getGlobalBounds().height;
	return FloatRect(pos, size);
}

FloatRect GamePanel::getMoneyRect() const
{
	return moneyCountText.getGlobalBounds();
}

FloatRect GamePanel::getHealthRect() const
{
	return life->fullValue.getGlobalBounds();
}

FloatRect GamePanel::getProgressRect() const
{
	return FloatRect(progress->pos().x, progress->pos().y,
					 Settings::Instance().getResolution().x * PROGRESS_WIDTH,
					 PROGRESS_OFFSET * Settings::Instance().getScaleFactor().y);
}

void GamePanel::setProgressMax(int progressMax)
{
	m_progressMax = progressMax;
}

void GamePanel::init()
{
	rTexture.setView(*Engine::Instance().camera()->getMiniMapView());
}

void GamePanel::setMapSize(const Vector2f& size)
{
	rTexture.create(static_cast<unsigned int>(size.x),
					static_cast<unsigned int>(size.y));

	const float minimap_scale_x = Settings::Instance().getScaleFactor().x * 344.f / Settings::Instance().getResolution().x;
	const float minimap_scale_y = Settings::Instance().getScaleFactor().y * 213.f / Settings::Instance().getResolution().y;
	miniMapSprite.setScale(minimap_scale_x * 1920.f/size.x, minimap_scale_y* 1088.f/size.y);
}

float GamePanel::getBottomValue() const
{
	const Vector2i pixelPos = Vector2i(0, m_bottomValue);
	const Vector2f pos = Engine::Instance().window()->mapPixelToCoords(
				pixelPos, *Engine::Instance().camera()->getView());

	return pos.y;
}
