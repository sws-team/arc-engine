#ifndef CLOSEWINDOW_H
#define CLOSEWINDOW_H

#include "arcwindow.h"
#include "timer.h"

class CloseWindow : public ArcWindow
{
public:
	CloseWindow();

	void init() override;
	void back() override;
};

#endif // CLOSEWINDOW_H
