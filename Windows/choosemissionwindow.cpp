#include "choosemissionwindow.h"
#include "controller.h"
#include "engine.h"
#include "enginedef.h"
#include "managers.h"
#include "gamemanagers.h"
#include "gameoptions.h"
#include "gamestatemanager.h"
#include "mainwindow.h"

const sf::Color ChooseMissionWindow::DISABLED_COLOR = sf::Color(0,34,52, 128);
const sf::Color ChooseMissionWindow::CURRENT_COLOR = sf::Color(0,85,130, 128);
const sf::Color ChooseMissionWindow::CURRENT_BORDER_COLOR = sf::Color(0,51,78, 150);

ChooseMissionWindow::ChooseMissionWindow()
	: StateWindow()
	,hovered(-1)
{
	setBackground(GAME_TEXTURE::MISSON_BACKGROUND);

	chooseRect.setOutlineThickness(5);
	chooseRect.setOutlineColor(CURRENT_BORDER_COLOR);
	chooseRect.setFillColor(CURRENT_COLOR);
	currentMission = 0;
	const unsigned int missionsCount = Engine::Instance().options<GameOptions>()->missionsCount();
	const int rowCount = static_cast<int>(ceil(static_cast<float>(missionsCount) / COLUMN_COUNT));
	const sf::Vector2f scaleFactor = Engine::Instance().settingsManager()->getScaleFactor();
	const float rectSizeX = 240 * scaleFactor.x;
	const float rectSizeY = 136 * scaleFactor.y;
	const float frameRectOffsetX = 16 * scaleFactor.x;
	const float frameRectOffsetY = 18 * scaleFactor.y;
	const float offsetX = 42 * scaleFactor.x;
	const float iconSizeX = 30 * scaleFactor.x;
	const float iconSizeY = 30 * scaleFactor.y;
	const float starOffsetX = 15 * scaleFactor.x;
	const float starOffsetY = 10 * scaleFactor.y;

	chooseRect.setSize(sf::Vector2f(rectSizeX, rectSizeY + iconSizeY + starOffsetY));
	const sf::Vector2f areaSize =
			sf::Vector2f(COLUMN_COUNT * (rectSizeX + frameRectOffsetX * 2),
				rowCount * (rectSizeY + iconSizeY + starOffsetY + frameRectOffsetY * 2));

	const float left = (Engine::Instance().settingsManager()->getResolution().x - areaSize.x) / 2;
	const float top = (Engine::Instance().settingsManager()->getResolution().y - areaSize.y) / 2;

	float x = left;
	float y = top;
	const unsigned int maxCompletedLevel = Engine::Instance().options<GameOptions>()->maxCompletedLevel();
	const bool hasCompletedMissions = !Engine::Instance().options<GameOptions>()->getCompletedMissions().empty();
	TextureType textureType = GAME_TEXTURE::MAP_ICON_MISSION_1;
	for (unsigned int i = 0; i < missionsCount; ++i)
	{
		if (i % COLUMN_COUNT == 0 && i != 0)
		{
			x = left;
			y += rectSizeY + iconSizeY + iconSizeY * 2;
		}

		MissionView mission;
		mission.rect.setPosition(x - frameRectOffsetX, y - frameRectOffsetY);
		mission.rect.setSize(sf::Vector2f(rectSizeX + frameRectOffsetX * 2,
										  rectSizeY + frameRectOffsetY * 2 + iconSizeY + starOffsetY));
		mission.rect.setTexture(&Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::FRAME));
		mission.mapRect.setPosition(x, y);
		mission.mapRect.setSize(sf::Vector2f(rectSizeX, rectSizeY));
		mission.highlight.setPosition(x, y);
		mission.highlight.setSize(sf::Vector2f(rectSizeX, rectSizeY + iconSizeY + starOffsetY));
		mission.numberText.setFont(Engine::Instance().fontManager()->font());
		mission.numberText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(60));
		mission.numberText.setScale(Engine::Instance().settingsManager()->getScaleFactor());
		mission.numberText.setString(std::to_string(i + 1));
		mission.numberText.setPosition(x, y);
		mission.numberText.setFillColor(sf::Color::White);
		mission.numberText.setOutlineColor(sf::Color::Black);
		mission.numberText.setOutlineThickness(3);
		mission.enabled = false;
		if (hasCompletedMissions)
		{
			if (i <= maxCompletedLevel)
				mission.enabled = true;
		}
		else if (i == 0)
			mission.enabled = true;
#ifdef DEMO_VERSION
			if (i > 1)
				mission.enabled = false;
