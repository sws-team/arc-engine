#include "gamecursor.h"
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

const Color GameCursor::TOWER_AREA_COLOR = Color(40,98,131, 100);
const Color GameCursor::INACTIVE_TOWER_AREA_COLOR = Color(213,84,66, 100);

GameCursor::GameCursor()
	: GameObject(RESOURCES::CURSOR_TEXTURE,
				 Vector2f(0,0),
				 Vector2i(GlobalVariables::CELL_SIZE, GlobalVariables::CELL_SIZE),
				 2)
	,m_inPanel(false)
	,m_highlight(false)
{
	m_cell = Vector2i(0, 0);
	m_maxCell = Vector2i(30, 17);
}

void GameCursor::setMaxCells(int maxWidth, int maxHeight)
{
	setMaxCells(Vector2i(maxWidth, maxHeight));
}

void GameCursor::setMaxCells(const Vector2i &maxCell)
{
	m_maxCell = maxCell;
}

bool GameCursor::canMove(GameCursor::MOVE_DIRECTIONS direction)
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

void GameCursor::moveLeft()
{
	if (m_inPanel)
		return Engine::Instance().panel()->moveCursorLeft();

	moveLeftCursor();
}

void GameCursor::moveRight()
{
	if (m_inPanel)
		return Engine::Instance().panel()->moveCursorRight();

	moveRightCursor();
}

void GameCursor::moveUp()
{
	if (m_inPanel)
		return;

	moveUpCursor();
}

void GameCursor::moveDown()
{
	if (m_inPanel)
		return;

	moveDownCursor();
}

Vector2i GameCursor::cell() const
{
	return m_cell;
}

Vector2f GameCursor::pixelPos() const
{
	return Vector2f(m_cell.x * GlobalVariables::Instance().tileSize().x,
					m_cell.y * GlobalVariables::Instance().tileSize().y);
}

