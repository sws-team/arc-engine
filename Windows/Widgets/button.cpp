#include "button.h"
#include "globalvariables.h"
#include "controller.h"

Button::Button()
{
	rect.setOutlineThickness(2);
	rect.setOutlineColor(Color::Black);
	rect.setFillColor(Color::White);

	m_text.setFont(GlobalVariables::Instance().font());
	m_text.setCharacterSize(30);
	m_text.setFillColor(Color::Black);
}

void Button::draw(RenderTarget *target)
{
	target->draw(rect);
	if (!m_text.getString().isEmpty())
		target->draw(m_text);
}

void Button::event(Event *event)
{
	if (event->type == Event::MouseButtonReleased)
	{
		if (rect.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
			m_callback();
	}
	else if (event->type == Event::KeyPressed)
	{
		if (event->key.code == Keyboard::Return)
			m_callback();
	}
	else if (event->type == Event::JoystickButtonPressed)
	{
		if (event->joystickButton.button == Controller::KEY_START)
			m_callback();
	}
}

void Button::update()
{
	rect.setPosition(m_pos);
	rect.setSize(m_size);

	m_text.setPosition(m_pos);
}

void Button::setCallback(function<void ()> callback)
{
	m_callback = callback;
}

void Button::setText(const String &text)
{
	m_text.setString(text);
	update();
}

void Button::setTextCharacterSize(unsigned int size)
{
	m_text.setCharacterSize(size);
}

void Button::setScale(const Vector2f &scaleFactor)
{
//	rect.setScale(scaleFactor);
	m_text.setScale(scaleFactor);
}

void Button::setFillColor(const Color &fillColor)
{
	rect.setFillColor(fillColor);
}

void Button::setBorderColor(const Color &borderColor)
{
	rect.setOutlineColor(borderColor);
}

void Button::setTextColor(const Color &textColor)
{
	m_text.setFillColor(textColor);
}
