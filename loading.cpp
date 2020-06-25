#include "gameobject.h"
#include "stdheader.h"
#include "managers.h"
#include "engine.h"
#include "gamemanagers.h"
#include "gameoptions.h"
#include "gameplatform.h"
#include "Game/Balance/balance.h"
#include "gameresource.h"
#include "base64.h"

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
	sf::Texture loadingTexture;
	const std::vector<GameResource::Resource> resources = GameResource::Resource::loadResources("screen.dat");
	for(const GameResource::Resource &resource : resources)
	{
		std::string out;
		Base64::Decode(resource.data, &out);

		if (resource.type == GameResource::TEXTURE
				&& resource.name == "loading")
		{
			const size_t size = out.size();
			void *textureData = malloc(size);
			std::memcpy(textureData,(void*)out.c_str(), size);
			loadingTexture.loadFromMemory(textureData, size);
		}
	}
	const sf::Vector2i windowSize = sf::Vector2i(320, 319);
	sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y),
							Engine::Instance().globalVariables()->appName(),
							sf::Style::None);
	window.setFramerateLimit(20);

	sf::Thread thread(&loading);
	thread.launch();

	Animation animation;
	animation.sprite.setTexture(loadingTexture);
	animation.size = windowSize;
	animation.animationSpeed = 75;
	animation.frameCount = 6;
	animation.rowCount = 12;
	animation.sprite.setPosition(0, 0);
	animation.loop = true;
	animation.cycled = true;

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
