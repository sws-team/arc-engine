#ifndef LEVEL_H
#define LEVEL_H

#include "stdheader.h"
#include "graphics.h"
#include "Game/gamedrawable.h"

class GameObject;
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

	Tile getTileByPos(const Vector2f& pos);
	Tile getTileByCell(const Vector2i& cell);

private:
	void calculateCollisions();
	void checkDeadZone();

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
	vector<GameObject*> enemies;

	void moveEnemy(GameObject* enemy, int direction);
};

#endif // LEVEL_H
