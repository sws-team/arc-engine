#include "introwindow.h"
#include "engine.h"
#include "managers.h"
#include "enginedef.h"

IntroWindow::IntroWindow()
	: StateWindow()
{
	setBackground(TexturesManager::INTRO_BACKGROUND);
	opacity = 255;
}

void IntroWindow::init()
{
	Engine::Instance().soundManager()->playOnce(SoundManager::INTRO);
}

void IntroWindow::paint(sf::RenderWindow *window)
{
	drawBackground(window);
}

void IntroWindow::eventFilter(sf::Event *event)
{
        if (event->type == sf::Event::KeyPressed)
	{
                if (event->key.code == sf::Keyboard::Escape)
			finish();
	}
        if (event->type == sf::Event::JoystickButtonPressed)
	{
		if (event->joystickButton.button == EngineDefs::KEY_ESCAPE)
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
                background.setColor(sf::Color(255,255,255, opacity));
	}
	if (opacity == 0)
		finish();
}

void IntroWindow::finish()
{
	Engine::Instance().soundManager()->stop(SoundManager::INTRO);
	Engine::Instance().stateManager()->setState(StateManager::MENU);
}
