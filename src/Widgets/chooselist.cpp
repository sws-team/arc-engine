#include "chooselist.h"
#include "engine.h"
#include "managers.h"
#include "mainwindow.h"

#include <algorithm>

ChooseList::ChooseList() :
	Widget()
  ,current(0)
  ,m_characterSize(24)
  ,m_textColor(sf::Color::Black)
  ,m_fillColor(sf::Color::Red)
  ,m_borderColor(sf::Color::Green)
  ,m_currentColor(sf::Color::Blue)
  ,hovered(-1)
  ,startValue(0)
  ,m_maxCount(3)
{
	currentRect.setFillColor(sf::Color::Transparent);
	previous.setTexture(&Engine::Instance().texturesManager()->getTexture(TexturesManager::CHOOSE_LIST_PREVIOUS));
	next.setTexture(&Engine::Instance().texturesManager()->getTexture(TexturesManager::CHOOSE_LIST_NEXT));
}

void ChooseList::draw(sf::RenderTarget *target)
{
	const unsigned int endValue = static_cast<unsigned int>(startValue + m_maxCount);
	for (unsigned int i = startValue; i < endValue; ++i)
	{
		const ChooseValue chooseValue = rects.at(i);
		target->draw(chooseValue.rect);
		target->draw(chooseValue.text);
	}

	if (rects.size() > m_maxCount)
	{
		target->draw(previous);
		target->draw(next);
	}
	target->draw(currentRect);	
}

void ChooseList::event(sf::Event *event)
{
	if (event->type == sf::Event::MouseButtonReleased)
	{
		if (event->mouseButton.button == sf::Mouse::Left)
		{
			const sf::Vector2i pixelPos = sf::Vector2i(event->mouseButton.x, event->mouseButton.y);
			const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																				   *Engine::Instance().window()->view());
			if (previous.getGlobalBounds().contains(pos))
			{
				if (startValue > 0)
					startValue--;
				Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
				update();
				return;
			}
			if (next.getGlobalBounds().contains(pos))
			{
				if (startValue <= rects.size() - m_maxCount - 1)
					startValue++;
				Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
				update();
				return;
			}

			const unsigned int endValue = static_cast<unsigned int>(startValue + m_maxCount);
			for (unsigned int i = startValue; i < endValue; ++i)
			{
				if (rects.at(i).rect.getGlobalBounds().contains(pos))
				{
					current = i;
					Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
					update();
					return;
				}
			}
		}
	}
	else if (event->type == sf::Event::MouseMoved)
	{
		const sf::Vector2i pixelPos = sf::Vector2i(event->mouseMove.x, event->mouseMove.y);
		const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																			   *Engine::Instance().window()->view());

		currentRect.setFillColor(sf::Color::Transparent);
		updateCurrentColors();
		if (previous.getGlobalBounds().contains(pos))
		{
			previous.setFillColor(Widget::HOVERED_COLOR);
			Engine::Instance().soundManager()->playOnce(SoundManager::HOVER);
			return;
		}
		if (next.getGlobalBounds().contains(pos))
		{
			next.setFillColor(Widget::HOVERED_COLOR);
			Engine::Instance().soundManager()->playOnce(SoundManager::HOVER);
			return;
		}

		int hoverId = -1;
		const unsigned int endValue = static_cast<unsigned int>(startValue + m_maxCount);
		for (unsigned int i = startValue; i < endValue; ++i)
		{
			const ChooseValue chooseValue = rects.at(i);
			if (chooseValue.rect.getGlobalBounds().contains(pos))
			{
				currentRect.setPosition(chooseValue.rect.getPosition());
				currentRect.setFillColor(Widget::HOVERED_COLOR);
				hoverId = i;
				break;
			}
		}
		if (hoverId != -1 && hovered != hoverId)
		{
			hovered = hoverId;
			Engine::Instance().soundManager()->playOnce(SoundManager::HOVER);
		}
	}
	else if (event->type == sf::Event::KeyPressed)
	{
		if (event->key.code == sf::Keyboard::Left)
			current--;
		if (event->key.code == sf::Keyboard::Right)
			current++;
		if (current < 0)
			current = 0;
		if (current >= rects.size())
			current = rects.size() - 1;
		update();
	}
	else if (event->type == sf::Event::JoystickMoved)
	{
		if (event->joystickMove.axis == sf::Joystick::X)
		{
			if (event->joystickMove.position > 50)
				current++;
			else if (event->joystickMove.position < -50)
				current--;

			if (current < 0)
				current = 0;
			if (current >= rects.size())
				current = rects.size() - 1;
			update();
		}
	}
}

