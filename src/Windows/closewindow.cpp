#include "closewindow.h"
#include "engine.h"
#include "managers.h"
#include "arcaction.h"
#include "arcsprite.h"

CloseWindow::CloseWindow()
	: ArcScene("CloseScene")
{
	setBackground(TexturesManager::CLOSE_BACKGROUND);

	ArcAction *fade = new FadeOutAction(ArcEngine::DEFAULT_WINDOW_FADE_TIME, bg);
	fade->setCompletedFunc([]() {
		CHANGE_STATE(SceneManager::EXIT);
	});
	bg->addAction(fade);
}

void CloseWindow::initWindow()
{
	ArcScene::initWindow();
	Engine::Instance().soundManager()->endBackgroundSound();
}

void CloseWindow::back()
{

}
