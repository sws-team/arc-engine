#include "instructions.h"
#include "settings.h"
#include "globalvariables.h"
#include "Game/gamepanel.h"
#include "Engine/engine.h"
#include "Translations/language.h"
#include "ResourcesManager/resourcesmanager.h"

Instructions::Instructions() :
  m_state(START)
{
	shadowRect.setSize(Vector2f(Settings::Instance().getResolution().x, Settings::Instance().getResolution().y));
	Color color = GlobalVariables::GrayColor;
	color.a = 150;
	shadowRect.setFillColor(color);

	textSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::INSTRUCTIONS_TEXTURE));
	textSprite.setScale(Settings::Instance().getScaleFactor());

	targetRect.setFillColor(Color::Transparent);
	targetRect.setOutlineThickness(3);
	targetRect.setOutlineColor(Color::Blue);

	text.setFont(GlobalVariables::Instance().font());
	text.setCharacterSize(25);
	text.setFillColor(Color::Red);
	text.setOutlineColor(Color::Black);
	text.setOutlineThickness(2);
	text.setScale(Settings::Instance().getScaleFactor());

	changeState(START);
}

void Instructions::draw(RenderTarget * const target)
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
	FloatRect rect;
	String textStr;
	switch (state)
	{
	case START:
	{
		textSprite.setPosition(INSTRUCTIONS_OFFSET * Settings::Instance().getScaleFactor().x,
							 INSTRUCTIONS_OFFSET * Settings::Instance().getScaleFactor().y);
		rect = FloatRect();
		textStr = Language::Instance().translate(Language::INSTRUCTION_WELCOME);
	}
		break;
	case TOWERS:
	{
		textSprite.setPosition(INSTRUCTIONS_OFFSET * Settings::Instance().getScaleFactor().x,
							 INSTRUCTIONS_OFFSET * Settings::Instance().getScaleFactor().y);
		rect = Engine::Instance().panel()->getTowersRect();
		textStr = Language::Instance().translate(Language::INSTRUCTION_TOWERS);
	}
		break;
	case ABILITIES:
	{
		textSprite.setPosition(INSTRUCTIONS_OFFSET * Settings::Instance().getScaleFactor().x,
							 INSTRUCTIONS_OFFSET * Settings::Instance().getScaleFactor().y);
		rect = Engine::Instance().panel()->getAbilitiesRect();
		textStr = Language::Instance().translate(Language::INSTRUCTION_ABILITIES);
	}
		break;
	case MONEY:
	{
		textSprite.setPosition(INSTRUCTIONS_OFFSET * Settings::Instance().getScaleFactor().x,
							 INSTRUCTIONS_OFFSET * Settings::Instance().getScaleFactor().y);
		rect = Engine::Instance().panel()->getMoneyRect();
		textStr = Language::Instance().translate(Language::INSTRUCTION_MONEY);
	}
		break;
	case HEALTH:
	{
		textSprite.setPosition(INSTRUCTIONS_OFFSET * Settings::Instance().getScaleFactor().x,
							 INSTRUCTIONS_OFFSET * Settings::Instance().getScaleFactor().y);
		rect = Engine::Instance().panel()->getHealthRect();
		textStr = Language::Instance().translate(Language::INSTRUCTION_HEALTH);
	}
		break;
	case PROGRESS:
	{
		textSprite.setPosition(INSTRUCTIONS_OFFSET * Settings::Instance().getScaleFactor().x,
							 INSTRUCTIONS_OFFSET * Settings::Instance().getScaleFactor().y);
		rect = Engine::Instance().panel()->getProgressRect();
		textStr = Language::Instance().translate(Language::INSTRUCTION_PROGRESS);
	}
		break;
	default:
		break;
	}
	textStr += "\n";
	textStr += Language::Instance().translate(Language::INSTRUCTION_SKIP);
	text.setString(textStr);
	text.setPosition(textSprite.getPosition().x,
					 textSprite.getPosition().y);
	targetRect.setPosition(rect.left, rect.top);
	targetRect.setSize(Vector2f(rect.width, rect.height));
	m_state = state;
}
