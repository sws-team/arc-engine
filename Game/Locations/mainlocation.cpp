#include "mainlocation.h"
#include "settings.h"
#include "globalvariables.h"
#include "shoplocation.h"
#include "garagelocation.h"
#include "savedgame.h"
#include "Game/SpaceShip/spaceship.h"
#include "Game/objectsfactory.h"
#include "Game/gamecontroller.h"

MainLocation::MainLocation()
	: StateWindow()
	,mission_number(0)
{
	m_state = DEFAULT_STATE;
	setBackground(RESOURCES::PLANETS_BACKGROUND);

	const float r = 150.f;
	shopLink.setRadius(r);
	shopLink.setFillColor(Color::Transparent);
	shopLink.setOutlineThickness(10);
	shopLink.setOutlineColor(Color::White);

	shopLink.setPosition(0.322f * static_cast<float>(Settings::Instance().getResolution().x - r),
						 0.3888f * static_cast<float>(Settings::Instance().getResolution().y - r));

	circle.texture.loadFromFile("images/Locations/circle.png");
	circle.sprite.setTexture(circle.texture);
	circle.sprite.setPosition(0.622f * static_cast<float>(Settings::Instance().getResolution().x - r),
							  0.2888f * static_cast<float>(Settings::Instance().getResolution().y - r));

	exit.texture.loadFromFile("images/Locations/exit.png");
	exit.sprite.setTexture(exit.texture);
	exit.sprite.setPosition(Settings::Instance().getResolution().x * 0.8f,
							Settings::Instance().getResolution().y * 0.1f);

	darkRect.setPosition(0, 0);
	darkRect.setSize(Vector2f(Settings::Instance().getResolution().x, Settings::Instance().getResolution().y));
	Color semiGray = GlobalVariables::GrayColor;
	semiGray.a = 200;
	darkRect.setFillColor(semiGray);


	missionImage.texture.loadFromFile("images/mission.png");
	missionImage.sprite.setTexture(missionImage.texture);
	missionImage.sprite.scale(0.5, 0.5);
	missionImage.sprite.setPosition(100, 100);

	missionText.setFillColor(Color::Blue);
	missionText.setFont(GlobalVariables::Instance().font());


	missionText.setPosition(missionImage.sprite.getPosition() + Vector2f(50, 50));


	acceptMissionRect.setSize(Vector2f(150, 150));
	acceptMissionRect.setPosition(missionImage.sprite.getPosition() +
								  Vector2f(missionImage.sprite.getGlobalBounds().width, missionImage.sprite.getGlobalBounds().height) -
								  acceptMissionRect.getSize());

	backMissionRect.setSize(Vector2f(150, 150));
	backMissionRect.setPosition(missionImage.sprite.getPosition() +
								  Vector2f(0, missionImage.sprite.getGlobalBounds().height) -
								  acceptMissionRect.getSize());

	MapLink link;
	link.isVisible = false;
	link.circle.setRadius(r/2);
	link.circle.setFillColor(Color::Transparent);
	link.circle.setOutlineThickness(10);


	const vector<unsigned int> completed = SavedGameLoader::Instance().getSavedGame().completedLevels;

	if(find(completed.begin(), completed.end(), 1) == completed.end())
	{
		link.circle.setOutlineColor(Color::Blue);
		link.circle.setPosition(0.1f * static_cast<float>(Settings::Instance().getResolution().x - r),
							 0.1f * static_cast<float>(Settings::Instance().getResolution().y - r));
		link.mission_number = 1;
		links.push_back(link);

	}
	if(find(completed.begin(), completed.end(), 2) == completed.end())
	{
		link.circle.setOutlineColor(Color::Red);
		link.circle.setPosition(0.1f * static_cast<float>(Settings::Instance().getResolution().x - r),
							 0.8f * static_cast<float>(Settings::Instance().getResolution().y - r));
		link.mission_number = 2;
		links.push_back(link);
	}
	if(find(completed.begin(), completed.end(), 3) == completed.end())
	{
		link.circle.setOutlineColor(Color::Green);
		link.circle.setPosition(0.8f * static_cast<float>(Settings::Instance().getResolution().x - r),
							 0.8f * static_cast<float>(Settings::Instance().getResolution().y - r));
		link.mission_number = 3;
		links.push_back(link);
	}
	if(find(completed.begin(), completed.end(), 4) == completed.end())
	{
		link.circle.setOutlineColor(Color::Magenta);
		link.circle.setPosition(0.4f * static_cast<float>(Settings::Instance().getResolution().x - r),
							 0.8f * static_cast<float>(Settings::Instance().getResolution().y - r));
		link.mission_number = 4;
		links.push_back(link);
	}
	if(find(completed.begin(), completed.end(), 5) == completed.end())
	{
		link.circle.setOutlineColor(Color::Cyan);
		link.circle.setPosition(0.8f * static_cast<float>(Settings::Instance().getResolution().x - r),
							 0.4f * static_cast<float>(Settings::Instance().getResolution().y - r));
		link.mission_number = 5;
		links.push_back(link);
	}

	shopLinkVisible = false;
	circleVisible = false;
}

