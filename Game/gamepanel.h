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

	void updatePanel();

	int cellsCount() const;

	constexpr static float ICON_SIZE = 64;
	constexpr static float MINIMAP_SCALE = 0.3f;

	Tower *selectedTower() const;
	void setSelectedTower(Tower *selectedTower);

//	ACTION_STATE getCurrentIcon(const Vector2f& pos) const;
//	TOWER_TYPES currentTower(const Vector2f &pos) const;
	ACTION_STATE getCurrentIcon() const;
	TOWER_TYPES currentTower() const;

	float getBottomValue() const;

//	void press(const Vector2i &pos);

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
	FloatRect getProgressRect() const;

	void updateCursor();
	void updateStartEndPos(const Vector2f &startPos, const Vector2f &endPos);

	void moveCursorLeft();
	void moveCursorRight();
	void moveCursorDown();
	void moveCursorUp();

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

	Sprite moneyIcon;
	Sprite energyIcon;
	Sprite healthIcon;

	Tower *m_selectedTower;
	Text info;


	Vector2f updatePos(const Vector2f& nullPos);

	float m_bottomValue;
	bool m_isPanelActive;

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

	vector<Sprite*> actionsSprites;
	constexpr static float ICONS_SPACE = 24;
	constexpr static float PROGRESS_OFFSET = 20;
	constexpr static float PROGRESS_WIDTH = 0.3f;
	void updateCurrentTower();

	Sprite startSprite;
	Sprite endSprite;
	Text readyText;
	bool waitBlink;
	Timer blinkTimer;
	constexpr static int BLINK_TIME = 500;

	unsigned int currentCursorPos;
	void updateCurrentCursor();
};

#endif // GAMEPANEL_H
