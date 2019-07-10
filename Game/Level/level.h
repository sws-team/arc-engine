#ifndef LEVEL_H
#define LEVEL_H

#include "stdheader.h"
#include "graphics.h"
#include "Game/gamedrawable.h"

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

	static constexpr float LEVEL_SPEED = 1;

	Vector2f getCenter() const;

	static constexpr float LEVEL_BORDER = 5.f;

	Vector2f getStartingPos() const;

	void startMission(const unsigned int n);

	bool isFinished() const;

	bool isFailed() const;

	void action();
	void change();

	void drawLevel(RenderTarget * const target);

	void spawn();
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


private:
	void choose(const Vector2i& cell, bool inPanel);
	void calculateCollisions();
	void checkDeadZone();
	void checkEnd();
	void checkAlive();
	Tower *getTowerAtPos(const Vector2f& pos);

	Vector2f m_startPos;

	bool m_finished;
	bool m_isFailed;	
	float difficulty;
	float resolutionOffsetX;

	Texture testTexture;
	Sprite m_background;

	RectangleShape deadZone;

	class Map *gameMap;
	vector<Enemy*> enemies;

	enum LEVEL_STATE
	{
		READY,

		ADD_TOWER,

		ABILITY_CARPET_BOMBING,
		ABILITY_BOMB,
		ABILITY_FREEZE_BOMB,
		ABILITY_INCREASE_TOWER_ATTACK_SPEED,
		ABILITY_INCREASE_TOWER_DAMAGE,

	};
	LEVEL_STATE m_state;

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

	float life;
	void hitPlayer(float damage);

	vector<Tower*> towers;
	Tower* selectedTower;
};

#endif // LEVEL_H
