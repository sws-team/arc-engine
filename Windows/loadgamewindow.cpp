#include "loadgamewindow.h"
#include "settings.h"
#include "globalvariables.h"
#include "Engine/engine.h"
#include "ResourcesManager/resourcesmanager.h"

LoadGameWindow::LoadGameWindow()
	: StateWindow()
{
	const float offset = Settings::Instance().getScaleFactor().y * 20;
	const float x = Settings::Instance().getResolution().x * 0.3f;
	float y = Settings::Instance().getResolution().y * 0.1f;
}

LoadGameWindow::~LoadGameWindow()
{

}

unsigned int LoadGameWindow::saveNum() const
{
	return 0;
}

bool LoadGameWindow::isNewGame() const
{
	return true;
}

void LoadGameWindow::back()
{
	Engine::Instance().setState(Engine::MAIN_MENU);
}

void LoadGameWindow::paint(RenderWindow *window)
{
	drawBackground(window);
}

void LoadGameWindow::eventFilter(Event *event)
{
	if (event->type == Event::MouseButtonPressed)
	{
//		cout <<event->mouseButton.x << " "  << event->mouseButton.y << endl
//			<< mission1Link.getGlobalBounds().top << " "
//			   << mission1Link.getGlobalBounds().left << " "
//				  << mission1Link.getGlobalBounds().width << " "
//				  << mission1Link.getGlobalBounds().height << " " << endl<<endl;

	}
	StateWindow::eventFilter(event);
}

void LoadGameWindow::accept()
{
	if (isNewGame())
		Engine::Instance().setState(Engine::NEW_GAME);
	else
	{
		SavedGameLoader::Instance().setCurrentSave(saveNum());
		Engine::Instance().setState(Engine::MAIN_LOCATION);
	}
}
