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

    Vector2i cell() const;
    Vector2f pixelPos() const;

    void setMaxCell(const Vector2i &maxCell);

	void draw(RenderTarget *const target) override;
	void update() override;

	void activateAbility(int x, int y, int hotX, int hotY);
	void deactivate();

	void activateTower(float radius, int type);

	Vector2i abilityCell() const;

	FloatRect getAbilityRect() const;


	void swap();
	bool inPanel() const;

private:
	Vector2i m_cell;
	Vector2i m_maxCell;

	bool canMove(MOVE_DIRECTIONS direction);
	void updateCell();

	enum CURSOR_STATES
	{
		NORMAL,
		ABILITY,
		TOWER
	};
	CURSOR_STATES m_state;

	RectangleShape abilityRect;

	Vector2i abilityHotsPot;

	bool m_inPanel;


	CircleShape towerRadius;
	Sprite towerSprite;
	int towerType;
};

#endif // CURSOR_H
