#include "instructions.h"
#include "gamepanel.h"
#include "engine.h"
#include "enginedef.h"
#include "managers.h"
#include "gamemanagers.h"
#include "gameoptions.h"
#include "gamepanel.h"
#include "Balance/balance.h"
#include "gameobject.h"

const std::map<int, std::vector<Instructions::STATES> > Instructions::INSTRUCTIONS =
{
	{1, { WELCOME, MONEY, HEALTH, PROGRESS, TOWERS, UPGRADE_SELL, INSTRUCTION_TOWER_ENERGY, INSTRUCTION_TOWER_BASE, INSTRUCTION_BOMB, GOOD_LUCK } },
	{2, { INSTRUCTION_TOWER_FREEZE, INSTRUCTION_FREEZE_BOMB} },
	{3, { INSTRUCTION_TOWER_ROCKET, INSTRUCTION_ACID } },
	{4, { INSTRUCTION_TOWER_LASER } },
	{5, { INSTRUCTION_INCREASE_DAMAGE, INSTRUCTION_INCREASE_ATTACK_SPEED, MAP_SMOKE } },
	{6, { INSTRUCTION_TOWER_IMPROVED, INSTRUCTION_STOP, MAP_REGRESS} },
	{7, { MAP_DRAIN} },
	{8, { MAP_EXPLOSION} },
};

const sf::Vector2i Instructions::demoSize = sf::Vector2i(320, 240);

Instructions::Instructions() :
	currentState(0)
  ,active(false)
  ,showArrow(false)
  ,demoObject(nullptr)
{
	shadowRect.setSize(sf::Vector2f(Engine::Instance().settingsManager()->getResolution().x,
									Engine::Instance().settingsManager()->getResolution().y));
	sf::Color color = EngineDefs::GrayColor;
	color.a = 150;
	shadowRect.setFillColor(color);

	const sf::Vector2f scaleFactor = Engine::Instance().settingsManager()->getScaleFactor();

	targetRect.setFillColor(sf::Color::Transparent);
	targetRect.setOutlineThickness(3);
	targetRect.setOutlineColor(sf::Color(33,79,29));

	const unsigned int textCharacterSize = 35;
	text.setFont(Engine::Instance().fontManager()->font());
	text.setCharacterSize(Engine::Instance().fontManager()->getCharSize(textCharacterSize));
	text.setFillColor(sf::Color(25,40,7));
	text.setOutlineColor(sf::Color(186,191,188));
	text.setOutlineThickness(2);
	text.setScale(scaleFactor);

	skipText.setFont(Engine::Instance().fontManager()->font());
	skipText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(textCharacterSize * 0.7f));
	skipText.setFillColor(text.getFillColor());
	skipText.setOutlineColor(text.getOutlineColor());
	skipText.setOutlineThickness(2);
	skipText.setScale(scaleFactor);

	character.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::CHARACTER));
	character.setScale(scaleFactor);

	bottom.setTexture(&Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::INSTRUCTIONS_BOTTOM));
	bottom.setScale(scaleFactor);

	top.setTexture(&Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::INSTRUCTIONS_TOP));
	top.setScale(scaleFactor);

	left.setTexture(&Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::INSTRUCTIONS_VERTICAL));
	left.setScale(scaleFactor);

	right.setTexture(&Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::INSTRUCTIONS_VERTICAL));
	right.setScale(scaleFactor);

	sf::Color textRectColor = GameOptions::primaryColor;
	textRectColor.a = 128;
	textRext.setFillColor(textRectColor);
	textRext.setScale(scaleFactor);

	arrow = new GameObject(GAME_TEXTURE::ARROW, sf::Vector2f(0,0), sf::Vector2i(64, 105), 8);
	arrow->animationSpeed = 75;

	demoRect.setSize(sf::Vector2f(demoSize.x + 2 * DEMO_FRAME_OFFSET,
								  demoSize.y + 2 * DEMO_FRAME_OFFSET));
	demoRect.setTexture(&Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::FRAME_DEMO));
	demoRect.setScale(scaleFactor);
}

Instructions::~Instructions()
{
	delete arrow;
	if (demoObject != nullptr)
		delete demoObject;
}

