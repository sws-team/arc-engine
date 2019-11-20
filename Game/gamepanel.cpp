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

#include <stdlib.h>

GamePanel::GamePanel() :
	GameDrawable()
  ,isCursorVisible(false)
{
	progress = new LifeBar();
	m_selectedTower = nullptr;

	m_sprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::PANEL_TEXTURE));
	m_sprite.setScale(Settings::Instance().getScaleFactor() * Settings::GAME_SCALE);

	info.setFont(GlobalVariables::Instance().font());
	info.setFillColor(Color::Black);
	info.setOutlineColor(Color::Yellow);
	info.setOutlineThickness(2);
	info.setCharacterSize(20);
	info.setScale(Settings::Instance().getScaleFactor());

	moneyCountText.setFont(GlobalVariables::Instance().font());
	moneyCountText.setFillColor(Color::Black);
	moneyCountText.setOutlineColor(Color::Yellow);
	moneyCountText.setOutlineThickness(2);
	moneyCountText.setCharacterSize(34);
	moneyCountText.setScale(Settings::Instance().getScaleFactor());

	lifeCountText.setFont(GlobalVariables::Instance().font());
	lifeCountText.setFillColor(Color::Black);
	lifeCountText.setOutlineColor(Color::Yellow);
	lifeCountText.setOutlineThickness(2);
	lifeCountText.setCharacterSize(34);
	lifeCountText.setScale(Settings::Instance().getScaleFactor());

	energyCountText.setFont(GlobalVariables::Instance().font());
	energyCountText.setFillColor(Color::Black);
	energyCountText.setOutlineColor(Color::Yellow);
	energyCountText.setOutlineThickness(2);
	energyCountText.setCharacterSize(34);
	energyCountText.setScale(Settings::Instance().getScaleFactor());

	cursorSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::PANEL_CURSOR));
	cursorSprite.setScale(Settings::Instance().getScaleFactor());

	sellSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::SELL_TEXTURE));
	sellSprite.setScale(Settings::Instance().getScaleFactor());

	upgradeSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::UPGRADE_TEXTURE));
	upgradeSprite.setScale(Settings::Instance().getScaleFactor());

	abilityBombSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_BOMB));
	abilityBombSprite.setScale(Settings::Instance().getScaleFactor());

	abilityFreezeBombSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_FREEZE_BOMB));
	abilityFreezeBombSprite.setScale(Settings::Instance().getScaleFactor());

	abilityVenomSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_CARPET_BOMBING));
	abilityVenomSprite.setScale(Settings::Instance().getScaleFactor());

	abilityIncreaseTowerDamageSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_INCREASE_TOWER_DAMAGE));
	abilityIncreaseTowerDamageSprite.setScale(Settings::Instance().getScaleFactor());

	abilityIncreaseTowerAttackSpeedSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_INCREASE_TOWER_ATTACK_SPEED));
	abilityIncreaseTowerAttackSpeedSprite.setScale(Settings::Instance().getScaleFactor());

	abilityUnknownSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::ABILITY_TIME_STOP));
	abilityUnknownSprite.setScale(Settings::Instance().getScaleFactor());


	towerBaseSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_BASE));
	towerBaseSprite.setScale(Settings::Instance().getScaleFactor());
	towerBaseSprite.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);

	towerLaserSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_LASER));
	towerLaserSprite.setScale(Settings::Instance().getScaleFactor());
	towerLaserSprite.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);

	towerFreezeSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_FREEZE));
	towerFreezeSprite.setScale(Settings::Instance().getScaleFactor());
	towerFreezeSprite.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);

	towerRocketSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_ROCKET));
	towerRocketSprite.setScale(Settings::Instance().getScaleFactor());
	towerRocketSprite.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);

	towerPowerSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_POWER));
	towerPowerSprite.setScale(Settings::Instance().getScaleFactor());
	towerPowerSprite.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);

	towerImprovedSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::TOWER_IMPROVED));
	towerImprovedSprite.setScale(Settings::Instance().getScaleFactor());
	towerImprovedSprite.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);

	spriteReady.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::READY_TEXTURE));
	spriteReady.setScale(Settings::Instance().getScaleFactor());

	actionsSprites.push_back(&sellSprite);
	actionsSprites.push_back(&upgradeSprite);
	actionsSprites.push_back(&towerBaseSprite);
	actionsSprites.push_back(&towerFreezeSprite);
	actionsSprites.push_back(&towerPowerSprite);
	actionsSprites.push_back(&towerLaserSprite);
	actionsSprites.push_back(&towerRocketSprite);
	actionsSprites.push_back(&towerImprovedSprite);
	actionsSprites.push_back(&abilityBombSprite);
	actionsSprites.push_back(&abilityFreezeBombSprite);
	actionsSprites.push_back(&abilityVenomSprite);
	actionsSprites.push_back(&abilityIncreaseTowerDamageSprite);
	actionsSprites.push_back(&abilityIncreaseTowerAttackSpeedSprite);
	actionsSprites.push_back(&abilityUnknownSprite);

	m_bottomValue = 0;
	progress->init(Vector2i(Settings::Instance().getResolution().x * PROGRESS_WIDTH,
							LifeBar::LIFE_BAR_HEIGHT * Settings::Instance().getScaleFactor().y), Color::Red);
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
//	progress->setPos(Vector2f(pos.x + Settings::Instance().getResolution().x * 0.3f, 20 * Settings::Instance().getScaleFactor().y));
	progress->setPos(Vector2f(pos.x + Settings::Instance().getResolution().x * PROGRESS_WIDTH,
							  pos.y - target->getView().getSize().y + PROGRESS_OFFSET * Settings::Instance().getScaleFactor().y));
	pos.y -= m_sprite.getGlobalBounds().height;
	m_bottomValue = pos.y;
	pos = updatePos(pos);

	//draw minimap

	rTexture.clear(Color::Transparent);
	Engine::Instance().level()->drawLevel(&rTexture);
	rTexture.display();

	Sprite miniMapSprite;
	miniMapSprite.setTexture(rTexture.getTexture());
	miniMapSprite.scale(Settings::Instance().gameScale() * MINIMAP_SCALE, Settings::Instance().gameScale() * MINIMAP_SCALE);
	miniMapSprite.setPosition(pos);

	//draw
	target->draw(m_sprite);

	target->draw(moneyCountText);
	target->draw(lifeCountText);
	target->draw(energyCountText);

	updateEnableAbilities();

	target->draw(info);

	target->draw(abilityBombSprite);
	target->draw(abilityFreezeBombSprite);
	target->draw(abilityVenomSprite);
	target->draw(abilityIncreaseTowerDamageSprite);
	target->draw(abilityIncreaseTowerAttackSpeedSprite);
	target->draw(abilityUnknownSprite);

	updateEnableTowers();

	target->draw(towerBaseSprite);
	target->draw(towerLaserSprite);
	target->draw(towerFreezeSprite);
	target->draw(towerRocketSprite);
	target->draw(towerPowerSprite);
	target->draw(towerImprovedSprite);

	target->draw(sellSprite);
	target->draw(upgradeSprite);

	if(Engine::Instance().level()->getState() == Level::WAIT_READY)
		target->draw(spriteReady);
	target->draw(miniMapSprite);
	if (Engine::Instance().getMission() != GlobalVariables::SURVIVAL_MODE_ID)
		progress->draw(target);

	if (isCursorVisible)
		target->draw(cursorSprite);
}

