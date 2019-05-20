#include "aboutwindow.h"
#include "Game/Audio/soundcontroller.h"
#include "settings.h"

AboutWindow::AboutWindow()
	: StateWindow()
{
	setBackground(RESOURCES::ABOUT_BACKGROUND);
}

void AboutWindow::init()
{
	SoundController::Instance().startBackgroundSound(ABOUT_SOUND_FILE);
}

void AboutWindow::paint(RenderWindow *window)
{
	drawBackground(window);
}

void AboutWindow::back()
{
	Engine::Instance().setState(Engine::MAIN_MENU);
}
