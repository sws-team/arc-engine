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

	Enemy *spawn(const sf::Vector2f &pos, ENEMY_TYPES type, float protection, int moveDirection);
	Tile getTileByPos(const sf::Vector2f& pos, unsigned int layer = 0);
	Tile getTileByCell(const sf::Vector2i& cell, unsigned int layer = 0) const;

	void chooseByPos(const sf::Vector2f& pos);
	void chooseCurrent();

	int getTileDirectionByCell(const sf::Vector2i &cell) const;

	float getMoneyCount() const;
	float getLifeCount() const;
	Tower *getTowerAtPos(const sf::Vector2f& pos) const;

	bool canAddTower(const sf::Vector2i& cell, bool isEnergy) const;

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

	Animation *addAnimation(const TextureType& texture_id,
					  const sf::Vector2f &pos,
					  const sf::Vector2i &size,
					  int duration,
					  int frameCount,
					  int row);
	void removeAnimation(Animation *animation);

	void clearCursor();
	std::vector<Tower *> getAllTowers() const;
	unsigned int getCurrentWave() const;

#ifndef RELEASE_BUILD
	void test();
	void giveMeMoney();
#endif

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
	void updateRadius();

	void enemyMove(Enemy *enemy);
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

	std::vector<Animation*> effects;

	Shake *shake;

	sf::CircleShape currentTowerRadius;
	sf::RectangleShape currentTowerRect;

	struct ActionPoint
	{
		GameObject *object;
		sf::RectangleShape rect;
		int direction;
	};
	std::vector<ActionPoint> spawnPoints;
	std::vector<ActionPoint> endPoints;
	void updateActionPoint(GameObject *object, const sf::Vector2f &pos);

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
//	void updateStartEndPos(const sf::Vector2f &startPos, const sf::Vector2f &endPos);
	void updateUpgrade();

	std::vector<LevelObject*> objects;
	ShadersFactory *shadersFactory;
	sf::RectangleShape smokeRect;

	//variables for achievements
	std::set<TOWER_TYPES> fullyUpgradedTowers;
	bool abilityActivated;
	bool attackTowerBuilded;
	static constexpr float ECONOMIST_MONEY_VALUE = 15000;

	bool showBuildRects;
	std::vector<sf::RectangleShape> buildCells;
	void updateBuildCells(TOWER_TYPES type);
};

#endif // LEVEL_H
