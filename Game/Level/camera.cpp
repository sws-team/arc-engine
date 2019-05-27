#include "camera.h"
#include "Game/Level/Cursor/cursor.h"
#include "settings.h"
#include "globalvariables.h"

Camera::Camera()
{

}

void Camera::init(const View &fixed, const Vector2i &levelSize)
{
    const FloatRect gameRect = FloatRect(0, 0, Settings::Instance().getResolution().x, Settings::Instance().getResolution().y);
    view = new View(gameRect);

    this->fixed = fixed;

    view->zoom(Settings::GAME_SCALE);

    centerOfTopLeftPoint = Vector2f(gameRect.width * Settings::GAME_SCALE/2, gameRect.height * Settings::GAME_SCALE/2);
    view->setCenter(centerOfTopLeftPoint);

    centerOfBottomRightPoint = Vector2f(levelSize.x * GlobalVariables::Instance().tileSize().x, levelSize.y * GlobalVariables::Instance().tileSize().y) - centerOfTopLeftPoint;
}

void Camera::setCursor(Cursor * const cursor)
{
    this->cursor = cursor;
}

void Camera::moveUp(float offset)
{
    if (view->getCenter().y > centerOfTopLeftPoint.y)
    {
        view->move(0.f, -offset);
        minimap->move(0.f, -offset);
    }
}

void Camera::moveDown(float offset)
{
    if (view->getCenter().y < centerOfBottomRightPoint.y + GlobalVariables::Instance().tileSize().y)
    {
        view->move(0.f, offset);
        minimap->move(0.f, offset);
    }
}

void Camera::moveLeft(float offset)
{
    if (view->getCenter().x > centerOfTopLeftPoint.x)
    {
        view->move(-offset, 0.f);
        minimap->move(-offset, 0.f);
    }
}

void Camera::moveRight(float offset)
{
    if (view->getCenter().x < centerOfBottomRightPoint.x)
    {
        view->move(offset, 0.f);
        minimap->move(offset, 0.f);
    }
}

void Camera::moveUpByCell()
{
    if (cursor->pos().y * GlobalVariables::Instance().tileSize().y < view->getCenter().y)
        moveUp(GlobalVariables::Instance().tileSize().y);
}

void Camera::moveDowByCell()
{
    if (cursor->pos().y * GlobalVariables::Instance().tileSize().y > view->getCenter().y)
        moveDown(GlobalVariables::Instance().tileSize().y);
}

void Camera::moveLeftByCell()
{
    if (cursor->pos().x * GlobalVariables::Instance().tileSize().x < view->getCenter().x)
        moveLeft(GlobalVariables::Instance().tileSize().x);
}

void Camera::moveRightByCell()
{
    if (cursor->pos().x * GlobalVariables::Instance().tileSize().x > view->getCenter().x)
        moveRight(GlobalVariables::Instance().tileSize().x);
}

void Camera::setView(RenderWindow * const window)
{
    window->setView(*view);
}

void Camera::restore(RenderWindow * const window)
{
    window->setView(fixed);
}

void Camera::setMiniMap(View *minimap)
{
	this->minimap = minimap;
}

void Camera::zoomIn()
{

}

void Camera::zoomOut()
{

}

void Camera::resetZoom()
{

}
