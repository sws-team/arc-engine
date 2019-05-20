#include "closewindow.h"
#include "Engine/engine.h"

CloseWindow::CloseWindow()
	: StateWindow()
{
	setBackground(RESOURCES::CLOSE_BACKGROUND);
	opacity = 255;
}

void CloseWindow::paint(RenderWindow *window)
{
	drawBackground(window);
}

void CloseWindow::update()
{
	if (timer.check(10))
	{
		opacity -= 1;
		background.setColor(Color(255,255,255, opacity));
	}
	if (opacity == 0)
		Engine::Instance().setState(Engine::EXIT);
}

void CloseWindow::back()
{

}
