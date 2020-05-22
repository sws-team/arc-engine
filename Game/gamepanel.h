#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include "stdheader.h"
#include "gameobject.h"
#include "leveldef.h"
#include "Balance/balance.h"

class Level;
class LifeBar;
class Tower;

class GamePanel : public GameDrawable
{
public:
	GamePanel();
	~GamePanel() override;

	void draw(sf::RenderTarget *const target) override;
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
	void setMapSize(const float width, const float height);

	void updateInfo();

	void initMission(unsigned int n);

	sf::FloatRect getTowerRect(const int n) const;
	sf::FloatRect getAbilityRect(const int n) const;
	sf::FloatRect getMoneyRect() const;
	sf::FloatRect getHealthRect() const;
	sf::FloatRect getProgressRect() const;
	sf::FloatRect getTowersRect() const;

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

	bool clickOnMiniMap(const sf::Vector2f& pos);

	std::vector<TowerStats> levelTowerStats(TOWER_TYPES type) const;
private:
	sf::Sprite m_sprite;
	sf::RenderTexture rTexture;
	sf::Text moneyCountText;

	sf::Sprite abilityBombSprite;
	sf::Sprite abilityFreezeBombSprite;
	sf::Sprite abilityAcidSprite;
	sf::Sprite abilityIncreaseTowerDamageSprite;
	sf::Sprite abilityIncreaseTowerAttackSpeedSprite;
	sf::Sprite abilityStopSprite;

	sf::Sprite towerBaseSprite;
	sf::Sprite towerLaserSprite;
	sf::Sprite towerFreezeSprite;
	sf::Sprite towerRocketSprite;
	sf::Sprite towerPowerSprite;
	sf::Sprite towerImprovedSprite;

	sf::Sprite moneyIcon;
	sf::Sprite lifeIcon;
	void initText(sf::Text *text, const unsigned int size,
				  const sf::Color& fillColor,
				  const sf::Color& outlineColor);

	sf::Text towerBaseCostText;
	sf::Text towerFreezeCostText;
	sf::Text towerRocketCostText;
	sf::Text towerLaserCostText;
	sf::Text towerPowerCostText;
	sf::Text towerImprovedCostText;
	void initCostText(sf::Text *costText);

	sf::Text abilityBombDurationText;
	sf::Text abilityFreezeBombDurationText;
	sf::Text abilityAcidDurationText;
	sf::Text abilityIncreaseTowerDamageDurationText;
	sf::Text abilityIncreaseTowerAttackSpeedDurationText;
	sf::Text abilityStopDurationText;
	void initDurationText(sf::Text *costText);

	sf::Text towerBaseLimitText;
	sf::Text towerFreezeLimitText;
	sf::Text towerRocketLimitText;
	sf::Text towerLaserLimitText;
	sf::Text towerPowerLimitText;
	sf::Text towerImprovedLimitText;
	void initLimitText(sf::Text *limitText);

	sf::Text info;

	sf::Vector2f updatePos();

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
		bool isAbilityAcidEnabled;
		bool isAbilityIncreaseTowerDamageEnabled;
		bool isAbilityIncreaseTowerAttackSpeedEnabled;
		bool isAbilityStopEnabled;
	};
	IconsAvaliable iconsAvaliable;

	void updateEnableTowers();
	void updateEnableAbilities();

	sf::String towerInfo(TOWER_TYPES type, Tower *tower);

	sf::Sprite cursorSprite;
	sf::RectangleShape currentIconRect;

	std::vector<sf::Sprite*> actionsSprites;
	constexpr static float ICONS_SPACE = 24;
	constexpr static float PROGRESS_OFFSET = 20;
	constexpr static float PROGRESS_WIDTH = 0.3f;

	sf::Text readyText;
	bool waitBlink;
	Timer blinkTimer;
	constexpr static int BLINK_TIME = 500;

	unsigned int currentCursorPos;
	void updateCurrentCursor();
	sf::Text waveText;

	LifeBar *life;

	static constexpr int PANEL_OFFSET = 73;

	bool m_drain;
	Timer drainBlinkTimer;
	sf::RectangleShape drainRect;
	bool drainState;
	sf::RectangleShape *miniMapRect;

	static constexpr unsigned int PANEL_POS_ID = 128;
	static constexpr float LIFE_BAR_HEIGHT = 16;
	sf::String towerStatsStr(TOWER_TYPES type, Tower *tower) const;

	sf::Vector2f minimapPos;
};

#endif // GAMEPANEL_H
