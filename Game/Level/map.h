#ifndef MAP_H
#define MAP_H

#include "stdheader.h"
#include "tile.h"

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
	{

	}
	~Map()
	{

	}


	int width;
    int height;

    string name;


	vector<Layer> layers;

    Texture icon;
    string decription;

	vector<MOVE_DIRECTIONS> directions;
	Vector2f spawnPos;
	MOVE_DIRECTIONS spawnDirection;

	FloatRect endRect;
};

#endif // MAP_H
