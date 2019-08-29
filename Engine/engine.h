#ifndef ENGINE_H
#define ENGINE_H

#include "stdheader.h"

class Camera;
class Cursor;
class Level;
class GamePanel;
class Map;

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

	bool loadMap(const String &fileName);
	void loadMaps(const String &path);

	void setWindow(RenderWindow *window);
	RenderWindow *window();

	Level *level();
	GamePanel *panel();

	void reset();

	unsigned int missionsCount() const;
	unsigned int getMission() const;
	void setMission(unsigned int mission);
	Map *getMap(unsigned int mission);

	void save();
	void load();

	static float getStartMoney(const unsigned int n);

	void setMissionFinished(unsigned int n, unsigned int rating);

	struct CompletedMission
	{
		unsigned int number;
		unsigned int stars;
		bool operator==(const CompletedMission& other)
		{
			return number == other.number;
		}
	};
	vector<CompletedMission> getCompletedMissions() const;
	unsigned int maxCompletedLevel() const;

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



	vector<CompletedMission> m_save;
	string saveFileName;
	vector<Map*> maps;
	unsigned int m_mission;
};

#endif // ENGINE_H
