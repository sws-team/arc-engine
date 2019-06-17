#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include "stdheader.h"
#include "gamedrawable.h"
#include "lifebar.h"

class SpaceShip;
class Level;

class GamePanel : public GameDrawable
{
public:
	GamePanel(Level *level);
	~GamePanel() override;

	void draw(RenderTarget *const target) override;
	void update() override;

	constexpr static int PANEL_CELLS_COUNT = 10;
private:	
	constexpr static float PANEL_WIDTH = 1920.f;
	const static float PANEL_HEIGHT;
	constexpr static float PANEL_SCALE = 0.2f;

	Level *p_level;
	Sprite m_sprite;
	RenderTexture rTexture;

};

#endif // GAMEPANEL_H
