#ifndef ARCACTION_H
#define ARCACTION_H

#include "enginedef.h"
#include "timer.h"

class ArcAction
{
public:
	ArcAction();

	void start();
	virtual void update();
};

class ActionFactory
{
public:
	static ArcAction createAction();
};

#endif // ARCACTION_H
