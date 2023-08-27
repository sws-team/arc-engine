#include "introscene.h"
#include <managers.h>
#include <enginedef.h>
#include <ArcAction>
#include <arcsprite.h>

IntroScene::IntroScene()
	: ArcScene("IntroScene")
{
	cursorVisible = true;
	setBackground(TexturesManager::INTRO_BACKGROUND);

	ArcAction *fade = new FadeInAction(ArcEngine::DEFAULT_WINDOW_FADE_TIME, bg);
	fade->setCompletedFunc(std::bind(&IntroScene::finish, this));
	bg->addAction(fade);
}

void IntroScene::init()
{
	ArcScene::init();
	PLAY_SOUND(SoundManager::INTRO);
}

bool IntroScene::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::KeyPressed) {
		if (event->key.code == sf::Keyboard::Escape) {
			finish();
			return true;
		}
	}
	return ArcScene::eventFilter(event);
}

void IntroScene::finish()
{
	STOP_SOUND(SoundManager::INTRO);
	CHANGE_SCENE(SceneManager::LOADING);
}
