#ifndef STATEWINDOW_H
#define STATEWINDOW_H

#include "stdheader.h"
#include "engine.h"
#include "enginedef.h"

class StateWindow
{
public:
	StateWindow();
	virtual ~StateWindow();

	virtual void init();
	virtual void paint(sf::RenderWindow *window) = 0;
	virtual void eventFilter(sf::Event* event);
	virtual void update();

	void setBackground(TextureType type);

protected:
	void drawBackground(sf::RenderWindow *window);
	sf::Sprite background;
	virtual void back();
};

#endif // STATEWINDOW_H
