#include "menu.h"
#include "globalvariables.h"
#include "Game/Audio/soundcontroller.h"
#include "Engine/engine.h"
#include "Game/Level/camera.h"
#include "controller.h"
#include "settings.h"

Menu::Menu()
	: StateWindow()
{
	m_pos = Vector2f(0, 0);
	m_color = Color::Red;
	m_currentColor = Color::Green;
	yPos = 0;
}

void Menu::paint(RenderWindow *window)
{
	drawBackground(window);
	for(const Text& menu : menus)
		window->draw(menu);
}

void Menu::eventFilter(Event *event)
{
	switch (event->type)
	{
	case Event::Closed:
		closeEvent();
		break;
	case Event::KeyPressed:
	{
		switch (event->key.code)
		{
		case Keyboard::Up:
			menuUp();
			break;
		case Keyboard::Down:
			menuDown();
			break;
		case Keyboard::Return:
			accept();
			break;
		default:
			break;
		}
	}
		break;
	case Event::MouseMoved:
	{
		const int current = getMenuAtPos(Vector2i(event->mouseMove.x, event->mouseMove.y));
		if (current != -1 && current != currentMenu)
		{
			currentMenu = current;
			SoundController::Instance().playOnce(CLICK_SOUND_FILE);
			updateColor();
		}
	}
		break;
	case Event::MouseButtonPressed:
	{
		const int current = getMenuAtPos(Vector2i(event->mouseButton.x, event->mouseButton.y));
		if (current != -1)
		{
			currentMenu = current;
			SoundController::Instance().playOnce(CLICK_SOUND_FILE);
			accept();
		}
	}
		break;
	case Event::JoystickButtonPressed:
	{
		if (event->joystickButton.button == Controller::KEY_START)
			accept();
	}
		break;
	case Event::JoystickMoved:
	{
		if (event->joystickMove.axis == Joystick::Y)
		{
			if (event->joystickMove.position > 50)
				menuDown();
			else if (event->joystickMove.position < -50)
				menuUp();
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
	Engine::Instance().setState(Engine::CLOSING);
}

void Menu::addItem(const String& str)
{
	Text text;

	text.setFont(GlobalVariables::Instance().font());
	text.setFillColor(Color::Black);

	text.setString(str);
	text.setCharacterSize(100);
	text.setStyle(Text::Bold);
	text.setOutlineThickness(2);
	text.setScale(Settings::Instance().getScaleFactor());

	text.setPosition(m_pos.x,
					 m_pos.y + yPos + Y_OFFSET * Settings::Instance().getScaleFactor().y * menus.size());
	yPos += text.getGlobalBounds().height;

	menus.push_back(text);

	updateColor();
}

void Menu::menuUp()
{
	if (currentMenu > 0)
		currentMenu -= 1;
	updateColor();
}

void Menu::menuDown()
{
	if (currentMenu < menus.size() - 1)
		currentMenu += 1;
	updateColor();
}

int Menu::getMenuAtPos(const Vector2i &point) const
{
	const Vector2f pos = Vector2f(point.x, point.y);
	int currentMenu = -1;
	unsigned int i = 0;
	while(i != menus.size())
	{
		const FloatRect rect = menus.at(i).getGlobalBounds();
		if (rect.contains(pos))
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
	for(Text& menu : menus)
		menu.setFillColor(m_color);
	menus.at(currentMenu).setFillColor(m_currentColor);
	menus.at(currentMenu).setOutlineColor(Color::Black);
}

void Menu::setCurrentColor(const Color &currentColor)
{
	m_currentColor = currentColor;
}

void Menu::setColor(const Color &color)
{
	m_color = color;
}

void Menu::setPos(const Vector2f &pos)
{
	m_pos = pos;
}
