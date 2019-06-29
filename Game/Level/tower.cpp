#include "tower.h"
#include "globalvariables.h"

Tower::Tower(const RESOURCES::TEXTURE_TYPE &texture_id, const Vector2f &pos)
	: GameObject(texture_id, pos,
				 Vector2i(GlobalVariables::CELL_SIZE, GlobalVariables::CELL_SIZE),
				 4)
{

}
