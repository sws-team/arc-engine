#include "checkbox.h"
#include "engine.h"
#include "enginedef.h"
#include "managers.h"
#include "mainwindow.h"

CheckBox::CheckBox() :
	m_isChecked(false)
  ,hovered(false)
{
	rect.setOutlineThickness(2);
	rect.setOutlineColor(sf::Color::Black);
	rect.setFillColor(sf::Color::Transparent);
	currentRect.setFillColor(sf::Color::Transparent);

	circle.setOutlineThickness(2);
	circle.setOutlineColor(sf::Color::Black);
	circle.setFillColor(sf::Color::Blue);
}

void CheckBox::draw(sf::RenderTarget *target)
{
	target->draw(rect);
	if (m_isChecked)
		target->draw(circle);
	target->draw(currentRect);
}

void CheckBox::event(sf::Event *event)
{
	if (event->type == sf::Event::MouseButtonReleased)
	{
		if (event->mouseButton.button == sf::Mouse::Left)
		{
			const sf::Vector2i pixelPos = sf::Vector2i(event->mouseButton.x, event->mouseButton.y);
			const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																				   *Engine::Instance().window()->view());

			if (rect.getGlobalBounds().contains(pos))
			{
				m_isChecked = !m_isChecked;
				Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
				update();
			}
		}
	}
	else if (event->type == sf::Event::MouseMoved)
	{
		const sf::Vector2i pixelPos = sf::Vector2i(event->mouseMove.x, event->mouseMove.y);
		const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																			   *Engine::Instance().window()->view());
		bool hover = false;
		currentRect.setFillColor(sf::Color::Transparent);
		if (rect.getGlobalBounds().contains(pos))
		{
			hover = true;
			currentRect.setFillColor(Widget::HOVERED_COLOR);
		}
		if (hover && !hovered)
		{
			hovered = true;
			Engine::Instance().soundManager()->playOnce(SoundManager::HOVER);
		}
		hovered = hover;
	}
	else if (event->type == sf::Event::KeyPressed)
	{
		if (event->key.code == sf::Keyboard::Return)
		{
			m_isChecked = !m_isChecked;
			update();
		}
	}
	else if (event->type == sf::Event::JoystickButtonPressed)
	{
		if (event->joystickButton.button == EngineDefs::KEY_ACCEPT)
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
	rect.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	currentRect.setSize(m_size);
	currentRect.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	const float radius = m_size.x / 3;
	circle.setPosition(m_pos.x + radius/2 * Engine::Instance().settingsManager()->getScaleFactor().x,
					   m_pos.y + radius/2 * Engine::Instance().settingsManager()->getScaleFactor().y);
	circle.setScale(Engine::Instance().settingsManager()->getScaleFactor());
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

void CheckBox::setBorderColor(const sf::Color &borderColor)
{
	rect.setOutlineColor(borderColor);
	circle.setOutlineColor(borderColor);
}

void CheckBox::setCheckedColor(const sf::Color &checkedColor)
{
	circle.setFillColor(checkedColor);
}
