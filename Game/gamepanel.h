#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include "stdheader.h"
#include "gamedrawable.h"
#include "leveldef.h"

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

	ACTION_STATE getCurrentIcon() const;
	TOWER_TYPES currentTower() const;

	float getBottomValue() const;

	void setProgressMax(int progressMax);

	void init();

	void updateInfo();

	void initMission(unsigned int n);

	FloatRect getTowersRect() const;
	FloatRect getAbilitiesRect() const;
	FloatRect getMoneyRect() const;
	FloatRect getHealthRect() const;
	FloatRect getRemovRect() const;
	FloatRect getUpgradeRect() const;
	FloatRect getProgressRect() const;

	void updateCursor();
	void updateStartEndPos(const Vector2f &startPos, const Vector2f &endPos);

	void moveCursorLeft();
	void moveCursorRight();
	void moveCursorDown();
	void moveCursorUp();

	bool isTowerIconActive(TOWER_TYPES type) const;
	bool isAbilityIconActive(ACTION_STATE type) const;

	int getProgressMax() const;
	void updateWaveText();

	float getTowerUpgradeCost(Tower* tower) const;
	float getTowerSellCost(Tower* tower) const;

	void updateAbilitiesDuration();

private:
	Sprite m_sprite;
	RenderTexture rTexture;
	Text moneyCountText;
	Text lifeCountText;

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
	Sprite healthIcon;

	Text towerBaseCostText;
	Text towerFreezeCostText;
	Text towerRocketCostText;
	Text towerLaserCostText;
	Text towerPowerCostText;
	Text towerImprovedCostText;

	Text abilityBombDurationText;
	Text abilityFreezeBombDurationText;
	Text abilityVenomDurationText;
	Text abilityIncreaseTowerDamageDurationText;
	Text abilityIncreaseTowerAttackSpeedDurationText;
	Text abilityUnknownAttackSpeedDurationText;

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
	static const String endline;
	Text waveText;
};

#endif // GAMEPANEL_H
