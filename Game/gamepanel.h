#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include "stdheader.h"
#include "gamedrawable.h"

class SpaceShip;
class Level;

class GamePanel : public GameDrawable
{
public:
	GamePanel();
	~GamePanel() override;

	void draw(RenderTarget *const target) override;
	void update() override;

	int cellsCount() const;

	constexpr static float ICON_SIZE = 96;
	constexpr static float TOP_MARGIN = ICON_SIZE/2;
	constexpr static float ICONS_START = 442;

	constexpr static int PANEL_CELLS_COUNT = 10;
private:
	constexpr static float PANEL_WIDTH = 1920.f;
//	constexpr static float BLOCK_WIDTH = PANEL_WIDTH / 3;
	const static float PANEL_HEIGHT;
	constexpr static float PANEL_SCALE = 0.2f;


	constexpr static int BLOCK_TOWERS = 1;
	constexpr static int BLOCK_ABILITIES = 2;
	constexpr static int BLOCK_MINIMAP = 3;

	Sprite m_sprite;
	RenderTexture rTexture;

	Sprite abilityBombSprite;
	Sprite abilityFreezeBombSprite;
	Sprite abilityCarpetBombingSprite;
	Sprite abilityIncreaseTowerDamageSprite;
	Sprite abilityIncreaseTowerAttackSpeedSprite;
	Sprite abilityTimeStopSprite;

	Sprite towerBaseSprite;
	Sprite towerLaserSprite;
	Sprite towerFreezeSprite;
	Sprite towerRocketSprite;
	Sprite towerSplashSprite;
	Sprite towerImprovedSprite;
};

#endif // GAMEPANEL_H
