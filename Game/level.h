#ifndef LEVEL_H
#define LEVEL_H

#include "stdheader.h"
#include "graphics.h"
#include "gamedrawable.h"
#include "mission.h"
#include "SpaceShip/levelenemy.h"

class GameObject;
class SpaceShip;
class GamePanel;

class Level : public GameDrawable
{
public:

	bool smokeEnabled;

	Level();
	~Level() override;

	void draw(RenderWindow *const window) override;
	void update() override;

	static constexpr float LEVEL_SPEED = 1;

	float leftBorder() const;
	float rightBorder() const;
	float topBorder() const;
	float bottomBorder() const;
	Vector2f getCenter() const;

	static constexpr float LEVEL_BORDER = 5.f;

	Vector2f getStartingPos() const;

	void startMission(const unsigned int n);

	bool isFinished() const;

	vector<SpaceShip *> getAllSpaceShips() const;

	bool isFailed() const;

	map<SpaceShip *, float> getLevelBonuses() const;

	SpaceShip *addEnemy(const Mission::Enemy& mEnemy);
	void removeEnemy(SpaceShip *enemy);

	void shakeOff();

private:
	View view;

	vector<Sprite> sprites;
	float m_speed;
	float m_topBorder;
	float m_bottomBorder;

	vector<LevelEnemy> enemies;
	void calculateCollisions();
	void collideSpaceShips(const vector<SpaceShip *> &attackers,
						   const vector<SpaceShip *> &defenders);
	void collideWithBonuses();

	GamePanel *panel;

	void checkRespawnLine(float y);
	float levelEndPos;

	Vector2f m_startPos;
	Mission mission;
	void addGraphics(const int n);
	void fillLevel();

	LevelEnemy boss;

	void collideSpaceShips();
	void checkAlive();

	bool m_finished;

	vector<class Bonus*> bonuses;

	struct Shake
	{
		bool isActive;
		Clock timer;
	};
	Shake shake;
	float m_viewCenter;

	bool m_isFailed;

	void checkDeathZone();

	map<SpaceShip*, float> levelBonuses;

	Shader levelShader;
	Clock smokeTimer;
	RectangleShape levelRect;

	float difficulty;

	float resolutionOffsetX;
};

#endif // LEVEL_H
