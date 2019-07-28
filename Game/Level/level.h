#ifndef LEVEL_H
#define LEVEL_H

#include "stdheader.h"
#include "graphics.h"
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

	int getEnergyCount() const;
	float getLifeCount() const;
	Tower *getTowerAtPos(const Vector2f& pos) const;

	bool canAddTower(const Vector2i& cell, int towerType) const;

	void highlightPowerTowersRadius(bool active);

	void setDoubleSpeed();
	void setNormalSpeed();
	void setFastSpeed();
	void setNullSpeed();

private:
	void choose(const Vector2i& cell, bool inPanel);
	void calculateCollisions();
	void checkDeadZone();
	void checkEnd();
	void checkAlive();

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

	//player data
	float life;
	int energy;


	void hitPlayer(float damage);

	vector<Tower*> towers;
};

#endif // LEVEL_H
