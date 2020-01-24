#include "camera.h"
#include "gamecursor.h"
#include "settings.h"
#include "globalvariables.h"
#include "Engine/engine.h"

Camera::Camera()
	: view(nullptr)
{

}

void Camera::init()
{
    const FloatRect gameRect = FloatRect(0, 0, Settings::Instance().getResolution().x, Settings::Instance().getResolution().y);
    view = new View(gameRect);
	minimap = new View(gameRect);

	resetZoom();
	view->zoom(Settings::GAME_SCALE);
	view->setCenter(Vector2f(gameRect.width * Settings::GAME_SCALE/2,
							 gameRect.height * Settings::GAME_SCALE/2));

	minimap->setCenter(Vector2f(150 * Settings::Instance().getScaleFactor().x,
								150 * Settings::Instance().getScaleFactor().y));
}

void Camera::destroy()
{
	Engine::Instance().window()->setView(Engine::Instance().window()->getDefaultView());
	delete view;
	delete minimap;
	view = nullptr;
}

void Camera::moveUp(float offset)
{
	view->move(0.f, -offset);
	minimap->move(0.f, -offset);
}

void Camera::moveDown(float offset)
{
	view->move(0.f, offset);
	minimap->move(0.f, offset);
}

void Camera::moveLeft(float offset)
{
	view->move(-offset, 0.f);
	minimap->move(-offset, 0.f);
}

void Camera::moveRight(float offset)
{
	view->move(offset, 0.f);
	minimap->move(offset, 0.f);
}

void Camera::moveUpByCell()
{
	moveUp(GlobalVariables::Instance().tileSize().y);
}

void Camera::moveDownByCell()
{
	moveDown(GlobalVariables::Instance().tileSize().y);
}

void Camera::moveLeftByCell()
{
	moveLeft(GlobalVariables::Instance().tileSize().x);
}

void Camera::moveRightByCell()
{
	moveRight(GlobalVariables::Instance().tileSize().x);
}

View *Camera::getView()
{
	return view;
}

View *Camera::getMiniMapView()
{
	return minimap;
}

void Camera::zoomIn()
{
	if (zoomRatio > MAX_ZOOM)
		return;
	view->zoom(1 - ZOOM_RATIO);
	minimap->zoom(1 - ZOOM_RATIO);
	zoomRatio++;
}

void Camera::zoomOut()
{
	if (zoomRatio <= 0)
		return;
	view->zoom(1 + ZOOM_RATIO);
	minimap->zoom(1 + ZOOM_RATIO);
	zoomRatio--;
}

void Camera::resetZoom()
{
	view->setSize(Settings::Instance().getResolution().x, Settings::Instance().getResolution().y);
//	view->zoom(Settings::GAME_SCALE);

	minimap->setSize(Settings::Instance().getScaleFactor().x, Settings::Instance().getScaleFactor().y);
	minimap->zoom(MINIMAP_ZOOM);
	zoomRatio = 0;
}

int Camera::viewTopCell() const
{
	return static_cast<int>(round((view->getCenter().y - view->getSize().y/2)/GlobalVariables::Instance().tileSize().y));
}

int Camera::viewLeftCell() const
{
	return static_cast<int>(round((view->getCenter().x - view->getSize().x/2)/GlobalVariables::Instance().tileSize().x));
}

int Camera::viewRightCell() const
{
	return static_cast<int>(floor((view->getCenter().x + view->getSize().x/2)/GlobalVariables::Instance().tileSize().x));
}

int Camera::viewBottomCell() const
{
	return static_cast<int>(floor((view->getCenter().y + view->getSize().y/2)/GlobalVariables::Instance().tileSize().y));
}

Vector2i Camera::viewCenter() const
{
	return Vector2i(static_cast<int>(view->getCenter().x/GlobalVariables::Instance().tileSize().x),
					static_cast<int>(view->getCenter().y/GlobalVariables::Instance().tileSize().y));
}

Vector2i Camera::posToCellMap(const Vector2f& pos) const
{
	return Vector2i(floor(pos.x / GlobalVariables::Instance().mapTileSize().x),
					floor(pos.y / GlobalVariables::Instance().mapTileSize().y));
}

Vector2f Camera::cellToPosMap(const Vector2i &cell) const
{
	return Vector2f(cell.x * GlobalVariables::Instance().mapTileSize().x,
					cell.y * GlobalVariables::Instance().mapTileSize().y);
}

Vector2i Camera::posToCell(const Vector2f &pos) const
{
	return Vector2i(floor(pos.x / GlobalVariables::Instance().tileSize().x),
					floor(pos.y / GlobalVariables::Instance().tileSize().y));
}

Vector2f Camera::cellToPos(const Vector2i &cell) const
{
	return Vector2f(cell.x * GlobalVariables::Instance().tileSize().x,
					cell.y * GlobalVariables::Instance().tileSize().y);
}

void Camera::resetView()
{
	resetZoom();
	view->setCenter(Settings::Instance().getResolution().x/2, Settings::Instance().getResolution().y/2);
}
