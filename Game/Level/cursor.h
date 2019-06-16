#ifndef CURSOR_H
#define CURSOR_H

#include "stdheader.h"
#include <vector>
#include "Game/gameobject.h"

class Cursor : public GameObject
{
public:
	Cursor();

	enum MOVE_DIRECTIONS
	{
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_UP,
		MOVE_DOWN
	};

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

    Vector2i pos() const;
    Vector2f pixelPos() const;

    void setMaxCell(const Vector2i &maxCell);

private:
	Vector2i m_cell;
	Vector2i m_maxCell;

	bool canMove(MOVE_DIRECTIONS direction);
	void updateCell();
};

#endif // CURSOR_H