void GamePanel::update()
{
	const int money = static_cast<int>(Engine::Instance().level()->getMoneyCount());
	const int life = static_cast<int>(Engine::Instance().level()->getLifeCount());
	const int energy = static_cast<int>(Engine::Instance().level()->getEnergyCount());
	moneyCountText.setString(String(to_string(money)));
	lifeCountText.setString(String(to_string(life)));
	energyCountText.setString(String(to_string(energy)));

	const float progressValue = static_cast<float>(Engine::Instance().level()->currentProgress()) / m_progressMax;
	progress->setValue(progressValue);

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

	if (abilityUnknownSprite.getGlobalBounds().contains(center))
		return ACTION_STATE::ABILITY_UNKNOWN;

	if (abilityVenomSprite.getGlobalBounds().contains(center))
		return ACTION_STATE::ABILITY_VENOM;

	if (abilityIncreaseTowerDamageSprite.getGlobalBounds().contains(center))
		return ACTION_STATE::ABILITY_INCREASE_TOWER_DAMAGE;

	if (abilityIncreaseTowerAttackSpeedSprite.getGlobalBounds().contains(center))
		return ACTION_STATE::ABILITY_INCREASE_TOWER_ATTACK_SPEED;

	if (sellSprite.getGlobalBounds().contains(center))
		return ACTION_STATE::SELL;

	if (upgradeSprite.getGlobalBounds().contains(center))
		return ACTION_STATE::UPGRADE;

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
	const float icons_space = ICONS_SPACE * Settings::GAME_SCALE * Settings::Instance().getScaleFactor().y;
	const float label_offset = 16 * Settings::GAME_SCALE * Settings::Instance().getScaleFactor().x;
	const float info_offset = 256 * Settings::GAME_SCALE * Settings::Instance().getScaleFactor().x;
	const float panel_offset = 62 * Settings::GAME_SCALE * Settings::Instance().getScaleFactor().x;
	const float text_offset = 320 * Settings::GAME_SCALE * Settings::Instance().getScaleFactor().x;

	Vector2f pos = nullPos;

	m_sprite.setPosition(pos);

	pos.y += icons_space;
	pos.x += panel_offset;
	pos.x += label_offset;
	moneyCountText.setPosition(pos);

	pos.y += moneyCountText.getGlobalBounds().height;
	lifeCountText.setPosition(pos);
	pos.y += moneyCountText.getGlobalBounds().height;
	energyCountText.setPosition(pos);
	pos.y -= 2 * moneyCountText.getGlobalBounds().height;

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

	abilityUnknownSprite.setPosition(pos);
	pos.x += icons_space;
	pos.x += iconSize.x;

	pos.y -= iconSize.y;
	pos.x += icons_space;


	spriteReady.setPosition(Vector2f(pos.x, pos.y - 400));

//	miniMapSprite.setPosition(pos);
	return pos;
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

	cost = TowersFactory::getTowerStats(TOWER_TYPES::POWER).cost;
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
	const float energy = Engine::Instance().level()->getEnergyCount();

	float cost = Level::BOMB_ABILITY_COST;
	if (energy < cost || !iconsAvaliable.isAbilityBombEnabled)
		abilityBombSprite.setColor(GlobalVariables::GrayColor);
	else
		abilityBombSprite.setColor(Color::White);

	cost = Level::FREEZE_BOMB_ABILITY_COST;
	if (energy < cost || !iconsAvaliable.isAbilityFreezeBombEnabled)
		abilityFreezeBombSprite.setColor(GlobalVariables::GrayColor);
	else
		abilityFreezeBombSprite.setColor(Color::White);

	cost = Level::VENOM_ABILITY_COST;
	if (energy < cost || !iconsAvaliable.isAbilityVenomEnabled)
		abilityVenomSprite.setColor(GlobalVariables::GrayColor);
	else
		abilityVenomSprite.setColor(Color::White);

	cost = Level::INC_TOWER_DMG_ABILITY_COST;
	if (energy < cost || !iconsAvaliable.isAbilityIncreaseTowerDamageEnabled)
		abilityIncreaseTowerDamageSprite.setColor(GlobalVariables::GrayColor);
	else
		abilityIncreaseTowerDamageSprite.setColor(Color::White);

	cost = Level::INC_TOWER_AS_ABILITY_COST;
	if (energy < cost || !iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled)
		abilityIncreaseTowerAttackSpeedSprite.setColor(GlobalVariables::GrayColor);
	else
		abilityIncreaseTowerAttackSpeedSprite.setColor(Color::White);

	cost = 10000;
	if (energy < cost || !iconsAvaliable.isAbilityUnknownEnabled)
		abilityUnknownSprite.setColor(GlobalVariables::GrayColor);
	else
		abilityUnknownSprite.setColor(Color::White);
}

