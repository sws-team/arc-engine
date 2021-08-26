#include "closewindow.h"
#include "engine.h"
#include "managers.h"

CloseWindow::CloseWindow()
	: ArcWindow("CloseWindow")
{
	setBackground(TexturesManager::CLOSE_BACKGROUND);
	opacity = 255;
}

void CloseWindow::init()
{
	Engine::Instance().soundManager()->endBackgroundSound();
}

void CloseWindow::update()
{
	if (timer.check(10))
	{
		opacity -= 1;
		//FIXME action
//		background.setColor(sf::Color(255,255,255, opacity));
	}
	if (opacity == 0)
		Engine::Instance().stateManager()->setState(StateManager::EXIT);
}

void CloseWindow::back()
{

}
