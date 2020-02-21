#include "choosemissionwindow.h"
#include "settings.h"
#include "globalvariables.h"
#include "Engine/engine.h"
#include "ResourcesManager/resourcesmanager.h"
#include "controller.h"
#include "Translations/language.h"
#include "Game/Audio/soundcontroller.h"

const Color ChooseMissionWindow::DISABLED_COLOR = Color(0,34,52, 128);
const Color ChooseMissionWindow::CURRENT_COLOR = Color(0,85,130, 128);
const Color ChooseMissionWindow::CURRENT_BORDER_COLOR = Color(0,51,78, 150);

ChooseMissionWindow::ChooseMissionWindow()
	: StateWindow()
	,hovered(-1)
{
	setBackground(RESOURCES::ABOUT_BACKGROUND);

	chooseRect.setOutlineThickness(5);
	chooseRect.setOutlineColor(CURRENT_BORDER_COLOR);
	chooseRect.setFillColor(Color::Transparent);
	currentMission = 0;

	initDifficults();

	const float rectSizeX = 160 * Settings::Instance().getScaleFactor().x;
	const float rectSizeY = 160 * Settings::Instance().getScaleFactor().y;
	const float offsetX = Settings::Instance().getScaleFactor().x * 20;
	const float iconSizeX = 32 * Settings::Instance().getScaleFactor().x;
	const float iconSizeY = 32 * Settings::Instance().getScaleFactor().y;
	const float topOffset = Settings::Instance().getScaleFactor().y * 45;

	chooseRect.setSize(Vector2f(rectSizeX, rectSizeY + iconSizeY));
	const float left = Settings::Instance().getResolution().x * 0.3f;
	float x = left;
	float y = topOffset;

	const float difficultHeight = 60;
	const Vector2f difficultSize = Vector2f(rectSizeX * COLUMN_COUNT + offsetX * (COLUMN_COUNT - 1),
											difficultHeight);
	difficultRect.setPosition(x, y);
	difficultRect.setSize(difficultSize);

	const Vector2f difficultRectSize = Vector2f(200 * Settings::Instance().getScaleFactor().x,
												30 * Settings::Instance().getScaleFactor().y);
	const float difficultOffsetX = (difficultSize.x - 3 * difficultRectSize.x)/4;
	const float difficultOffsetY = (difficultSize.y - difficultRectSize.y)/2;

	y += difficultOffsetY;

	currentDifficultRect.setSize(difficultRectSize);
	choosedDifficultRect.setSize(difficultRectSize);
	currentDifficultRect.setFillColor(Color::Transparent);

	easyRect.setPosition(x + difficultOffsetX, y);
	easyRect.setSize(difficultRectSize);
	easyText.setPosition(easyRect.getPosition().x + difficultRectSize.x/2 - easyText.getGlobalBounds().width/2,
						 easyRect.getPosition().y - difficultRectSize.y/2);

	normalRect.setPosition(x + difficultOffsetX * 2 + difficultRectSize.x, y);
	normalRect.setSize(difficultRectSize);
	normalText.setPosition(normalRect.getPosition().x + difficultRectSize.x/2 - normalText.getGlobalBounds().width/2,
						   normalRect.getPosition().y - difficultRectSize.y/2);

	hardRect.setPosition(x + difficultOffsetX * 3 + difficultRectSize.x * 2, y);
	hardRect.setSize(difficultRectSize);
	hardText.setPosition(hardRect.getPosition().x + difficultRectSize.x/2 - hardText.getGlobalBounds().width/2,
						 hardRect.getPosition().y - difficultRectSize.y/2);

	if (Settings::Instance().difficult() == 1)
		choosedDifficultRect.setPosition(normalRect.getPosition());
	else if (Settings::Instance().difficult() == 2)
		choosedDifficultRect.setPosition(easyRect.getPosition());
	else
		choosedDifficultRect.setPosition(hardRect.getPosition());

	y += difficultHeight;
	y += difficultOffsetY;
	y += topOffset;

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
		x += rectSizeX;
		x += offsetX;
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
	window->draw(difficultRect);
	window->draw(currentDifficultRect);

	window->draw(easyRect);
	window->draw(normalRect);
	window->draw(hardRect);

	window->draw(choosedDifficultRect);

	window->draw(easyText);
	window->draw(normalText);
	window->draw(hardText);

	window->draw(currentDifficultRect);
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
		const Vector2f pos = Vector2f(event->mouseButton.x, event->mouseButton.y);
		for (unsigned int mission = 0; mission < missions.size(); ++mission)
			if (missions.at(mission).highlight.getGlobalBounds().contains(pos))
			{
				if (missions.at(mission).enabled)
				{
					SoundController::Instance().playOnce(CLICK_SOUND_FILE);
					accept(mission);
				}
			}
		if (easyRect.getGlobalBounds().contains(pos))
		{
			choosedDifficultRect.setPosition(easyRect.getPosition());
			SoundController::Instance().playOnce(CLICK_SOUND_FILE);
			Settings::Instance().setEasyDifficult();
		}
		if (normalRect.getGlobalBounds().contains(pos))
		{
			choosedDifficultRect.setPosition(normalRect.getPosition());
			SoundController::Instance().playOnce(CLICK_SOUND_FILE);
			Settings::Instance().setNormalDifficult();
		}
		if (hardRect.getGlobalBounds().contains(pos))
		{
			choosedDifficultRect.setPosition(hardRect.getPosition());
			SoundController::Instance().playOnce(CLICK_SOUND_FILE);
			Settings::Instance().setHardDifficult();
		}
	}
	else if (event->type == Event::MouseMoved)
	{
		int hoverId = -1;
		const Vector2f pos = Vector2f(event->mouseMove.x, event->mouseMove.y);
		for (unsigned int mission = 0; mission < missions.size(); ++mission)
		{
			if (missions.at(mission).highlight.getGlobalBounds().contains(pos))
			{
				currentMission = mission;
				hoverId = mission;
			}
		}
		updateRect();
		currentDifficultRect.setFillColor(Color::Transparent);
		if (easyRect.getGlobalBounds().contains(pos))
		{
			currentDifficultRect.setFillColor(DISABLED_COLOR);
			currentDifficultRect.setPosition(easyRect.getPosition());
			hoverId = missions.size() + 1;
		}
		if (normalRect.getGlobalBounds().contains(pos))
		{
			currentDifficultRect.setFillColor(DISABLED_COLOR);
			currentDifficultRect.setPosition(normalRect.getPosition());
			hoverId = missions.size() + 2;
		}
		if (hardRect.getGlobalBounds().contains(pos))
		{
			currentDifficultRect.setFillColor(DISABLED_COLOR);
			currentDifficultRect.setPosition(hardRect.getPosition());
			hoverId = missions.size() + 3;
		}
		if (hoverId != -1 && hovered != hoverId)
		{
			hovered = hoverId;
			SoundController::Instance().playOnce(BUTTON_HOVER_SOUND_FILE);
		}
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

void ChooseMissionWindow::initDifficults()
{
	const Color difficultRectFillColor = Color(16, 32, 64, 128);
	const Color difficultRectBorderColor = Color(64, 32, 16, 192);
	const Color choosedRectBorderColor = Color(32, 128, 16, 192);
	const float difficultCharatersSize = 40;
	const float thickness = 3 * Settings::Instance().getScaleFactor().x;

	choosedDifficultRect.setOutlineThickness(thickness);
	choosedDifficultRect.setOutlineColor(choosedRectBorderColor);
	choosedDifficultRect.setFillColor(Color::Transparent);

	difficultRect.setOutlineThickness(thickness);
	difficultRect.setFillColor(CURRENT_COLOR);
	difficultRect.setOutlineColor(CURRENT_COLOR);

	easyRect.setOutlineThickness(thickness);
	easyRect.setFillColor(difficultRectFillColor);
	easyRect.setOutlineColor(difficultRectBorderColor);

	normalRect.setOutlineThickness(thickness);
	normalRect.setFillColor(difficultRectFillColor);
	normalRect.setOutlineColor(difficultRectBorderColor);

	hardRect.setOutlineThickness(thickness);
	hardRect.setFillColor(difficultRectFillColor);
	hardRect.setOutlineColor(difficultRectBorderColor);

	easyText.setFont(GlobalVariables::Instance().font());
	easyText.setCharacterSize(difficultCharatersSize);
	easyText.setScale(Settings::Instance().getScaleFactor());
	easyText.setString(Language::Instance().translate(Language::EASY));
	easyText.setFillColor(Color::White);
	easyText.setOutlineColor(Color::Black);
	easyText.setOutlineThickness(thickness);

	normalText.setFont(GlobalVariables::Instance().font());
	normalText.setCharacterSize(difficultCharatersSize);
	normalText.setScale(Settings::Instance().getScaleFactor());
	normalText.setString(Language::Instance().translate(Language::NORMAL));
	normalText.setFillColor(Color::White);
	normalText.setOutlineColor(Color::Black);
	normalText.setOutlineThickness(thickness);

	hardText.setFont(GlobalVariables::Instance().font());
	hardText.setCharacterSize(difficultCharatersSize);
	hardText.setScale(Settings::Instance().getScaleFactor());
	hardText.setString(Language::Instance().translate(Language::HARD));
	hardText.setFillColor(Color::White);
	hardText.setOutlineColor(Color::Black);
	hardText.setOutlineThickness(thickness);
}
