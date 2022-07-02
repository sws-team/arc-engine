#ifndef INTROWINDOW_H
#define INTROWINDOW_H

#include "arcscene.h"
#include "timer.h"

class IntroWindow : public ArcScene
{
public:
	IntroWindow();

	void init() override;
	bool eventFilter(sf::Event* event) override;
private:
	void finish();
};

#endif // INTROWINDOW_H
