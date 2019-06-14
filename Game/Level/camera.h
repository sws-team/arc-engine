#ifndef CAMERA_H
#define CAMERA_H

#include "stdheader.h"

class Cursor;

class Camera
{
public:
    Camera();
	void init();

    void moveUp(float offset = CAMERA_OFFSET);
    void moveDown(float offset = CAMERA_OFFSET);
    void moveLeft(float offset = CAMERA_OFFSET);
    void moveRight(float offset = CAMERA_OFFSET);

    void moveUpByCell();
	void moveDownByCell();
    void moveLeftByCell();
    void moveRightByCell();

	View *getView();
	View *getMiniMapView();

	void zoomIn();
	void zoomOut();
	void resetZoom();

private:
    constexpr static float CAMERA_OFFSET = 4.f;

	View *view;
    View *minimap;

	Vector2f m_pos;
	int zoomRatio;
	constexpr static int MAX_ZOOM = 10;
	constexpr static float ZOOM_RATIO = 0.1f;

};

#endif // CAMERA_H
