#include "checkbox.h"

CheckBox::CheckBox() :
	m_isChecked(false)
{
	rect.setOutlineThickness(2);
	rect.setOutlineColor(Color::Black);
	rect.setFillColor(Color::Transparent);

	circle.setOutlineThickness(2);
	circle.setFillColor(Color::Blue);
	circle.setOutlineColor(Color::Green);
}

void CheckBox::draw(RenderTarget *target)
{
	target->draw(rect);
	if (m_isChecked)
		target->draw(circle);
}

void CheckBox::event(Event *event)
{
	if (event->type == Event::MouseButtonReleased)
	{
		if (rect.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
		{
			m_isChecked = !m_isChecked;
			update();
		}
	}
}

void CheckBox::update()
{
	rect.setPosition(m_pos);
	rect.setSize(m_size);

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
