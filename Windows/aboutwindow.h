#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include "statewindow.h"
#include "timer.h"

class AboutWindow : public StateWindow
{
public:
    AboutWindow();

	void init() override;
	void paint(RenderWindow *window) override;
	void back() override;
	void update() override;

private:

	struct Creator
	{
		Creator(const string& str);
		Text text;
		bool visible;
	};
	vector<Creator> creators;

	RectangleShape rect;
	Timer timer;
	static constexpr float CREDITS_SPEED = 50;
	static constexpr float CREDITS_STEP = 3;
};

#endif // ABOUTWINDOW_H
