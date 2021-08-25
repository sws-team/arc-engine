#ifndef CLOSEWINDOW_H
#define CLOSEWINDOW_H

#include "statewindow.h"
#include "timer.h"

class CloseWindow : public ArcWindow
{
public:
	CloseWindow();

	void init() override;
	void update() override;
	void back() override;

private:
	Timer timer;
	sf::Uint8 opacity;
};

#endif // CLOSEWINDOW_H
