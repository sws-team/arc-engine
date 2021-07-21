#include "button.h"
#include "engine.h"
#include "managers.h"
#include "enginedef.h"
#include "mainwindow.h"
#include "collisions.h"

Button::Button() :
	hovered(false)
	,m_characterSize(30)
	,color(sf::Color::White)
	,m_enabled(true)
{
	rect.setOutlineThickness(2);
	rect.setOutlineColor(sf::Color::Black);
	rect.setFillColor(color);

	m_text.setFont(Engine::Instance().fontManager()->font());
	m_text.setCharacterSize(Engine::Instance().fontManager()->getCharSize(m_characterSize));
	m_text.setFillColor(sf::Color::Black);
	currentRect.setFillColor(sf::Color::Transparent);
}

void Button::draw(sf::RenderTarget *target)
{
	target->draw(rect);
	if (!m_text.getString().isEmpty())
		target->draw(m_text);
	target->draw(currentRect);
}

void Button::event(sf::Event *event)
{
	if (!m_enabled)
		return;
	if (m_callback == nullptr)
		return;
	if (event->type == sf::Event::MouseButtonReleased)
	{
		if (event->mouseButton.button == sf::Mouse::Left)
		{
			const sf::Vector2i pixelPos = sf::Vector2i(event->mouseButton.x, event->mouseButton.y);
			const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos, *Engine::Instance().window()->view());

			if (Collision::rectContains(rect, pos))
//			if (rect.getGlobalBounds().contains(pos))
			{
				Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
				m_callback();
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
		if (Collision::rectContains(rect, pos))
//		if (rect.getGlobalBounds().contains(pos))
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
			m_callback();
	}
	else if (event->type == sf::Event::JoystickButtonPressed)
	{
		if (event->joystickButton.button == EngineDefs::KEY_ACCEPT)
			m_callback();
	}
}

void Button::update()
{
	rect.setPosition(m_pos);
	currentRect.setPosition(m_pos);

	const sf::Vector2f willSize = sf::Vector2f(m_text.getGlobalBounds().width,
										 m_text.getGlobalBounds().height);

	if (willSize.x > m_size.x || willSize.y > m_size.y)
		m_size += willSize;

	rect.setSize(m_size);
	currentRect.setSize(m_size);
	m_text.setPosition(m_pos.x + m_size.x/2 - m_text.getGlobalBounds().width/2,
					   m_pos.y + m_size.y/2 - m_text.getGlobalBounds().height/2);

	rect.setRotation(m_rotation);
	m_text.setRotation(m_rotation);
	currentRect.setRotation(m_rotation);
}

void Button::setCallback(std::function<void ()> callback)
{
	m_callback = callback;
}

void Button::setText(const sf::String &text)
{
	m_text.setString(text);
	update();
}

void Button::setTextCharacterSize(unsigned int size)
{
	m_text.setCharacterSize(size);
}

void Button::setScale(const sf::Vector2f &scaleFactor)
{
//	rect.setScale(scaleFactor);
	m_text.setScale(scaleFactor);
}

void Button::setFillColor(const sf::Color &fillColor)
{
	rect.setFillColor(fillColor);
}

void Button::setBorderColor(const sf::Color &borderColor)
{
	rect.setOutlineColor(borderColor);
}

void Button::setTextColor(const sf::Color &textColor)
{
	m_text.setFillColor(textColor);
}

void Button::setCharacterSize(unsigned int characterSize)
{
	m_characterSize = characterSize;
	m_text.setCharacterSize(m_characterSize);
	update();
}

void Button::setTexture(const sf::Texture* texture)
{
	rect.setTexture(texture);
}

void Button::setButtonColor(const sf::Color &color)
{
	this->color = color;
	rect.setFillColor(color);
}

void Button::setEnabled(bool enabled)
{
	m_enabled = enabled;
}

bool Button::isEnabled() const
{
	return m_enabled;
}
