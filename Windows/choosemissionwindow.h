#ifndef CHOOSEMISSIONWINDOW_H
#define CHOOSEMISSIONWINDOW_H

#include "statewindow.h"
#include "savedgame.h"

class ChooseMissionWindow : public StateWindow
{
public:
	ChooseMissionWindow();
	~ChooseMissionWindow() override;

	void back() override;

protected:
	void paint(RenderWindow *window) override;
	void eventFilter(Event *event) override;

private:
	void accept(int num);

	RectangleShape mission;
};

#endif // CHOOSEMISSIONWINDOW_H
