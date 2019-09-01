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


	const float left = Settings::Instance().getResolution().x * 0.3f;
	float x = left;
	float y = Settings::Instance().getResolution().y * 0.1f;
	const unsigned int maxCompletedLevel = Engine::Instance().maxCompletedLevel();

	for (unsigned int i = 0; i < Engine::Instance().missionsCount(); ++i)
	{
		if (i % 5 == 0 && i != 0)
		{
			x = left;
			y += 160 + 32 + 64;
		}

		MissionView mission;
		mission.rect.setPosition(x, y);
		mission.rect.setSize(Vector2f(160, 160));
		mission.enabled = false;
		if (i <= maxCompletedLevel + 1)
			mission.enabled = true;
		if (mission.enabled)
			mission.rect.setFillColor(Color::Red);
		else
			mission.rect.setFillColor(GlobalVariables::GrayColor);

		unsigned int rating = getRating(i);
		float posX = x;
		for (unsigned int j = 0; j < STARS_COUNT; ++j)
		{
			RectangleShape starRect;
			starRect.setFillColor(Color::Cyan);
			starRect.setSize(Vector2f(32, 32));
			starRect.setPosition(posX, y + 160);
			if (j >= rating)
				starRect.setTexture(&ResourcesManager::Instance().getTexture(RESOURCES::EMPTY_STAR_TEXTURE));
			else
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
			{
				if (missions.at(mission).enabled)
					accept(mission);
			}
	}
	if (event->type == Event::MouseMoved)
	{
		for (unsigned int mission = 0; mission < missions.size(); ++mission)
		{
			if (missions.at(mission).rect.getGlobalBounds().contains(event->mouseMove.x, event->mouseMove.y))
				missions[mission].rect.setFillColor(Color::Blue);
			else
			{
				if (missions.at(mission).enabled)
					missions[mission].rect.setFillColor(Color::Red);
				else
					missions[mission].rect.setFillColor(GlobalVariables::GrayColor);
			}
		}
	}
	StateWindow::eventFilter(event);
}

void ChooseMissionWindow::accept(unsigned int num)
{
	Engine::Instance().setMission(num);
	Engine::Instance().setState(Engine::IN_GAME);
}

unsigned int ChooseMissionWindow::getRating(unsigned int n) const
{
	for(const Engine::CompletedMission& mission : Engine::Instance().getCompletedMissions())
		if (mission.number == n)
			return mission.stars;
	return 0;
}
