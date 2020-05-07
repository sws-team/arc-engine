#ifndef CHOOSEMISSIONWINDOW_H
#define CHOOSEMISSIONWINDOW_H

#include "statewindow.h"

//#define WITH_DIFFICULT

class ChooseMissionWindow : public StateWindow
{
public:
	ChooseMissionWindow();
	~ChooseMissionWindow() override;

	void back() override;

protected:
	void paint(sf::RenderWindow *window) override;
	void eventFilter(sf::Event *event) override;

private:
	void accept(unsigned int num);

	constexpr static int STARS_COUNT = 5;
	struct MissionView
	{
		sf::RectangleShape rect;
		sf::Text numberText;
		sf::RectangleShape mapRect;
		sf::RectangleShape highlight;
		bool completed;
		int activeStars;
		std::vector<sf::RectangleShape> stars; //5
		bool enabled;
	};
	std::vector<MissionView> missions;
	sf::RectangleShape chooseRect;
	int currentMission;
	void updateRect();

	constexpr static int COLUMN_COUNT = 5;
	unsigned int getRating(unsigned int n) const;

	static const sf::Color DISABLED_COLOR;
	static const sf::Color CURRENT_COLOR;
	static const sf::Color CURRENT_BORDER_COLOR;


#ifdef WITH_DIFFICULT
	sf::RectangleShape difficultRect;
	sf::RectangleShape currentDifficultRect;
	sf::RectangleShape choosedDifficultRect;

	sf::RectangleShape easyRect;
	sf::RectangleShape normalRect;
	sf::RectangleShape hardRect;

	sf::Text easyText;
	sf::Text normalText;
	sf::Text hardText;
	void initDifficults();
#endif
	int hovered;
};

#endif // CHOOSEMISSIONWINDOW_H
