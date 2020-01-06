#include "gamepanel.h"
#include "globalvariables.h"
#include "settings.h"
#include "Level/level.h"
#include "ResourcesManager/resourcesmanager.h"
#include "Engine/engine.h"
#include "Level/camera.h"
#include "Level/tower.h"
#include "Level/lifebar.h"
#include "Level/cursor.h"
#include "Translations/language.h"
#include "Game/Level/gameability.h"

#include <stdlib.h>

const String GamePanel::endline = "\n";

GamePanel::GamePanel() :
	GameDrawable()
  ,waitBlink(false)
  ,currentCursorPos(0)
{
	life = new LifeBar();
	progress = new LifeBar();
	m_selectedTower = nullptr;

	const Vector2f scaleFactor = Settings::Instance().getGameScaleFactor();

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

	sellSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::SELL_TEXTURE));
	sellSprite.setScale(scaleFactor);

	upgradeSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::UPGRADE_TEXTURE));
	upgradeSprite.setScale(scaleFactor);

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

	actionsSprites.push_back(&sellSprite);
	actionsSprites.push_back(&towerBaseSprite);
	actionsSprites.push_back(&towerFreezeSprite);
	actionsSprites.push_back(&towerRocketSprite);
	actionsSprites.push_back(&towerPowerSprite);
	actionsSprites.push_back(&towerLaserSprite);
	actionsSprites.push_back(&towerImprovedSprite);
	actionsSprites.push_back(&upgradeSprite);
	actionsSprites.push_back(&abilityBombSprite);
	actionsSprites.push_back(&abilityFreezeBombSprite);
	actionsSprites.push_back(&abilityVenomSprite);
	actionsSprites.push_back(&abilityIncreaseTowerDamageSprite);
	actionsSprites.push_back(&abilityIncreaseTowerAttackSpeedSprite);
	actionsSprites.push_back(&abilityStopSprite);

	const float costTextCharacterSize = 20;
	const Color costTextFillColor = Color::Blue;
	const Color costTextOutlineColor = Color::White;
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

	startSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::START_TEXTURE));
	startSprite.setScale(Settings::Instance().getScaleFactor());

	endSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::END_TEXTURE));
	endSprite.setScale(Settings::Instance().getScaleFactor());

	waveText.setFont(GlobalVariables::Instance().font());
	waveText.setFillColor(Color::Magenta);
	waveText.setOutlineThickness(2);
	waveText.setOutlineColor(Color::Yellow);
	waveText.setCharacterSize(25);
	waveText.setScale(scaleFactor);

	m_bottomValue = 0;
	progress->init(Vector2i(Settings::Instance().getResolution().x * PROGRESS_WIDTH,
							LifeBar::LIFE_BAR_HEIGHT * Settings::Instance().getScaleFactor().y), Color::Red);
	life->init(Vector2i(128 * Settings::Instance().getScaleFactor().x,
						16 * Settings::Instance().getScaleFactor().y),
			   Color::Red);
	updateCurrentTower();
}

GamePanel::~GamePanel()
{

}

