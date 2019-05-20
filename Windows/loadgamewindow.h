#ifndef LOADGAMEWINDOW_H
#define LOADGAMEWINDOW_H

#include "statewindow.h"
#include "savedgame.h"

class LoadGameWindow : public StateWindow
{
public:
	LoadGameWindow();
	~LoadGameWindow() override;

	unsigned int saveNum() const;
	bool isNewGame() const;
	void back() override;

protected:
	void paint(RenderWindow *window) override;
	void eventFilter(Event *event) override;

private:
	struct SavedGamePanel
	{
		GraphicImage image;
		bool newGame;

		vector<Sprite> charactersIcons;
//		Text text;
	};
	vector<SavedGamePanel*> panels;
	unsigned int currentPanel;

	void accept();
};

#endif // LOADGAMEWINDOW_H
