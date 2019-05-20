#ifndef LEVEL_H
#define LEVEL_H

#include "stdheader.h"

class GameObject;
class Speech;
#include "Level/map.h"
#include "playerleveldata.h"
#include "Game/player.h"

class Level
{
public:
    Level();

    ~Level();

//=======DRAW LEVEL========
    void update(RenderWindow *const window);
//=========================

    int getTileIdFromPos(const Vector2i &pos) const;    

    Building *getBuildingFromPos(int player, const Vector2i &pos) const;
    Building *getBuildingFromPos(const Vector2i &pos) const;
    Unit *getUnitFromPos(int player, const Vector2i &pos) const;
    Unit *getUnitFromPos(const Vector2i &pos) const;

    CellInfo getCellInfoFromPos(const Vector2i &pos) const;

    Vector2i size() const;

    void addUnit(int player, Unit *const unit);
    void playerTurn();

    bool canMoveHere(const Vector2i& pos) const;
    bool canAttackHere(Unit *const unit) const;

    void execAction(const Vector2i& pos);

	void initialize();
    void setMap(Map *levelMap);
    void nextTurn();

    int resources(int player) const;

    int getCurrentPlayer() const;

    bool getLevelEnd() const;

    int getPlayerWin() const;

    Unit *getSelectedUnit() const;

    void setSelectedUnit(Unit *const unit);

    void resetSelectedUnit();

    Vector2i levelPos(GameObject const* object) const;

	Json::Value toJson() const;
	void loadJson(const Json::Value& json, const vector<Player> &players);

	Map *currentMap();
	void setCurrentPlayer(int value);

private:
	int currentPlayer;

	bool levelEnd;
    int playerWin;

    class Unit *selectedUnit;

    Vector2f pixelPos(const Vector2i& pos) const;

    int avaliableAttackCount();

    Map *levelMap;

    Tile::TileProperties getTileProperties(int tileId) const;
    void checkPoints();
};

#endif // LEVEL_H