void GamePanel::draw(RenderTarget * const target)
{
	//update pos
	Vector2f pos;
	pos.x = target->getView().getCenter().x - target->getView().getSize().x/2;
	pos.y = target->getView().getCenter().y + target->getView().getSize().y/2;
	readyText.setPosition(Vector2f(pos.x + Settings::Instance().getResolution().x/2 * Settings::GAME_SCALE - readyText.getGlobalBounds().width/2,
									pos.y - Settings::Instance().getResolution().y/2 * Settings::GAME_SCALE + readyText.getGlobalBounds().height/2));

	progress->setPos(Vector2f(pos.x + Settings::Instance().getResolution().x * PROGRESS_WIDTH,
							  pos.y - target->getView().getSize().y + PROGRESS_OFFSET *
							  Settings::Instance().getScaleFactor().y));

	waveText.setPosition(progress->pos());

	pos.y -= m_sprite.getGlobalBounds().height;
	m_bottomValue = pos.y;
	pos = updatePos(pos);

	//draw minimap
	rTexture.clear(Color::Transparent);
	Engine::Instance().level()->drawLevel(&rTexture);
	rTexture.display();

	Sprite miniMapSprite;
	miniMapSprite.setTexture(rTexture.getTexture());

	const float minimap_scale_x = Settings::Instance().getScaleFactor().x * Settings::GAME_SCALE * 537.f / Settings::Instance().getResolution().x;
	const float minimap_scale_y = Settings::Instance().getScaleFactor().y * Settings::GAME_SCALE * 152.f / Settings::Instance().getResolution().y;

	miniMapSprite.scale(minimap_scale_x, minimap_scale_y);
	miniMapSprite.setPosition(pos);

	//draw
	if(Engine::Instance().level()->getState() == Level::WAIT_READY)
	{
		target->draw(readyText);
		target->draw(startSprite);
		target->draw(endSprite);
	}

	target->draw(m_sprite);

	target->draw(moneyCountText);

	updateEnableAbilities();

	target->draw(info);

	target->draw(abilityBombSprite);
	target->draw(abilityFreezeBombSprite);
	target->draw(abilityVenomSprite);
	target->draw(abilityIncreaseTowerDamageSprite);
	target->draw(abilityIncreaseTowerAttackSpeedSprite);
	target->draw(abilityStopSprite);

	updateEnableTowers();

	target->draw(towerBaseSprite);
	target->draw(towerLaserSprite);
	target->draw(towerFreezeSprite);
	target->draw(towerRocketSprite);
	target->draw(towerPowerSprite);
	target->draw(towerImprovedSprite);

	target->draw(sellSprite);
	target->draw(upgradeSprite);

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

	target->draw(miniMapSprite);
	progress->draw(target);
	life->draw(target);

	target->draw(waveText);

	if (Engine::Instance().cursor()->inPanel())
		target->draw(cursorSprite);
}

void GamePanel::update()
{	
	if(Engine::Instance().level()->getState() == Level::WAIT_READY)
	{
		if (blinkTimer.check(BLINK_TIME))
		{
			waitBlink = !waitBlink;
			readyText.setFillColor(waitBlink?Color::Black:Color::Red);
			startSprite.setColor(waitBlink?Color::Yellow:Color::Red);
			endSprite.setColor(waitBlink?Color::Yellow:Color::Red);
		}
	}
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
	return static_cast<int>(m_sprite.getGlobalBounds().height / GlobalVariables::Instance().tileSize().y);
}

Tower *GamePanel::selectedTower() const
{
	return m_selectedTower;
}

void GamePanel::setSelectedTower(Tower *selectedTower)
{
	m_selectedTower = selectedTower;
	updateCurrentTower();
}

ACTION_STATE GamePanel::getCurrentIcon() const
{
	switch (currentCursorPos)
	{
	case 0:
		return ACTION_STATE::SELL;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		return ACTION_STATE::ADD_TOWER;
	case 7:
		return ACTION_STATE::UPGRADE;
	case 8:
		return ACTION_STATE::ABILITY_BOMB;
	case 9:
		return ACTION_STATE::ABILITY_FREEZE_BOMB;
	case 10:
		return ACTION_STATE::ABILITY_VENOM;
	case 11:
		return ACTION_STATE::ABILITY_INCREASE_TOWER_DAMAGE;
	case 12:
		return ACTION_STATE::ABILITY_INCREASE_TOWER_ATTACK_SPEED;
	case 13:
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
	case 1:
		return BASE;
	case 2:
		return FREEZE;
	case 3:
		return ROCKET;
	case 4:
		return POWER;
	case 5:
		return LASER;
	case 6:
		return IMPROVED;
	default:
		break;
	}
	return POWER;
}