void Instructions::init(const unsigned int level)
{
	skipText.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_SKIP));
	character.setPosition(Engine::Instance().settingsManager()->getResolution().x - character.getGlobalBounds().width,
						  Engine::Instance().options<GameOptions>()->panel()->getBottomValue() - character.getGlobalBounds().height);
	active = true;
	states.clear();
	if (Instructions::INSTRUCTIONS.find(level) != Instructions::INSTRUCTIONS.end())
		states = INSTRUCTIONS.at(level);
	if (level == 0)
	{
		for (int i = WELCOME; i < FINISHED; ++i)
			states.push_back(static_cast<STATES>(i));
	}
	currentState = 0;
	updateState();
}

void Instructions::draw(sf::RenderTarget * const target)
{
	if (!active)
		return;

	target->draw(shadowRect);

	target->draw(textRext);
	target->draw(left);
	target->draw(right);
	target->draw(bottom);
	target->draw(top);
	target->draw(text);
	target->draw(skipText);
	if (demoObject != nullptr)
	{
		demoObject->draw(target);
		target->draw(demoRect);
	}
	if (showArrow)
	{
		target->draw(targetRect);
		arrow->draw(target);
	}
	target->draw(character);
}

void Instructions::update()
{
	arrow->update();
	if (demoObject != nullptr)
		demoObject->update();
}

bool Instructions::isActive() const
{
	return active;
}

void Instructions::next()
{
	currentState++;
	updateState();
}

void Instructions::skip()
{
	active = false;
}

sf::String Instructions::towerInfoText(TOWER_TYPES type)
{
	sf::String str;
	switch (type)
	{
	case BASE:
	{		
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY);
		str += EngineDefs::separator;
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::BASE_TOWER_ABILITY);
	}
		break;
	case POWER:
	{
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::POWER_TOWER_TRAIT);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY);
		str += EngineDefs::separator;
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::POWER_TOWER_ABILITY);
	}
		break;
	case ROCKET:
	{		
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY);
		str += EngineDefs::separator;
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ROCKET_TOWER_ABILITY);
	}
		break;
	case FREEZE:
	{		
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY);
		str += EngineDefs::separator;
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::FREEZE_TOWER_ABILITY);
	}
		break;
	case LASER:
	{		
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY);
		str += EngineDefs::separator;
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::LASER_TOWER_ABILITY);
	}
		break;
	case IMPROVED:
	{		
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY);
		str += EngineDefs::separator;
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY);
	}
		break;
	}
	str += EngineDefs::endline;
	if (type == POWER)
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::CAN_BUILD_ANYWHERE);
	else
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::CAN_BUILD_NEAR_ENERGY_TOWER);
	str += EngineDefs::endline;

	const sf::String levelStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::LEVEL);
	sf::String dpsStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DAMAGE_PER_SECOND);
	sf::String radiusStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::RADIUS);
	sf::String costStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COST);
	dpsStr += EngineDefs::separator + EngineDefs::endline;
	radiusStr += EngineDefs::separator + EngineDefs::endline;
	costStr += EngineDefs::separator + EngineDefs::endline;
	const sf::String sep = ";";
	const sf::String space = " ";
	const std::vector<TowerStats> towersStats = Engine::Instance().options<GameOptions>()->panel()->levelTowerStats(type);
	for (size_t level = 0; level < towersStats.size(); ++level)
	{
		bool newLine = level % 2 != 0;
		const int printLevel = level + 1;
		const TowerStats stats = towersStats.at(level);

		const float dps = stats.damage / (stats.attackSpeed * 1 / EngineDefs::MSEC);
		dpsStr += levelStr;
		dpsStr + space;
		dpsStr += EngineDefs::separator;
		dpsStr += std::to_string(printLevel);
		dpsStr += EngineDefs::separator;
		dpsStr += GlobalVariables::to_string_with_precision(dps, 2);
		if (newLine)
			dpsStr += EngineDefs::endline;
		else
			dpsStr += sep + space;

		radiusStr += levelStr;
		radiusStr += space;
		radiusStr += std::to_string(printLevel);
		radiusStr += EngineDefs::separator;
		radiusStr += GlobalVariables::to_string_with_precision(stats.radius, 2);
		if (newLine)
			radiusStr += EngineDefs::endline;
		else
			radiusStr += sep + space;

		costStr += levelStr;
		costStr += space;
		costStr += std::to_string(printLevel);
		costStr += EngineDefs::separator;
		costStr += GlobalVariables::to_string_with_precision(stats.cost, 2);
		if (newLine)
			costStr += EngineDefs::endline;
		else
			costStr += sep  + space;
	}
	str += dpsStr;
	str += radiusStr;
	str += costStr;
	return str;
}

