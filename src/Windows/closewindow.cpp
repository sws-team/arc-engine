#include "closewindow.h"
#include "engine.h"
#include "managers.h"
#include "arcaction.h"
#include "arcsprite.h"

CloseWindow::CloseWindow()
	: ArcWindow("CloseWindow")
{
	setBackground(TexturesManager::CLOSE_BACKGROUND);

	ArcAction *fade = new FadeOutAction(ArcEngine::DEFAULT_WINDOW_FADE_TIME, bg);
	fade->setCompletedFunc([]() {
		Engine::Instance().stateManager()->setState(StateManager::EXIT);
	});
	bg->addAction(fade);
}

void CloseWindow::initWindow()
{
	ArcWindow::initWindow();
	Engine::Instance().soundManager()->endBackgroundSound();
}

void CloseWindow::back()
{

}
