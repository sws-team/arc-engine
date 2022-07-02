#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stdheader.h"
#include "enginedef.h"



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
	class ArcScene *currentState;
	GameState state;
};

#endif // MAINWINDOW_H
