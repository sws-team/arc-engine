#ifndef LEVEL_H
#define LEVEL_H

#include "stdheader.h"
#include "gameobject.h"
#include "Game/leveldef.h"

class LevelObject;
class Enemy;
class GamePanel;
struct Tile;
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

	void draw(sf::RenderTarget * const target) override;
	void update() override;

	void startMission(const unsigned int n);
	void clear();

	void drawLevel(sf::RenderTarget * const target);

	void spawn(const sf::Vector2f &pos, ENEMY_TYPES type, float protection, int moveDirection);
	Tile getTileByPos(const sf::Vector2f& pos, unsigned int layer = 0);
	Tile getTileByCell(const sf::Vector2i& cell, unsigned int layer = 0) const;

	void chooseByPos(const sf::Vector2f& pos);
	void chooseCurrent();

	int getTileDirectionByCell(const sf::Vector2i &cell) const;

	float getMoneyCount() const;
	float getLifeCount() const;
	Tower *getTowerAtPos(const sf::Vector2f& pos) const;

	bool canAddTower(const sf::Vector2i& cell, TOWER_TYPES towerType) const;

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

	std::vector<Enemy *> getAllEnemies() const;

	void addAnimation(const TextureType& texture_id,
					  const sf::Vector2f &pos,
					  const sf::Vector2i &size,
					  int duration,
					  int frameCount,
					  int row);
	void removeAnimation(Animation *animation);
	unsigned int getPowerTowersCount() const;

	void clearCursor();
	std::vector<Tower *> getAllTowers() const;
	sf::FloatRect getEndRect() const;

	unsigned int getCurrentWave() const;

	//delete
	void test();

	Abilities *getAbilities();

	void checkAlive();

	void deleteTower(Tower *tower);

	void drainMoney(float m);

	float getStartLife() const;

	void updateCurrentTower();

	Tower *selectedTower() const;

	void activateBombAbility();
	void activateFreezeBombAbility();
	void activateVenomAbility();
	void activateIncreaseTowerDamageAbility();
	void activateIncreaseTowerAttackSpeedAbility();
	void activateStopAbility();
	void upgradeTower(Tower *tower);
	void sellTower(Tower *tower);

	bool isFinalWave() const;

private:
	void choose(const sf::Vector2i& cell, bool inPanel);
	void calculateCollisions();
	void checkDeadZone();
	void checkEnd();
	void checkRespawn();
	void checkEnemyMove();

	sf::RectangleShape deadZone;

	class Map *gameMap;
	std::vector<Enemy*> enemies;


	ACTION_STATE m_actionState;

	//player data
	float life;
	float money;

	void hitPlayer(float damage);

	std::vector<Tower*> towers;

	std::vector<Wave> waves;
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

	std::vector<Animation*> effects;


	Shake *shake;


	sf::CircleShape currentTowerRadius;
	sf::RectangleShape currentTowerRect;
	void updateRadius();
	unsigned int m_powerTowersCount;

	sf::RectangleShape spawnRect;
	sf::RectangleShape endRect;
	constexpr static float DEAD_ZONE_SIZE = 300;
	void showAnimations();

	Abilities *abilities;
	MapExplosion *mapExplosion;
	Smoke *smoke;
	MoneyDrain *moneyDrain;
	TowersRegress *towersRegress;


	sf::Sprite sellSprite;
	sf::Sprite upgradeSprite;
	sf::Text sellCostText;
	sf::Text upgradeCostText;
	ACTION_STATE isFieldButtons(const sf::Vector2f& pos) const;
	Tower *m_selectedTower;
	void setSelectedTower(Tower* tower);

	GameObject *startObject;
	GameObject *endObject;

	void updateStartEndPos(const sf::Vector2f &startPos, const sf::Vector2f &endPos);
	void updateUpgrade();

	std::vector<LevelObject*> objects;
	ShadersFactory *shadersFactory;

	//variables for achievements
	std::set<TOWER_TYPES> fullyUpgradedTowers;
	bool abilityActivated;
	bool attackTowerBuilded;
	static constexpr float ECONOMIST_MONEY_VALUE = 30000;

	sf::RectangleShape smokeRect;
};

#endif // LEVEL_H
