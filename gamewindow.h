#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "Windows/menu.h"
#include "timer.h"

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
	class Controller *controller;

	Text paused;
	Sprite menuImg;
	Sprite finishedImg;
	Sprite gameOverImg;

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

	Text text;
	Clock clock;
	Int64 lastTime;
	Int64 tick;
};

#endif // GAMEWINDOW_H
