#include "introwindow.h"
#include "Engine/engine.h"
#include "controller.h"
#include "Game/Audio/soundcontroller.h"

IntroWindow::IntroWindow()
	: StateWindow()
{
	setBackground(RESOURCES::INTRO_BACKGROUND);
	opacity = 255;
}

void IntroWindow::init()
{
	SoundController::Instance().playOnce(INTRO_SOUND_FILE);
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
		opacity -= 2;
		if (opacity < 0)
			opacity = 0;
		background.setColor(Color(255,255,255, opacity));
	}
	if (opacity == 0)
		finish();
}

void IntroWindow::finish()
{
	SoundController::Instance().stop(INTRO_SOUND_FILE);
	Engine::Instance().setState(Engine::MAIN_MENU);
}
