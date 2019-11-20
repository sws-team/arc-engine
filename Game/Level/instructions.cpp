#include "instructions.h"
#include "settings.h"
#include "globalvariables.h"
#include "Game/gamepanel.h"
#include "Engine/engine.h"

Instructions::Instructions() :
  m_state(START)
{
	shadowRect.setSize(Vector2f(Settings::Instance().getResolution().x, Settings::Instance().getResolution().y));
	Color color = GlobalVariables::GrayColor;
	color.a = 150;
	shadowRect.setFillColor(color);

	textRect.setFillColor(Color::Green);
	textRect.setSize(Vector2f(300, 100));

	targetRect.setFillColor(Color::Transparent);
	targetRect.setOutlineThickness(3);
	targetRect.setOutlineColor(Color::Blue);

	text.setFont(GlobalVariables::Instance().font());
	text.setCharacterSize(25);
	text.setFillColor(Color::Red);
	text.setOutlineColor(Color::Black);
	text.setOutlineThickness(2);
	text.setScale(Settings::Instance().getScaleFactor());
}

void Instructions::draw(RenderTarget * const target)
{
	if (m_state == FINISHED)
		return;

	target->draw(shadowRect);

	target->draw(textRect);
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
	case TOWERS:
	{
		textRect.setPosition(500, 500);
		rect = Engine::Instance().panel()->getTowersRect();
		textStr = "Towers";
	}
		break;
	case ABILITIES:
	{
		textRect.setPosition(500, 500);
		rect = Engine::Instance().panel()->getAbilitiesRect();
		textStr = "Abilities";
	}
		break;
	case MONEY:
	{
		textRect.setPosition(500, 500);
		rect = Engine::Instance().panel()->getMoneyRect();
		textStr = "Money";
	}
		break;
	case HEALTH:
	{
		textRect.setPosition(500, 500);
		rect = Engine::Instance().panel()->getHealthRect();
		textStr = "Health";
	}
		break;

	case ENERGY:
	{
		textRect.setPosition(500, 500);
		rect = Engine::Instance().panel()->getEnergyRect();
		textStr = "Energy";
	}
		break;
	case REMOVE:
	{
		textRect.setPosition(500, 500);
		rect = Engine::Instance().panel()->getRemovRect();
		textStr = "Remove";
	}
		break;
	case UPGRADE:
	{
		textRect.setPosition(500, 500);
		rect = Engine::Instance().panel()->getUpgradeRect();
		textStr = "Upgrade";
	}
		break;
	case PROGRESS:
	{
		textRect.setPosition(500, 500);
		rect = Engine::Instance().panel()->getProgressRect();
		textStr = "Progress";
	}
		break;
	default:
		break;
	}
	textStr += "\nPress Return to continue or Space to skip";
	text.setString(textStr);
	text.setPosition(textRect.getPosition().x,
					 textRect.getPosition().y);
	targetRect.setPosition(rect.left, rect.top);
	targetRect.setSize(Vector2f(rect.width, rect.height));
	m_state = state;
}