sf::String Instructions::abilityInfoText(ACTION_STATE type)
{
	sf::String str;
	switch (type)
	{
	case ACTION_STATE::ABILITY_BOMB:
	{		
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COOLDOWN_TIME);
		str += EngineDefs::separator;
		str += GlobalVariables::to_string_with_precision(Balance::Instance().getBombCooldown(), 1);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DAMAGE_ATTRIBUTE);
		str += EngineDefs::separator;
		str += GlobalVariables::to_string_with_precision(Balance::Instance().getBombDamage(), 1);
	}
		break;
	case ACTION_STATE::ABILITY_FREEZE_BOMB:
	{		
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COOLDOWN_TIME);
		str += EngineDefs::separator;
		str += GlobalVariables::to_string_with_precision(Balance::Instance().getFreezeCooldown(), 1);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DURATION_ATTRIBUTE);
		str += EngineDefs::separator;
		str += GlobalVariables::to_string_with_precision(Balance::Instance().getFreezeDuration(), 1);
	}
		break;
	case ACTION_STATE::ABILITY_ACID:
	{
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COOLDOWN_TIME);
		str += EngineDefs::separator;
		str += GlobalVariables::to_string_with_precision(Balance::Instance().getAcidCooldown(), 1);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DAMAGE_ATTRIBUTE);
		str += EngineDefs::separator;
		str += GlobalVariables::to_string_with_precision(Balance::Instance().getAcidCount() * Balance::Instance().getAcidDamage(), 1);
	}
		break;
	case ACTION_STATE::ABILITY_INCREASE_TOWER_DAMAGE:
	{
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COOLDOWN_TIME);
		str += EngineDefs::separator;
		str += GlobalVariables::to_string_with_precision(Balance::Instance().getIncreaseDamageCooldown(), 1);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DURATION_ATTRIBUTE);
		str += EngineDefs::separator;
		str += GlobalVariables::to_string_with_precision(Balance::Instance().getIncreaseDamageDuration(), 1);
	}
		break;
	case ACTION_STATE::ABILITY_INCREASE_TOWER_ATTACK_SPEED:
	{
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COOLDOWN_TIME);
		str += EngineDefs::separator;
		str += GlobalVariables::to_string_with_precision(Balance::Instance().getIncreaseAttackSpeedCooldown(), 1);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DURATION_ATTRIBUTE);
		str += EngineDefs::separator;
		str += GlobalVariables::to_string_with_precision(Balance::Instance().getIncreaseAttackSpeedDuration(), 1);
	}
		break;
	case ACTION_STATE::ABILITY_STOP:
	{
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COOLDOWN_TIME);
		str += EngineDefs::separator;
		str += GlobalVariables::to_string_with_precision(Balance::Instance().getStopCooldown(), 1);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DURATION_ATTRIBUTE);
		str += EngineDefs::separator;
		str += GlobalVariables::to_string_with_precision(Balance::Instance().getStopDuration(), 1);
	}
		break;
	default:
		break;
	}
	return str;
}

sf::String Instructions::mapEffectInfoText(Instructions::MAP_EFFECTS type)
{
	sf::String str;
	switch (type)
	{
	case SMOKE:
		str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::SMOKE_DESCRIPTION);
		break;
	case DRAIN:
		str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DRAIN_DESCRIPTION);
		break;
	case REGRESS:
		str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::REGRESS_DESCRIPTION);
		break;
	case EXPLOSION:
		str = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION);
		break;
	default:
		break;
	}
	return str;
}