Vector2f GamePanel::updatePos(const Vector2f &nullPos)
{
	const Vector2f iconSize = Vector2f(ICON_SIZE * Settings::Instance().getScaleFactor().x * Settings::GAME_SCALE,
								 ICON_SIZE * Settings::Instance().getScaleFactor().y * Settings::GAME_SCALE);

	const float info_icons_size = 32 * Settings::GAME_SCALE * Settings::Instance().getScaleFactor().x;
	const float info_icons_offset = 16 * Settings::GAME_SCALE * Settings::Instance().getScaleFactor().y;
	const float block_space = iconSize.x/2;
	const float icons_space = ICONS_SPACE * Settings::GAME_SCALE * Settings::Instance().getScaleFactor().y;
	const float label_offset = 16 * Settings::GAME_SCALE * Settings::Instance().getScaleFactor().x;
	const float info_offset = 256 * Settings::GAME_SCALE * Settings::Instance().getScaleFactor().x;
	const float panel_offset = 62 * Settings::GAME_SCALE * Settings::Instance().getScaleFactor().x;
	const float text_offset = 320 * Settings::GAME_SCALE * Settings::Instance().getScaleFactor().x;

	Vector2f pos = Vector2f(ceil(nullPos.x), ceil(nullPos.y));
	m_sprite.setPosition(pos);

	pos.y += icons_space;

	pos.x += panel_offset;
	pos.x += label_offset;
	pos.y += info_icons_offset;
	moneyIcon.setPosition(pos);
	pos.x += info_icons_size;
	moneyCountText.setPosition(pos);
	pos.x -= info_icons_size;

	pos.y += info_icons_offset;
	pos.y += moneyCountText.getGlobalBounds().height;

	life->setPos(pos);
//	pos.x += info_icons_size;
//	lifeCountText.setPosition(pos);
//	pos.x -= info_icons_size;

	pos.y -= 2 * info_icons_offset;
	pos.y -= moneyCountText.getGlobalBounds().height;

	pos.x -= label_offset;

	pos.x += info_offset;

	pos.x += block_space;
	sellSprite.setPosition(pos);

	pos.y += iconSize.y;
	pos.y += icons_space;

	upgradeSprite.setPosition(pos);

	pos.y -= icons_space;
	pos.y -= iconSize.y;
	pos.x += iconSize.x;

	pos.x += block_space;

	info.setPosition(pos);

	//text
	pos.x += text_offset;
	pos.x += block_space;

	const float iconsPosX = pos.x;
	//towers
	towerBaseSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	towerFreezeSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	towerRocketSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	towerPowerSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	towerLaserSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	towerImprovedSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	pos.x = iconsPosX;
	pos.y += iconSize.y;
	pos.y += icons_space;

	//abilities
	abilityBombSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	abilityFreezeBombSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	abilityVenomSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	abilityIncreaseTowerDamageSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	abilityIncreaseTowerAttackSpeedSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	abilityStopSprite.setPosition(pos);
	pos.x += icons_space;

	pos.y -= icons_space;
	pos.y -= iconSize.y;
	pos.x += iconSize.x;
	pos.x += icons_space;

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
	waveText.setString("Wave #" + to_string(Engine::Instance().level()->getCurrentWave() + 1));
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
	const float dps = towerStats.damage / (towerStats.attackSpeed * 0.001f);

	str += Language::Instance().translate(Language::DAMAGE_PER_SECOND) + separator + GlobalVariables::to_string_with_precision(dps, 2);
	str += endline;

	str += Language::Instance().translate(Language::RADIUS) + separator + GlobalVariables::to_string_with_precision(towerStats.radius, 1);
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
	m_sprite.setColor(Engine::Instance().cursor()->inPanel()?Color::Red:Color::Green);

	if (!Engine::Instance().cursor()->inPanel())
		return;
	const Vector2f pos = Engine::Instance().cursor()->windowCursorPos();

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

void GamePanel::updateStartEndPos(const Vector2f &startPos, const Vector2f& endPos)
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
			resultEndPos.x -= GlobalVariables::Instance().mapTileSize().x + GlobalVariables::Instance().tileSize().x;
		}
		else
		{
			//+x
			resultEndPos.x += GlobalVariables::Instance().mapTileSize().x;
		}
		resultEndPos.y -= GlobalVariables::Instance().mapTileSize().y + GlobalVariables::Instance().tileSize().y;
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

