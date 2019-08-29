#include "cursor.h"
#include "globalvariables.h"
#include "Game/Audio/soundcontroller.h"
#include "ResourcesManager/textures_types.h"
#include "camera.h"
#include "Engine/engine.h"
#include "settings.h"
#include "Game/gamepanel.h"
#include "level.h"
#include "ResourcesManager/resourcesmanager.h"
#include "tower.h"

Cursor::Cursor()
	: GameObject(RESOURCES::CURSOR_TEXTURE,
				 Vector2f(0,0),
				 Vector2i(GlobalVariables::CELL_SIZE, GlobalVariables::CELL_SIZE),
				 2)
	,m_inPanel(false)
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
	if (!canMove(Cursor::MOVE_LEFT))
		return;

	if (Engine::Instance().camera()->isDetached())
		Engine::Instance().camera()->centerOnCursor(m_cell);
	m_cell.x--;
	if (m_cell.x < Engine::Instance().camera()->currentCenterCell().x &&
			m_cell.x >= Engine::Instance().camera()->currentViewCells().x / 2)
		Engine::Instance().camera()->moveLeftByCell();
	updateCell();
}

void Cursor::moveRight()
{
	if (!canMove(Cursor::MOVE_RIGHT))
		return;

	if (Engine::Instance().camera()->isDetached())
		Engine::Instance().camera()->centerOnCursor(m_cell);
	m_cell.x++;
	if (m_cell.x > Engine::Instance().camera()->currentCenterCell().x &&
			m_cell.x < m_maxCell.x - Engine::Instance().camera()->currentViewCells().x / 2)
		Engine::Instance().camera()->moveRightByCell();
	updateCell();
}

void Cursor::moveUp()
{
	if (!canMove(Cursor::MOVE_UP))
		return;

	if (Engine::Instance().camera()->isDetached())
		Engine::Instance().camera()->centerOnCursor(m_cell);
	m_cell.y--;
	if (m_cell.y < Engine::Instance().camera()->currentCenterCell().y &&
			m_cell.y >= Engine::Instance().camera()->currentViewCells().y / 2)
		Engine::Instance().camera()->moveUpByCell();
	updateCell();
}

void Cursor::moveDown()
{
	if (!canMove(Cursor::MOVE_DOWN))
		return;

	if (Engine::Instance().camera()->isDetached())
		Engine::Instance().camera()->centerOnCursor(m_cell);
	m_cell.y++;
	if (m_cell.y > Engine::Instance().camera()->currentCenterCell().y &&
			m_cell.y < Engine::Instance().panel()->cellsCount() + m_maxCell.y - Engine::Instance().camera()->currentViewCells().y / 2)
		Engine::Instance().camera()->moveDownByCell();
	updateCell();
}

Vector2i Cursor::cell() const
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

void Cursor::draw(RenderTarget * const target)
{
	switch (m_state)
	{
	case NORMAL:
		GameObject::draw(target);
		break;
	case ABILITY:
		target->draw(abilityRect);
		break;
	case TOWER:
		target->draw(towerRadius);
		target->draw(towerSprite);
		break;
	}
}

void Cursor::update()
{
	GameObject::update();
	const Vector2i pixelPos = Mouse::getPosition(*Engine::Instance().window());
	const Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos, *Engine::Instance().camera()->getView());
	const Vector2i newCell = Engine::Instance().camera()->posToCell(pos);
	if (newCell != m_cell)
	{
		m_cell = newCell;
		updateCell();
	}
}

void Cursor::activateAbility(int x, int y, int hotX, int hotY)
{
	abilityHotsPot.x = hotX;
	abilityHotsPot.y = hotY;

	abilityRect.setFillColor(Color(255, 0, 0, 100));
	abilityRect.setSize(Vector2f(GlobalVariables::Instance().tileSize().x * x, GlobalVariables::Instance().tileSize().y * y));
	m_state = ABILITY;
}

void Cursor::deactivate()
{
	m_state = NORMAL;
}

void Cursor::activateTower(float radius, TOWER_TYPES type)
{
	abilityHotsPot.x = 0;
	abilityHotsPot.y = 0;

	towerRadius.setRadius(radius);

	towerType = type;

	RESOURCES::TEXTURE_TYPE textureType;
	switch (towerType)
	{
	case BASE:
		textureType = RESOURCES::TOWER_BASE;
		break;
	case POWER:
		textureType = RESOURCES::TOWER_POWER;
		break;
	case ROCKET:
		textureType = RESOURCES::TOWER_ROCKET;
		break;
	case FREEZE:
		textureType = RESOURCES::TOWER_FREEZE;
		break;
	case LASER:
		textureType = RESOURCES::TOWER_LASER;
		break;
	case IMPROVED:
		textureType = RESOURCES::TOWER_IMPROVED;
		break;
	}

	towerSprite.setTexture(ResourcesManager::Instance().getTexture(textureType));
	towerSprite.setScale(Settings::Instance().getScaleFactor());
	towerSprite.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);
	towerSprite.setColor(sf::Color(255, 255, 255, 128)); // half transparent
	m_state = TOWER;

	updateCell();
}

Vector2i Cursor::abilityCell() const
{
	return m_cell - abilityHotsPot;
}

FloatRect Cursor::getAbilityRect() const
{
	return abilityRect.getGlobalBounds();
}

void Cursor::swap()
{
	m_inPanel = !m_inPanel;
}

bool Cursor::inPanel() const
{
	return m_inPanel;
}

TOWER_TYPES Cursor::getTowerType() const
{
	return towerType;
}

void Cursor::updateCell()
{
	Vector2i cell = m_cell;

	switch (m_state)
	{
	case NORMAL:

		break;
	case ABILITY:
		cell -= abilityHotsPot;
		break;
	case TOWER:
	{
		const bool canCreate = Engine::Instance().level()->canAddTower(Engine::Instance().camera()->posToCell(Engine::Instance().camera()->cellToPos(m_cell)), towerType);
		towerRadius.setFillColor(canCreate ? Color(0, 255, 0, 100) : Color(255, 0, 0, 100));
	}
		break;
	}

	const Vector2f pos = Vector2f(GlobalVariables::Instance().tileSize().x * cell.x,
								  GlobalVariables::Instance().tileSize().y * cell.y);

	setPos(pos);
//	sprite.setOrigin(GlobalVariables::Instance().tileSize());
	towerSprite.setPosition(pos);
	abilityRect.setPosition(pos);
	towerRadius.setPosition(pos);
	towerRadius.setOrigin(towerRadius.getRadius() - GlobalVariables::Instance().tileSize().x/2,
						  towerRadius.getRadius() - GlobalVariables::Instance().tileSize().y/2);

//	SoundController::Instance().playOnce(CURSOR_MOVE_SOUND_FILE);
}