void Instructions::updateState()
{
	if (currentState >= states.size())
	{
		active = false;
		return;
	}
	GAME_TEXTURE::TEXTURE_ID textureId = GAME_TEXTURE::FRAME;
	arrow->sprite.setRotation(0);
	sf::Vector2f offset = sf::Vector2f(0,0);
	const STATES state = states.at(currentState);
	sf::FloatRect rect = sf::FloatRect();
	sf::String textStr;
	switch (state)
	{
	case WELCOME:
	{
		showArrow = false;
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_WELCOME);
	}
		break;
	case MONEY:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getMoneyRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_MONEY);
	}
		break;
	case HEALTH:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getHealthRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_HEALTH);
	}
		break;
	case PROGRESS:
	{
		showArrow = true;
		arrow->sprite.setRotation(180);
		offset.y += arrow->size.y * 2;
		rect = Engine::Instance().options<GameOptions>()->panel()->getProgressRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_PROGRESS);
	}
		break;
	case TOWERS:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getTowersRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_TOWERS);
	}
		break;
	case UPGRADE_SELL:
	{
		showArrow = false;
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_UPGRADE_SELL);
	}
		break;
	case GOOD_LUCK:
	{
		showArrow = false;
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_GOOD_LUCK);
	}
		break;
	case INSTRUCTION_TOWER_BASE:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getTowerRect(0);
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_BASE);
		textStr += EngineDefs::endline;
		textStr += towerInfoText(BASE);
	}
		break;
	case INSTRUCTION_TOWER_FREEZE:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getTowerRect(1);
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_FREEZE);
		textStr += EngineDefs::endline;
		textStr += towerInfoText(FREEZE);
	}
		break;
	case INSTRUCTION_TOWER_ENERGY:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getTowerRect(3);
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_POWER);
		textStr += EngineDefs::endline;
		textStr += towerInfoText(POWER);
	}
		break;
	case INSTRUCTION_TOWER_ROCKET:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getTowerRect(2);
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_ROCKET);
		textStr += EngineDefs::endline;
		textStr += towerInfoText(ROCKET);
	}
		break;
	case INSTRUCTION_TOWER_LASER:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getTowerRect(4);
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_LASER);
		textStr += EngineDefs::endline;
		textStr += towerInfoText(LASER);
	}
		break;
	case INSTRUCTION_TOWER_IMPROVED:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getTowerRect(5);
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_IMPROVED);
		textStr += EngineDefs::endline;
		textStr += towerInfoText(IMPROVED);
	}
		break;
	case INSTRUCTION_BOMB:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getAbilityRect(0);
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_BOMB);
		textStr += EngineDefs::endline;
		textStr += abilityInfoText(ABILITY_BOMB);
		textureId = GAME_TEXTURE::SHOW_BOMB_ABILITY;
	}
		break;
	case INSTRUCTION_FREEZE_BOMB:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getAbilityRect(1);
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_FREEZE_BOMB);
		textStr += EngineDefs::endline;
		textStr += abilityInfoText(ABILITY_FREEZE_BOMB);
		textureId = GAME_TEXTURE::SHOW_FREEZE_BOMB_ABILITY;
	}
		break;
	case INSTRUCTION_ACID:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getAbilityRect(2);
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_ACID);
		textStr += EngineDefs::endline;
		textStr += abilityInfoText(ABILITY_ACID);
	}
		break;
	case INSTRUCTION_INCREASE_DAMAGE:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getAbilityRect(3);
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE);
		textStr += EngineDefs::endline;
		textStr += abilityInfoText(ABILITY_INCREASE_TOWER_DAMAGE);
	}
		break;
	case INSTRUCTION_INCREASE_ATTACK_SPEED:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getAbilityRect(4);
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED);
		textStr += EngineDefs::endline;
		textStr += abilityInfoText(ABILITY_INCREASE_TOWER_ATTACK_SPEED);
	}
		break;
	case INSTRUCTION_STOP:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getAbilityRect(5);
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_STOP);
		textStr += EngineDefs::endline;
		textStr += abilityInfoText(ABILITY_STOP);
	}
		break;
	case MAP_SMOKE:
	{
		showArrow = false;
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::SMOKE_NAME);
		textStr += EngineDefs::endline;
		textStr += mapEffectInfoText(SMOKE);
	}
		break;
	case MAP_REGRESS:
	{
		showArrow = false;
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::REGRESS_NAME);
		textStr += EngineDefs::endline;
		textStr += mapEffectInfoText(REGRESS);
	}
		break;
	case MAP_DRAIN:
	{
		showArrow = false;
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DRAIN_NAME);
		textStr += EngineDefs::endline;
		textStr += mapEffectInfoText(DRAIN);
	}
		break;
	case MAP_EXPLOSION:
	{
		showArrow = false;
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_EXPLOSION_NAME);
		textStr += EngineDefs::endline;
		textStr += mapEffectInfoText(EXPLOSION);
	}
		break;
	default:
		break;
	}
	if (demoObject != nullptr)
	{
		delete demoObject;
		demoObject = nullptr;
	}
	if (textureId != GAME_TEXTURE::FRAME)
	{
		demoObject = new GameObject(textureId, sf::Vector2f(0,0), demoSize, 10);
		demoObject->animationSpeed = 50;
		demoObject->cycled = true;
		demoObject->rowCount = 5;
	}
	text.setString(textStr);
	targetRect.setPosition(rect.left, rect.top);
	targetRect.setSize(sf::Vector2f(rect.width, rect.height));
	updateTextRect();
	updateArrowPos(offset);
}

