#include "statewindow.h"
#include "engine.h"
#include "managers.h"

StateWindow::StateWindow()
	: transparency(false)
{

}

StateWindow::~StateWindow()
{

}

void StateWindow::init()
{

}

void StateWindow::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::Closed)
		this->back();
	if (event->type == sf::Event::MouseButtonPressed)
	{
		if (event->mouseButton.button == sf::Mouse::Right)
			back();
	}
	else if (event->type == sf::Event::KeyPressed)
	{
		if (event->key.code == sf::Keyboard::Escape)
			this->back();
	}
	else if (event->type == sf::Event::JoystickButtonPressed)
	{
		if (event->joystickButton.button == EngineDefs::KEY_ESCAPE)
			this->back();
	}
}

void StateWindow::update()
{

}

void StateWindow::setBackground(TextureType type)
{
	background.setTexture(Engine::Instance().texturesManager()->getTexture(type));
	background.setScale(Engine::Instance().settingsManager()->getLowScaleFactor());
}

void StateWindow::setTransparency(bool transparency)
{
	this->transparency = transparency;
}

void StateWindow::drawBackground(sf::RenderWindow *window)
{
	if (transparency)
		return;
	window->draw(background);
}

void StateWindow::back()
{

}

