#ifndef TILE_H
#define TILE_H

#include "stdheader.h"

struct Tile
{
	Tile()
	{

	}
	Tile (int id)
		: id(id)
	{

	}
	int id;
	sf::Vector2i cell;
	sf::Sprite sprite;

	struct TileProperties
	{
		TileProperties()
			: direction(0)
			,alternate_direction1(-1)
			,alternate_direction2(-1)
		{

		}
		int direction;
		int alternate_direction1;
		int alternate_direction2;
	};
};

#endif // TILE_H
