#include "chooselist.h"
#include "globalvariables.h"

ChooseList::ChooseList() :
	current(0)
  ,m_characterSize(24)
{

}

void ChooseList::draw(RenderTarget *target)
{
	for (const ChooseValue& chooseValue : rects)
	{
		target->draw(chooseValue.rect);
		target->draw(chooseValue.text);
	}
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
}

void ChooseList::update()
{
	float x = m_pos.x;
	for (size_t i = 0; i < rects.size(); ++i)
	{
		rects[i].rect.setSize(m_size);
		rects[i].rect.setPosition(x, m_pos.y);
		rects[i].text.setPosition(x, m_pos.y);

		if(i == current)
			rects[i].rect.setOutlineColor(Color::Blue);
		else
			rects[i].rect.setOutlineColor(Color::Red);

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
	chooseValue.rect.setFillColor(Color::Green);
	chooseValue.rect.setOutlineColor(Color::Red);

	chooseValue.text.setString(item);
	chooseValue.text.setFont(GlobalVariables::Instance().font());
	chooseValue.text.setCharacterSize(m_characterSize);
	chooseValue.text.setFillColor(Color::Black);
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

void ChooseList::setCharacterSize(unsigned int characterSize)
{
	m_characterSize = characterSize;
}