void GamePanel::moveCursorDown()
{
	currentCursorPos += 7;
	updateCurrentCursor();
}

void GamePanel::moveCursorUp()
{
	currentCursorPos -= 7;
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

void GamePanel::updateCurrentTower()
{
	Color color;
	int level = 0;
	if (m_selectedTower == nullptr)
		color = GlobalVariables::GrayColor;
	else
	{
		color = Color::White;
		level = m_selectedTower->level();
	}
	upgradeSprite.setTextureRect(IntRect(64*level,0,64,64));
	sellSprite.setColor(color);
	upgradeSprite.setColor(color);

	bool canUpgrade = false;;
	if (m_selectedTower != nullptr)
	{
		const float cost = getTowerUpgradeCost(m_selectedTower);
		canUpgrade = Engine::Instance().level()->getMoneyCount() < cost;
	}
	if (level > 2 || canUpgrade)
		upgradeSprite.setColor(GlobalVariables::GrayColor);
}

void GamePanel::updateCurrentCursor()
{
	if(currentCursorPos >= actionsSprites.size())
		currentCursorPos = 0;

	cursorSprite.setPosition(actionsSprites.at(currentCursorPos)->getPosition());
	updateInfo();
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
		case SELL:
		{
			str = Language::Instance().translate(Language::SELL);

			if (m_selectedTower != nullptr)
			{
				str += endline;
				str += "Cost" +  GlobalVariables::to_string_with_precision(getTowerSellCost(m_selectedTower), 1);
			}
		}
			break;
		case UPGRADE:
		{
			str = Language::Instance().translate(Language::UPGRADE);

			if (m_selectedTower != nullptr)
			{
				str += endline;
				str += "Cost" +  GlobalVariables::to_string_with_precision(getTowerUpgradeCost(m_selectedTower), 1);
			}
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
	const float icons_space = ICONS_SPACE * Settings::GAME_SCALE * Settings::Instance().getScaleFactor().y;
	Vector2f size;
	size.x = towerBaseSprite.getGlobalBounds().width * Settings::GAME_SCALE + icons_space;
	size.x *= 6;
	size.y = towerBaseSprite.getGlobalBounds().height;
	return FloatRect(pos, size);
}

FloatRect GamePanel::getAbilitiesRect() const
{
	const Vector2f pos = Vector2f(abilityBombSprite.getGlobalBounds().left, abilityBombSprite.getGlobalBounds().top);
	const float icons_space = ICONS_SPACE * Settings::GAME_SCALE * Settings::Instance().getScaleFactor().y;
	Vector2f size;
	size.x = abilityBombSprite.getGlobalBounds().width * Settings::GAME_SCALE + icons_space;
	size.x *= 6;
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

FloatRect GamePanel::getRemovRect() const
{
	return sellSprite.getGlobalBounds();
}

FloatRect GamePanel::getUpgradeRect() const
{
	return upgradeSprite.getGlobalBounds();
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
	rTexture.create(static_cast<unsigned int>(Settings::Instance().getResolution().x),
					static_cast<unsigned int>(Settings::Instance().getResolution().y));

}

float GamePanel::getBottomValue() const
{
	return m_bottomValue;
}

//void GamePanel::press(const Vector2i &pos)
//{
//	if (Engine::Instance().level()->getState() == Level::WAIT_READY)
//		if (spriteReady.getGlobalBounds().contains(pos.x, pos.y))
//			Engine::Instance().level()->ready();
//}

