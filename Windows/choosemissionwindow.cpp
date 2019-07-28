#include "choosemissionwindow.h"
#include "settings.h"
#include "globalvariables.h"
#include "Engine/engine.h"
#include "ResourcesManager/resourcesmanager.h"

ChooseMissionWindow::ChooseMissionWindow()
	: StateWindow()
{
	setBackground(RESOURCES::ABOUT_BACKGROUND);

	const float offset = 160 + Settings::Instance().getScaleFactor().y * 20;

	float x = Settings::Instance().getResolution().x * 0.3f;
	float y = Settings::Instance().getResolution().y * 0.1f;

	for (unsigned int i = 0; i < Engine::Instance().missionsCount(); ++i)
	{
		MissionView mission;
		mission.rect.setPosition(x, y);
		mission.rect.setSize(Vector2f(160, 160));
		mission.rect.setFillColor(Color::Red);

		float posX = x;
		for (int j = 0; j < STARS_COUNT; ++j)
		{
			RectangleShape starRect;
			starRect.setFillColor(Color::Cyan);
			starRect.setSize(Vector2f(32, 32));
			starRect.setPosition(posX, y + 160);
			starRect.setTexture(&ResourcesManager::Instance().getTexture(RESOURCES::STAR_TEXTURE));
			mission.stars.push_back(starRect);
			posX += 32;
		}

		missions.push_back(mission);
		x += offset;
	}
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
	for (const MissionView& mission : missions)
	{
		window->draw(mission.rect);
		for(const RectangleShape& rect : mission.stars)
		{
			window->draw(rect);
		}
	}
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
		for (unsigned int mission = 0; mission < missions.size(); ++mission)
			if (missions.at(mission).rect.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
				accept(mission);
	}
	StateWindow::eventFilter(event);
}

void ChooseMissionWindow::accept(unsigned int num)
{
	cout << "ACCEPT "<<num <<endl;
	Engine::Instance().setMission(num);
	Engine::Instance().setState(Engine::IN_GAME);
}
