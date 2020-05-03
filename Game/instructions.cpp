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
	{0, { WELCOME, MONEY, HEALTH, PROGRESS, INSTRUCTION_TOWER_ENERGY, INSTRUCTION_TOWER_BASE, INSTRUCTION_BOMB } },
	{1, { INSTRUCTION_TOWER_FREEZE, INSTRUCTION_FREEZE_BOMB} },
	{2, { INSTRUCTION_TOWER_ROCKET, INSTRUCTION_ACID } },
	{3, { INSTRUCTION_TOWER_LASER } },
	{4, { INSTRUCTION_INCREASE_DAMAGE, INSTRUCTION_INCREASE_ATTACK_SPEED, MAP_SMOKE } },
	{5, { INSTRUCTION_TOWER_IMPROVED, INSTRUCTION_STOP, MAP_REGRESS} },
	{6, { MAP_DRAIN} },
	{7, { MAP_EXPLOSION} },
};

Instructions::Instructions() :
	currentState(0)
  ,active(false)
  ,showArrow(false)
{
	shadowRect.setSize(sf::Vector2f(Engine::Instance().settingsManager()->getResolution().x,
									Engine::Instance().settingsManager()->getResolution().y));
	sf::Color color = EngineDefs::GrayColor;
	color.a = 150;
	shadowRect.setFillColor(color);

	const sf::Vector2f scaleFactor = Engine::Instance().settingsManager()->getScaleFactor();

	targetRect.setFillColor(sf::Color::Transparent);
	targetRect.setOutlineThickness(3);
	targetRect.setOutlineColor(sf::Color::Blue);

	const unsigned int textCharacterSize = 35;
	text.setFont(Engine::Instance().fontManager()->font());
	text.setCharacterSize(Engine::Instance().fontManager()->getCharSize(textCharacterSize));
	text.setFillColor(GameOptions::secondaryColor);
	text.setOutlineColor(sf::Color::White);
	text.setOutlineThickness(2);
	text.setScale(scaleFactor);

	skipText.setFont(Engine::Instance().fontManager()->font());
	skipText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(textCharacterSize * 0.7f));
	skipText.setFillColor(GameOptions::secondaryColor);
	skipText.setOutlineColor(sf::Color::White);
	skipText.setOutlineThickness(2);
	skipText.setScale(scaleFactor);
	skipText.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_SKIP));

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
	textRext.setPosition((INSTRUCTIONS_OFFSET + RECT_OFFSET) * Engine::Instance().settingsManager()->getScaleFactor().x,
						 (INSTRUCTIONS_OFFSET + RECT_OFFSET) * Engine::Instance().settingsManager()->getScaleFactor().y);

	arrow = new GameObject(GAME_TEXTURE::ARROW, sf::Vector2f(0,0), sf::Vector2i(64, 105), 8);
	arrow->animationSpeed = 75;
}

Instructions::~Instructions()
{
	delete arrow;
}

void Instructions::init(const unsigned int level)
{
	character.setPosition(Engine::Instance().settingsManager()->getResolution().x - character.getGlobalBounds().width,
						  Engine::Instance().options<GameOptions>()->panel()->getBottomValue() - character.getGlobalBounds().height);
	active = true;
	if (level < Instructions::INSTRUCTIONS.size())
		states = INSTRUCTIONS.at(level);
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
	arrow->sprite.setRotation(0);
	sf::Vector2f offset = sf::Vector2f(0,0);
	const STATES state = states.at(currentState);
	sf::FloatRect rect = sf::FloatRect();;
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
	}
		break;
	case INSTRUCTION_FREEZE_BOMB:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getAbilityRect(1);
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_FREEZE_BOMB);
		textStr += EngineDefs::endline;
		textStr += abilityInfoText(ABILITY_FREEZE_BOMB);
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
	text.setString(textStr);
	targetRect.setPosition(rect.left, rect.top);
	targetRect.setSize(sf::Vector2f(rect.width, rect.height));
	updateTextRect();
	updateArrowPos(offset);
}

void Instructions::updateTextRect()
{
	const sf::Vector2f lineSize = sf::Vector2f(26, 47);
	const float textHeight = text.getLocalBounds().height +
			lineSize.y * 4 * Engine::Instance().settingsManager()->getScaleFactor().y +
			skipText.getGlobalBounds().height;
	const sf::Vector2f rectSize = sf::Vector2f(TEXT_WIDTH, textHeight);	
	const sf::Vector2f rectOffset = sf::Vector2f(RECT_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().x,
												 RECT_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().y);
	const sf::Vector2f rectPos = sf::Vector2f(INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().x,
											  (INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().y));

	top.setSize(sf::Vector2f(rectSize.x, lineSize.y));
	top.setPosition(rectPos.x, rectPos.y);

	left.setSize(sf::Vector2f(lineSize.x, rectSize.y - rectOffset.y * 2));
	left.setPosition(rectPos.x, rectPos.y + rectOffset.y);

	right.setSize(sf::Vector2f(lineSize.x, rectSize.y - rectOffset.y * 2));
	right.setPosition(rectPos.x + rectSize.x * Engine::Instance().settingsManager()->getScaleFactor().x -
					  right.getGlobalBounds().width,
					  rectPos.y + rectOffset.y);

	bottom.setSize(sf::Vector2f(rectSize.x, lineSize.y));
	bottom.setPosition(rectPos.x,
					   rectPos.y + rectSize.y * Engine::Instance().settingsManager()->getScaleFactor().y -
					   bottom.getGlobalBounds().height);

	text.setPosition(RUBRIC_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().x +
					 rectPos.x + left.getGlobalBounds().width,
					 rectPos.y + top.getGlobalBounds().height);
	skipText.setPosition(text.getPosition().x, text.getPosition().y +
						 text.getGlobalBounds().height + lineSize.y * Engine::Instance().settingsManager()->getScaleFactor().y);

	textRext.setSize(sf::Vector2f(rectSize.x - 2 * RECT_OFFSET,
								  rectSize.y - 2 * RECT_OFFSET));
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
