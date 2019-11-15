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
	constexpr static float MINIMAP_SCALE = 0.3f;

	Tower *selectedTower() const;
	void setSelectedTower(Tower *selectedTower);

	ACTION_STATE getCurrentIcon(const Vector2f& pos) const;
	TOWER_TYPES currentTower(const Vector2f &pos) const;

	float getBottomValue() const;

	void press(const Vector2i &pos);

	void setProgressMax(int progressMax);

	void init();

	void updateInfo();

	void initMission(unsigned int n);

	FloatRect getTowersRect() const;
	FloatRect getAbilitiesRect() const;
	FloatRect getMoneyRect() const;
	FloatRect getHealthRect() const;
	FloatRect getEnergyRect() const;
	FloatRect getRemovRect() const;
	FloatRect getUpgradeRect() const;

	void updateCursor();

private:
	Sprite m_sprite;
	RenderTexture rTexture;
	Text moneyCountText;
	Text lifeCountText;
	Text energyCountText;

	Sprite sellSprite;
	Sprite upgradeSprite;

	Sprite abilityBombSprite;
	Sprite abilityFreezeBombSprite;
	Sprite abilityVenomSprite;
	Sprite abilityIncreaseTowerDamageSprite;
	Sprite abilityIncreaseTowerAttackSpeedSprite;
	Sprite abilityUnknownSprite;

	Sprite towerBaseSprite;
	Sprite towerLaserSprite;
	Sprite towerFreezeSprite;
	Sprite towerRocketSprite;
	Sprite towerPowerSprite;
	Sprite towerImprovedSprite;

	Tower *m_selectedTower;
	Text info;


	Vector2f updatePos(const Vector2f& nullPos);

	float m_bottomValue;
	bool m_isPanelActive;

	Sprite spriteReady;

	class LifeBar *progress;
	int m_progressMax;

	struct IconsAvaliable
	{
		bool isRocketEnabled;
		bool isFreezeEnabled;
		bool isLaserEnabled;
		bool isImprovedEnabled;

		bool isAbilityBombEnabled;
		bool isAbilityFreezeBombEnabled;
		bool isAbilityVenomEnabled;
		bool isAbilityIncreaseTowerDamageEnabled;
		bool isAbilityIncreaseTowerAttackSpeedEnabled;
		bool isAbilityUnknownEnabled;
	};
	IconsAvaliable iconsAvaliable;

	void updateEnableTowers();
	void updateEnableAbilities();

	String towerInfo(TOWER_TYPES type, Tower *tower);

	Sprite cursorSprite;
	bool isCursorVisible;

	vector<Sprite*> actionsSprites;
	constexpr static float ICONS_SPACE = 24;
	void updateCurrentTower();
};

#endif // GAMEPANEL_H
