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

	void centerOnCursor(const Vector2i& cell);
	Vector2i currentCenterCell() const;

	// count of cells on currenr view
	Vector2i currentViewCells() const;
	int topCell() const;

	void detach();
	bool isDetached() const;



	Vector2i posToCell(const Vector2f &pos) const;
	Vector2f cellToPos(const Vector2i &cell) const;

private:
    constexpr static float CAMERA_OFFSET = 4.f;
	constexpr static int MAX_ZOOM = 10;
	constexpr static float ZOOM_RATIO = 0.1f;
	constexpr static float MINIMAP_ZOOM = 0.1f;

	View *view;
    View *minimap;

	int zoomRatio;


	bool m_detached;

};

#endif // CAMERA_H
