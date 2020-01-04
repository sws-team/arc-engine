#ifndef ENGINE_H
#define ENGINE_H

#include "stdheader.h"

class Camera;
class Cursor;
class Level;
class GamePanel;
class Map;
class Controller;
class Instructions;

#ifdef STEAM_API
#include "steam_api.h"
#endif
#include "Game/achievements.h"
#include "tile.h"

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

	void loadMaps(const String &path);

	void setWindow(RenderWindow *window);
	RenderWindow *window();

	Level *level();
	GamePanel *panel();
	Controller *controller();
	Instructions *instructions();

	void reset();

	unsigned int missionsCount() const;
	unsigned int getMission() const;
	void setMission(unsigned int mission);
	Map *getMap(unsigned int mission);

	void save();
	void load();

	static float getStartHealth(const unsigned int n);
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

	void checkAchievments();
	bool unlockAchievment(GameAchievements::AchievmentsTypes type);

	map<int, Tile::TileProperties> getTileProperties() const;

private:
	Engine();
	Engine(const Engine& root) = delete;
	Engine& operator=(const Engine&) = delete;

	GAME_STATE m_state;

	Camera *m_camera;
	Cursor *m_cursor;
	Level *m_level;
	GamePanel *m_panel;
	Controller *m_controller;
	Instructions *m_instructions;

	RenderWindow *p_window;

	vector<CompletedMission> m_save;
	string saveFileName;
	vector<Map*> maps;
	unsigned int m_mission;
#ifdef STEAM_API
	STEAM_CALLBACK(Engine, OnGameOverlayActivated, GameOverlayActivated_t);
#endif
	Map *findMapByNumber(unsigned int num);
	bool loadMap(const String &fileName);
	bool loadTiles(const String& fileName);


	map<int, Tile::TileProperties> tileProperties;
	Texture tilesetImage;
};

#endif // ENGINE_H
