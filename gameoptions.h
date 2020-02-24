#ifndef GAMEOPTIONS_H
#define GAMEOPTIONS_H

#include "managers.h"
#include "tile.h"
#include "Game/objects_types.h"

class LevelObject;
class Camera;
class GameCursor;
class Level;
class GamePanel;
class Map;
class Controller;
class Instructions;

class GameOptions : public Options
{
public:
	GameOptions();

	void reset() override;

	Camera *camera() const;
	GameCursor *cursor() const;
	Level *level();
	GamePanel *panel();
	Controller *controller();
	Instructions *instructions();

	Vector2f tileSize() const;
	Vector2f mapTileSize() const;
	constexpr static const int CELL_SIZE = 64;
	constexpr static const int MAP_CELL_SIZE = 32;
	constexpr static float GAME_SCALE = 0.8f;

	void loadMaps(const String &path);
	bool checkMaps(const String &path) const;

	unsigned int missionsCount() const;
	unsigned int getMission() const;
	void setMission(unsigned int mission);
	Map *getMap(unsigned int mission);

	void save();
	void load();

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

	map<int, Tile::TileProperties> getTileProperties() const;

	LevelObject *createObject(OBJECTS::OBJECT_TYPES type, const Vector2f &pos);

	float gameSpeed() const;
	void setNormalSpeed();
	void setFastSpeed();
	void setExtraFastSpeed();


	void setEasyDifficult();
	void setNormalDifficult();
	void setHardDifficult();

	void setDifficult(float value);
	float difficult() const;

	void updateWindow() override;

private:
	Camera *m_camera;
	GameCursor *m_cursor;
	Level *m_level;
	GamePanel *m_panel;
	Controller *m_controller;
	Instructions *m_instructions;

	vector<CompletedMission> m_save;
	vector<Map*> maps;
	unsigned int m_mission;

	Map *findMapByNumber(unsigned int num);
	bool loadMap(const String &fileName);
	bool loadTiles(const String& fileName);

	map<int, Tile::TileProperties> tileProperties;
	Texture tilesetImage;
	float m_gameSpeed;
	void setSpeed(float speed);
	float m_difficult;
};

#endif // GAMEOPTIONS_H
