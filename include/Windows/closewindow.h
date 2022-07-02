#ifndef CLOSEWINDOW_H
#define CLOSEWINDOW_H

#include "arcscene.h"
#include "timer.h"

class CloseWindow : public ArcScene
{
public:
	CloseWindow();

	void initWindow() override;
	void back() override;
};

#endif // CLOSEWINDOW_H
