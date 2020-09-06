#include "mainwindow.h"
#include "engine.h"
#include "managers.h"
#include "statewindow.h"

#include <SFML/Window/Cursor.hpp>
#include "Swoosh/ActivityController.h"

MainWindow::MainWindow()
	: sf::RenderWindow ()
{
	m_view = new sf::View();
}

MainWindow::~MainWindow()
{

}

void MainWindow::exec()
{
	Engine::Instance().getOptions()->changeState(Options::INTRO);

	setMouseCursorGrabbed(true);
	setMouseCursorVisible(false);

	const sf::Image img = Engine::Instance().texturesManager()->getTexture(TexturesManager::CURSOR).copyToImage();
	sf::Cursor cursor;

	const sf::Image icon = Engine::Instance().texturesManager()->getTexture(TexturesManager::ICON).copyToImage();
	this->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	bool active = true;
	updateView();

	const bool cursorLoaded = cursor.loadFromPixels(img.getPixelsPtr(), sf::Vector2u(32,32), sf::Vector2u(0,0));
	if (cursorLoaded)
		setMouseCursor(cursor);

	sf::Clock clock;
	float elapsed = 0.0f;
	while (isOpen())
	{
		clock.restart();

		sf::Event event;
		while (active ? pollEvent(event) : waitEvent(event))
		{
			Engine::Instance().getOptions()->globalEventFilter(&event);
			switch (event.type)
			{
			case sf::Event::Resized:
				updateView();
				break;
			case sf::Event::MouseLeft:
				active = false;
				break;
			case sf::Event::MouseEntered:
				active = true;
				break;
			default:
				break;
			}
			if (Engine::Instance().getOptions()->controller()->currentState() != nullptr)
				Engine::Instance().getOptions()->controller()->currentState()->eventFilter(&event);
		}
		if (active)
		{
			Engine::Instance().getOptions()->globalCallbacks();
			if (Engine::Instance().getOptions()->controller() != nullptr)
				Engine::Instance().getOptions()->controller()->update(elapsed);

			clear(sf::Color::Black);
			setView(*m_view);
			Engine::Instance().getOptions()->globalDraw();

			if (Engine::Instance().getOptions()->controller() != nullptr)
				Engine::Instance().getOptions()->controller()->draw();

			updateFPS();

			display();
		}
		else
			sf::sleep(sf::milliseconds(100));

		elapsed = static_cast<float>(clock.getElapsedTime().asSeconds());
	}
}

void MainWindow::updateView()
{
	const sf::Vector2f resolution = Engine::Instance().Instance().settingsManager()->getResolutionF();
	Engine::Instance().getOptions()->mainWindow()->view()->setSize(resolution.x, resolution.y);
	Engine::Instance().getOptions()->mainWindow()->view()->setCenter(resolution.x / 2, resolution.y / 2);

	float windowRatio = resolution.x / (float) resolution.y;
	constexpr float viewRatio = 16.f / 9.f;
	float sizeX = 1;
	float sizeY = 1;
	float posX = 0;
	float posY = 0;

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	if (horizontalSpacing)
	{
		sizeX = viewRatio / windowRatio;
		posX = (1 - sizeX) / 2.f;
	}
	else
	{
		sizeY = windowRatio / viewRatio;
		posY = (1 - sizeY) / 2.f;
	}
	m_view->setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
}

void MainWindow::resetView()
{
	m_view->setViewport(sf::FloatRect(0, 0, 1, 1));
}

sf::View *MainWindow::view() const
{
	return m_view;
}

void MainWindow::updateFPS()
{
	const float currentTime = fpsClock.restart().asSeconds();
	const float fpsValue = 1.f / currentTime;
	Engine::Instance().globalVariables()->setFps(static_cast<int>(fpsValue));
}
