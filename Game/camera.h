#ifndef CAMERA_H
#define CAMERA_H

#include "stdheader.h"

class GameCursor;

class Camera
{
public:
	Camera();
	void init();
	void destroy();

    void moveUp(float offset = CAMERA_OFFSET);
    void moveDown(float offset = CAMERA_OFFSET);
    void moveLeft(float offset = CAMERA_OFFSET);
    void moveRight(float offset = CAMERA_OFFSET);

    void moveUpByCell();
	void moveDownByCell();
    void moveLeftByCell();
    void moveRightByCell();

	sf::View *getView();
	sf::View *getMiniMapView();

	void zoomIn();
	void zoomOut();
	void resetZoom();

	int viewTopCell() const;
	int viewLeftCell() const;
	int viewRightCell() const;
	int viewBottomCell() const;
	sf::Vector2i viewCenter() const;

	sf::Vector2i posToCellMap(const sf::Vector2f &pos) const;
	sf::Vector2f cellToPosMap(const sf::Vector2i &cell) const;

	sf::Vector2i posToCell(const sf::Vector2f &pos) const;
	sf::Vector2f cellToPos(const sf::Vector2i &cell) const;

	void resetView();

	void checkBorders();

	void setCenter(const sf::Vector2f& pos);

private:
    constexpr static float CAMERA_OFFSET = 4.f;
	constexpr static int MAX_ZOOM = 20;
	constexpr static float MINIMAP_ZOOM = 0.1f;
	constexpr static float ZOOM_STEP = 0.95f;

	sf::View *view;
	sf::View *minimap;

	int zoomRatio;
};

#endif // CAMERA_H
