#include "chooselist.h"
#include "globalvariables.h"

ChooseList::ChooseList() :
	Widget()
  ,current(0)
  ,m_characterSize(24)
  ,m_textColor(Color::Black)
  ,m_fillColor(Color::Red)
  ,m_borderColor(Color::Green)
  ,m_currentColor(Color::Blue)
{
	currentRect.setFillColor(Color::Transparent);
}

void ChooseList::draw(RenderTarget *target)
{
	for (const ChooseValue& chooseValue : rects)
	{
		target->draw(chooseValue.rect);
		target->draw(chooseValue.text);
	}
	target->draw(currentRect);
}

void ChooseList::event(Event *event)
{
	if (event->type == Event::MouseButtonReleased)
	{
		for (size_t i = 0; i < rects.size(); ++i)
		{
			if (rects.at(i).rect.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
			{
				current = i;
				update();
				break;
			}
		}
	}
	else if (event->type == Event::MouseMoved)
	{
		currentRect.setFillColor(Color::Transparent);
		for(const ChooseValue& chooseValue : rects)
		{
			if (chooseValue.rect.getGlobalBounds().contains(event->mouseMove.x, event->mouseMove.y))
			{
				currentRect.setPosition(chooseValue.rect.getPosition());
				currentRect.setFillColor(Widget::HOVERED_COLOR);
				break;
			}
		}
	}
	else if (event->type == Event::KeyPressed)
	{
		if (event->key.code == Keyboard::Left)
			current--;
		if (event->key.code == Keyboard::Right)
			current++;
		if (current < 0)
			current = 0;
		if (current >= rects.size())
			current = rects.size() - 1;
		update();
	}
	else if (event->type == Event::JoystickMoved)
	{
		if (event->joystickMove.axis == Joystick::X)
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
	float x = m_pos.x;
	for (size_t i = 0; i < rects.size(); ++i)
	{
		rects[i].rect.setSize(m_size);
		rects[i].rect.setPosition(x, m_pos.y);
		rects[i].text.setPosition(x + rects[i].rect.getSize().x/2 - rects[i].text.getGlobalBounds().width/2,
								  m_pos.y);

		if(i == current)
			rects[i].rect.setOutlineColor(m_currentColor);
		else
			rects[i].rect.setOutlineColor(m_borderColor);

		x += m_size.x + m_size.x / 5;
	}
}

void ChooseList::setList(const vector<String> &list)
{
	m_list = list;
	updateList();
	update();
}

void ChooseList::addItem(const String &item)
{
	ChooseValue chooseValue;
	chooseValue.rect.setOutlineThickness(4);
	chooseValue.rect.setFillColor(m_fillColor);
	chooseValue.rect.setOutlineColor(m_borderColor);

	chooseValue.text.setString(item);
	chooseValue.text.setFont(GlobalVariables::Instance().font());
	chooseValue.text.setCharacterSize(m_characterSize);
	chooseValue.text.setFillColor(m_textColor);
	rects.push_back(chooseValue);
}

void ChooseList::setCurrent(int n)
{
	current = n;
	update();
}

void ChooseList::setCurrent(const String &text)
{
	for (size_t i = 0; i < rects.size(); ++i)
	{
		if (rects.at(i).text.getString() == text)
		{
			current = i;
			update();
			break;
		}
	}
}

String ChooseList::currentText() const
{
	return rects.at(current).text.getString();
}

void ChooseList::setScale(const Vector2f &scaleFactor)
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

void ChooseList::setCurrentColor(const Color &currentColor)
{
	m_currentColor = currentColor;
}

void ChooseList::setBorderColor(const Color &borderColor)
{
	m_borderColor = borderColor;
}

void ChooseList::setFillColor(const Color &fillColor)
{
	m_fillColor = fillColor;
}

void ChooseList::setTextColor(const Color &textColor)
{
	m_textColor = textColor;
}

void ChooseList::setCharacterSize(unsigned int characterSize)
{
	m_characterSize = characterSize;
}
