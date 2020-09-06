#include "valuescale.h"
#include "engine.h"
#include "managers.h"
#include "mainwindow.h"

ValueScale::ValueScale():
  m_scales(10)
  ,currentTriangle(0)
  ,m_max(100)
  ,m_min(0)
{
	update();
}

void ValueScale::draw(sf::RenderTarget *target)
{
	for(const sf::ConvexShape& shape: triangles)
		target->draw(shape);
}

void ValueScale::event(sf::Event *event)
{
	if (event->type == sf::Event::MouseButtonReleased)
	{
		if (event->mouseButton.button == sf::Mouse::Left)
		{
			const sf::Vector2i pixelPos = sf::Vector2i(event->mouseButton.x, event->mouseButton.y);
			const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																				   *Engine::Instance().window()->view());
			int i = getCurrentTriangle(pos);
			if (i != -1)
			{
				currentTriangle = i;
				Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
				updateValue();
			}
		}
	}
	else if (event->type == sf::Event::MouseMoved)
	{
		updateValue();
		const sf::Vector2i pixelPos = sf::Vector2i(event->mouseMove.x, event->mouseMove.y);
		const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																			   *Engine::Instance().window()->view());
		int i = getCurrentTriangle(pos);
		if (i != -1)
		{
			sf::Color color = triangles[i].getFillColor();
			color.a = 128;
			color.r = 255;
			triangles[i].setFillColor(color);
		}
	}
	else if (event->type == sf::Event::KeyPressed)
	{
		if (event->key.code == sf::Keyboard::Left)
			currentTriangle++;
		if (event->key.code == sf::Keyboard::Right)
			currentTriangle--;
		if (currentTriangle < 0)
			currentTriangle = 0;
		if (currentTriangle >= triangles.size())
			currentTriangle = triangles.size() - 1;
		updateValue();
	}
	else if (event->type == sf::Event::JoystickMoved)
	{
		if (event->joystickMove.axis == sf::Joystick::X)
		{
			if (event->joystickMove.position > 50)
				currentTriangle--;
			else if (event->joystickMove.position < -50)
				currentTriangle++;

			if (currentTriangle < 0)
				currentTriangle = 0;
			if (currentTriangle >= triangles.size())
				currentTriangle = triangles.size() - 1;

			updateValue();
		}
	}
}

void ValueScale::update()
{
	updateTriangle();
}

void ValueScale::setScalesCount(int scales)
{
	m_scales = scales;
	updateTriangle();
}

void ValueScale::setRange(float min, float max)
{
	m_min = min;
	m_max = max;
}

void ValueScale::setMinValue(float min)
{
	m_min = min;
}

void ValueScale::setMaxValue(float max)
{
	m_max = max;
}

float ValueScale::value() const
{
	return m_max - m_max * (currentTriangle * m_scales/m_max - m_min);
}

void ValueScale::setValue(float value)
{
	const float currentValue = m_max - value;
	currentTriangle = currentValue * m_scales/m_max + m_min;
	updateTriangle();
}

void ValueScale::updateTriangle()
{
	triangles.clear();
	for (int i = 0; i < m_scales; ++i)
	{
		sf::ConvexShape shape;
		shape.setPointCount(3);
		const int x = static_cast<int>(trunc(m_size.x - m_size.x / m_scales * i));
		const int y = static_cast<int>(trunc(m_size.y - m_size.y / m_scales * i));

		shape.setPoint(0, sf::Vector2f(0, 0));
		shape.setPoint(1, sf::Vector2f(x, y));
		shape.setPoint(2, sf::Vector2f(x, 0));

		shape.setPosition(m_pos);
		shape.setOutlineThickness(1);
		shape.setOutlineColor(sf::Color::Black);

		triangles.push_back(shape);
	}
	updateValue();
}

void ValueScale::updateValue()
{
	for (unsigned int i = 0; i < triangles.size(); ++i)
	{
		if (i < currentTriangle)		
			triangles[i].setFillColor(m_colorInactive);
		else
			triangles[i].setFillColor(m_colorActive);
	}
}

int ValueScale::getCurrentTriangle(const sf::Vector2f &pos)
{
	const sf::Vector2f localPos = sf::Vector2f(pos.x - m_pos.x,
									   pos.y - m_pos.y);
	if (localPos.y < m_size.y || localPos.y > 0)
		return -1;

	for (int i = triangles.size() - 1; i >= 0 ; --i)
	{
		const int x = m_size.x - m_size.x / m_scales * i;
		if (localPos.x > 0 && localPos.x < x)
			return i;
	}
	return -1;
}

void ValueScale::setColorInactive(const sf::Color &colorInactive)
{
	m_colorInactive = colorInactive;
}

void ValueScale::setColorActive(const sf::Color &colorActive)
{
	m_colorActive = colorActive;
}
