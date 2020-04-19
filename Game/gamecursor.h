#ifndef CURSOR_H
#define CURSOR_H

#include "stdheader.h"
#include <vector>
#include "gameobject.h"
#include "Game/leveldef.h"

class GameCursor : public GameObject
{
public:
	GameCursor();

	enum MOVE_DIRECTIONS
	{
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_UP,
		MOVE_DOWN
	};

	void setMaxCells(int maxWidth, int maxHeight);
	void setMaxCells(const sf::Vector2i &maxCell);

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	sf::Vector2i cell() const;
	sf::Vector2f pixelPos() const;


	void draw(sf::RenderTarget *const target) override;
	void update() override;

	void activateAbility(int x, int y, int hotX, int hotY);
	void deactivate();

	void activateTower(float radius, TOWER_TYPES type);

	sf::FloatRect getAbilityRect() const;

	void swap();
	bool inPanel() const;

	TOWER_TYPES getTowerType() const;

	void initCell();

	void updatePanel();

	sf::Vector2f windowScreenPos() const;

	static const sf::Color TOWER_AREA_COLOR;
	static const sf::Color INACTIVE_TOWER_AREA_COLOR;

	void setHighlight(bool highlight);

	sf::Vector2i getMaxCell() const;

private:
	sf::Vector2i m_cell;
	sf::Vector2i m_maxCell;

	bool canMove(MOVE_DIRECTIONS direction);
	void updateCell();

	enum CURSOR_STATES
	{
		NORMAL,
		ABILITY,
		TOWER
	};
	CURSOR_STATES m_state;

	sf::RectangleShape abilityRect;

	sf::Vector2i abilityHotsPot;

	bool m_inPanel;


	sf::CircleShape towerRadius;
	sf::RectangleShape towerRect;
	sf::Sprite towerSprite;
	TOWER_TYPES towerType;

	void updateMousePos();
	void checkBorders();
	void moveDownCursor();
	void moveUpCursor();
	void moveLeftCursor();
	void moveRightCursor();

	bool m_highlight;
};

#endif // CURSOR_H
