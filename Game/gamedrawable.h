#ifndef GAMEDRAWABLE_H
#define GAMEDRAWABLE_H

#include "stdheader.h"
#include "timer.h"

class GameDrawable
{
public:
	GameDrawable()
	{

	}

	virtual ~GameDrawable()
	{

	}

	virtual void draw(RenderWindow *const window) = 0;
	virtual void update() = 0;

protected:
	Timer timer;
};

#endif // GAMEDRAWABLE_H
