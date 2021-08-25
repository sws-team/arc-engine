#include "introwindow.h"
#include "engine.h"
#include "managers.h"
#include "enginedef.h"

IntroWindow::IntroWindow()
	: ArcWindow()
{
	setBackground(TexturesManager::INTRO_BACKGROUND);
	opacity = 255;
}

void IntroWindow::init()
{
	Engine::Instance().soundManager()->playOnce(SoundManager::INTRO);
}

bool IntroWindow::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::KeyPressed)
	{
		if (event->key.code == sf::Keyboard::Escape)
			finish();
	}
	if (event->type == sf::Event::JoystickButtonPressed)
	{
		if (event->joystickButton.button == ArcEngine::KEY_ESCAPE)
			finish();
	}
	return ArcWindow::eventFilter(event);
}

void IntroWindow::update()
{
	if (timer.check(10))
	{
		opacity -= 2;
		if (opacity < 0)
			opacity = 0;
		//FIXME action
//		background.setColor(sf::Color(255,255,255, opacity));
	}
	if (opacity == 0)
		finish();
}

void IntroWindow::finish()
{
	Engine::Instance().soundManager()->stop(SoundManager::INTRO);
	Engine::Instance().stateManager()->setState(StateManager::LOADING);
}
