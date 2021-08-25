#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stdheader.h"
#include "enginedef.h"
#include "arcwindow.h"

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
	ArcWindow *currentState;
	GameState state;
	sf::Clock fpsClock;

	void updateFPS();
};

#endif // MAINWINDOW_H
