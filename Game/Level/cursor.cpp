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

const Color Cursor::TOWER_AREA_COLOR = Color(0, 255, 0, 128);
const Color Cursor::INACTIVE_TOWER_AREA_COLOR = Color(255, 0, 0, 128);

Cursor::Cursor()
	: GameObject(RESOURCES::CURSOR_TEXTURE,
				 Vector2f(0,0),
				 Vector2i(GlobalVariables::CELL_SIZE, GlobalVariables::CELL_SIZE),
				 2)
	,m_inPanel(false)
{
	m_cell = Vector2i(0, 0);
	m_maxCell = Vector2i(30, 17);
}

void Cursor::setMaxCells(int maxWidth, int maxHeight)
{
	setMaxCells(Vector2i(maxWidth, maxHeight));
}

void Cursor::setMaxCells(const Vector2i &maxCell)
{
	m_maxCell = maxCell;
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
	if (m_inPanel)
		return Engine::Instance().panel()->moveCursorLeft();

	moveLeftCursor();
}

void Cursor::moveRight()
{
	if (m_inPanel)
		return Engine::Instance().panel()->moveCursorRight();

	moveRightCursor();
}

void Cursor::moveUp()
{
	if (m_inPanel)
		return Engine::Instance().panel()->moveCursorUp();

	moveUpCursor();
}

void Cursor::moveDown()
{
	if (m_inPanel)
		return Engine::Instance().panel()->moveCursorDown();

	moveDownCursor();
}

Vector2i Cursor::cell() const
{
	return m_cell;
}

Vector2f Cursor::pixelPos() const
{
	return Vector2f(m_cell.x * GlobalVariables::Instance().tileSize().x, m_cell.y * GlobalVariables::Instance().tileSize().y);
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
		if (towerType == POWER)
			target->draw(towerRect);
		else
			target->draw(towerRadius);
		target->draw(towerSprite);
		break;
	}
}

void Cursor::update()
{
	GameObject::update();
	checkBorders();
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

	if (type == POWER)
		towerRect.setSize(Vector2f(radius, radius));
	else
		towerRadius.setRadius(radius);

	towerType = type;

	RESOURCES::TEXTURE_TYPE textureType = RESOURCES::TOWER_POWER;;
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
	towerSprite.setColor(Color(255, 255, 255, 128));
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
	Engine::Instance().panel()->updateCursor();
	Engine::Instance().window()->setMouseCursorVisible(m_inPanel);
}

bool Cursor::inPanel() const
{
	return m_inPanel;
}

TOWER_TYPES Cursor::getTowerType() const
{
	return towerType;
}

void Cursor::initCell()
{
	const Vector2i pixelPos = Mouse::getPosition(*Engine::Instance().window());
	const Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos, *Engine::Instance().camera()->getView());
	const Vector2i newCell = Engine::Instance().camera()->posToCell(pos);
	if (newCell != m_cell)
	{
		m_cell = newCell;
		updateCell();
	}
}

void Cursor::updatePanel()
{
	m_inPanel = windowCursorPos().y > Engine::Instance().panel()->getBottomValue();
	Engine::Instance().panel()->updateCursor();
	Engine::Instance().window()->setMouseCursorVisible(m_inPanel);
}

Vector2f Cursor::windowCursorPos() const
{
	const Vector2i pixelPos = Mouse::getPosition(*Engine::Instance().window());
	const Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos, *Engine::Instance().camera()->getView());
	return pos;
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
		const bool canCreate = Engine::Instance().level()->canAddTower(Vector2i(m_cell.x * 2, m_cell.y * 2), towerType);
		towerRadius.setFillColor(canCreate ? TOWER_AREA_COLOR : INACTIVE_TOWER_AREA_COLOR);
		towerRect.setFillColor(canCreate ? PowerTower::POWER_TOWER_AREA_COLOR : INACTIVE_TOWER_AREA_COLOR);
	}
		break;
	}

	const Vector2f pos = Vector2f(GlobalVariables::Instance().tileSize().x * cell.x,
								  GlobalVariables::Instance().tileSize().y * cell.y);

	setPos(pos);
