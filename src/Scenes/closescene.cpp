#include "closescene.h"
#include <engine.h>
#include <managers.h>
#include <arcaction.h>
#include <arcsprite.h>

CloseScene::CloseScene()
	: ArcScene("CloseScene")
{
	setBackground(TexturesManager::CLOSE_BACKGROUND);

	ArcAction *fade = new FadeOutAction(ArcEngine::DEFAULT_WINDOW_FADE_TIME, bg);
	fade->setCompletedFunc([]() {
		CHANGE_SCENE(SceneManager::EXIT);
	});
	bg->addAction(fade);
}

void CloseScene::init()
{
	ArcScene::init();
	Engine::Instance().soundManager()->endBackgroundSound();
}
