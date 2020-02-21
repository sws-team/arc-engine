#ifndef INTROWINDOW_H
#define INTROWINDOW_H

#include "statewindow.h"
#include "timer.h"

class IntroWindow : public StateWindow
{
public:
	IntroWindow();

	void init() override;
	void paint(RenderWindow *window) override;
	void eventFilter(Event* event) override;
	void update() override;
private:
	Timer timer;
	int opacity;

	void finish();
};

#endif // INTROWINDOW_H
