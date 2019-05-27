#ifndef CAMERA_H
#define CAMERA_H

#include "stdheader.h"

class Cursor;

class Camera
{
public:
    Camera();
    void init(const View& fixed, const Vector2i& levelSize);

    void setCursor(Cursor *const cursor);

    void moveUp(float offset = CAMERA_OFFSET);
    void moveDown(float offset = CAMERA_OFFSET);
    void moveLeft(float offset = CAMERA_OFFSET);
    void moveRight(float offset = CAMERA_OFFSET);

    void moveUpByCell();
    void moveDowByCell();
    void moveLeftByCell();
    void moveRightByCell();

    void setView(RenderWindow *const window);
    void restore(RenderWindow *const window);
    void setMiniMap(View *minimap);

	void zoomIn();
	void zoomOut();
	void resetZoom();

private:
    Cursor *cursor;
    constexpr static float CAMERA_OFFSET = 4.f;

    View *view;
    View fixed;

    Vector2f centerOfTopLeftPoint;
    Vector2f centerOfBottomRightPoint;

    View *minimap;

	float m_zoomRatio;
};

#endif // CAMERA_H
