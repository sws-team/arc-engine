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

private:	
	constexpr static float PANEL_WIDTH = 1920.f;
	constexpr static float PANEL_HEIGHT = 320.f;
	constexpr static float PANEL_SCALE = 0.2f;

	Level *p_level;
	Sprite m_sprite;
	RenderTexture rTexture;

};

#endif // GAMEPANEL_H
