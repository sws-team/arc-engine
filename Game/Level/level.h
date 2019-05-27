#ifndef LEVEL_H
#define LEVEL_H

#include "stdheader.h"
#include "graphics.h"
#include "gamedrawable.h"

class GameObject;
class GamePanel;
class Camera;

class Level : public GameDrawable
{
public:
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

	bool isFailed() const;

	void action();
	void change();

	void setCamera(Camera *camera);

private:

	float m_topBorder;
	float m_bottomBorder;

	void calculateCollisions();
	GamePanel *panel;

	Vector2f m_startPos;
	void fillLevel();

	bool m_finished;
	bool m_isFailed;	
	float difficulty;
	float resolutionOffsetX;

	void updateRatio();

	void drawLevel(RenderWindow *const window);

	Camera *p_camera;
};

#endif // LEVEL_H
