#ifndef LEVEL_H
#define LEVEL_H

#include "stdheader.h"
#include "Game/gamedrawable.h"
#include "Game/leveldef.h"

class Enemy;
class GamePanel;
class Tile;
class Tower;

class Level : public GameDrawable
{
public:
	Level();
	~Level() override;

	void draw(RenderTarget * const target) override;
	void update() override;

	Vector2f getCenter() const;

	void startMission(const unsigned int n);

	void action();
	void change();

	void drawLevel(RenderTarget * const target);

	void spawn(ENEMY_TYPES type);
	void test();
	void left();
	void right();
	void down();
	void up();

	Tile getTileByPos(const Vector2f& pos);
	Tile getTileByCell(const Vector2i& cell) const;

	void chooseByPos(const Vector2f& pos);
	void chooseCurrent();

	int getTileDirectionByCell(const Vector2i &cell) const;

	float getEnergyCount() const;
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
private:
	void choose(const Vector2i& cell, bool inPanel);
	void calculateCollisions();
	void checkDeadZone();
	void checkEnd();
	void checkAlive();
	void checkRespawn();

	float difficulty;
	float resolutionOffsetX;

	Texture testTexture;
	Sprite m_background;

	RectangleShape deadZone;

	class Map *gameMap;
	vector<Enemy*> enemies;


	ACTION_STATE m_actionState;

//	union StateOptions
//	{
//		struct AddTower
//		{

//		};

//		struct CarpetBombing
//		{

//		};
//		struct Bomb
//		{

//		};
//		struct FreezeBomb
//		{

//		};

//		AddTower addTower;
//		CarpetBombing carpetBombing;
//		Bomb bomb;
//		FreezeBomb freezeBomb;
//	};

	//player data
	float life;
	float energy;


	void hitPlayer(float damage);

	vector<Tower*> towers;


	vector<ENEMY_TYPES> spawnEnemies;
	Timer spawnTimer;


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
};

#endif // LEVEL_H
