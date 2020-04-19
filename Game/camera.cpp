#include "camera.h"
#include "gamecursor.h"
#include "engine.h"
#include "gamepanel.h"
#include "managers.h"
#include "enginedef.h"
#include "gameoptions.h"
#include "mainwindow.h"

Camera::Camera()
	: view(nullptr)
{

}

void Camera::init()
{
	const sf::FloatRect gameRect = sf::FloatRect(0, 0,
										 Engine::Instance().settingsManager()->getResolution().x,
										 Engine::Instance().settingsManager()->getResolution().y);

//	view = Engine::Instance().window()->view();
	view = new sf::View(gameRect);
	view->setViewport(Engine::Instance().window()->view()->getViewport());
	minimap = new sf::View(gameRect);

	resetZoom();
	view->zoom(GameOptions::GAME_SCALE);
	view->setCenter(sf::Vector2f(gameRect.width * GameOptions::GAME_SCALE/2,
							 gameRect.height * GameOptions::GAME_SCALE/2));

	minimap->setCenter(sf::Vector2f(150 * Engine::Instance().settingsManager()->getScaleFactor().x,
								150 * Engine::Instance().settingsManager()->getScaleFactor().y));
}

void Camera::destroy()
{
	Engine::Instance().window()->setView(*Engine::Instance().window()->view());
	delete view;
	delete minimap;
	view = nullptr;
}

void Camera::moveUp(float offset)
{
	view->move(0.f, -offset);
	minimap->move(0.f, -offset);

	checkBorders();
}

void Camera::moveDown(float offset)
{
	view->move(0.f, offset);
	minimap->move(0.f, offset);

	checkBorders();
}

void Camera::moveLeft(float offset)
{
	view->move(-offset, 0.f);
	minimap->move(-offset, 0.f);

	checkBorders();
}

void Camera::moveRight(float offset)
{
	view->move(offset, 0.f);
	minimap->move(offset, 0.f);

	checkBorders();
}

void Camera::moveUpByCell()
{
	moveUp(Engine::Instance().options<GameOptions>()->tileSize().y);
}

void Camera::moveDownByCell()
{
	moveDown(Engine::Instance().options<GameOptions>()->tileSize().y);
}

void Camera::moveLeftByCell()
{
	moveLeft(Engine::Instance().options<GameOptions>()->tileSize().x);
}

void Camera::moveRightByCell()
{
	moveRight(Engine::Instance().options<GameOptions>()->tileSize().x);
}

sf::View *Camera::getView()
{
	return view;
}

sf::View *Camera::getMiniMapView()
{
	return minimap;
}

void Camera::zoomIn()
{
	if (zoomRatio > MAX_ZOOM)
		return;
	view->zoom(ZOOM_STEP);
	zoomRatio++;
}

void Camera::zoomOut()
{
	if (zoomRatio <= 0)
		return;
	view->zoom(1/ZOOM_STEP);
	zoomRatio--;

	checkBorders();
}

void Camera::resetZoom()
{
	view->setSize(Engine::Instance().settingsManager()->getResolution().x,
				  Engine::Instance().settingsManager()->getResolution().y);
	view->setViewport(Engine::Instance().window()->view()->getViewport());
	minimap->setSize(Engine::Instance().settingsManager()->getScaleFactor().x,
					 Engine::Instance().settingsManager()->getScaleFactor().y);
	minimap->zoom(MINIMAP_ZOOM);
	zoomRatio = 0;
}

int Camera::viewTopCell() const
{
	return static_cast<int>(round((view->getCenter().y - view->getSize().y/2)/Engine::Instance().options<GameOptions>()->tileSize().y));
}

int Camera::viewLeftCell() const
{
	return static_cast<int>(round((view->getCenter().x - view->getSize().x/2)/Engine::Instance().options<GameOptions>()->tileSize().x));
}

int Camera::viewRightCell() const
{
	return static_cast<int>(floor((view->getCenter().x + view->getSize().x/2)/Engine::Instance().options<GameOptions>()->tileSize().x));
}

int Camera::viewBottomCell() const
{
	return static_cast<int>(floor((view->getCenter().y + view->getSize().y/2)/Engine::Instance().options<GameOptions>()->tileSize().y));
}

sf::Vector2i Camera::viewCenter() const
{
	return sf::Vector2i(static_cast<int>(view->getCenter().x/Engine::Instance().options<GameOptions>()->tileSize().x),
					static_cast<int>(view->getCenter().y/Engine::Instance().options<GameOptions>()->tileSize().y));
}

sf::Vector2i Camera::posToCellMap(const sf::Vector2f& pos) const
{
	return sf::Vector2i(floor(pos.x / Engine::Instance().options<GameOptions>()->mapTileSize().x),
					floor(pos.y / Engine::Instance().options<GameOptions>()->mapTileSize().y));
}

sf::Vector2f Camera::cellToPosMap(const sf::Vector2i &cell) const
{
	return sf::Vector2f(cell.x * Engine::Instance().options<GameOptions>()->mapTileSize().x,
					cell.y * Engine::Instance().options<GameOptions>()->mapTileSize().y);
}

sf::Vector2i Camera::posToCell(const sf::Vector2f &pos) const
{
	return sf::Vector2i(floor(pos.x / Engine::Instance().options<GameOptions>()->tileSize().x),
					floor(pos.y / Engine::Instance().options<GameOptions>()->tileSize().y));
}

sf::Vector2f Camera::cellToPos(const sf::Vector2i &cell) const
{
	return sf::Vector2f(cell.x * Engine::Instance().options<GameOptions>()->tileSize().x,
					cell.y * Engine::Instance().options<GameOptions>()->tileSize().y);
}

void Camera::resetView()
{
	resetZoom();
	view->setCenter(Engine::Instance().settingsManager()->getResolution().x/2,
					Engine::Instance().settingsManager()->getResolution().y/2);
}

void Camera::checkBorders()
{
	const sf::Vector2f topLeft = sf::Vector2f(view->getCenter().x - view->getSize().x/2,
								view->getCenter().y - view->getSize().y/2);
	if (topLeft.x < 0)
		view->setCenter(view->getSize().x/2, view->getCenter().y);

	if (topLeft.y < 0)
		view->setCenter(view->getCenter().x, view->getSize().y/2);

	const sf::Vector2f bottomRight = sf::Vector2f(view->getCenter().x + view->getSize().x/2,
										  view->getCenter().y + view->getSize().y/2);

	const float maxX = Engine::Instance().options<GameOptions>()->cursor()->getMaxCell().x * Engine::Instance().options<GameOptions>()->tileSize().x;
	if (bottomRight.x > maxX)
		view->setCenter(maxX - view->getSize().x/2, view->getCenter().y);

	const float maxY = (Engine::Instance().options<GameOptions>()->cursor()->getMaxCell().y + Engine::Instance().options<GameOptions>()->panel()->cellsCount())
			* Engine::Instance().options<GameOptions>()->tileSize().y;
	if (bottomRight.y > maxY)
		view->setCenter(view->getCenter().x, maxY - Engine::Instance().options<GameOptions>()->mapTileSize().y - view->getSize().y/2);	
}

void Camera::setCenter(const sf::Vector2f &pos)
{
	view->setCenter(pos);
	checkBorders();
}
