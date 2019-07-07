#ifndef ENGINE_H
#define ENGINE_H

#include "stdheader.h"

class Camera;
class Cursor;
class Level;
class GamePanel;

class Engine
{
public:
	static Engine &Instance();

	enum GAME_STATE
	{
		UNKNNOWN,
		INTRO,
		MAIN_MENU,
		OPTIONS,
		ABOUT,
		CLOSING,
		EXIT,

		CHOOSE_MISSION,
		IN_GAME,
	};

	GAME_STATE getState() const;
	void setState(const GAME_STATE &state);

	class StateWindow *createState(const GAME_STATE &state);

	Camera *camera() const;
	Cursor *cursor() const;

	void setView(const View &view);
	View getView() const;

	bool loadMap(const String &fileName);
	void loadMaps(const String &path);

	void setWindow(RenderWindow *window);
	RenderWindow *window();

	Level *level();
	GamePanel *panel();

	void reset();

private:
	Engine();
	Engine(const Engine& root) = delete;
	Engine& operator=(const Engine&) = delete;

	GAME_STATE m_state;

	Camera *m_camera;
	Cursor *m_cursor;
	Level *m_level;
	GamePanel *m_panel;

	RenderWindow *p_window;
};

#endif // ENGINE_H
