#ifndef INTROWINDOW_H
#define INTROWINDOW_H

#include "arcwindow.h"
#include "timer.h"

class IntroWindow : public ArcWindow
{
public:
	IntroWindow();

	void initWindow() override;
	bool eventFilter(sf::Event* event) override;
private:
	void finish();
};

#endif // INTROWINDOW_H
