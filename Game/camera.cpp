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
	,zoomRatio(0)
	,zoomFactor(1)
{

}

void Camera::init()
{
	const sf::FloatRect gameRect = sf::FloatRect(0, 0,
										 Engine::Instance().settingsManager()->getResolution().x,
										 Engine::Instance().settingsManager()->getResolution().y);

	view = new sf::View(gameRect);
	view->setViewport(Engine::Instance().window()->view()->getViewport());
}

void Camera::destroy()
{
	Engine::Instance().window()->setView(*Engine::Instance().window()->view());
	delete view;
	view = nullptr;
}

void Camera::moveUp(float offset)
{
	view->move(0.f, -offset);
}

void Camera::moveDown(float offset)
{
	view->move(0.f, offset);
}

void Camera::moveLeft(float offset)
{
	view->move(-offset, 0.f);
}

void Camera::moveRight(float offset)
{
	view->move(offset, 0.f);
}

void Camera::moveUpByCell()
{
	moveUp(Engine::Instance().options<GameOptions>()->tileSize().y);
	checkTopLeft();
}

void Camera::moveDownByCell()
{
	moveDown(Engine::Instance().options<GameOptions>()->tileSize().y);
	checkBottomRight();
}

void Camera::moveLeftByCell()
{
	moveLeft(Engine::Instance().options<GameOptions>()->tileSize().x);
	checkTopLeft();
}

void Camera::moveRightByCell()
{
	moveRight(Engine::Instance().options<GameOptions>()->tileSize().x);
	checkBottomRight();
}

sf::View *Camera::getView()
{
	return view;
}

void Camera::zoomIn()
{
	if (zoomRatio > MAX_ZOOM)
		return;
	view->zoom(ZOOM_STEP);
	zoomFactor *= ZOOM_STEP;
	zoomRatio++;
}

void Camera::zoomOut()
{
	if (zoomRatio <= 1)
		return;
	view->zoom(1/ZOOM_STEP);
	zoomFactor /= ZOOM_STEP;
	zoomRatio--;
	fitScreen();
}

void Camera::resetZoom()
{
	view->setSize(viewSize);
	view->setViewport(Engine::Instance().window()->view()->getViewport());
	zoomRatio = 0;
	zoomFactor = 1.f;
}

float Camera::viewTop() const
{
	return view->getCenter().y - view->getSize().y/2;
}

float Camera::viewLeft() const
{
	return view->getCenter().x - view->getSize().x/2;
}

float Camera::viewRight() const
{
	return view->getCenter().x + view->getSize().x/2;
}

float Camera::viewBottom() const
{
	return view->getCenter().y + view->getSize().y/2;
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
	view->setCenter(viewSize.x/2, viewSize.y/2);
}

void Camera::fitScreen()
{
	checkTopLeft();
	checkBottomRight(true);
}

void Camera::checkTopLeft()
{
	const sf::Vector2f topLeft = sf::Vector2f(view->getCenter().x - view->getSize().x/2,
											  view->getCenter().y - view->getSize().y/2);
	const sf::Vector2i topleftCell = posToCell(topLeft);

	if (topleftCell.x < 0)
		view->setCenter(view->getSize().x/2, view->getCenter().y);

	if (topleftCell.y < 0)
		view->setCenter(view->getCenter().x, view->getSize().y/2);
}

void Camera::checkBottomRight(bool zoom)
{
	int extraCells = Engine::Instance().options<GameOptions>()->panel()->cellsCount();
	const sf::Vector2i max = Engine::Instance().options<GameOptions>()->cursor()->getMaxCell();

	const sf::Vector2f bottomRight = sf::Vector2f(view->getCenter().x + view->getSize().x/2,
												  view->getCenter().y + view->getSize().y/2);

	const sf::Vector2i bottomRightCell = posToCell(bottomRight);

	extraCells--;
	if (zoom) extraCells--;

	if (bottomRightCell.x >= max.x)
		view->setCenter(max.x * Engine::Instance().options<GameOptions>()->tileSize().x
						- view->getSize().x/2,
						view->getCenter().y);

	if (bottomRightCell.y >= max.y + extraCells)
	{
		view->setCenter(view->getCenter().x,
						max.y * Engine::Instance().options<GameOptions>()->tileSize().y
						- view->getSize().y/2
						+ Engine::Instance().options<GameOptions>()->panel()->getPanelHeight() * zoomFactor
						);
	}
}

void Camera::setCenter(const sf::Vector2f &pos)
{
	view->setCenter(pos);
	fitScreen();
}

void Camera::updateScaleByMap(const sf::Vector2f &size)
{
	viewSize = size;
	resetView();
	for (int i = 0; i < GameOptions::GAME_SCALE; ++i)
		zoomIn();
}
