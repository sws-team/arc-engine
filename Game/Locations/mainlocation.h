#ifndef MAINLOCATION_H
#define MAINLOCATION_H

#include "statewindow.h"
#include "graphics.h"

class MainLocation : public StateWindow
{
public:
	MainLocation();
	~MainLocation() override;

	void init() override;
	void paint(RenderWindow *window) override;
	void eventFilter(Event *event) override;
	void back() override;

private:
	CircleShape shopLink;
	bool shopLinkVisible;


	struct MapLink
	{
		CircleShape circle;
		bool isVisible;
		unsigned int mission_number;
	};

	vector<MapLink> links;

	GraphicImage exit;

	unsigned int mission_number;

	GraphicImage circle;
	bool circleVisible;


	enum MAIN_LOCATION_STATES
	{
		DEFAULT_STATE,
		BRIEFING,
	};
	MAIN_LOCATION_STATES m_state;
	void changeState(MAIN_LOCATION_STATES state);

	Text missionText;
	RectangleShape darkRect;

	GraphicImage missionImage;
	RectangleShape acceptMissionRect;
	RectangleShape backMissionRect;
};

#endif // MAINLOCATION_H