void GameCursor::draw(RenderTarget * const target)
{
	switch (m_state)
	{
	case NORMAL:
		GameObject::draw(target);
		if (m_highlight)
			target->draw(abilityRect);
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

void GameCursor::update()
{
	GameObject::update();
	checkBorders();
}

void GameCursor::activateAbility(int x, int y, int hotX, int hotY)
{
	abilityHotsPot.x = hotX;
	abilityHotsPot.y = hotY;

	abilityRect.setFillColor(Color(255, 0, 0, 100));
	abilityRect.setSize(Vector2f(GlobalVariables::Instance().tileSize().x * x,
								 GlobalVariables::Instance().tileSize().y * y));
	m_state = ABILITY;
}

void GameCursor::deactivate()
{
	m_state = NORMAL;
	abilityRect.setFillColor(Color(200, 200, 200, 100));
	abilityHotsPot.x = 0;
	abilityHotsPot.y = 0;
	abilityRect.setSize(Vector2f(GlobalVariables::Instance().tileSize().x,
								 GlobalVariables::Instance().tileSize().y));
}

void GameCursor::activateTower(float radius, TOWER_TYPES type)
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

FloatRect GameCursor::getAbilityRect() const
{
	return abilityRect.getGlobalBounds();
}

void GameCursor::swap()
{
	m_inPanel = !m_inPanel;
	Engine::Instance().panel()->updateCursor();
//	Engine::Instance().window()->setMouseCursorVisible(m_inPanel);
}

bool GameCursor::inPanel() const
{
	return m_inPanel;
}

TOWER_TYPES GameCursor::getTowerType() const
{
	return towerType;
}

void GameCursor::initCell()
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

void GameCursor::updatePanel()
{
	m_inPanel = windowCursorPos().y > Engine::Instance().panel()->getBottomValue();
	Engine::Instance().panel()->updateCursor();
}

Vector2f GameCursor::windowCursorPos() const
{
	const Vector2i pixelPos = Mouse::getPosition(*Engine::Instance().window());
	const Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos, *Engine::Instance().camera()->getView());
	return pos;
}

Vector2f GameCursor::windowScreenPos() const
{
	const Vector2i pixelPos = Mouse::getPosition(*Engine::Instance().window());
	const Vector2f pos = Vector2f(pixelPos.x, pixelPos.y);
	return pos;
}

void GameCursor::updateCell()
{
	const Vector2f pos = Vector2f(GlobalVariables::Instance().tileSize().x * m_cell.x,
								  GlobalVariables::Instance().tileSize().y * m_cell.y) -
			Vector2f(abilityHotsPot.x * GlobalVariables::Instance().mapTileSize().x,
					 abilityHotsPot.y * GlobalVariables::Instance().mapTileSize().y);
	if (m_state == TOWER)
	{
		const bool canCreate = Engine::Instance().level()->canAddTower(Vector2i(m_cell.x * 2, m_cell.y * 2), towerType);
		towerRadius.setFillColor(canCreate ? TOWER_AREA_COLOR : INACTIVE_TOWER_AREA_COLOR);
		towerRect.setFillColor(canCreate ? PowerTower::POWER_TOWER_AREA_COLOR : INACTIVE_TOWER_AREA_COLOR);
	}
	setPos(pos);
	towerSprite.setPosition(pos);
	abilityRect.setPosition(pos);
	towerRadius.setPosition(pos);
	towerRadius.setOrigin(towerRadius.getRadius() - GlobalVariables::Instance().mapTileSize().x,
						  towerRadius.getRadius() - GlobalVariables::Instance().mapTileSize().y);
	towerRect.setPosition(pos - GlobalVariables::Instance().tileSize());

	Engine::Instance().panel()->updateInfo();
}

void GameCursor::updateMousePos()
{
	const Vector2f pos = Engine::Instance().camera()->cellToPos(m_cell)
			+ Vector2f(GlobalVariables::Instance().tileSize().x/2,GlobalVariables::Instance().tileSize().y/2);

	const Vector2i coords = Engine::Instance().window()->mapCoordsToPixel(pos, *Engine::Instance().camera()->getView());
	Mouse::setPosition(coords, *Engine::Instance().window());
}

void GameCursor::checkBorders()
{
	const Vector2i pixelPos = Mouse::getPosition(*Engine::Instance().window());
	const Vector2f pos = Engine::Instance().window()->mapPixelToCoords(
				pixelPos, *Engine::Instance().camera()->getView()) + Vector2f(1, 1);
	const Vector2i cell = Vector2i(pos.x/GlobalVariables::Instance().tileSize().x,
								   pos.y/GlobalVariables::Instance().tileSize().y);

	if (cell.x == Engine::Instance().camera()->viewLeftCell() && !m_inPanel)
		moveLeftCursor();
	if (cell.x >= Engine::Instance().camera()->viewRightCell() && !m_inPanel)
		moveRightCursor();
	if (cell.y == Engine::Instance().camera()->viewTopCell())
		moveUpCursor();
	if (cell.y == Engine::Instance().camera()->viewBottomCell()/* - Engine::Instance().panel()->cellsCount() - 1*/)
		moveDownCursor();
}

void GameCursor::moveDownCursor()
{
	if (!canMove(GameCursor::MOVE_DOWN))
		return;

	m_cell.y++;
	if (Engine::Instance().camera()->viewBottomCell() != m_maxCell.y + Engine::Instance().panel()->cellsCount() &&
			m_cell.y > Engine::Instance().camera()->viewCenter().y)
		Engine::Instance().camera()->moveDownByCell();
	else
		updateMousePos();
	updateCell();
}

void GameCursor::moveUpCursor()
{
	if (!canMove(GameCursor::MOVE_UP))
		return;

	m_cell.y--;
	if (Engine::Instance().camera()->viewTopCell() != 0 &&
			m_cell.y < Engine::Instance().camera()->viewCenter().y)
		Engine::Instance().camera()->moveUpByCell();	
	else
		updateMousePos();

	updateCell();
}

void GameCursor::moveLeftCursor()
{
	if (!canMove(GameCursor::MOVE_LEFT))
		return;

	m_cell.x--;
	if (Engine::Instance().camera()->viewLeftCell() != 0 &&
			m_cell.x < Engine::Instance().camera()->viewCenter().x)
		Engine::Instance().camera()->moveLeftByCell();
	else
		updateMousePos();
	updateCell();
}

void GameCursor::moveRightCursor()
{
	if (!canMove(GameCursor::MOVE_RIGHT))
		return;

	m_cell.x++;
	if (Engine::Instance().camera()->viewRightCell() != m_maxCell.x - 1 &&
			m_cell.x > Engine::Instance().camera()->viewCenter().x)
		Engine::Instance().camera()->moveRightByCell();
	else
		updateMousePos();
	updateCell();
}

void GameCursor::setHighlight(bool highlight)
{
	m_highlight = highlight;
}

Vector2i GameCursor::getMaxCell() const
{
	return m_maxCell;
}

