#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "Windows/menu.h"
#include "timer.h"

class GameWindow : public Menu
{
public:
	GameWindow();
	~GameWindow() override;

	void init() override;
	void paint(sf::RenderWindow *window) override;
	void eventFilter(sf::Event* event) override;
	void update() override;
	void back() override;
	void pause();

protected:
	void accept() override;
	void closeEvent() override;

private:
	sf::Sprite menuImg;
	sf::Sprite windowSprite;

	enum GAME_MENU
	{
		CONTINUE_GAME,
		RESET_MISSION,
		EXIT_FROM_MISSION,
		EXIT_FROM_GAME,
	};

	enum GAME_STATE
	{
		PLAYING,
		PAUSED,
		FINISHED,
		GAME_OVER
	};
	GAME_STATE m_state;
	void setState(const GAME_STATE &state);

	void finish();

	sf::Text text;
	sf::Clock clock;
	sf::Int64 lastTime;
	sf::Int64 tick;
	void updateTextPos();
	void nextTrack();
	int currentTrack;
	std::vector<MusicType> tracks;
	void fillTracks(const std::vector<MusicType> &trackList);
	bool isFinal;
};

#endif // GAMEWINDOW_H