void ChooseList::update()
{
	currentRect.setSize(m_size);
	previous.setSize(sf::Vector2f(m_size.x / 4, m_size.y));
	next.setSize(sf::Vector2f(m_size.x / 4, m_size.y));
	const float interval = m_size.x / 5;

	updateCurrentColors();

	float x = m_pos.x;
	previous.setPosition(x, m_pos.y);
	if (rects.size() > m_maxCount)
	{
		x += previous.getGlobalBounds().width;
		x += interval;
	}

	const unsigned int endValue = static_cast<unsigned int>(startValue + m_maxCount);
	for (unsigned int i = startValue; i < endValue; ++i)
	{
		rects[i].rect.setSize(m_size);
		rects[i].rect.setPosition(x, m_pos.y);
		rects[i].text.setPosition(x + rects[i].rect.getSize().x/2 - rects[i].text.getGlobalBounds().width/2,
								  m_pos.y + m_size.y / 2 - rects[i].text.getGlobalBounds().height / 2);

		if(i == current)
			rects[i].rect.setOutlineColor(m_currentColor);
		else
			rects[i].rect.setOutlineColor(m_borderColor);

		x += m_size.x;
		x += interval;
	}
	next.setPosition(x, m_pos.y);
}

void ChooseList::setList(const std::vector<sf::String> &list)
{
	m_list = list;
	updateList();
	update();
}

void ChooseList::addItem(const sf::String &item)
{
	ChooseValue chooseValue;
	chooseValue.rect.setOutlineThickness(4);
	chooseValue.rect.setFillColor(m_fillColor);
	chooseValue.rect.setOutlineColor(m_borderColor);

	chooseValue.text.setString(item);
	chooseValue.text.setFont(Engine::Instance().fontManager()->font());
	chooseValue.text.setCharacterSize(Engine::Instance().fontManager()->getCharSize(m_characterSize));
	chooseValue.text.setFillColor(m_textColor);
	rects.push_back(chooseValue);
}

void ChooseList::setCurrent(int n)
{
	current = n;
	update();
}

void ChooseList::setCurrent(const sf::String &text)
{
	for (size_t i = 0; i < rects.size(); ++i)
	{
		if (rects.at(i).text.getString() == text)
		{
			setCurrent(i);
			break;
		}
	}
}

sf::String ChooseList::currentText() const
{
	return rects.at(current).text.getString();
}

void ChooseList::setScale(const sf::Vector2f &scaleFactor)
{
	for(ChooseValue& chooseValue: rects)
	{
//		chooseValue.rect.setScale(scaleFactor);
		chooseValue.text.setScale(scaleFactor);
	}
}

void ChooseList::updateList()
{
	rects.clear();
	for (size_t i = 0; i < m_list.size(); ++i)
		addItem(m_list.at(i));

	update();
}

void ChooseList::updateCurrentColors()
{
	previous.setFillColor(sf::Color::White);
	next.setFillColor(sf::Color::White);

	if (startValue == 0)
		previous.setFillColor(Widget::DISABLED_COLOR);
	else
		previous.setFillColor(sf::Color::White);

	if (startValue > rects.size() - m_maxCount - 1)
		next.setFillColor(Widget::DISABLED_COLOR);
	else
		next.setFillColor(sf::Color::White);
}

void ChooseList::setMaxCount(int maxCount)
{
	m_maxCount = maxCount;
}

void ChooseList::setCurrentColor(const sf::Color &currentColor)
{
	m_currentColor = currentColor;
}

void ChooseList::setBorderColor(const sf::Color &borderColor)
{
	m_borderColor = borderColor;
}

void ChooseList::setFillColor(const sf::Color &fillColor)
{
	m_fillColor = fillColor;
}

void ChooseList::setTextColor(const sf::Color &textColor)
{
	m_textColor = textColor;
}

void ChooseList::setCharacterSize(unsigned int characterSize)
{
	m_characterSize = characterSize;
}
