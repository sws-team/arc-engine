#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include "statewindow.h"

class AboutWindow : public StateWindow
{
public:
    AboutWindow();

	void init() override;
	void paint(RenderWindow *window) override;
	void back() override;

private:

};

#endif // ABOUTWINDOW_H
