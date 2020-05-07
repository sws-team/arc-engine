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
#ifdef WITH_DIFFICULT
	initDifficults();
#endif
	const sf::Vector2f scaleFactor = Engine::Instance().settingsManager()->getScaleFactor();
	const float rectSizeX = 240 * scaleFactor.x;
	const float rectSizeY = 135 * scaleFactor.y;
	const float frameRectOffsetX = 16 * scaleFactor.x;
	const float frameRectOffsetY = 18 * scaleFactor.y;
	const float offsetX = 42 * scaleFactor.x;
	const float iconSizeX = 30 * scaleFactor.x;
	const float iconSizeY = 30 * scaleFactor.y;
	const float topOffset =  64 * scaleFactor.y;
	const float starOffsetX = 15 * scaleFactor.x;
	const float starOffsetY = 10 * scaleFactor.y;

	chooseRect.setSize(sf::Vector2f(rectSizeX, rectSizeY + iconSizeY + starOffsetY));
	const float left = scaleFactor.x * (Engine::Instance().settingsManager()->getResolution().x - COLUMN_COUNT * 160 + (COLUMN_COUNT - 1) * 20) / 2;

	float x = left;
	float y = topOffset;
#ifdef WITH_DIFFICULT
	const float difficultHeight = 60;
	const sf::Vector2f difficultSize = sf::Vector2f(rectSizeX * COLUMN_COUNT + offsetX * (COLUMN_COUNT - 1),
											difficultHeight);
	difficultRect.setPosition(x, y);
	difficultRect.setSize(difficultSize);

	const sf::Vector2f difficultRectSize = sf::Vector2f(200 * scaleFactor.x,
												30 * scaleFactor.y);
	const float difficultOffsetX = (difficultSize.x - 3 * difficultRectSize.x)/4;
	const float difficultOffsetY = (difficultSize.y - difficultRectSize.y)/2;

	y += difficultOffsetY;

	currentDifficultRect.setSize(difficultRectSize);
	choosedDifficultRect.setSize(difficultRectSize);
	currentDifficultRect.setFillColor(sf::Color::Transparent);

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

	if (Engine::Instance().options<GameOptions>()->difficult() == 1)
		choosedDifficultRect.setPosition(normalRect.getPosition());
	else if (Engine::Instance().options<GameOptions>()->difficult() == 2)
		choosedDifficultRect.setPosition(easyRect.getPosition());
	else
		choosedDifficultRect.setPosition(hardRect.getPosition());

	y += difficultHeight;
	y += difficultOffsetY;
#else
	y += 64 * scaleFactor.y;
#endif
	y += topOffset;
	const unsigned int maxCompletedLevel = Engine::Instance().options<GameOptions>()->maxCompletedLevel();
	const bool hasCompletedMissions = !Engine::Instance().options<GameOptions>()->getCompletedMissions().empty();
	TextureType textureType = GAME_TEXTURE::MAP_ICON_MISSION_1;
	const unsigned int missionsCount =
#ifdef DEMO_VERSION
			2;
#else
	Engine::Instance().options<GameOptions>()->missionsCount();
#endif
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
			if (i <= maxCompletedLevel + 1)
				mission.enabled = true;
		}
		else if (i == 0)
			mission.enabled = true;

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

		unsigned int rating = getRating(i);
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
#ifdef WITH_DIFFICULT
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
#endif
}

void ChooseMissionWindow::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::MouseButtonPressed)
	{
		const sf::Vector2i pixelPos = sf::Vector2i(event->mouseButton.x, event->mouseButton.y);
		const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																			   *Engine::Instance().window()->view());
		for (unsigned int mission = 0; mission < missions.size(); ++mission)
			if (missions.at(mission).highlight.getGlobalBounds().contains(pos))
			{
#ifndef TEST_WAVES
				if (missions.at(mission).enabled)
#endif
				{
					Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
					accept(mission);
				}
			}
#ifdef WITH_DIFFICULT
		if (easyRect.getGlobalBounds().contains(pos))
		{
			choosedDifficultRect.setPosition(easyRect.getPosition());
			Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
			Engine::Instance().options<GameOptions>()->setEasyDifficult();
		}
		if (normalRect.getGlobalBounds().contains(pos))
		{
			choosedDifficultRect.setPosition(normalRect.getPosition());
			Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
			Engine::Instance().options<GameOptions>()->setNormalDifficult();
		}
		if (hardRect.getGlobalBounds().contains(pos))
		{
			choosedDifficultRect.setPosition(hardRect.getPosition());
			Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
			Engine::Instance().options<GameOptions>()->setHardDifficult();
		}
#endif
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
#ifdef WITH_DIFFICULT
		currentDifficultRect.setFillColor(sf::Color::Transparent);
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
			Engine::Instance().soundManager()->playOnce(SoundManager::HOVER);
		}
#endif
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
	Engine::Instance().options<GameOptions>()->setMission(num);
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
#ifdef WITH_DIFFICULT
void ChooseMissionWindow::initDifficults()
{
	const sf::Color difficultRectFillColor = sf::Color(16, 32, 64, 128);
	const sf::Color difficultRectBorderColor = sf::Color(64, 32, 16, 192);
	const sf::Color choosedRectBorderColor = sf::Color(32, 128, 16, 192);
	const float difficultCharatersSize = 40;
	const float thickness = 3 * Engine::Instance().settingsManager()->getScaleFactor().x;

	choosedDifficultRect.setOutlineThickness(thickness);
	choosedDifficultRect.setOutlineColor(choosedRectBorderColor);
	choosedDifficultRect.setFillColor(sf::Color::Transparent);

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

	easyText.setFont(Engine::Instance().fontManager()->font());
	easyText.setCharacterSize(difficultCharatersSize);
	easyText.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	easyText.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::EASY));
	easyText.setFillColor(sf::Color::White);
	easyText.setOutlineColor(sf::Color::Black);
	easyText.setOutlineThickness(thickness);

	normalText.setFont(Engine::Instance().fontManager()->font());
	normalText.setCharacterSize(difficultCharatersSize);
	normalText.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	normalText.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::NORMAL));
	normalText.setFillColor(sf::Color::White);
	normalText.setOutlineColor(sf::Color::Black);
	normalText.setOutlineThickness(thickness);

	hardText.setFont(Engine::Instance().fontManager()->font());
	hardText.setCharacterSize(difficultCharatersSize);
	hardText.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	hardText.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::HARD));
	hardText.setFillColor(sf::Color::White);
	hardText.setOutlineColor(sf::Color::Black);
	hardText.setOutlineThickness(thickness);
}
#endif
