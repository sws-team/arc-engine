#ifndef LEVEL_H
#define LEVEL_H

#include "stdheader.h"
#include "Game/gamedrawable.h"
#include "Game/leveldef.h"
#include "ResourcesManager/textures_types.h"

class Enemy;
class GamePanel;
class Tile;
class Tower;
class Animation;

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

	void spawn(ENEMY_TYPES type);
	Tile getTileByPos(const Vector2f& pos, unsigned int layer = 0);
	Tile getTileByCell(const Vector2i& cell, unsigned int layer = 0) const;

	void chooseByPos(const Vector2f& pos);
	void chooseCurrent();

	int getTileDirectionByCell(const Vector2i &cell) const;

	float getMoneyCount() const;
	float getLifeCount() const;
	float getEnergyCount() const;
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

	static const int VENOM_ABILITY_COST;
	static const int BOMB_ABILITY_COST;
	static const int FREEZE_BOMB_ABILITY_COST;
	static const int INC_TOWER_AS_ABILITY_COST;
	static const int INC_TOWER_DMG_ABILITY_COST;

	void addAnimation(const RESOURCES::TEXTURE_TYPE& texture_id,
					  const Vector2f &pos,
					  const Vector2i &size,
					  int duration,
					  int frameCount,
					  int row);
	void removeAnimation(Animation *animation);
	unsigned int getPowerTowersCount() const;

private:
	void choose(const Vector2i& cell, bool inPanel);
	void calculateCollisions();
	void checkDeadZone();
	void checkEnd();
	void checkAlive();
	void checkRespawn();
	void spawnEnemy();

	float resolutionOffsetX;

	RectangleShape deadZone;

	class Map *gameMap;
	vector<Enemy*> enemies;


	ACTION_STATE m_actionState;

	//player data
	float life;
	float money;
	float energy;


	void hitPlayer(float damage);

	vector<Tower*> towers;


	vector<ENEMY_TYPES> spawnEnemies;
	Timer spawnTimer;
	ENEMY_TYPES bossType;
	constexpr static int RESPAWN_TIME = 1500;
	constexpr static int RESPAWN_OFFSET = 250;

	LEVEL_STATE m_state;
	void changeState(LEVEL_STATE state);

	struct VenomAbility
	{
		bool isActive;
		static const float VENOM_DAMAGE;
		static const int VENOM_DAMAGE_COUNT;
		static const int VENOM_ATTACK_SPEED;

		static const Vector2i VENOM_SIZE;
		class GameObject *object;
		Timer timer;
		int count;
	};
	VenomAbility venomAbility;


	static const float FREEZE_ABILITY_K;
	static const int FREEZE_ABILITY_DURATION;

	static const int INCREASE_DAMAGE_ABILITY_VALUE;
	static const int INCREASE_DAMAGE_ABILITY_DURATION;

	static const int INCREASE_ATTACK_SPEED_ABILITY_VALUE;
	static const int INCREASE_ATTACK_SPEED_ABILITY_DURATION;

	Timer timerRegenEnergy;
	static const int REGEN_ENERGY_TIMEOUT;
	static const int REGEN_ENERGY_VALUE;
	static const float BOMB_ABILITY_DAMAGE;

	vector<Animation*> effects;

	struct Shake
	{
		RectangleShape dangerRect;
		Timer dangerTimer;
		bool isActive;
		bool state;
		int count;
		float offset;


		static const int MAX_SHAKE_COUNT;
		static const int MAX_SHAKE_OFFSET;
		static const int SHAKE_TIME;

		void startShake();
	};
	Shake shake;


	CircleShape currentTowerRadius;
	void updateRadius();
	unsigned int m_powerTowersCount;
};

#endif // LEVEL_H
