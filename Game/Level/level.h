#ifndef LEVEL_H
#define LEVEL_H

#include "stdheader.h"
#include "Game/gamedrawable.h"
#include "Game/leveldef.h"
#include "ResourcesManager/textures_types.h"

class LevelObject;
class Enemy;
class GamePanel;
class Tile;
class Tower;
class Animation;
class Abilities;
class Shake;
class MapExplosion;
class Smoke;
class MoneyDrain;
class TowersRegress;
class ShadersFactory;

class Level : public GameDrawable
{
public:
	Level();
	~Level() override;

	void draw(RenderTarget * const target) override;
	void update() override;

	void startMission(const unsigned int n);
	void clear();

	void drawLevel(RenderTarget * const target);

	void spawn(const Vector2f &pos, ENEMY_TYPES type, float protection, int moveDirection);
	Tile getTileByPos(const Vector2f& pos, unsigned int layer = 0);
	Tile getTileByCell(const Vector2i& cell, unsigned int layer = 0) const;

	void chooseByPos(const Vector2f& pos);
	void chooseCurrent();

	int getTileDirectionByCell(const Vector2i &cell) const;

	float getMoneyCount() const;
	float getLifeCount() const;
	Tower *getTowerAtPos(const Vector2f& pos) const;

	bool canAddTower(const Vector2i& cell, TOWER_TYPES towerType) const;

	void highlightPowerTowersRadius(bool active);

	enum LEVEL_STATE
	{
		WAIT_READY,
		PLAYING,
		WIN,
		LOSE
	};
	LEVEL_STATE getState() const;

	void ready();

	int currentProgress() const;

	vector<Enemy *> getAllEnemies() const;

	void addAnimation(const RESOURCES::TEXTURE_TYPE& texture_id,
					  const Vector2f &pos,
					  const Vector2i &size,
					  int duration,
					  int frameCount,
					  int row);
	void removeAnimation(Animation *animation);
	unsigned int getPowerTowersCount() const;

	void clearCursor();
	vector<Tower *> getAllTowers() const;
	FloatRect getEndRect() const;

	unsigned int getCurrentWave() const;

	//delete
	void test();
	void plus();
	void minus();

	Abilities *getAbilities();

	void checkAlive();

	void deleteTower(Tower *tower);

	void drainMoney(float m);

	float getStartLife() const;

	void updateCurrentTower();

	Tower *selectedTower() const;

	void blinkStartEnd(bool state);

	void activateBombAbility();
	void activateFreezeBombAbility();
	void activateVenomAbility();
	void activateIncreaseTowerDamageAbility();
	void activateIncreaseTowerAttackSpeedAbility();
	void activateStopAbility();
	void upgradeTower(Tower *tower);
	void sellTower(Tower *tower);

private:
	void choose(const Vector2i& cell, bool inPanel);
	void calculateCollisions();
	void checkDeadZone();
	void checkEnd();
	void checkRespawn();
	void checkEnemyMove();

	RectangleShape deadZone;

	class Map *gameMap;
	vector<Enemy*> enemies;


	ACTION_STATE m_actionState;

	//player data
	float life;
	float money;

	void hitPlayer(float damage);

	vector<Tower*> towers;

	vector<Wave> waves;
	unsigned int currentWave;

	Timer spawnTimer;
	ENEMY_TYPES bossType;

	LEVEL_STATE m_state;
	void changeState(LEVEL_STATE state);

	static const float FREEZE_ABILITY_K;
	static const int FREEZE_ABILITY_DURATION;

	static const int INCREASE_DAMAGE_ABILITY_VALUE;
	static const int INCREASE_DAMAGE_ABILITY_DURATION;

	static const int INCREASE_ATTACK_SPEED_ABILITY_VALUE;
	static const int INCREASE_ATTACK_SPEED_ABILITY_DURATION;

	Timer timerRegenEnergy;

	vector<Animation*> effects;


	Shake *shake;


	CircleShape currentTowerRadius;
	RectangleShape currentTowerRect;
	void updateRadius();
	unsigned int m_powerTowersCount;

	RectangleShape spawnRect;
	RectangleShape endRect;
	constexpr static float DEAD_ZONE_SIZE = 300;
	void showAnimations();

	Abilities *abilities;
	MapExplosion *mapExplosion;
	Smoke *smoke;
	MoneyDrain *moneyDrain;
	TowersRegress *towersRegress;


	Sprite sellSprite;
	Sprite upgradeSprite;
	Text sellCostText;
	Text upgradeCostText;
	ACTION_STATE isFieldButtons(const Vector2f& pos) const;
	Tower *m_selectedTower;
	void setSelectedTower(Tower* tower);

	Sprite startSprite;
	Sprite endSprite;

	void updateStartEndPos(const Vector2f &startPos, const Vector2f &endPos);
	void updateUpgrade();

	vector<LevelObject*> objects;
	ShadersFactory *shadersFactory;
};

#endif // LEVEL_H