void Instructions::updateTextRect()
{
	const sf::Vector2f scaleFactor = Engine::Instance().settingsManager()->getScaleFactor();
	const sf::Vector2f lineSize = sf::Vector2f(26, 47);
	const float textHeight = text.getLocalBounds().height +
			lineSize.y * 4 * scaleFactor.y +
			skipText.getGlobalBounds().height;
	const sf::Vector2f rectSize = sf::Vector2f(TEXT_WIDTH, textHeight);	
	const sf::Vector2f rectOffset = sf::Vector2f(RECT_OFFSET * scaleFactor.x,
												 RECT_OFFSET * scaleFactor.y);
	const sf::Vector2f rectPos =
			sf::Vector2f(INSTRUCTIONS_OFFSET * scaleFactor.x,
						 Engine::Instance().settingsManager()->getResolution().y/2 - rectSize.y/2);
	top.setSize(sf::Vector2f(rectSize.x, lineSize.y));
	top.setPosition(rectPos.x, rectPos.y);

	left.setSize(sf::Vector2f(lineSize.x, rectSize.y - rectOffset.y * 2));
	left.setPosition(rectPos.x, rectPos.y + rectOffset.y);

	right.setSize(sf::Vector2f(lineSize.x, rectSize.y - rectOffset.y * 2));
	right.setPosition(rectPos.x + rectSize.x * scaleFactor.x -
					  right.getGlobalBounds().width,
					  rectPos.y + rectOffset.y);

	bottom.setSize(sf::Vector2f(rectSize.x, lineSize.y));
	bottom.setPosition(rectPos.x,
					   rectPos.y + rectSize.y * scaleFactor.y -
					   bottom.getGlobalBounds().height);

	textRext.setPosition(rectPos + sf::Vector2f(RECT_OFFSET * scaleFactor.x,
												RECT_OFFSET * scaleFactor.y));
	text.setPosition(RUBRIC_OFFSET * scaleFactor.x +
					 rectPos.x + left.getGlobalBounds().width,
					 rectPos.y + top.getGlobalBounds().height);
	skipText.setPosition(text.getPosition().x, text.getPosition().y +
						 text.getGlobalBounds().height + lineSize.y * scaleFactor.y);

	textRext.setSize(sf::Vector2f(rectSize.x - 2 * RECT_OFFSET,
								  rectSize.y - 2 * RECT_OFFSET));

	if (demoObject != nullptr)
	{
		const float framesOffset = (DEMO_FRAME_OFFSET + 16) * scaleFactor.x;
		const sf::Vector2f demoPosOffset = sf::Vector2f(demoObject->size.x * scaleFactor.x + framesOffset,
														(demoObject->size.y - textHeight)/2);
		const sf::Vector2f demoPos = rectPos - demoPosOffset;
		demoObject->setPos(demoPos);
		demoRect.setPosition(demoPos - sf::Vector2f(DEMO_FRAME_OFFSET * scaleFactor.x,
													DEMO_FRAME_OFFSET * scaleFactor.y));
	}
}

void Instructions::updateArrowPos(const sf::Vector2f& offset)
{
	float x = targetRect.getGlobalBounds().left;
	x += targetRect.getGlobalBounds().width / 2;
	x -= arrow->getSize().x / 2;
	float y = targetRect.getGlobalBounds().top;
	y -= arrow->getSize().y;
	arrow->setPos(sf::Vector2f(x, y) + offset);
}