MainLocation::~MainLocation()
{

}

void MainLocation::init()
{
	GameController::Instance().clear();
	const Vector2f startPos = Vector2f(Settings::Instance().getScaleFactor().x * static_cast<float>(Settings::Instance().getResolution().x)/2,
										Settings::Instance().getScaleFactor().y * static_cast<float>(Settings::Instance().getResolution().y)/2);

	const SavedGame savedGame = SavedGameLoader::Instance().getSavedGame();
	for(const GameState& data : savedGame.playersData)
	{
		SpaceShip *player = ObjectsFactory::Instance().createSpaceShip(data.spaceShip, startPos);
		player->loadData(data);
		GameController::Instance().addPlayer(player);
	}
}

void MainLocation::paint(RenderWindow *window)
{
	drawBackground(window);

	switch (m_state)
	{
	case DEFAULT_STATE:
	{
		if (shopLinkVisible)
			window->draw(shopLink);

		for(const MapLink& link : links)
		{
			if (link.isVisible)
				window->draw(link.circle);
		}

		if(circleVisible)
			window->draw(circle.sprite);
		window->draw(exit.sprite);
	}
		break;
	case BRIEFING:
	{
		window->draw(darkRect);
		window->draw(missionImage.sprite);
		window->draw(acceptMissionRect);
		window->draw(backMissionRect);
	}
		break;
	}
}

void MainLocation::eventFilter(Event *event)
{
	switch (m_state)
	{
	case DEFAULT_STATE:
	{
		if (event->type == Event::MouseMoved)
		{
			shopLinkVisible = shopLink.getGlobalBounds().contains(event->mouseMove.x, event->mouseMove.y);

			for(MapLink& link : links)
				link.isVisible = link.circle.getGlobalBounds().contains(event->mouseMove.x, event->mouseMove.y);

			circleVisible = circle.sprite.getGlobalBounds().contains(event->mouseMove.x, event->mouseMove.y);
		}
		else if (event->type == Event::MouseButtonPressed)
		{
	//		cout <<event->mouseButton.x << " "  << event->mouseButton.y << endl
	//			<< mission1Link.getGlobalBounds().top << " "
	//			   << mission1Link.getGlobalBounds().left << " "
	//				  << mission1Link.getGlobalBounds().width << " "
	//				  << mission1Link.getGlobalBounds().height << " " << endl<<endl;


			for (unsigned int i = 0; i < links.size(); ++i)
			{
				if (links.at(i).circle.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
				{
					mission_number = links.at(i).mission_number;
					missionText.setString("sdfhkjdshfgkjdhfgjdfhgjk");
					changeState(BRIEFING);
					return;
				}
			}

			if (shopLink.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
			{
				Engine::Instance().setState(Engine::GARAGE_LOCATION);
				return;
			}
			if (circle.sprite.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
			{
				Engine::Instance().setState(Engine::SHOP_LOCATION);
				return;
			}
			if (exit.sprite.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
			{
				Engine::Instance().setState(Engine::CLOSING);
				return;
			}
		}
		else
			StateWindow::eventFilter(event);
	}
		break;
	case BRIEFING:
	{
		if (event->type == Event::MouseButtonPressed)
		{
			if (acceptMissionRect.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
			{
				SavedGameLoader::Instance().setCurrentMission(mission_number);
				Engine::Instance().setState(Engine::IN_GAME);
			}
			if (backMissionRect.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
			{
				changeState(DEFAULT_STATE);
			}
		}
	}
		break;
	}

}

void MainLocation::back()
{
	Engine::Instance().setState(Engine::LOAD_GAME);
}

void MainLocation::changeState(MainLocation::MAIN_LOCATION_STATES state)
{
	m_state = state;
}
