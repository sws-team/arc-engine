#ifndef CLOSEWINDOW_H
#define CLOSEWINDOW_H

#include "statewindow.h"
#include "timer.h"

class CloseWindow : public StateWindow
{
public:
	CloseWindow();

//	void init() override;
	void paint(RenderWindow *window) override;
//	void eventFilter(Event* event) override;
	void update() override;
	void back() override;

private:
	Timer timer;
	Uint8 opacity;
};

#endif // CLOSEWINDOW_H
