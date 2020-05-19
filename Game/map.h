#ifndef MAP_H
#define MAP_H

#include "stdheader.h"
#include "tile.h"
#include "objects_types.h"
#include "Game/Balance/balance.h"

struct Layer
{
	float opacity;
	bool visibility;

	std::vector<Tile> tiles;

	sf::VertexArray vertices;
};

class Map
{
public:

	enum MOVE_DIRECTIONS
	{
		STAY,
		DOWN,
		UP,
		LEFT,
		RIGHT,
		NO_MOVE
	};

	Map():
		width(0)
	  ,height(0)
	  ,directionsLayer(3)
	  ,movingLayer(-1)
	  ,waterLayer(-1)
	{

	}
	~Map()
	{

	}
	MapStats stats;
	unsigned int number;

	int width;
    int height;

	std::vector<Layer> layers;

	std::vector<MOVE_DIRECTIONS> directions;
	struct SpawnInfo
	{
		sf::Vector2f pos;
		MOVE_DIRECTIONS direction;
	};
	std::vector<SpawnInfo> spawnInfos;
	std::vector<sf::FloatRect> endRects;

	int directionsLayer;
	int movingLayer;
	int waterLayer;

	struct MapObject
	{
		sf::Vector2f pos;
		OBJECTS::OBJECT_TYPES type;
		OBJECTS::SHADER_TYPES shader_type;
		int layer;
	};

	std::vector<MapObject> objects;
};

#endif // MAP_H
