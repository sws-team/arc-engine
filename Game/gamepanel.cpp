#include "gamepanel.h"
#include "globalvariables.h"
#include "settings.h"
#include "Level/level.h"
#include "ResourcesManager/resourcesmanager.h"
#include "Engine/engine.h"
#include "Level/camera.h"
#include "Level/tower.h"
#include "Level/lifebar.h"

#include <stdlib.h>

GamePanel::GamePanel() :
	GameDrawable()
{
	progress = new LifeBar();
	m_selectedTower = nullptr;

	m_sprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::PANEL_TEXTURE));
	m_sprite.setScale(Settings::Instance().getScaleFactor() * Settings::GAME_SCALE);

	energyCountText.setFont(GlobalVariables::Instance().font());
	energyCountText.setFillColor(Color::Black);
	energyCountText.setOutlineColor(Color::Yellow);
	energyCountText.setOutlineThickness(2);
	energyCountText.setCharacterSize(34);

	lifeCountText.setFont(GlobalVariables::Instance().font());
	lifeCountText.setFillColor(Color::Black);
	lifeCountText.setOutlineColor(Color::Yellow);
	lifeCountText.setOutlineThickness(2);
	lifeCountText.setCharacterSize(34);

	sellRect.setTexture(&ResourcesManager::Instance().getTexture(RESOURCES::SELL_TEXTURE));
	sellRect.setFillColor(Color::Cyan);
	sellRect.setSize(GlobalVariables::Instance().tileSize());

	upgradeRect.setTexture(&ResourcesManager::Instance().getTexture(RESOURCES::UPGRADE_TEXTURE));
	upgradeRect.setFillColor(Color::Cyan);
	upgradeRect.setSize(GlobalVariables::Instance().tileSize());

	abilityBombSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_BOMB));
	abilityBombSprite.setScale(Settings::Instance().getScaleFactor());

	abilityFreezeBombSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_FREEZE_BOMB));
	abilityFreezeBombSprite.setScale(Settings::Instance().getScaleFactor());

	abilityCarpetBombingSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_CARPET_BOMBING));
	abilityCarpetBombingSprite.setScale(Settings::Instance().getScaleFactor());

	abilityIncreaseTowerDamageSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_INCREASE_TOWER_DAMAGE));
	abilityIncreaseTowerDamageSprite.setScale(Settings::Instance().getScaleFactor());

	abilityIncreaseTowerAttackSpeedSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_INCREASE_TOWER_ATTACK_SPEED));
	abilityIncreaseTowerAttackSpeedSprite.setScale(Settings::Instance().getScaleFactor());

	abilityTimeStopSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_TIME_STOP));
	abilityTimeStopSprite.setScale(Settings::Instance().getScaleFactor());


	towerBaseSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_BASE));
	towerBaseSprite.setScale(Settings::Instance().getScaleFactor());

	towerLaserSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_LASER));
	towerLaserSprite.setScale(Settings::Instance().getScaleFactor());

	towerFreezeSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_FREEZE));
	towerFreezeSprite.setScale(Settings::Instance().getScaleFactor());

	towerRocketSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_ROCKET));
	towerRocketSprite.setScale(Settings::Instance().getScaleFactor());

	towerPowerSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_POWER));
	towerPowerSprite.setScale(Settings::Instance().getScaleFactor());

	towerImprovedSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_IMPROVED));
	towerImprovedSprite.setScale(Settings::Instance().getScaleFactor());

	spriteReady.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::READY_TEXTURE));
	spriteReady.setScale(Settings::Instance().getScaleFactor());

	m_bottomValue = 0;
	progress->init(Vector2i(Settings::Instance().getResolution().x * 0.3f, LifeBar::LIFE_BAR_HEIGHT * Settings::Instance().getScaleFactor().y), Color::Red);
}

GamePanel::~GamePanel()
{

}

void GamePanel::draw(RenderTarget * const target)
{
	//update pos
	Vector2f pos;
	pos.x = target->getView().getCenter().x - target->getView().getSize().x/2;
	pos.y = target->getView().getCenter().y + target->getView().getSize().y/2 - m_sprite.getGlobalBounds().height;
	m_bottomValue = pos.y;
	pos = updatePos(pos);

	//draw minimap
	Sprite miniMapSprite;
	rTexture.setView(*Engine::Instance().camera()->getMiniMapView());
	rTexture.create(static_cast<unsigned int>(Settings::Instance().getResolution().x),
					static_cast<unsigned int>(Settings::Instance().getResolution().y));
	rTexture.clear(Color::Transparent);
	Engine::Instance().level()->drawLevel(&rTexture);
	rTexture.display();

	miniMapSprite.setTexture(rTexture.getTexture());
	miniMapSprite.scale(Settings::Instance().gameScale() * 0.3f, Settings::Instance().gameScale() * 0.3f);
	miniMapSprite.setPosition(pos);

	//draw
	target->draw(m_sprite);

	target->draw(energyCountText);
	target->draw(lifeCountText);

	target->draw(abilityBombSprite);
	target->draw(abilityFreezeBombSprite);
	target->draw(abilityCarpetBombingSprite);
	target->draw(abilityIncreaseTowerDamageSprite);
	target->draw(abilityIncreaseTowerAttackSpeedSprite);
	target->draw(abilityTimeStopSprite);

	target->draw(towerBaseSprite);
	target->draw(towerLaserSprite);
	target->draw(towerFreezeSprite);
	target->draw(towerRocketSprite);
	target->draw(towerPowerSprite);
	target->draw(towerImprovedSprite);

	if (m_selectedTower != nullptr)
	{
		target->draw(sellRect);
		target->draw(upgradeRect);
	}
	if(Engine::Instance().level()->getState() == Level::WAIT_READY)
		target->draw(spriteReady);
	target->draw(miniMapSprite);
	progress->draw(target);
}

