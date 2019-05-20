#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include "stdheader.h"
#include "gamedrawable.h"
#include "SpaceShip/lifebar.h"

class SpaceShip;
class Level;

class GamePanel : public GameDrawable
{
public:
	GamePanel(Level *level);
	~GamePanel() override;

	void draw(RenderWindow *const window) override;
	void update() override;

	void setLeftBorder(float left);

	float height() const;

	void setTop(float y);

private:

	struct Panel
	{
		Sprite sprite;
		LifeBar health;
		LifeBar shield;
		LifeBar energy;
		Text ammoText;
		Text scoreText;
	};
	vector<Panel> panels;

	float m_height;

	float m_top;
	float m_left;

	constexpr static int CELL_SIZE = 48;
	Vector2f m_cellSize;

	constexpr static float HORIZONTAL_OFFSET = 16.f;
	float m_panelHorizontalOffset;
	constexpr static float VERTICAL_OFFSET = 8.f;
	float m_panelVerticalOffset;

	constexpr static int LINE_WIDTH = 152.f;
	constexpr static int LINE_HEIGHT = 24.f;
	constexpr static int AMMO_SIZE = 40.f;

	Level *p_level;
};

#endif // GAMEPANEL_H
