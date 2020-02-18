#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stdheader.h"
#include "Engine/engine.h"

class MainWindow : public RenderWindow
{
public:
	MainWindow();
    ~MainWindow();

    int exec();
private:
	StateWindow *currentState;
	Engine::GAME_STATE state;
	void drawFPS();

	Clock clock;
};


#endif // MAINWINDOW_H