#endif
		if (mission.enabled)
		{
			mission.highlight.setFillColor(sf::Color::Transparent);
			mission.mapRect.setTexture(&Engine::Instance().texturesManager()->getTexture(textureType));
		}
		else
		{
			mission.mapRect.setTexture(&Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::LOCKED_ICON));
			mission.highlight.setFillColor(DISABLED_COLOR);
		}

		textureType = static_cast<TextureType>(static_cast<int>(textureType) + 1);

		unsigned int rating = getRating(i + 1);
		float posX = x;
		posX += starOffsetX;
		for (unsigned int j = 0; j < STARS_COUNT; ++j)
		{
			sf::RectangleShape starRect;
			starRect.setFillColor(sf::Color::White);
			starRect.setSize(sf::Vector2f(iconSizeX, iconSizeY));
			starRect.setPosition(posX, y + rectSizeY + starOffsetY);
			if (j >= rating)
				starRect.setTexture(&Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::EMPTY_STAR_TEXTURE));
			else
				starRect.setTexture(&Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::STAR_TEXTURE));

			mission.stars.push_back(starRect);
			posX += iconSizeX;
			posX += starOffsetX;
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
	Engine::Instance().stateManager()->setState(GameStateManager::MENU);
}

void ChooseMissionWindow::paint(sf::RenderWindow *window)
{
	drawBackground(window);
	for (const MissionView& mission : missions)
	{
		window->draw(mission.rect);
		for(const sf::RectangleShape& rect : mission.stars)
			window->draw(rect);
		window->draw(mission.mapRect);
		window->draw(mission.numberText);
		window->draw(mission.highlight);
	}
	window->draw(chooseRect);
}

void ChooseMissionWindow::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::MouseButtonPressed)
	{
		if (event->mouseButton.button == sf::Mouse::Left)
		{
			const sf::Vector2i pixelPos = sf::Vector2i(event->mouseButton.x, event->mouseButton.y);
			const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																				   *Engine::Instance().window()->view());
			for (unsigned int mission = 0; mission < missions.size(); ++mission)
				if (missions.at(mission).highlight.getGlobalBounds().contains(pos))
				{
#ifdef RELEASE_BUILD
					if (missions.at(mission).enabled)
#endif
					{
						Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
						accept(mission);
					}
				}
		}
	}
	else if (event->type == sf::Event::MouseMoved)
	{
		const sf::Vector2i pixelPos = sf::Vector2i(event->mouseMove.x, event->mouseMove.y);
		const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																			   *Engine::Instance().window()->view());
		int hoverId = -1;
		for (unsigned int mission = 0; mission < missions.size(); ++mission)
		{
			if (missions.at(mission).highlight.getGlobalBounds().contains(pos))
			{
				currentMission = mission;
				hoverId = mission;
			}
		}
		updateRect();
	}
	else if (event->type == sf::Event::KeyPressed)
	{
		switch (event->key.code)
		{
		case sf::Keyboard::Left:
			currentMission--;
			break;
		case sf::Keyboard::Right:
			currentMission++;
			break;
		case sf::Keyboard::Up:
			currentMission -= COLUMN_COUNT;
			break;
		case sf::Keyboard::Down:
			currentMission += COLUMN_COUNT;
			break;
		case sf::Keyboard::Return:
			if (missions.at(currentMission).enabled)
			{
				accept(currentMission);
				return;
			}
			break;
		case sf::Keyboard::Tab:
		{
			if (Engine::Instance().options<GameOptions>()->getDev())
				accept(-1);
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
	else if (event->type == sf::Event::JoystickButtonPressed)
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
	else if (event->type == sf::Event::JoystickMoved)
	{
		if (event->joystickMove.axis == sf::Joystick::X)
		{
			if (event->joystickMove.position > 50)
				currentMission++;
			else if (event->joystickMove.position < -50)
				currentMission--;
		}
		else if (event->joystickMove.axis == sf::Joystick::Y)
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
	Engine::Instance().options<GameOptions>()->setMission(num + 1);
	Engine::Instance().stateManager()->setState(GameStateManager::IN_GAME);
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
				missions[mission].highlight.setFillColor(sf::Color::Transparent);
			else
				missions[mission].highlight.setFillColor(DISABLED_COLOR);
		}
	}
}

unsigned int ChooseMissionWindow::getRating(unsigned int n) const
{
	for(const GameOptions::CompletedMission& mission : Engine::Instance().options<GameOptions>()->getCompletedMissions())
		if (mission.number == n)
			return mission.stars;
	return 0;
}
