#ifndef CLOSESCENE_H
#define CLOSESCENE_H

#include "arcscene.h"
#include "timer.h"

class CloseScene : public ArcScene
{
public:
	CloseScene();

	void init() override;
};

#endif // CLOSESCENE_H
