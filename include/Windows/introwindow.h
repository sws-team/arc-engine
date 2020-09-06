#ifndef INTROWINDOW_H
#define INTROWINDOW_H

#include "statewindow.h"
#include "gametimer.h"

class IntroWindow : public StateWindow
{
public:
	IntroWindow(swoosh::ActivityController& controller);

	void onStart() override;
	void onUpdate(double elapsed) override;
	void onDraw(sf::RenderTexture& surface) override;

	void eventFilter(sf::Event* event) override;
private:
	GameTimer timer;
	int opacity;

	void finish();
};

#endif // INTROWINDOW_H
