#include "instructions.h"
#include "gamepanel.h"
#include "engine.h"
#include "enginedef.h"
#include "managers.h"
#include "gamemanagers.h"
#include "gameoptions.h"
#include "gamepanel.h"
#include "Balance/balance.h"

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

	text.setFont(Engine::Instance().fontManager()->font());
	text.setCharacterSize(35);
	text.setFillColor(sf::Color::Red);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(2);
	text.setScale(scaleFactor);

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

	textRext.setFillColor(sf::Color(191,145,255, 128));
	textRext.setScale(scaleFactor);
	textRext.setPosition(INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().x,
						 INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().y);

	arrow.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::ARROW));
	arrow.setScale(scaleFactor);
}

void Instructions::init(const unsigned int level)
{
	character.setPosition(Engine::Instance().settingsManager()->getResolution().x - character.getGlobalBounds().width,
						  Engine::Instance().options<GameOptions>()->panel()->getBottomValue() - character.getGlobalBounds().height);
	active = true;
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
	if (showArrow)
	{
		target->draw(targetRect);
		target->draw(arrow);
	}
	target->draw(character);
}

void Instructions::update()
{

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
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_BASE);
		str += EngineDefs::endline;
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
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_POWER);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::POWER_TOWER_DESCRIPTION);
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
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_ROCKET);
		str += EngineDefs::endline;
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
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_FREEZE);
		str += EngineDefs::endline;
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
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_LASER);
		str += EngineDefs::endline;
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
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_IMPROVED);
		str += EngineDefs::endline;
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
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_BOMB);
		str += EngineDefs::endline;
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
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_FREEZE_BOMB);
		str += EngineDefs::endline;
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
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_ACID);
		str += EngineDefs::endline;
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
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE);
		str += EngineDefs::endline;
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
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED);
		str += EngineDefs::endline;
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
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY_STOP);
		str += EngineDefs::endline;
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
	{
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::SMOKE_NAME);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::SMOKE_DESCRIPTION);
	}
		break;
	case DRAIN:
	{
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DRAIN_NAME);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DRAIN_DESCRIPTION);
	}
		break;
	case REGRESS:
	{
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::REGRESS_NAME);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::REGRESS_DESCRIPTION);
	}
		break;
	case EXPLOSION:
	{
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_EXPLOSION_NAME);
		str += EngineDefs::endline;
		str += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION);
	}
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
		showArrow = false;
		rect = Engine::Instance().options<GameOptions>()->panel()->getProgressRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_PROGRESS);
	}
		break;
	case INSTRUCTION_TOWER_BASE:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getTowerRect(BASE);
		textStr = towerInfoText(BASE);
	}
		break;
	case INSTRUCTION_TOWER_FREEZE:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getTowerRect(FREEZE);
		textStr = towerInfoText(FREEZE);
	}
		break;
	case INSTRUCTION_TOWER_ENERGY:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getTowerRect(POWER);
		textStr = towerInfoText(POWER);
	}
		break;
	case INSTRUCTION_TOWER_ROCKET:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getTowerRect(ROCKET);
		textStr = towerInfoText(ROCKET);
	}
		break;
	case INSTRUCTION_TOWER_LASER:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getTowerRect(LASER);
		textStr = towerInfoText(LASER);
	}
		break;
	case INSTRUCTION_TOWER_IMPROVED:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getTowerRect(IMPROVED);
		textStr = towerInfoText(IMPROVED);
	}
		break;
	case INSTRUCTION_BOMB:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getAbilityRect(ABILITY_BOMB);
		textStr = abilityInfoText(ABILITY_BOMB);
	}
		break;
	case INSTRUCTION_FREEZE_BOMB:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getAbilityRect(ABILITY_FREEZE_BOMB);
		textStr = abilityInfoText(ABILITY_FREEZE_BOMB);
	}
		break;
	case INSTRUCTION_ACID:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getAbilityRect(ABILITY_ACID);
		textStr = abilityInfoText(ABILITY_ACID);
	}
		break;
	case INSTRUCTION_INCREASE_DAMAGE:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getAbilityRect(ABILITY_INCREASE_TOWER_DAMAGE);
		textStr = abilityInfoText(ABILITY_INCREASE_TOWER_DAMAGE);
	}
		break;
	case INSTRUCTION_INCREASE_ATTACK_SPEED:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getAbilityRect(ABILITY_INCREASE_TOWER_ATTACK_SPEED);
		textStr = abilityInfoText(ABILITY_INCREASE_TOWER_ATTACK_SPEED);
	}
		break;
	case INSTRUCTION_STOP:
	{
		showArrow = true;
		rect = Engine::Instance().options<GameOptions>()->panel()->getAbilityRect(ABILITY_STOP);
		textStr = abilityInfoText(ABILITY_STOP);
	}
		break;
	case MAP_SMOKE:
	{
		showArrow = false;
		textStr = mapEffectInfoText(SMOKE);
	}
		break;
	case MAP_REGRESS:
	{
		showArrow = false;
		textStr = mapEffectInfoText(REGRESS);
	}
		break;
	case MAP_DRAIN:
	{
		showArrow = false;
		textStr = mapEffectInfoText(DRAIN);
	}
		break;
	case MAP_EXPLOSION:
	{
		showArrow = false;
		textStr = mapEffectInfoText(EXPLOSION);
	}
		break;
	default:
		break;
	}
	textStr += EngineDefs::endline;
	textStr += EngineDefs::endline;
	textStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_SKIP);
	text.setString(textStr);
	targetRect.setPosition(rect.left, rect.top);
	targetRect.setSize(sf::Vector2f(rect.width, rect.height));
	updateTextRect();
	updateArrowPos();
}

void Instructions::updateTextRect()
{
	const sf::Vector2f lineSize = sf::Vector2f(32 * Engine::Instance().settingsManager()->getScaleFactor().x,
											   32 * Engine::Instance().settingsManager()->getScaleFactor().y);
	const float textHeight = text.getLocalBounds().height + lineSize.y * 3;
	const sf::Vector2f rectSize = sf::Vector2f(TEXT_WIDTH, textHeight);
	textRext.setSize(rectSize);

	top.setSize(sf::Vector2f(rectSize.x, lineSize.y));
	top.setPosition(textRext.getGlobalBounds().left, textRext.getGlobalBounds().top);

	left.setSize(sf::Vector2f(lineSize.x, rectSize.y));
	left.setPosition(textRext.getGlobalBounds().left, textRext.getGlobalBounds().top);

	right.setSize(sf::Vector2f(lineSize.x, rectSize.y));
	right.setPosition(textRext.getGlobalBounds().left + textRext.getGlobalBounds().width - right.getGlobalBounds().width,
					  textRext.getGlobalBounds().top);

	bottom.setSize(sf::Vector2f(rectSize.x, lineSize.y));
	bottom.setPosition(textRext.getGlobalBounds().left,
					   textRext.getGlobalBounds().top + textRext.getGlobalBounds().height - bottom.getGlobalBounds().height);

	text.setPosition(textRext.getPosition().x + left.getGlobalBounds().width,
					 textRext.getPosition().y + top.getGlobalBounds().height);
}

void Instructions::updateArrowPos()
{
	float x = targetRect.getGlobalBounds().left;
	x += targetRect.getGlobalBounds().width / 2;
	x -= arrow.getGlobalBounds().width / 2;
	float y = targetRect.getGlobalBounds().top;
	y -= arrow.getGlobalBounds().height;
	arrow.setPosition(x, y);
}
