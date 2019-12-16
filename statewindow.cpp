#include "statewindow.h"
#include "settings.h"
#include "ResourcesManager/resourcesmanager.h"
#include "controller.h"

StateWindow::StateWindow()
{

}

StateWindow::~StateWindow()
{

}

void StateWindow::init()
{

}

void StateWindow::eventFilter(Event *event)
{
	if (event->type == Event::Closed)
		this->back();
	else if (event->type == Event::KeyPressed)
	{
		if (event->key.code == Keyboard::Escape)
			this->back();
	}
	else if (event->type == Event::JoystickButtonPressed)
	{
		if (event->joystickButton.button == Controller::KEY_ESCAPE)
			this->back();
	}
}

void StateWindow::update()
{

}

void StateWindow::setBackground(RESOURCES::TEXTURE_TYPE type)
{
	background.setTexture(ResourcesManager::Instance().getTexture(type));
	background.setScale(Settings::Instance().getScaleFactor());
}

void StateWindow::drawBackground(RenderWindow *window)
{
	window->draw(background);
}

void StateWindow::back()
{

}

