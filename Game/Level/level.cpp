#include "level.h"
#include "tinyxml.h"
#include "../GameObjects/Buildings/buildings.h"
#include "../GameObjects/Units/unit.h"
#include "Engine/engine.h"
#include "settings.h"
#include "globalvariables.h"
#include "../Engine/gamecontroller.h"
#include "Engine/CPU/cpu.h"
#include "Game/soundcontroller.h"

#include <regex>

Level::Level() :
	currentPlayer(1)
  ,levelEnd(false)
  ,selectedUnit(nullptr)
  ,levelMap(nullptr)
{

}

Level::~Level()
{

}

void Level::update(RenderWindow *const window)
{
	for (size_t layer = 0; layer < levelMap->layers.size(); layer++)
		for (size_t tile = 0; tile < levelMap->layers[layer].tiles.size(); tile++)
		{
			const Vector2i pos = levelMap->layers[layer].tiles[tile].pos;
			Sprite sprite = levelMap->layers[layer].tiles[tile].sprite;
			if (!GlobalVariables::Instance().getControl() && selectedUnit != nullptr)
			{
				if (canMoveHere(pos))
					sprite.setColor(Color::Yellow);
			}
			window->draw(sprite);
		}

    for (auto it = levelMap->gameController->data->playerLevelData.begin(); it != levelMap->gameController->data->playerLevelData.end(); ++it)
    {
        const list<Unit*> units = it->second.units;
        for (auto unitsIt = units.begin(); unitsIt!=units.end(); ++unitsIt)
        {
            Unit *const unit = *unitsIt;

            if (selectedUnit != nullptr)
            {
                if (canAttackHere(unit))
                    unit->setPaintState(Unit::PAINT_STATE::ATTACKABLE);
            }

            unit->draw(window);
        }
    }
}

int Level::getTileIdFromPos(const Vector2i &pos) const
{
    for (size_t layer = 0; layer < levelMap->layers.size(); layer++)
        for (size_t tile = 0; tile < levelMap->layers[layer].tiles.size(); tile++)
            if (levelMap->layers[layer].tiles[tile].pos == pos)
                return levelMap->layers[layer].tiles[tile].id;
    return -1;
}

Tile::TileProperties Level::getTileProperties(int tileId) const
{
    if (levelMap->tileProperties.count(tileId) == 1)
        return levelMap->tileProperties.at(tileId);
    return Tile::TileProperties();
}

void Level::checkPoints()
{
    const map<int, PlayerLevelData::LevelData> checkMap = levelMap->gameController->data->playerLevelData;
    for(auto playerData : checkMap)
    {
        for(Building *const building : playerData.second.buildings)
        {
            Unit *const unit = getUnitFromPos(levelPos(building));
            if (unit != nullptr)
            {
                if(building->number() != unit->number())
                {
                    levelMap->gameController->data->playerLevelData[building->number()].buildings.remove(building);
                    levelMap->gameController->data->playerLevelData[unit->number()].buildings.push_back(building);

                    building->setNumber(unit->number());
                }
            }
        }
    }
}

Building *Level::getBuildingFromPos(int player, const Vector2i &pos) const
{
    for(Building *const building : levelMap->gameController->data->playerLevelData[player].buildings)
    {
        const Vector2i point = levelPos(building);
        if (point == pos)
            return building;
    }
    return nullptr;
}

Building *Level::getBuildingFromPos(const Vector2i &pos) const
{
    for(auto playerData : levelMap->gameController->data->playerLevelData)
    {
        for(Building *const building : playerData.second.buildings)
        {
            const Vector2i point = levelPos(building);
            if (point == pos)
                return building;
        }
    }
    return nullptr;
}

Unit *Level::getUnitFromPos(int player, const Vector2i &pos) const
{
    for(Unit *const unit : levelMap->gameController->data->playerLevelData[player].units)
    {
        const Vector2i point = levelPos(unit);
        if (point == pos)
            return unit;
    }
    return nullptr;
}

Unit *Level::getUnitFromPos(const Vector2i &pos) const
{
    for(auto playerData : levelMap->gameController->data->playerLevelData)
    {
        for(Unit *const unit : playerData.second.units)
        {
            const Vector2i point = levelPos(unit);
            if (point == pos)
                return unit;
        }
    }
    return nullptr;
}

CellInfo Level::getCellInfoFromPos(const Vector2i &pos) const
{
    CellInfo info;
    Unit *unit = getUnitFromPos(pos);
    const int tileId = getTileIdFromPos(pos);
    if (tileId != -1)
    {
         if (unit != nullptr)
         {
             info.name = String("Unit - ");
             info.description = String("Unit Decription\n\n");
             info.unitTexture = &unit->getIcon();
         }
         info.name += String("Tile");
         info.description += String("Tile Description");
         info.tileSprite = levelMap->tileSprites.at(tileId);
    }
    else
    {
        info.name = String("Unknown");
        info.description = info.name;
        info.unitTexture = GlobalVariables::Instance().getUnknowUnit();
    }
    return info;
}

