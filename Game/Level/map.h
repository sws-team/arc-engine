#ifndef MAP_H
#define MAP_H

#include "stdheader.h"
#include "tile.h"
#include "objects_types.h"

struct Layer
{
	float opacity;//непрозрачность слоя
	bool visibility;
	vector<Tile> tiles;//закидываем в вектор тайлы
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


	int width;
    int height;

	vector<Layer> layers;

	vector<MOVE_DIRECTIONS> directions;
	Vector2f spawnPos;
	MOVE_DIRECTIONS spawnDirection;

	FloatRect endRect;

	string name;
	string decription;
	float money;
	float life;
	int directionsLayer;
	int movingLayer;
	int waterLayer;

	struct MapDanger
	{
		MapDanger()
			: enabled(false)
			,time(10000)
			,duration(2000)
			,count(1)
		{

		}
		bool enabled;
		float time;
		float duration;
		int count;
	};

	MapDanger explosions;
	MapDanger smoke;
	MapDanger moneyDrain;
	MapDanger regress;

	struct MapObject
	{
		Vector2f pos;
		OBJECTS::OBJECT_TYPES type;
		OBJECTS::SHADER_TYPES shader_type;
		int layer;
	};

	vector<MapObject> objects;
};

#endif // MAP_H
