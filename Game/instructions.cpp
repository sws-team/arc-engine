#include "instructions.h"
#include "gamepanel.h"
#include "engine.h"
#include "enginedef.h"
#include "managers.h"
#include "gamemanagers.h"
#include "gameoptions.h"
#include "gamepanel.h"

Instructions::Instructions() :
  m_state(START)
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
	text.setCharacterSize(40);
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

	const sf::Vector2f rectSize = sf::Vector2f(1024, 256);
	textRext.setFillColor(sf::Color(191,145,255, 128));
	textRext.setSize(rectSize);
	textRext.setScale(scaleFactor);
	textRext.setPosition(INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().x,
						 INSTRUCTIONS_OFFSET * Engine::Instance().settingsManager()->getScaleFactor().y);

	const sf::Vector2f lineSize = sf::Vector2f(32 * scaleFactor.x, 32 * scaleFactor.y);

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

void Instructions::init()
{
	changeState(START);
}

void Instructions::draw(sf::RenderTarget * const target)
{
	if (m_state == FINISHED)
		return;

	target->draw(shadowRect);

	target->draw(textRext);
	target->draw(targetRect);
	target->draw(left);
	target->draw(right);
	target->draw(bottom);
	target->draw(top);
	target->draw(text);

	target->draw(character);
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

		character.setPosition(Engine::Instance().settingsManager()->getResolution().x - character.getGlobalBounds().width,
							  Engine::Instance().options<GameOptions>()->panel()->getBottomValue() - character.getGlobalBounds().height);
		rect = sf::FloatRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_WELCOME);
	}
		break;
	case TOWERS:
	{
		rect = Engine::Instance().options<GameOptions>()->panel()->getTowersRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_TOWERS);
	}
		break;
	case ABILITIES:
	{
		rect = Engine::Instance().options<GameOptions>()->panel()->getAbilitiesRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_ABILITIES);
	}
		break;
	case MONEY:
	{

		rect = Engine::Instance().options<GameOptions>()->panel()->getMoneyRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_MONEY);
	}
		break;
	case HEALTH:
	{
		rect = Engine::Instance().options<GameOptions>()->panel()->getHealthRect();
		textStr = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INSTRUCTION_HEALTH);
	}
		break;
	case PROGRESS:
	{
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
	targetRect.setPosition(rect.left, rect.top);
	targetRect.setSize(sf::Vector2f(rect.width, rect.height));
	m_state = state;
}
