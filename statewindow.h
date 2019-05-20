#ifndef STATEWINDOW_H
#define STATEWINDOW_H

#include "stdheader.h"
#include "Engine/engine.h"
#include "ResourcesManager/textures_types.h"

class StateWindow
{
public:
	StateWindow();
	virtual ~StateWindow();

	virtual void init();
	virtual void paint(RenderWindow *window) = 0;
	virtual void eventFilter(Event* event);
	virtual void update();

	void setBackground(RESOURCES::TEXTURE_TYPE type);

protected:
	void drawBackground(RenderWindow *window);
	Sprite background;
	virtual void back();
};

#endif // STATEWINDOW_H
