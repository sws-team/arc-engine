#include "closingwindow.h"
#include "engine.h"
#include "managers.h"

ClosingWindow::ClosingWindow(swoosh::ActivityController &controller)
	: StateWindow(controller)
{
	setBackground(TexturesManager::CLOSE_BACKGROUND);
	opacity = 255;
}

void ClosingWindow::onStart()
{
	Engine::Instance().soundManager()->endBackgroundSound();
}

void ClosingWindow::onUpdate(double elapsed)
{
	if (opacity == 0)
		return;;
	if (timer.check(10))
	{
		opacity--;
		background.setColor(sf::Color(255,255,255, opacity));
	}
	if (opacity == 0)
		Engine::Instance().getOptions()->changeState(Options::EXIT);
}

void ClosingWindow::onDraw(sf::RenderTexture &surface)
{
	drawBackground(&surface);
}

void ClosingWindow::back()
{

}
