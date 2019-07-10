#include "choosemissionwindow.h"
#include "settings.h"
#include "globalvariables.h"
#include "Engine/engine.h"
#include "ResourcesManager/resourcesmanager.h"

ChooseMissionWindow::ChooseMissionWindow()
	: StateWindow()
{
	setBackground(RESOURCES::ABOUT_BACKGROUND);

	const float offset = Settings::Instance().getScaleFactor().y * 20;
	const float x = Settings::Instance().getResolution().x * 0.3f;
	float y = Settings::Instance().getResolution().y * 0.1f;

	mission.setPosition(x, y);
	mission.setFillColor(Color::Red);
	mission.setSize(Vector2f(100, 100));
}

ChooseMissionWindow::~ChooseMissionWindow()
{

}

void ChooseMissionWindow::back()
{
	Engine::Instance().setState(Engine::MAIN_MENU);
}

void ChooseMissionWindow::paint(RenderWindow *window)
{
	drawBackground(window);
	window->draw(mission);
}

void ChooseMissionWindow::eventFilter(Event *event)
{
	if (event->type == Event::MouseButtonPressed)
	{
//		cout <<event->mouseButton.x << " "  << event->mouseButton.y << endl
//			<< mission.getGlobalBounds().top << " "
//			   << mission.getGlobalBounds().left << " "
//				  << mission.getGlobalBounds().width << " "
//				  << mission.getGlobalBounds().height << " " << endl<<endl;
		if (mission.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
		{
			accept(0);
		}
	}
	StateWindow::eventFilter(event);
}

void ChooseMissionWindow::accept(int num)
{
	SavedGameLoader::Instance().setCurrentMission(num);
	Engine::Instance().setState(Engine::IN_GAME);
}
