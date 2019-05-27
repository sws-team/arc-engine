#include "cursor.h"
#include "globalvariables.h"
#include "Game/Audio/soundcontroller.h"
#include "ResourcesManager/textures_types.h"

Cursor::Cursor()
	: GameObject(RESOURCES::CURSOR_TEXTURE, Vector2f(0,0), GlobalVariables::Instance().tileSize(), 2)
{
	m_cell = Vector2i(0, 0);
	m_maxCell = Vector2i(60, 34);
}

bool Cursor::canMove(Cursor::MOVE_DIRECTIONS direction)
{
	switch (direction)
	{
	case MOVE_LEFT:
	{
		if (m_cell.x <= 0)
			return false;
	}
		break;
	case MOVE_RIGHT:
	{
		if (m_cell.x >= m_maxCell.x - 1)
			return false;
	}
		break;
	case MOVE_UP:
	{
		if (m_cell.y <= 0)
			return false;
	}
		break;
	case MOVE_DOWN:
	{
		if (m_cell.y >= m_maxCell.y - 1)
			return false;
	}
		break;
	}
	return true;
}

void Cursor::moveLeft()
{
	m_cell.x--;
	updateCell();
}

void Cursor::moveRight()
{
	m_cell.x++;
	updateCell();
}

void Cursor::moveUp()
{
	m_cell.y--;
	updateCell();
}

void Cursor::moveDown()
{
	m_cell.y++;
	updateCell();
}

Vector2i Cursor::pos() const
{
	return m_cell;
}

Vector2f Cursor::pixelPos() const
{
	return Vector2f(m_cell.x * GlobalVariables::Instance().tileSize().x, m_cell.y * GlobalVariables::Instance().tileSize().y);
}

void Cursor::setMaxCell(const Vector2i &maxCell)
{
	m_maxCell = maxCell;
}

void Cursor::updateCell()
{
	setPos(Vector2f(GlobalVariables::Instance().tileSize().x * m_cell.x, GlobalVariables::Instance().tileSize().y * m_cell.y));
	SoundController::Instance().playOnce(CURSOR_MOVE_SOUND_FILE);
}