Vector2i Level::size() const
{
    return Vector2i(levelMap->width, levelMap->height);
}

void Level::addUnit(int player, Unit * const unit)
{
    unit->setNumber(player);
    levelMap->gameController->data->playerLevelData[player].resources -= unit->getUnitData().stats.price;
    levelMap->gameController->data->playerLevelData[player].units.push_back(unit);
}

bool Level::canMoveHere(const Vector2i &pos) const
{
    const Vector2i selectionPos = levelPos(selectedUnit);
    if (abs(pos.x - selectionPos.x) < selectedUnit->getUnitParametrs().move
            && abs(pos.y - selectionPos.y) < selectedUnit->getUnitParametrs().move)
    {
        if (getTileProperties(getTileIdFromPos(pos)).isSolid)
            return false;
        for(auto playerData : levelMap->gameController->data->playerLevelData)
        {
            for(Unit *const unit : playerData.second.units)
            {
                const Vector2i point = levelPos(unit);
                if (point == pos)
                    return false;
            }
        }
        return true;
    }
    return false;
}

bool Level::canAttackHere(Unit *const unit) const
{
    if (selectedUnit == nullptr)
        return false;

    if (unit != nullptr)
    if (unit->number() != currentPlayer) // only enemy
    {
        const Vector2i pos = levelPos(unit);
        const Vector2i selectionPos = levelPos(selectedUnit);
        if (abs(pos.x - selectionPos.x) < selectedUnit->getUnitData().stats.attackRange
                && abs(pos.y - selectionPos.y) < selectedUnit->getUnitData().stats.attackRange)
            return true;
    }
    return false;
}

void Level::playerTurn()
{
    currentPlayer++;
	if (currentPlayer >= levelMap->gameController->playersCount)
    {
        currentPlayer = 1;
        checkPoints();
        nextTurn();
        const int player = levelMap->gameController->checkWin();
        if (player != -1)
        {
            playerWin = player;
            levelEnd = true;
        }
    }
    for(Unit *const unit : levelMap->gameController->data->playerLevelData[currentPlayer].units)
    {
        unit->setState(Unit::READY);
        unit->setMoved(unit->getUnitData().stats.move);
    }
}

void Level::execAction(const Vector2i &pos)
{
    if (canMoveHere(pos))
    {
        const Vector2i startPos = levelPos(selectedUnit);
        const int moved = abs(startPos.x - pos.x) + abs(startPos.y - pos.y);
		selectedUnit->move(pos);
        selectedUnit->setMoved(selectedUnit->getUnitParametrs().move - moved);

        // unit moved

        if (selectedUnit->getUnitParametrs().move == 0)
            if (avaliableAttackCount() == 0)
                selectedUnit->setState(Unit::FINISHED);

        resetSelectedUnit();
        return;
    }
    Unit *const enemyUnit = getUnitFromPos(pos);
    if (enemyUnit != nullptr)
    if(canAttackHere(enemyUnit))
    {
        Engine::combat(selectedUnit, enemyUnit);

        levelMap->gameController->checkUnitHeath(enemyUnit);

        selectedUnit->setState(Unit::FINISHED);
        resetSelectedUnit();
        return;
    }
    SoundController::Instance().playOnce(NO_ACTION_SOUND_FILE);
    if (selectedUnit->state() == Unit::WAIT_COMMAND)
        selectedUnit->setState(Unit::READY);
}

void Level::initialize()
{
    levelMap->gameController->playersCount = levelMap->gameController->data->playerLevelData.size();
    currentPlayer = 1;
    for(auto playerData : levelMap->gameController->data->playerLevelData)
    {
        levelMap->gameController->data->playerLevelData[playerData.first].resources = GlobalVariables::START_RESOURCES;
        levelMap->gameController->data->playerLevelData[playerData.first].isActive = true;
    }


	for (Building *building : this->levelMap->buildings)
		levelMap->gameController->data->playerLevelData[building->number()].buildings.push_back(building);
}

void Level::setMap(Map *levelMap)
{
    this->levelMap = levelMap;
    CPU::Instance().setMap(this->levelMap);

   this->levelMap->gameController = this->levelMap->gameController;
}

