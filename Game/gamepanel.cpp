#include "gamepanel.h"
#include "globalvariables.h"
#include "settings.h"
#include "Level/level.h"
#include "ResourcesManager/resourcesmanager.h"
#include "Engine/engine.h"
#include "Level/camera.h"

#include <stdlib.h>

const float GamePanel::PANEL_HEIGHT = GlobalVariables::CELL_SIZE * GamePanel::PANEL_CELLS_COUNT;

GamePanel::GamePanel() :
	GameDrawable()
{
	const Vector2f iconSize = Vector2f(ICON_SIZE * Settings::Instance().getScaleFactor().x,
								 ICON_SIZE * Settings::Instance().getScaleFactor().y);


	m_sprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::PANEL_TEXTURE));

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

	towerLaserSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_POWER));
	towerLaserSprite.setScale(Settings::Instance().getScaleFactor());

	towerFreezeSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_FREEZE));
	towerFreezeSprite.setScale(Settings::Instance().getScaleFactor());

	towerRocketSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_ROCKET));
	towerRocketSprite.setScale(Settings::Instance().getScaleFactor());

	towerSplashSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_SPLASH));
	towerSplashSprite.setScale(Settings::Instance().getScaleFactor());

	towerImprovedSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_IMPROVED));
	towerImprovedSprite.setScale(Settings::Instance().getScaleFactor());
}

GamePanel::~GamePanel()
{

}

void GamePanel::draw(RenderTarget * const target)
{
	m_sprite.setScale(target->getView().getSize().x / PANEL_WIDTH,
					  (target->getView().getSize().y / PANEL_HEIGHT) * PANEL_SCALE);

	Vector2f pos;
	pos.x = target->getView().getCenter().x - target->getView().getSize().x/2;
	pos.y = target->getView().getCenter().y + target->getView().getSize().y/2 - m_sprite.getGlobalBounds().height;
	m_sprite.setPosition(pos);

	pos.y += GlobalVariables::Instance().tileSize().y;
	float x = pos.x;

	pos.x += GlobalVariables::Instance().tileSize().x;
	towerBaseSprite.setPosition(pos);
	pos.x += GlobalVariables::Instance().tileSize().x * 2;
	towerFreezeSprite.setPosition(pos);
	pos.x += GlobalVariables::Instance().tileSize().x * 2;
	towerRocketSprite.setPosition(pos);
	pos.x += GlobalVariables::Instance().tileSize().x * 2;
	abilityBombSprite.setPosition(pos);
	pos.x += GlobalVariables::Instance().tileSize().x * 2;
	abilityFreezeBombSprite.setPosition(pos);
	pos.x += GlobalVariables::Instance().tileSize().x * 2;
	abilityCarpetBombingSprite.setPosition(pos);
	pos.x = x;
	pos.y += GlobalVariables::Instance().tileSize().y;
	towerImprovedSprite.setPosition(pos);
	pos.x += GlobalVariables::Instance().tileSize().x * 2;
	towerLaserSprite.setPosition(pos);
	pos.x += GlobalVariables::Instance().tileSize().x * 2;
	towerSplashSprite.setPosition(pos);
	pos.x += GlobalVariables::Instance().tileSize().x * 4;
	abilityIncreaseTowerAttackSpeedSprite.setPosition(pos);
	pos.x += GlobalVariables::Instance().tileSize().x * 2;
	abilityIncreaseTowerDamageSprite.setPosition(pos);
	pos.x += GlobalVariables::Instance().tileSize().x * 2;
	abilityTimeStopSprite.setPosition(pos);


	target->draw(m_sprite);
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
	target->draw(towerSplashSprite);
	target->draw(towerImprovedSprite);

	//draw minimap
	rTexture.setView(*Engine::Instance().camera()->getMiniMapView());
	rTexture.create(static_cast<unsigned int>(Settings::Instance().getResolution().x),
					static_cast<unsigned int>(Settings::Instance().getResolution().y));
	rTexture.clear(Color::Transparent);
	Engine::Instance().level()->drawLevel(&rTexture);
	rTexture.display();

	Sprite sprite;
	sprite.setTexture(rTexture.getTexture());
	sprite.setPosition(0, 0);
	sprite.scale(Settings::Instance().gameScale() * 0.25f, Settings::Instance().gameScale() * 0.25f);

	const float minimapOffset = m_sprite.getGlobalBounds().height - sprite.getGlobalBounds().height;


	sprite.setPosition(m_sprite.getPosition().x + m_sprite.getGlobalBounds().width - sprite.getGlobalBounds().width + minimapOffset,
					   m_sprite.getPosition().y);

	target->draw(sprite);
}

void GamePanel::update()
{

}

int GamePanel::cellsCount() const
{
	return static_cast<int>(m_sprite.getGlobalBounds().height / GlobalVariables::Instance().tileSize().y);
}

