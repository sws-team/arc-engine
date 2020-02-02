#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include "stdheader.h"
#include "gamedrawable.h"
#include "leveldef.h"

class Level;
class LifeBar;
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
	FloatRect getProgressRect() const;

	void updateCursor();

	void moveCursorLeft();
	void moveCursorRight();

	bool isTowerIconActive(TOWER_TYPES type) const;
	bool isAbilityIconActive(ACTION_STATE type) const;

	int getProgressMax() const;
	void updateWaveText();

	float getTowerUpgradeCost(Tower* tower) const;
	float getTowerSellCost(Tower* tower) const;

	void updateAbilitiesDuration();

	void setLifeMax(int lifeMax);

	void setDrain(bool drain);

	void setCurrentIcon(const ACTION_STATE& state);

	void resetPanelIcon();

private:
	Sprite m_sprite;
	RenderTexture rTexture;
	Text moneyCountText;

	Sprite abilityBombSprite;
	Sprite abilityFreezeBombSprite;
	Sprite abilityVenomSprite;
	Sprite abilityIncreaseTowerDamageSprite;
	Sprite abilityIncreaseTowerAttackSpeedSprite;
	Sprite abilityStopSprite;

	Sprite towerBaseSprite;
	Sprite towerLaserSprite;
	Sprite towerFreezeSprite;
	Sprite towerRocketSprite;
	Sprite towerPowerSprite;
	Sprite towerImprovedSprite;

	Sprite moneyIcon;
	Sprite lifeIcon;

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
	Text abilityStopDurationText;

	Text info;

	Vector2f updatePos();

	int m_bottomValue;
	bool m_isPanelActive;

	LifeBar *progress;
	int m_progressMax;
	int m_lifeMax;

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
		bool isAbilityStopEnabled;
	};
	IconsAvaliable iconsAvaliable;

	void updateEnableTowers();
	void updateEnableAbilities();

	String towerInfo(TOWER_TYPES type, Tower *tower);

	Sprite cursorSprite;
	RectangleShape currentIconRect;

	vector<Sprite*> actionsSprites;
	constexpr static float ICONS_SPACE = 24;
	constexpr static float PROGRESS_OFFSET = 20;
	constexpr static float PROGRESS_WIDTH = 0.3f;

	Text readyText;
	bool waitBlink;
	Timer blinkTimer;
	constexpr static int BLINK_TIME = 500;

	unsigned int currentCursorPos;
	void updateCurrentCursor();
	static const String endline;
	Text waveText;

	LifeBar *life;

	static constexpr int PANEL_OFFSET = 73;

	bool m_drain;
	Timer drainBlinkTimer;
	RectangleShape drainRect;
	bool drainState;
	Sprite miniMapSprite;

	static constexpr unsigned int PANEL_POS_ID = 128;
};

#endif // GAMEPANEL_H
