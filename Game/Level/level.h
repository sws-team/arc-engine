#ifndef LEVEL_H
#define LEVEL_H

#include "stdheader.h"
#include "graphics.h"
#include "Game/gamedrawable.h"

class Enemy;
class GamePanel;
class Tile;

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

	Tile getTileByPos(const Vector2f& pos);
	Tile getTileByCell(const Vector2i& cell);

private:
	void calculateCollisions();
	void checkDeadZone();
	void checkEnd();

	GamePanel *panel;

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
		CHOOSED_TOWER,

		ADD_TOWER,

		ABILITY_CARPET_BOMBING_PLACE,
		ABILITY_CARPET_BOMBING_DIRECTION,
		ABILITY_BOMB,
		ABILITY_FREEZE_BOMB,
		ABILITY_INCREASE_TOWER_ATTACK_SPEED,
		ABILITY_INCREASE_TOWER_DAMAGE,

	};
	LEVEL_STATE m_state;

	float life;
	void hitPlayer(float damage);
};

#endif // LEVEL_H
