#include "menu.h"
#include "globalvariables.h"
#include "Game/Audio/soundcontroller.h"
#include "Engine/engine.h"

Menu::Menu()
	: StateWindow()
{

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
	text.setCharacterSize(80);
	text.setStyle(Text::Bold);

	text.setPosition(x, offset * (menus.size()));

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
	int currentMenu = -1;
	unsigned int i = 0;
	while(i != menus.size())
	{
		const IntRect rect = IntRect(menus.at(i).getGlobalBounds());
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
	for(Text& menu : menus)
		menu.setFillColor(Color::Red);

	menus.at(currentMenu).setFillColor(Color::Green);
}