void GamePanel::update()
{
	const int energy = static_cast<int>(Engine::Instance().level()->getEnergyCount());
	const int life = static_cast<int>(Engine::Instance().level()->getLifeCount());
	energyCountText.setString(String(to_string(energy)));
	lifeCountText.setString(String(to_string(life)));

	const float progressValue = static_cast<float>(Engine::Instance().level()->currentProgress()) / m_progressMax;
	progress->setValue(progressValue);
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
}

ACTION_STATE GamePanel::getCurrentIcon(const Vector2f &pos) const
{
	Vector2f center = pos;
	center += Vector2f(ICON_SIZE/2, ICON_SIZE/2);

	if (towerBaseSprite.getGlobalBounds().contains(center) ||
			towerLaserSprite.getGlobalBounds().contains(center) ||
			towerFreezeSprite.getGlobalBounds().contains(center) ||
			towerRocketSprite.getGlobalBounds().contains(center) ||
			towerPowerSprite.getGlobalBounds().contains(center) ||
			towerImprovedSprite.getGlobalBounds().contains(center)
			)
		return ACTION_STATE::ADD_TOWER;

	if (abilityBombSprite.getGlobalBounds().contains(center))
		return ACTION_STATE::ABILITY_BOMB;

	if (abilityFreezeBombSprite.getGlobalBounds().contains(center))
		return ACTION_STATE::ABILITY_FREEZE_BOMB;

	if (abilityTimeStopSprite.getGlobalBounds().contains(center))
		return ACTION_STATE::ABILITY_STOP_TIME;

	if (abilityCarpetBombingSprite.getGlobalBounds().contains(center))
		return ACTION_STATE::ABILITY_CARPET_BOMBING;

	if (abilityIncreaseTowerDamageSprite.getGlobalBounds().contains(center))
		return ACTION_STATE::ABILITY_INCREASE_TOWER_DAMAGE;

	if (abilityIncreaseTowerAttackSpeedSprite.getGlobalBounds().contains(center))
		return ACTION_STATE::ABILITY_INCREASE_TOWER_ATTACK_SPEED;

	return ACTION_STATE::READY;
}

TOWER_TYPES GamePanel::currentTower(const Vector2f &pos) const
{
	Vector2f center = pos;
	center += Vector2f(ICON_SIZE/2, ICON_SIZE/2);

	if (towerBaseSprite.getGlobalBounds().contains(center))
		return BASE;

	if (towerLaserSprite.getGlobalBounds().contains(center))
		return LASER;

	if (towerFreezeSprite.getGlobalBounds().contains(center))
		return FREEZE;

	if (towerRocketSprite.getGlobalBounds().contains(center))
		return ROCKET;

	if (towerPowerSprite.getGlobalBounds().contains(center))
		return POWER;

	if (towerImprovedSprite.getGlobalBounds().contains(center))
		return IMPROVED;

	return POWER;
}

Vector2f GamePanel::updatePos(const Vector2f &nullPos)
{
	const Vector2f iconSize = Vector2f(ICON_SIZE * Settings::Instance().getScaleFactor().x * Settings::GAME_SCALE,
								 ICON_SIZE * Settings::Instance().getScaleFactor().y * Settings::GAME_SCALE);

	const float block_space = iconSize.x/2;
	const float icons_space = 24 * Settings::GAME_SCALE;
	const float label_offset = 16 * Settings::GAME_SCALE;
	const float info_offset = 256 * Settings::GAME_SCALE;
	const float panel_offset = 62 * Settings::GAME_SCALE;
	const float text_offset = 320 * Settings::GAME_SCALE;

	Vector2f pos = nullPos;


	progress->setPos(Vector2f(pos.x + Settings::Instance().getResolution().x * 0.3f, 20));

	m_sprite.setPosition(pos);

	pos.y += icons_space;
	pos.x += panel_offset;
	pos.x += label_offset;
	energyCountText.setPosition(pos);

	pos.y += energyCountText.getGlobalBounds().height;
	lifeCountText.setPosition(pos);
	pos.y -= energyCountText.getGlobalBounds().height;
	pos.x -= label_offset;

	pos.x += info_offset;

	pos.x += block_space;
	sellRect.setPosition(pos);

	pos.y += iconSize.y;
	upgradeRect.setPosition(pos);

	pos.y -= iconSize.y;
	pos.x += iconSize.x;

	pos.x += block_space;

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
	abilityIncreaseTowerAttackSpeedSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	abilityBombSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	abilityFreezeBombSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	abilityCarpetBombingSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	abilityIncreaseTowerDamageSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	abilityTimeStopSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	pos.y -= iconSize.y;
	pos.x += icons_space;


	spriteReady.setPosition(Vector2f(pos.x, pos.y - 400));

//	miniMapSprite.setPosition(pos);
	return pos;
}

void GamePanel::setProgressMax(int progressMax)
{
	m_progressMax = progressMax;
}

float GamePanel::getBottomValue() const
{
	return m_bottomValue;
}

void GamePanel::press(const Vector2i &pos)
{
	if (Engine::Instance().level()->getState() == Level::WAIT_READY)
		if (spriteReady.getGlobalBounds().contains(pos.x, pos.y))
			Engine::Instance().level()->ready();
}