//	sprite.setOrigin(GlobalVariables::Instance().mapTileSize());
	towerSprite.setPosition(pos);
	abilityRect.setPosition(pos);
	towerRadius.setPosition(pos);
	towerRadius.setOrigin(towerRadius.getRadius() - GlobalVariables::Instance().mapTileSize().x,
						  towerRadius.getRadius() - GlobalVariables::Instance().mapTileSize().y);
	towerRect.setPosition(pos - GlobalVariables::Instance().tileSize());

	Engine::Instance().panel()->updateInfo();
	//	SoundController::Instance().playOnce(CURSOR_MOVE_SOUND_FILE);
}

void Cursor::updateMousePos()
{
	const Vector2f pos = Engine::Instance().camera()->cellToPos(m_cell)
			+ Vector2f(GlobalVariables::Instance().tileSize().x/2,GlobalVariables::Instance().tileSize().y/2);

	const Vector2i coords = Engine::Instance().window()->mapCoordsToPixel(pos, *Engine::Instance().camera()->getView());
	Mouse::setPosition(coords, *Engine::Instance().window());
}

void Cursor::checkBorders()
{
	const Vector2i pixelPos = Mouse::getPosition(*Engine::Instance().window());
	const Vector2f pos = Engine::Instance().window()->mapPixelToCoords(
				pixelPos, *Engine::Instance().camera()->getView()) + Vector2f(1, 1);
	const Vector2i cell = Vector2i(pos.x/GlobalVariables::Instance().tileSize().x,
								   pos.y/GlobalVariables::Instance().tileSize().y);

	if (cell.x == Engine::Instance().camera()->viewLeftCell())
		moveLeftCursor();
	if (cell.x >= Engine::Instance().camera()->viewRightCell())
		moveRightCursor();
	if (cell.y == Engine::Instance().camera()->viewTopCell())
		moveUpCursor();
	if (cell.y == Engine::Instance().camera()->viewBottomCell()/* - Engine::Instance().panel()->cellsCount() - 1*/)
		moveDownCursor();
}

void Cursor::moveDownCursor()
{
	if (!canMove(Cursor::MOVE_DOWN))
		return;

	m_cell.y++;
	if (Engine::Instance().camera()->viewBottomCell() != m_maxCell.y + Engine::Instance().panel()->cellsCount() &&
			m_cell.y > Engine::Instance().camera()->viewCenter().y)
		Engine::Instance().camera()->moveDownByCell();
	else
		updateMousePos();
	updateCell();
}

void Cursor::moveUpCursor()
{
	if (!canMove(Cursor::MOVE_UP))
		return;

	m_cell.y--;
	if (Engine::Instance().camera()->viewTopCell() != 0 &&
			m_cell.y < Engine::Instance().camera()->viewCenter().y)
		Engine::Instance().camera()->moveUpByCell();	
	else
		updateMousePos();

	updateCell();
}

void Cursor::moveLeftCursor()
{
	if (!canMove(Cursor::MOVE_LEFT))
		return;

	m_cell.x--;
	if (Engine::Instance().camera()->viewLeftCell() != 0 &&
			m_cell.x < Engine::Instance().camera()->viewCenter().x)
		Engine::Instance().camera()->moveLeftByCell();
	else
		updateMousePos();
	updateCell();
}

void Cursor::moveRightCursor()
{
	if (!canMove(Cursor::MOVE_RIGHT))
		return;

	m_cell.x++;
	if (Engine::Instance().camera()->viewRightCell() != m_maxCell.x - 1 &&
			m_cell.x > Engine::Instance().camera()->viewCenter().x)
		Engine::Instance().camera()->moveRightByCell();
	else
		updateMousePos();
	updateCell();
}

