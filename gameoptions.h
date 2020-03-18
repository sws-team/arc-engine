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

	sf::Vector2f tileSize() const;
	sf::Vector2f mapTileSize() const;
	constexpr static const int CELL_SIZE = 64;
	constexpr static const int MAP_CELL_SIZE = 32;
	constexpr static float GAME_SCALE = 0.8f;

	bool verifyChecksum();
	void loadMaps();

	unsigned int missionsCount() const;
	unsigned int getMission() const;
	void setMission(unsigned int mission);
	Map *getMap(unsigned int mission);

	void save();
	void load();

	void setMissionFinished(unsigned int n, unsigned int rating);
	int missionStars(unsigned int n) const;

	struct CompletedMission
	{
		unsigned int number;
		unsigned int stars;
		bool operator==(const CompletedMission& other)
		{
			return number == other.number;
		}
	};
	std::vector<CompletedMission> getCompletedMissions() const;
	unsigned int maxCompletedLevel() const;

	std::map<int, Tile::TileProperties> getTileProperties() const;

	LevelObject *createObject(OBJECTS::OBJECT_TYPES type, const sf::Vector2f &pos);

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

	void loadAchievements();
	void globalCallbacks() override;

	void clearSaves();

private:
	Camera *m_camera;
	GameCursor *m_cursor;
	Level *m_level;
	GamePanel *m_panel;
	Controller *m_controller;
	Instructions *m_instructions;

	std::vector<CompletedMission> m_save;
	std::vector<Map*> maps;
	unsigned int m_mission;

	Map *findMapByNumber(unsigned int num);
	bool loadMap(int id);
	bool loadTiles();

	std::map<int, Tile::TileProperties> tileProperties;
	sf::Texture tilesetImage;
	float m_gameSpeed;
	void setSpeed(float speed);
	float m_difficult;
};

#endif // GAMEOPTIONS_H
