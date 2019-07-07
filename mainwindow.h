#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stdheader.h"
#include "savedgame.h"
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
};


#endif // MAINWINDOW_H
