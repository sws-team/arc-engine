#include "instructions.h"
#include "gamepanel.h"
#include "engine.h"
#include "enginedef.h"
#include "managers.h"
#include "gamemanagers.h"
#include "gameoptions.h"

Instructions::Instructions() :
  m_state(START)
{
	shadowRect.setSize(sf::Vector2f(Engine::Instance().settingsManager()->getResolution().x, Engine::Instance().settingsManager()->getResolution().y));
	sf::Color color = EngineDefs::GrayColor;
	color.a = 150;
	shadowRect.setFillColor(color);

	textSprite.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::INSTRUCTIONS_TEXTURE));
	textSprite.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	targetRect.setFillColor(sf::Color::Transparent);
	targetRect.setOutlineThickness(3);
	targetRect.setOutlineColor(sf::Color::Blue);

	text.setFont(Engine::Instance().fontManager()->font());
	text.setCharacterSize(25);
	text.setFillColor(sf::Color::Red);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(2);
	text.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	changeState(START);
}

void Instructions::draw(sf::RenderTarget * const target)
{
	if (m_state == FINISHED)
		return;

	target->draw(shadowRect);

	target->draw(textSprite);
	target->draw(text);
	target->draw(targetRect);
}

void Instructions::update()
{

}

bool Instructions::isActive() const
{
	return m_state != FINISHED;
}

void Instructions::next()
{
	changeState(static_cast<STATES>(m_state + 1));
}

void Instructions::skip()
{
	changeState(FINISHED);
}

void Instructions::changeState(Instructions::STATES state)
{
	sf::FloatRect rect;
	sf::String textStr;
	switch (state)
	{
	case START:
	{
		textSprite.setPosition(INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().x,
							 INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().y);
		rect = sf::FloatRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_WELCOME);
	}
		break;
	case TOWERS:
	{
		textSprite.setPosition(INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().x,
							 INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().y);
		rect = Engine::Instance().options<GameOptions>()->panel()->getTowersRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_TOWERS);
	}
		break;
	case ABILITIES:
	{
		textSprite.setPosition(INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().x,
							 INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().y);
		rect = Engine::Instance().options<GameOptions>()->panel()->getAbilitiesRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_ABILITIES);
	}
		break;
	case MONEY:
	{
		textSprite.setPosition(INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().x,
							 INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().y);
		rect = Engine::Instance().options<GameOptions>()->panel()->getMoneyRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_MONEY);
	}
		break;
	case HEALTH:
	{
		textSprite.setPosition(INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().x,
							 INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().y);
		rect = Engine::Instance().options<GameOptions>()->panel()->getHealthRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_HEALTH);
	}
		break;
	case PROGRESS:
	{
		textSprite.setPosition(INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().x,
							 INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().y);
		rect = Engine::Instance().options<GameOptions>()->panel()->getProgressRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_PROGRESS);
	}
		break;
	default:
		break;
	}
	textStr += "\n";
	textStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_SKIP);
	text.setString(textStr);
	text.setPosition(textSprite.getPosition().x,
					 textSprite.getPosition().y);
	targetRect.setPosition(rect.left, rect.top);
	targetRect.setSize(sf::Vector2f(rect.width, rect.height));
	m_state = state;
}
