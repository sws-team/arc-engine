#include "introwindow.h"
#include "engine.h"
#include "managers.h"
#include "enginedef.h"
#include "arcaction.h"
#include "arcsprite.h"

IntroWindow::IntroWindow()
	: ArcScene("IntroScene")
{
	setBackground(TexturesManager::INTRO_BACKGROUND);

	ArcAction *fade = new FadeInAction(ArcEngine::DEFAULT_WINDOW_FADE_TIME, bg);
	fade->setCompletedFunc(std::bind(&IntroWindow::finish, this));
	bg->addAction(fade);
}

void IntroWindow::initWindow()
{
	ArcScene::initWindow();
	PLAY_SOUND(SoundManager::INTRO);
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
	return ArcScene::eventFilter(event);
}

void IntroWindow::finish()
{
	STOP_SOUND(SoundManager::INTRO);
	CHANGE_STATE(SceneManager::LOADING);
}
