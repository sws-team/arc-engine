#include "gameobject.h"
#include "stdheader.h"
#include "managers.h"
#include "engine.h"
#include "gamemanagers.h"
#include "gameoptions.h"
#include "gameplatform.h"
#include "Game/Balance/balance.h"

namespace  {
	bool loadingFinihed = false;
}

void loading()
{
	GameManagers::loadResources();
	GameManagers::loadTranslations();
	Balance::Instance().load();
	loadingFinihed = true;
}

void showLoading()
{
	const sf::Vector2i windowSize = sf::Vector2i(400, 400);
	sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y),
							Engine::Instance().globalVariables()->appName(),
							sf::Style::None);
	window.setFramerateLimit(20);

	sf::Thread thread(&loading);
	thread.launch();

	const sf::Vector2i animationSize = sf::Vector2i(256, 256);
	Animation animation;
	sf::Texture texture;
	texture.loadFromFile("blast.png");
	animation.sprite.setTexture(texture);
	animation.size = sf::Vector2i(256, 256);
	animation.animationSpeed = 100;
	animation.frameCount = 4;
	animation.sprite.setPosition(windowSize.x/2 - animationSize.x/2,
								 windowSize.y/2 - animationSize.y/2);
	animation.loop = true;

	while (!loadingFinihed)
	{
		sf::Event event;
		window.pollEvent(event);

		window.clear(sf::Color::Red);
		animation.update();
		animation.draw(&window);
		window.display();
	}
}