String GamePanel::towerInfo(TOWER_TYPES type, Tower *tower)
{
	String str;
	switch (type)
	{
	case BASE:
		str = Language::Instance().translate(Language::TOWER_BASE);
		break;
	case POWER:
		str = Language::Instance().translate(Language::TOWER_POWER);
		break;
	case ROCKET:
		str = Language::Instance().translate(Language::TOWER_ROCKET);
		break;
	case FREEZE:
		str = Language::Instance().translate(Language::TOWER_FREEZE);
		break;
	case LASER:
		str = Language::Instance().translate(Language::TOWER_LASER);
		break;
	case IMPROVED:
		str = Language::Instance().translate(Language::TOWER_IMPROVED);
		break;
	}

	const String endline = "\n";
	str += endline;
	const String separator = ": ";
	if (tower != nullptr)
		str += Language::Instance().translate(Language::LEVEL) + separator + to_string(tower->level()) + endline;

	const TowerStats towerStats = TowersFactory::getTowerStats(type);

	str += Language::Instance().translate(Language::DAMAGE) + separator + GlobalVariables::to_string_with_precision(towerStats.damage, 2);
	if (tower != nullptr)
		str += " + " + GlobalVariables::to_string_with_precision(tower->data().damage - towerStats.damage, 2);
	str += endline;

	str += Language::Instance().translate(Language::ATTACK_SPEED) + separator + GlobalVariables::to_string_with_precision(towerStats.attackSpeed, 2);
	if (tower != nullptr)
		str += " + " + GlobalVariables::to_string_with_precision(tower->data().attackSpeed - towerStats.attackSpeed, 2);
	str += endline;

	str += Language::Instance().translate(Language::RADIUS) + separator + to_string(towerStats.radius);
	if (tower != nullptr)
		str += " + " + to_string(tower->data().radius - towerStats.radius);
	str += endline;

	str += Language::Instance().translate(Language::PROJECTILE_SPEED) + separator + GlobalVariables::to_string_with_precision(towerStats.projectileSpeed, 2);
	str += endline;

	if (tower == nullptr)
	{
		str += Language::Instance().translate(Language::COST);
		str += separator;
		str += GlobalVariables::to_string_with_precision(towerStats.cost, 2);
	}
	else
	{
		str += Language::Instance().translate(Language::SELL_COST);
		str += separator;
		str += GlobalVariables::to_string_with_precision(tower->data().cost/2, 2);
	}
	return str;
}

