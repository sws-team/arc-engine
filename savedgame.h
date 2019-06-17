#ifndef SAVEDGAME_H
#define SAVEDGAME_H

#include "stdheader.h"
#include "Game/gamestate.h"
#include "Game/Level/map.h"

struct SavedGame
{	
	vector<unsigned int> completedLevels;
	vector<GameState> playersData;
};

class SavedGameLoader
{
public:
	static SavedGameLoader &Instance();

	void save();
	void load();

	vector<SavedGame> savedGames() const;

	static constexpr int MAX_SAVE_SLOTS = 3;

	void addSaves(const SavedGame& saveGame);

	void addCompletedLevel(const unsigned int level);

	void setCurrentSave(const unsigned int n);

	SavedGame getSavedGame() const;

	void setCurrentMission(const unsigned int n);
	unsigned int getCurrentMission() const;
	bool loadMap(const String &fileName);
	void loadMaps(const String &path);

	vector<Map*> maps;
private:
	SavedGameLoader();
	SavedGameLoader(const SavedGameLoader& root) = delete;
	SavedGameLoader& operator=(const SavedGameLoader&) = delete;

	vector<SavedGame> saves;
	string saveFileName;
	unsigned int currentSave;
	unsigned int currentMission;
};

#endif // SAVEDGAME_H
