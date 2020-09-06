#ifndef INTROWINDOW_H
#define INTROWINDOW_H

#include "statewindow.h"
#include "timer.h"

class IntroWindow : public StateWindow
{
public:
	IntroWindow();

	void init() override;
	void paint(sf::RenderWindow *window) override;
	void eventFilter(sf::Event* event) override;
	void update() override;
private:
	Timer timer;
	int opacity;

	void finish();
};

#endif // INTROWINDOW_H
