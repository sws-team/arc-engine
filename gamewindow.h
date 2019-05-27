#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "Windows/menu.h"
#include "graphics.h"

class GameWindow : public Menu
{
public:
	GameWindow();

	void init() override;
	void paint(RenderWindow *window) override;
	void eventFilter(Event* event) override;
	void update() override;
	void back() override;
	void pause();

protected:
	void accept() override;
	void closeEvent() override;

private:
	class Level *level;
	class Camera *camera;
	class Cursor *cursor;
	class Controller *controller;

	Text paused;
	GraphicImage menuImg;

	enum GAME_MENU
	{
		CONTINUE_GAME,
		EXIT_FROM_MISSION,
		EXIT_FROM_GAME,
		EXIT_TO_OS
	};

	enum GAME_STATE
	{
		PLAYING,
		PAUSED,
		IN_MENU,
		FINISHED,
		GAME_OVER
	};
	GAME_STATE m_state;
	void setState(const GAME_STATE &state);

	void finish();
	GraphicImage finishedImg;
	GraphicImage gameOverImg;

	struct ScoreInfo
	{
		TextInfo kills;
		TextInfo gears;
		TextInfo points;
	};
	Text scoreText;
	vector<ScoreInfo> scores;
};

#endif // GAMEWINDOW_H
