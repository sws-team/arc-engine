#include "mainwindow.h"
#include "engine.h"
#include "managers.h"
#include "statewindow.h"

#include <SFML/Window/Cursor.hpp>

MainWindow::MainWindow()
	: sf::RenderWindow ()
	,currentState(nullptr)
{
	state = StateManager::UNKNOWN;
	m_view = new sf::View();
	Engine::Instance().setWindow(this);
}

MainWindow::~MainWindow()
{

}

void MainWindow::exec()
{
	setMouseCursorGrabbed(true);
	setMouseCursorVisible(false);

	const sf::Image icon = Engine::Instance().texturesManager()->getTexture(TexturesManager::ICON).copyToImage();
	this->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	bool active = true;
	updateView();

	sf::Cursor cursor;
	const sf::Image img = Engine::Instance().texturesManager()->getTexture(TexturesManager::CURSOR).copyToImage();
	const bool cursorLoaded = cursor.loadFromPixels(img.getPixelsPtr(), sf::Vector2u(32,32), sf::Vector2u(0,0));
	while (isOpen())
	{
		if (Engine::Instance().stateManager()->getState() != state)
		{
			if (currentState != nullptr)
				delete currentState;

			if (cursorLoaded)
				setMouseCursor(cursor);

			if (Engine::Instance().stateManager()->getState() == StateManager::EXIT)
			{
				this->close();
				return;
			}
			currentState = Engine::Instance().stateManager()->createState(
						Engine::Instance().stateManager()->getState());
			currentState->init();
			state = Engine::Instance().stateManager()->getState();
			setMouseCursorVisible(state != StateManager::INTRO);
		}

		if (currentState == nullptr)
			continue;

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
			currentState->eventFilter(&event);
		}
		currentState->update();

		Engine::Instance().getOptions()->globalCallbacks();

		if (active)
		{
			clear(sf::Color::Black);
			setView(*m_view);
			currentState->paint(this);
			updateFPS();

			Engine::Instance().getOptions()->globalDraw();

			display();
		}
		else
			sf::sleep(sf::milliseconds(100));
	}
}

void MainWindow::updateView()
{
	const sf::Vector2f resolution = Engine::Instance().Instance().settingsManager()->getResolutionF();
	Engine::Instance().window()->view()->setSize(resolution.x, resolution.y);
	Engine::Instance().window()->view()->setCenter(resolution.x / 2, resolution.y / 2);

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
