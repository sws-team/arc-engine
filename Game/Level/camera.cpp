#include "camera.h"
#include "cursor.h"
#include "settings.h"
#include "globalvariables.h"
#include "Engine/engine.h"

Camera::Camera()
{

}

void Camera::init()
{
    const FloatRect gameRect = FloatRect(0, 0, Settings::Instance().getResolution().x, Settings::Instance().getResolution().y);
    view = new View(gameRect);
	minimap = new View(gameRect);

	resetZoom();

	view->setCenter(Vector2f(gameRect.width * Settings::GAME_SCALE/2, gameRect.height * Settings::GAME_SCALE/2));
	minimap->setCenter(Vector2f(150, 150));
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
//	if (Engine::Instance().cursor()->pos().y * GlobalVariables::CELL_SIZE < view->getCenter().y)
		moveUp(GlobalVariables::Instance().tileSize().y);
}

void Camera::moveDownByCell()
{
//	if (Engine::Instance().cursor()->pos().y * GlobalVariables::CELL_SIZE > view->getCenter().y)
		moveDown(GlobalVariables::Instance().tileSize().y);
}

void Camera::moveLeftByCell()
{
//	if (Engine::Instance().cursor()->pos().x * GlobalVariables::CELL_SIZE < view->getCenter().x)
		moveLeft(GlobalVariables::Instance().tileSize().x);
}

void Camera::moveRightByCell()
{
//	if (Engine::Instance().cursor()->pos().x * GlobalVariables::CELL_SIZE > view->getCenter().x)
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
//	if (zoomRatio > MAX_ZOOM)
//		return;
	view->zoom(1 - ZOOM_RATIO);
	minimap->zoom(1 - ZOOM_RATIO);
	zoomRatio++;
}

void Camera::zoomOut()
{
//	if (zoomRatio < -MAX_ZOOM)
//		return;
	view->zoom(1 + ZOOM_RATIO);
	minimap->zoom(1 + ZOOM_RATIO);
	zoomRatio--;
}

void Camera::resetZoom()
{
	view->setSize(Settings::Instance().getResolution().x, Settings::Instance().getResolution().y);
	view->zoom(Settings::GAME_SCALE);

	minimap->setSize(300, 300);
	minimap->zoom(0.1f);
	zoomRatio = 0;
}
