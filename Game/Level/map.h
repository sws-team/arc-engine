#ifndef MAP_H
#define MAP_H

#include "stdheader.h"

struct Tile
{
	int id;
	Vector2i cell;
	Sprite sprite;

	struct TileProperties
	{
		TileProperties()
			: direction(0)
		{

		}
		int direction;
	};
};

struct Layer
{
	int opacity;//непрозрачность слоя
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
		RIGHT
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

    map<int, Tile::TileProperties> tileProperties;
	Texture tilesetImage;
	vector<Layer> layers;

    Texture icon;
    string decription;

	vector<MOVE_DIRECTIONS> directions;
	Vector2i spawnPos;
	MOVE_DIRECTIONS spawnDirection;

	FloatRect endRect;
};

#endif // MAP_H
