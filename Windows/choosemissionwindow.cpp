#include "choosemissionwindow.h"
#include "settings.h"
#include "globalvariables.h"
#include "Engine/engine.h"
#include "ResourcesManager/resourcesmanager.h"
#include "controller.h"

const Color ChooseMissionWindow::DISABLED_COLOR = Color(0,34,52, 128);
const Color ChooseMissionWindow::CURRENT_COLOR = Color(0,85,130, 128);
const Color ChooseMissionWindow::CURRENT_BORDER_COLOR = Color(0,51,78, 150);

ChooseMissionWindow::ChooseMissionWindow()
	: StateWindow()
{
	setBackground(RESOURCES::ABOUT_BACKGROUND);

	chooseRect.setOutlineThickness(5);
	chooseRect.setOutlineColor(CURRENT_BORDER_COLOR);
	chooseRect.setFillColor(Color::Transparent);
	currentMission = 0;

	const float rectSizeX = 160 * Settings::Instance().getScaleFactor().x;
	const float rectSizeY = 160 * Settings::Instance().getScaleFactor().y;
	const float offset = rectSizeY + Settings::Instance().getScaleFactor().y * 20;
	const float iconSizeX = 32 * Settings::Instance().getScaleFactor().x;
	const float iconSizeY = 32 * Settings::Instance().getScaleFactor().y;

	chooseRect.setSize(Vector2f(rectSizeX, rectSizeY + iconSizeY));
	const float left = Settings::Instance().getResolution().x * 0.3f;
	float x = left;
	float y = Settings::Instance().getResolution().y * 0.1f;
	const unsigned int maxCompletedLevel = Engine::Instance().maxCompletedLevel();

	RESOURCES::TEXTURE_TYPE textureType = RESOURCES::MAP_ICON_MISSION_1;
	for (unsigned int i = 0; i < Engine::Instance().missionsCount(); ++i)
	{
		if (i % COLUMN_COUNT == 0 && i != 0)
		{
			x = left;
			y += rectSizeY + iconSizeY + iconSizeY * 2;
		}

		MissionView mission;
		mission.rect.setPosition(x, y);
		mission.rect.setSize(Vector2f(rectSizeX, rectSizeY));
		mission.highlight.setPosition(x, y);
		mission.highlight.setSize(Vector2f(rectSizeX, rectSizeY + iconSizeY));
		mission.numberText.setFont(GlobalVariables::Instance().font());
		mission.numberText.setCharacterSize(60);
		mission.numberText.setScale(Settings::Instance().getScaleFactor());
		mission.numberText.setString(to_string(i + 1));
		mission.numberText.setPosition(x, y);
		mission.numberText.setFillColor(Color::White);
		mission.numberText.setOutlineColor(Color::Black);
		mission.numberText.setOutlineThickness(3);
		mission.enabled = false;
		if (i <= maxCompletedLevel + 1)
			mission.enabled = true;
		if (mission.enabled)
		{
			mission.highlight.setFillColor(Color::Transparent);
			mission.rect.setTexture(&ResourcesManager::Instance().getTexture(textureType));
		}
		else
		{
			mission.rect.setTexture(&ResourcesManager::Instance().getTexture(RESOURCES::LOCKED_ICON));
			mission.highlight.setFillColor(DISABLED_COLOR);
		}

		textureType = static_cast<RESOURCES::TEXTURE_TYPE>(static_cast<int>(textureType) + 1);

		unsigned int rating = getRating(i);
		float posX = x;
		for (unsigned int j = 0; j < STARS_COUNT; ++j)
		{
			RectangleShape starRect;
			starRect.setFillColor(Color::White);
			starRect.setSize(Vector2f(iconSizeX, iconSizeY));
			starRect.setPosition(posX, y + rectSizeY);
			if (j >= rating)
				starRect.setTexture(&ResourcesManager::Instance().getTexture(RESOURCES::EMPTY_STAR_TEXTURE));
			else
				starRect.setTexture(&ResourcesManager::Instance().getTexture(RESOURCES::STAR_TEXTURE));

			mission.stars.push_back(starRect);
			posX += iconSizeX;
		}

		missions.push_back(mission);
		x += offset;
	}
	updateRect();
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
		for(const RectangleShape& rect : mission.stars)
			window->draw(rect);
		window->draw(mission.rect);
		window->draw(mission.numberText);
		window->draw(mission.highlight);
	}
	window->draw(chooseRect);
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
			if (missions.at(mission).highlight.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
			{
				if (missions.at(mission).enabled)
					accept(mission);
			}
	}
	else if (event->type == Event::MouseMoved)
	{
		for (unsigned int mission = 0; mission < missions.size(); ++mission)
		{
			if (missions.at(mission).highlight.getGlobalBounds().contains(event->mouseMove.x, event->mouseMove.y))
				currentMission = mission;
		}
		updateRect();
	}
	else if (event->type == Event::KeyPressed)
	{
		switch (event->key.code)
		{
		case Keyboard::Left:
			currentMission--;
			break;
		case Keyboard::Right:
			currentMission++;
			break;
		case Keyboard::Up:
			currentMission -= COLUMN_COUNT;
			break;
		case Keyboard::Down:
			currentMission += COLUMN_COUNT;
			break;
		case Keyboard::Return:
			if (missions.at(currentMission).enabled)
			{
				accept(currentMission);
				return;
			}
			break;
		default:
			break;
		}
		if (currentMission < 0)
			currentMission = 0;
		if (currentMission > missions.size() - 1)
			currentMission = missions.size() - 1;
		updateRect();
	}
	else if (event->type == Event::JoystickButtonPressed)
	{
		if (event->joystickButton.button == Controller::KEY_START)
		{
			if (missions.at(currentMission).enabled)
			{
				accept(currentMission);
				return;
			}
		}
	}
	else if (event->type == Event::JoystickMoved)
	{
		if (event->joystickMove.axis == Joystick::X)
		{
			if (event->joystickMove.position > 50)
				currentMission++;
			else if (event->joystickMove.position < -50)
				currentMission--;
		}
		else if (event->joystickMove.axis == Joystick::Y)
		{
			if (event->joystickMove.position > 50)
				currentMission += COLUMN_COUNT;
			else if (event->joystickMove.position < -50)
				currentMission -= COLUMN_COUNT;
		}
		if (currentMission < 0)
			currentMission = 0;
		if (currentMission > missions.size() - 1)
			currentMission = missions.size() - 1;
		updateRect();
	}
	StateWindow::eventFilter(event);
}

void ChooseMissionWindow::accept(unsigned int num)
{
	Engine::Instance().setMission(num);
	Engine::Instance().setState(Engine::IN_GAME);
}

void ChooseMissionWindow::updateRect()
{
	for (unsigned int mission = 0; mission < missions.size(); ++mission)
	{
		if (currentMission == mission)
		{
			missions[mission].highlight.setFillColor(CURRENT_COLOR);
			chooseRect.setPosition(missions[mission].highlight.getPosition());
		}
		else
		{
			if (missions.at(mission).enabled)
				missions[mission].highlight.setFillColor(Color::Transparent);
			else
				missions[mission].highlight.setFillColor(DISABLED_COLOR);
		}
	}
}

unsigned int ChooseMissionWindow::getRating(unsigned int n) const
{
	for(const Engine::CompletedMission& mission : Engine::Instance().getCompletedMissions())
		if (mission.number == n)
			return mission.stars;
	return 0;
}
