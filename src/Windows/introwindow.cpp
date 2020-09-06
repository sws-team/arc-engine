#include "introwindow.h"
#include "engine.h"
#include "managers.h"
#include "enginedef.h"
#include "Windows/loadingwindow.h"

IntroWindow::IntroWindow(swoosh::ActivityController &controller)
	: StateWindow(controller)
{
	setBackground(TexturesManager::INTRO_BACKGROUND);
	opacity = 255;
}

void IntroWindow::onStart()
{
	Engine::Instance().soundManager()->playOnce(SoundManager::INTRO);
}

void IntroWindow::onUpdate(double elapsed)
{
	if (opacity == 0)
		return;
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

void IntroWindow::onDraw(sf::RenderTexture &surface)
{
	drawBackground(&surface);
}

void IntroWindow::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::KeyPressed)
	{
		if (event->key.code == sf::Keyboard::Escape)
			finish();
	}
}

void IntroWindow::finish()
{
	Engine::Instance().soundManager()->stop(SoundManager::INTRO);	
	Engine::Instance().getOptions()->changeState(Options::LOADING);
}
