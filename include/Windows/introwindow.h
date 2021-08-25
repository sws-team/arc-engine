#ifndef INTROWINDOW_H
#define INTROWINDOW_H

#include "arcwindow.h"
#include "timer.h"

class IntroWindow : public ArcWindow
{
public:
	IntroWindow();

	void init() override;
	bool eventFilter(sf::Event* event) override;
	void update() override;
private:
	Timer timer;
	int opacity;

	void finish();
};

#endif // INTROWINDOW_H
