#ifndef CHOOSEMISSIONWINDOW_H
#define CHOOSEMISSIONWINDOW_H

#include "statewindow.h"

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
	void accept(unsigned int num);

	constexpr static int STARS_COUNT = 5;
	struct MissionView
	{
		RectangleShape rect;
		bool completed;
		int activeStars;
		vector<RectangleShape> stars; //5
	};
	vector<MissionView> missions;
};

#endif // CHOOSEMISSIONWINDOW_H