void Level::nextTurn()
{
    for(auto playerData : levelMap->gameController->data->playerLevelData)
    {
        int resourcesForTurn = 0;
        for(Building *const building : playerData.second.buildings)
        {
            switch (building->buildingType())
            {
            case Building::RESOURCE_POINT:
                resourcesForTurn += GlobalVariables::POINT_RESOURCES;
                break;
            case Building::MAINBASE:
                resourcesForTurn += GlobalVariables::BASE_RESOURCES;
                break;
            default:
                break;
            }
        }
        levelMap->gameController->data->playerLevelData[playerData.first].resources += resourcesForTurn;

        const int buildingsCount = playerData.second.buildings.size();
        const int unitCount = playerData.second.units.size();
        if (buildingsCount == 0 && unitCount == 0)
            levelMap->gameController->data->playerLevelData[playerData.first].isActive = false;//player lose;
    }
}

int Level::resources(int player) const
{
    return levelMap->gameController->data->playerLevelData.at(player).resources;
}

int Level::getCurrentPlayer() const
{
    return currentPlayer;
}



Vector2i Level::levelPos(const GameObject *object) const
{
    Vector2i point;
    point.x = object->pos().x / GlobalVariables::Instance().tileSize().x;
    point.y = object->pos().y / GlobalVariables::Instance().tileSize().y;
	return point;
}

Json::Value Level::toJson() const
{
	Json::Value json;	
	for(auto playerData : levelMap->gameController->data->playerLevelData)
	{
		Json::Value jsonPlayerLevelData;
		const int playerNum = playerData.first;
		jsonPlayerLevelData["num"] = playerNum;

		Json::Value jsonUnits;		
		for(Unit *const units : playerData.second.units)
		{
			Json::Value jsonUnit;
			jsonUnit["exp"] = units->getUnitParametrs().expirience;
			jsonUnit["health"] = units->getUnitParametrs().health;
			jsonUnit["lvl"] = units->getUnitParametrs().level;
			jsonUnit["move"] = units->getUnitParametrs().move;
			jsonUnit["number"] = units->number();
			jsonUnit["posX"] = units->pos().x;
			jsonUnit["posY"] = units->pos().y;
			jsonUnit["id"] = units->getUnitData().id;

			jsonUnits.append(jsonUnit);
		}
		jsonPlayerLevelData["units"] = jsonUnits;
		json.append(jsonPlayerLevelData);
	}
	return json;
}

void Level::loadJson(const Json::Value &json, const vector<Player> &players)
{
	for (unsigned int i = 0; i < json.size(); ++i)
        {
		const Json::Value jsonPlayerLevelData = json[i];
		const int playerNum = jsonPlayerLevelData["num"].asInt();
		const Json::Value jsonUnits = jsonPlayerLevelData["units"];
		const Player player = players.at(playerNum);
		for (unsigned int j = 0; j < jsonUnits.size(); ++j)
		{
					cout << "444 "<<j<<endl;
			Json::Value jsonUnit = jsonUnits[j];
			UnitParametrs params;
			params.expirience = jsonUnit["exp"].asInt();
			params.health = jsonUnit["health"].asInt();
			params.level = jsonUnit["lvl"].asInt();
			params.move = jsonUnit["move"].asInt();
			const int number = jsonUnit["number"].asInt();
			Vector2f pos;
			pos.x = jsonUnit["posX"].asFloat();
			pos.y = jsonUnit["posY"].asFloat();

			const int id = jsonUnit["id"].asInt();
			Unit *unit = player.race->createUnit(id, player.color);
			unit->setPos(pos);
			unit->setUnitParametrs(params);

			addUnit(number, unit);
		}
	}
}

Map *Level::currentMap()
{
	return levelMap;
}

void Level::setCurrentPlayer(int value)
{
	currentPlayer = value;
}

Vector2f Level::pixelPos(const Vector2i &pos) const
{
	Vector2f point;
    point.x = pos.x * GlobalVariables::Instance().tileSize().x;
    point.y = pos.y * GlobalVariables::Instance().tileSize().y;
    return point;
}

int Level::avaliableAttackCount()
{
    int count = 0;
    if (selectedUnit != nullptr)
    for (auto it = levelMap->gameController->data->playerLevelData.begin(); it != levelMap->gameController->data->playerLevelData.end(); ++it)
    {
        const list<Unit*> units = it->second.units;
        for (auto unitsIt = units.begin(); unitsIt!=units.end(); ++unitsIt)
        {
            Unit *const unit = *unitsIt;
            if (selectedUnit != nullptr)
            {
                if (canAttackHere(unit))
                    count++;
            }
        }
    }
    return count;
}

void Level::setSelectedUnit(Unit * const unit)
{
    selectedUnit = unit;
    if (unit != nullptr)
        selectedUnit->setState(Unit::WAIT_COMMAND);
}

void Level::resetSelectedUnit()
{
    selectedUnit = nullptr;
}

Unit *Level::getSelectedUnit() const
{
    return selectedUnit;
}

int Level::getPlayerWin() const
{
    return playerWin;
}

bool Level::getLevelEnd() const
{
    return levelEnd;
}

