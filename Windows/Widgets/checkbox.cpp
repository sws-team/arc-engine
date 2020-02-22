#include "checkbox.h"
#include "controller.h"
#include "Game/Audio/soundcontroller.h"

CheckBox::CheckBox() :
	m_isChecked(false)
  ,hovered(false)
{
	rect.setOutlineThickness(2);
	rect.setOutlineColor(Color::Black);
	rect.setFillColor(Color::Transparent);
	currentRect.setFillColor(Color::Transparent);

	circle.setOutlineThickness(2);
	circle.setOutlineColor(Color::Black);
	circle.setFillColor(Color::Blue);
}

void CheckBox::draw(RenderTarget *target)
{
	target->draw(rect);
	if (m_isChecked)
		target->draw(circle);
	target->draw(currentRect);
}

void CheckBox::event(Event *event)
{
	if (event->type == Event::MouseButtonReleased)
	{
		if (rect.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
		{
			m_isChecked = !m_isChecked;
			SoundController::Instance().playOnce(CLICK_SOUND_FILE);
			update();
		}
	}
	else if (event->type == Event::MouseMoved)
	{
		bool hover = false;
		currentRect.setFillColor(Color::Transparent);
		if (rect.getGlobalBounds().contains(event->mouseMove.x, event->mouseMove.y))
		{
			hover = true;
			currentRect.setFillColor(Widget::HOVERED_COLOR);
		}
		if (hover && !hovered)
		{
			hovered = true;
			SoundController::Instance().playOnce(HOVER_SOUND_FILE);
		}
		hovered = hover;
	}
	else if (event->type == Event::KeyPressed)
	{
		if (event->key.code == Keyboard::Return)
		{
			m_isChecked = !m_isChecked;
			update();
		}
	}
	else if (event->type == Event::JoystickButtonPressed)
	{
		if (event->joystickButton.button == Controller::KEY_START)
		{
			m_isChecked = !m_isChecked;
			update();
		}
	}
}

void CheckBox::update()
{
	rect.setPosition(m_pos);
	currentRect.setPosition(m_pos);
	rect.setSize(m_size);
	currentRect.setSize(m_size);

	const float radius = m_size.x/3;
	circle.setPosition(m_pos.x + radius/2, m_pos.y + radius/2);
	circle.setRadius(radius);
}

bool CheckBox::isChecked() const
{
	return m_isChecked;
}

void CheckBox::setChecked(bool checked)
{
	m_isChecked = checked;
}

void CheckBox::setBorderColor(const Color &borderColor)
{
	rect.setOutlineColor(borderColor);
	circle.setOutlineColor(borderColor);
}

void CheckBox::setCheckedColor(const Color &checkedColor)
{
	circle.setFillColor(checkedColor);
}
