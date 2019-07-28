#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include "stdheader.h"
#include "gamedrawable.h"
#include "leveldef.h"

class SpaceShip;
class Level;

class Tower;

class GamePanel : public GameDrawable
{
public:
	GamePanel();
	~GamePanel() override;

	void draw(RenderTarget *const target) override;
	void update() override;

	int cellsCount() const;

	constexpr static float ICON_SIZE = 64;

	Tower *selectedTower() const;
	void setSelectedTower(Tower *selectedTower);

	ACTION_STATE getCurrentIcon(const Vector2f& pos) const;
	TOWER_TYPES currentTower(const Vector2f &pos) const;

	float getBottomValue() const;

	void press(const Vector2i &pos);

	void setProgressMax(int progressMax);

private:
	Sprite m_sprite;
	RenderTexture rTexture;
	Text energyCountText;
	Text lifeCountText;

	RectangleShape sellRect;
	RectangleShape upgradeRect;

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
	Sprite towerPowerSprite;
	Sprite towerImprovedSprite;

	Tower *m_selectedTower;


	Vector2f updatePos(const Vector2f& nullPos);

	float m_bottomValue;
	bool m_isPanelActive;

	Sprite spriteReady;

	class LifeBar *progress;
	int m_progressMax;
};

#endif // GAMEPANEL_H
