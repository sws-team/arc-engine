#include "introwindow.h"
#include "Engine/engine.h"
#include "controller.h"

IntroWindow::IntroWindow()
	: StateWindow()
{
	setBackground(RESOURCES::INTRO_BACKGROUND);
	opacity = 255;
}

void IntroWindow::init()
{

}

void IntroWindow::paint(RenderWindow *window)
{
	drawBackground(window);
}

void IntroWindow::eventFilter(Event *event)
{
	if (event->type == Event::KeyPressed)
	{
		if (event->key.code == Keyboard::Escape)
			finish();
	}
	if (event->type == Event::JoystickButtonPressed)
	{
		if (event->joystickButton.button == Controller::KEY_ESCAPE)
			finish();
	}
}

void IntroWindow::update()
{
	if (timer.check(10))
	{
		opacity -= 1;
		background.setColor(Color(255,255,255, opacity));
	}
	if (opacity == 0)
		finish();
}

void IntroWindow::finish()
{
	Engine::Instance().setState(Engine::MAIN_MENU);
}
