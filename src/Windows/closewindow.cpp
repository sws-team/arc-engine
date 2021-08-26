#include "closewindow.h"
#include "engine.h"
#include "managers.h"
#include "arcaction.h"
#include "arcsprite.h"

CloseWindow::CloseWindow()
	: ArcWindow("CloseWindow")
{
	setBackground(TexturesManager::CLOSE_BACKGROUND);

	ArcAction *fade = new FadeOutAction(2000, bg);
	fade->setCompletedFunc([]() {
		Engine::Instance().stateManager()->setState(StateManager::EXIT);
	});
	bg->addAction(fade);
}

void CloseWindow::init()
{
	Engine::Instance().soundManager()->endBackgroundSound();
}

void CloseWindow::back()
{

}
