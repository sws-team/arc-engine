#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stdheader.h"
#include "enginedef.h"

class MainWindow : public sf::RenderWindow
{
public:
	MainWindow();
	~MainWindow();

	void prepare();
	void exec();
	sf::View *view() const;
	void updateView();
	void resetView();

private:
	sf::View *m_view;
	SceneType currentSceneType;
};

#endif // MAINWINDOW_H
