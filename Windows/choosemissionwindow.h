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
		Text numberText;
		RectangleShape rect;
		RectangleShape highlight;
		bool completed;
		int activeStars;
		vector<RectangleShape> stars; //5
		bool enabled;
	};
	vector<MissionView> missions;
	RectangleShape chooseRect;
	int currentMission;
	void updateRect();

	constexpr static int COLUMN_COUNT = 5;
	unsigned int getRating(unsigned int n) const;

	static const Color DISABLED_COLOR;
	static const Color CURRENT_COLOR;
	static const Color CURRENT_BORDER_COLOR;


	RectangleShape difficultRect;
	RectangleShape currentDifficultRect;
	RectangleShape choosedDifficultRect;

	RectangleShape easyRect;
	RectangleShape normalRect;
	RectangleShape hardRect;

	Text easyText;
	Text normalText;
	Text hardText;

	void initDifficults();
};

#endif // CHOOSEMISSIONWINDOW_H
