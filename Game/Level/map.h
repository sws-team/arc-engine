#ifndef MAP_H
#define MAP_H

#include "stdheader.h"

class Building;

struct Tile
{
    int id;
    Vector2i pos;
    Sprite sprite;

    struct TileProperties
    {
        TileProperties(float _defenceBonus = 0.f)
            : defenceBonus(_defenceBonus)
            ,isSolid(false)
        {

        }

        float defenceBonus;
        bool isSolid;
    };
};

struct Layer
{
    int opacity;//непрозрачность слоя
    vector<Tile> tiles;//закидываем в вектор тайлы
};

class Speech;
class GameController;

class Map
{
public:

	Map():
		width(0)
	  ,height(0)
	  ,playersCount(0)
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
    int playersCount;

    map<int, Sprite> tileSprites;
    Texture icon;
    string decription;
};

#endif // MAP_H
