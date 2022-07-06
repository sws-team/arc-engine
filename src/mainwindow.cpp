#include "mainwindow.h"
#include "engine.h"
#include "managers.h"
#include "arcwindow.h"
#include "timer.h"
#include <arcscene.h>

#include <SFML/Window/Cursor.hpp>

MainWindow::MainWindow()
	: sf::RenderWindow ()
{
	currentSceneType = SceneManager::UNKNOWN;
	m_view = new sf::View();
	Engine::Instance().setWindow(this);
	setMouseCursorGrabbed(true);
	setMouseCursorVisible(false);
}

MainWindow::~MainWindow()
{

}

void MainWindow::prepare()
{
	const sf::Image icon = Engine::Instance().texturesManager()->getTexture(TexturesManager::ICON).copyToImage();
	this->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	sf::Cursor cursor;
	const sf::Image img = Engine::Instance().texturesManager()->getTexture(TexturesManager::CURSOR).copyToImage();
	const bool cursorLoaded = cursor.loadFromPixels(img.getPixelsPtr(), sf::Vector2u(32,32), sf::Vector2u(0,0));
	if (cursorLoaded)
		setMouseCursor(cursor);
}

void MainWindow::exec()
{
	bool active = true;
	while (isOpen()) {
		ArcScene* currentScene = SCENE_MANAGER->currentScene();
		if (SCENE_MANAGER->currentSceneType() != currentSceneType) {
			if (currentScene != nullptr) {
				currentScene->deinit();
				delete currentScene;
			}
			if (SCENE_MANAGER->currentSceneType() == SceneManager::EXIT) {
				this->close();
				return;
			}
			currentScene = SCENE_MANAGER->createScene(SCENE_MANAGER->currentSceneType());
			currentScene->init();
			if (currentScene != nullptr) {
				currentSceneType = SCENE_MANAGER->currentSceneType();
				SCENE_MANAGER->setCurrentScene(currentScene);
			}
			setMouseCursorVisible(currentScene->isCursorVisible());
		}

		if (currentScene == nullptr)
			continue;

		sf::Event event;
		while (active ? pollEvent(event) : waitEvent(event)) {
			const bool accepted = Engine::Instance().getOptions()->globalEventFilter(&event);
			switch (event.type)
			{
			case sf::Event::Resized:
				updateView();
				break;
			case sf::Event::MouseLeft:
			case sf::Event::LostFocus:
				active = false;
				break;
			case sf::Event::GainedFocus:
			case sf::Event::MouseEntered:
				active = true;
				break;
			case sf::Event::Closed:
				CHANGE_SCENE(SceneManager::CLOSING);
				break;
			default:
				break;
			}
			if (accepted)
				continue;
			if (!active)
				break;
			if (currentScene->event(&event))
				break;
		}
		TimersManager::Instance().setPaused(!active);
		if (active) {
			currentScene->process();
			Engine::Instance().getOptions()->globalCallbacks();
			Engine::Instance().windowsManager()->update();

			clear(sf::Color::Black);
			setView(*m_view);
			currentScene->paint(this);

			Engine::Instance().getOptions()->globalDraw(this);

			display();
		}
		else {
			sf::sleep(sf::milliseconds(100));
		}
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