void GamePanel::updateCursor()
{
	isCursorVisible = false;
	if (!Engine::Instance().cursor()->inPanel())
		return;
	const Vector2f pos = Engine::Instance().cursor()->pos();
	Vector2f center = pos;
	center += Vector2f(ICON_SIZE/2, ICON_SIZE/2);
	for(Sprite *sprite : actionsSprites)
	{
		if (sprite->getGlobalBounds().contains(center))
		{
			cursorSprite.setPosition(sprite->getPosition());
			isCursorVisible = true;
			break;
		}
	}
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
	if (level > 2)
		upgradeSprite.setColor(GlobalVariables::GrayColor);
}

void GamePanel::updateInfo()
{
	String str;
	if (Engine::Instance().cursor()->inPanel())
	{
		const ACTION_STATE state = getCurrentIcon(Engine::Instance().cursor()->pos());
		switch (state)
		{
		case ADD_TOWER:
		{
			const TOWER_TYPES type = currentTower(Engine::Instance().cursor()->pos());
			str = towerInfo(type, nullptr);
		}
			break;
		case ABILITY_VENOM:
			str = Language::Instance().translate(Language::ABILITY_VENOM);
			break;
		case ABILITY_BOMB:
			str = Language::Instance().translate(Language::ABILITY_BOMB);
			break;
		case ABILITY_FREEZE_BOMB:
			str = Language::Instance().translate(Language::ABILITY_FREEZE_BOMB);
			break;
		case ABILITY_INCREASE_TOWER_ATTACK_SPEED:
			str = Language::Instance().translate(Language::ABILITY_INCREASE_TOWER_ATTACK_SPEED);
			break;
		case ABILITY_INCREASE_TOWER_DAMAGE:
			str = Language::Instance().translate(Language::ABILITY_INCREASE_TOWER_DAMAGE);
			break;
		case ABILITY_UNKNOWN:
			str = Language::Instance().translate(Language::ABILITY_UNKNOWN);
			break;
		case SELL:
			str = Language::Instance().translate(Language::SELL);
			break;
		case UPGRADE:
			str = Language::Instance().translate(Language::UPGRADE);
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
	iconsAvaliable.isAbilityUnknownEnabled = true;

	switch (n)
	{
	case 0:
		iconsAvaliable.isRocketEnabled = false;
		iconsAvaliable.isFreezeEnabled = false;
		iconsAvaliable.isLaserEnabled = false;
		iconsAvaliable.isImprovedEnabled = false;
		iconsAvaliable.isAbilityBombEnabled = false;
		iconsAvaliable.isAbilityFreezeBombEnabled = false;
		iconsAvaliable.isAbilityVenomEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerDamageEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled = false;
		iconsAvaliable.isAbilityUnknownEnabled = false;
		break;
	case 1:
		iconsAvaliable.isRocketEnabled = false;
		iconsAvaliable.isLaserEnabled = false;
		iconsAvaliable.isImprovedEnabled = false;
		iconsAvaliable.isAbilityBombEnabled = false;
		iconsAvaliable.isAbilityVenomEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerDamageEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled = false;
		iconsAvaliable.isAbilityUnknownEnabled = false;
		break;
	case 2:
		iconsAvaliable.isLaserEnabled = false;
		iconsAvaliable.isImprovedEnabled = false;
		iconsAvaliable.isAbilityVenomEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerDamageEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled = false;
		iconsAvaliable.isAbilityUnknownEnabled = false;
		break;
	case 3:
		iconsAvaliable.isImprovedEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerDamageEnabled = false;
		iconsAvaliable.isAbilityIncreaseTowerAttackSpeedEnabled = false;
		iconsAvaliable.isAbilityUnknownEnabled = false;
		break;
	case 4:
		iconsAvaliable.isImprovedEnabled = false;
		iconsAvaliable.isAbilityUnknownEnabled = false;
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
	return lifeCountText.getGlobalBounds();
}

FloatRect GamePanel::getEnergyRect() const
{
	return energyCountText.getGlobalBounds();
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

void GamePanel::press(const Vector2i &pos)
{
	if (Engine::Instance().level()->getState() == Level::WAIT_READY)
		if (spriteReady.getGlobalBounds().contains(pos.x, pos.y))
			Engine::Instance().level()->ready();
}

