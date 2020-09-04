#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stdheader.h"
#include "enginedef.h"
#include "statewindow.h"

class MainWindow : public sf::RenderWindow
{
public:
	MainWindow();
    ~MainWindow();

	void exec();
	sf::View *view() const;
	void updateView();
	void resetView();

private:
	sf::View *m_view;
	StateWindow *currentState;
	GameState state;
	sf::Clock fpsClock;

	void drawFPS();
};


#endif // MAINWINDOW_H
