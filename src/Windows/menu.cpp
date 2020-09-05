#include "menu.h"
#include "engine.h"
#include "managers.h"
#include "enginedef.h"
#include "mainwindow.h"

Menu::Menu()
	: StateWindow()
	,m_pos(sf::Vector2f(0, 0))
	,m_color(sf::Color::Red)
	,m_currentColor(sf::Color::Green)
	,m_borderColor(sf::Color::Black)
	,yPos(0)
	,m_characterSize(100)
	,m_maxMenu(0)
	,m_textYOffset(DEFAULT_Y_OFFSET)
{

}

void Menu::paint(sf::RenderWindow *window)
{
	drawBackground(window);
	for(const sf::Text& menu : menus)
		window->draw(menu);
}

void Menu::eventFilter(sf::Event *event)
{
	switch (event->type)
	{
	case sf::Event::Closed:
		closeEvent();
		break;
	case sf::Event::KeyPressed:
	{
		switch (event->key.code)
		{
		case sf::Keyboard::Up:
			menuUp();
			break;
		case sf::Keyboard::Down:
			menuDown();
			break;
		case sf::Keyboard::Return:
			accept();
			break;
		default:
			break;
		}
	}
		break;
	case sf::Event::MouseMoved:
	{
		const sf::Vector2i pixelPos = sf::Vector2i(event->mouseMove.x, event->mouseMove.y);
		const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																			   *Engine::Instance().window()->view());
		const int current = getMenuAtPos(pos);
		if (current != -1 && current != currentMenu)
		{
			currentMenu = current;
			Engine::Instance().soundManager()->playOnce(SoundManager::HOVER);
			updateColor();
		}
	}
		break;
	case sf::Event::MouseButtonPressed:
	{
		if (event->mouseButton.button == sf::Mouse::Left)
		{
			const sf::Vector2i pixelPos = sf::Vector2i(event->mouseButton.x, event->mouseButton.y);
			const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																				   *Engine::Instance().window()->view());
			const int current = getMenuAtPos(pos);
			if (current != -1)
			{
				currentMenu = current;
				Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
				accept();
			}
		}
	}
		break;
	case sf::Event::TouchBegan:
	{
		const sf::Vector2i pixelPos = sf::Vector2i(event->touch.x, event->touch.y);
		const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																			   *Engine::Instance().window()->view());
		const int current = getMenuAtPos(pos);
		if (current != -1)
		{
			currentMenu = current;
			Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
			accept();
		}
	}
		break;
	default:
		break;
	}
	StateWindow::eventFilter(event);
}

void Menu::back()
{
	Engine::Instance().stateManager()->setState(StateManager::CLOSING);
}

void Menu::addItem(const sf::String& str)
{
	sf::Text text;

	text.setFont(Engine::Instance().fontManager()->font());
	text.setFillColor(m_color);
	text.setString(str);
	text.setCharacterSize(Engine::Instance().fontManager()->getCharSize(m_characterSize));
	text.setStyle(sf::Text::Bold);
	text.setOutlineThickness(2);
	text.setOutlineColor(m_borderColor);
	text.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	text.setPosition(m_pos.x,
					 m_pos.y + yPos + m_textYOffset * menus.size());
	yPos += text.getGlobalBounds().height;

	menus.push_back(text);

	updateColor();
}

void Menu::menuUp()
{
	if (currentMenu > 0)
		currentMenu--;
	updateColor();
}

void Menu::menuDown()
{
	if (currentMenu < m_maxMenu)
		currentMenu++;
	updateColor();
}

void Menu::setTextYOffset(float textYOffset)
{
	m_textYOffset = textYOffset * Engine::Instance().settingsManager()->getScaleFactor().y;
}

void Menu::setMaxMenu(int maxMenu)
{
	m_maxMenu = maxMenu;
}

int Menu::getMenuAtPos(const sf::Vector2f &point) const
{
	int currentMenu = -1;
	unsigned int i = 0;
	while(i != menus.size())
	{
		const sf::FloatRect rect = menus.at(i).getGlobalBounds();
		if (rect.contains(point))
		{
			currentMenu = i;
			break;
		}
		i++;
	}
	return currentMenu;
}

void Menu::updateColor()
{
    for(sf::Text& menu : menus)
	{
        menu.setFillColor(m_color);
		menu.setOutlineColor(m_borderColor);
	}
	if (menus.size() > currentMenu)
	{
		menus.at(currentMenu).setFillColor(m_currentColor);
		menus.at(currentMenu).setOutlineColor(m_borderColor);
	}
}

sf::Vector2f Menu::getPos() const
{
	return m_pos;
}

unsigned int Menu::getCharacterSize() const
{
	return m_characterSize;
}

sf::Color Menu::getBorderColor() const
{
	return m_borderColor;
}

sf::Color Menu::getCurrentColor() const
{
	return m_currentColor;
}

sf::Color Menu::getColor() const
{
	return m_color;
}

void Menu::setCharacterSize(unsigned int characterSize)
{
	m_characterSize = characterSize;
}

void Menu::setOutlineColor(const sf::Color &color)
{
	m_borderColor = color;
}

void Menu::setCurrentColor(const sf::Color &currentColor)
{
	m_currentColor = currentColor;
}

void Menu::setColor(const sf::Color &color)
{
	m_color = color;
}

void Menu::setPos(const sf::Vector2f &pos)
{
	m_pos = pos;
}
