#include "statewindow.h"
#include "engine.h"
#include "managers.h"

#include "Swoosh/ActivityController.h"

StateWindow::StateWindow(swoosh::ActivityController &controller)
	: swoosh::Activity(&controller)
{

}

StateWindow::~StateWindow()
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
}

void StateWindow::onStart()
{

}

void StateWindow::onLeave()
{

}

void StateWindow::onExit()
{

}

void StateWindow::onEnter()
{

}

void StateWindow::onResume()
{

}

void StateWindow::onEnd()
{

}

void StateWindow::onUpdate(double elapsed)
{

}

void StateWindow::onDraw(sf::RenderTexture &surface)
{

}

void StateWindow::setBackground(TextureType type)
{
	background.setTexture(Engine::Instance().texturesManager()->getTexture(type));
	background.setScale(Engine::Instance().settingsManager()->getScaleFactor());
}

void StateWindow::drawBackground(sf::RenderTarget *target)
{
	target->draw(background);
}

void StateWindow::back()
{

}

