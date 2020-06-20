#include "gamecursor.h"
#include "camera.h"
#include "engine.h"
#include "gamepanel.h"
#include "level.h"
#include "tower.h"
#include "managers.h"
#include "gamemanagers.h"
#include "gameoptions.h"
#include "mainwindow.h"

const sf::Color GameCursor::TOWER_AREA_COLOR = sf::Color(40,98,131, 100);
const sf::Color GameCursor::BUILD_AREA_COLOR = sf::Color(121,154,185, 100);
const sf::Color GameCursor::INACTIVE_TOWER_AREA_COLOR = sf::Color(213,84,66, 100);

GameCursor::GameCursor()
	: GameObject(GAME_TEXTURE::CURSOR_TEXTURE,
				 sf::Vector2f(0,0),
				 sf::Vector2i(GameOptions::CELL_SIZE,
						  GameOptions::CELL_SIZE),
				 2)
	,m_state(NORMAL)
	,m_inPanel(false)
	,m_highlight(false)
{
	m_cell = sf::Vector2i(0, 0);
	m_maxCell = sf::Vector2i(30, 17);
}

void GameCursor::setMaxCells(int maxWidth, int maxHeight)
{
	setMaxCells(sf::Vector2i(maxWidth, maxHeight));
}

void GameCursor::setMaxCells(const sf::Vector2i &maxCell)
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
		if (m_cell.x >= m_maxCell.x)
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
		return Engine::Instance().options<GameOptions>()->panel()->moveCursorLeft();

	moveLeftCursor();
}

void GameCursor::moveRight()
{
	if (m_inPanel)
		return Engine::Instance().options<GameOptions>()->panel()->moveCursorRight();

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

sf::Vector2i GameCursor::cell() const
{
	return m_cell;
}

sf::Vector2f GameCursor::pixelPos() const
{
	return sf::Vector2f(m_cell.x * Engine::Instance().options<GameOptions>()->tileSize().x,
					m_cell.y * Engine::Instance().options<GameOptions>()->tileSize().y);
}

void GameCursor::draw(sf::RenderTarget * const target)
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
	if (checkBordersTimer.check(50))
		checkBorders();
}

void GameCursor::activateAbility(int x, int y, int hotX, int hotY)
{
	abilityHotsPot.x = hotX;
	abilityHotsPot.y = hotY;

	abilityRect.setFillColor(sf::Color(255, 0, 0, 100));
	abilityRect.setSize(sf::Vector2f(Engine::Instance().options<GameOptions>()->tileSize().x * x,
								 Engine::Instance().options<GameOptions>()->tileSize().y * y));
	m_state = ABILITY;
}

void GameCursor::deactivate()
{
	m_state = NORMAL;
	abilityRect.setFillColor(sf::Color(200, 200, 200, 100));
	abilityHotsPot.x = 0;
	abilityHotsPot.y = 0;
	abilityRect.setSize(sf::Vector2f(Engine::Instance().options<GameOptions>()->tileSize().x,
								 Engine::Instance().options<GameOptions>()->tileSize().y));
}

void GameCursor::activateTower(float radius, TOWER_TYPES type)
{
	abilityHotsPot.x = 0;
	abilityHotsPot.y = 0;

	if (type == POWER)
		towerRect.setSize(sf::Vector2f(radius, radius));
	else
		towerRadius.setRadius(radius);

	towerRect.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	towerRadius.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	towerType = type;

	TextureType textureType = GAME_TEXTURE::TOWER_POWER;;
	switch (towerType)
	{
	case BASE:
		textureType = GAME_TEXTURE::TOWER_BASE;
		break;
	case POWER:
		textureType = GAME_TEXTURE::TOWER_POWER;
		break;
	case ROCKET:
		textureType = GAME_TEXTURE::TOWER_ROCKET;
		break;
	case FREEZE:
		textureType = GAME_TEXTURE::TOWER_FREEZE;
		break;
	case LASER:
		textureType = GAME_TEXTURE::TOWER_LASER;
		break;
	case IMPROVED:
		textureType = GAME_TEXTURE::TOWER_IMPROVED;
		break;
	}

	towerSprite.setTexture(Engine::Instance().texturesManager()->getTexture(textureType));
	towerSprite.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	towerSprite.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);
	towerSprite.setColor(sf::Color(255, 255, 255, 128));
	towerSprite.setTextureRect(sf::IntRect(0, 0,
										   GameOptions::CELL_SIZE / Tower::TOWER_SCAlE,
										   GameOptions::CELL_SIZE / Tower::TOWER_SCAlE));
	m_state = TOWER;

	updateCell();
}

sf::FloatRect GameCursor::getAbilityRect() const
{
	return abilityRect.getGlobalBounds();
}

void GameCursor::swap()
{
	m_inPanel = !m_inPanel;
	Engine::Instance().options<GameOptions>()->panel()->updateCursor();
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
	const sf::Vector2i pixelPos = sf::Mouse::getPosition(*Engine::Instance().window());
	const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																		   *Engine::Instance().options<GameOptions>()->camera()->getView())
			+ sf::Vector2f(1, 1);
	const sf::Vector2i newCell = Engine::Instance().options<GameOptions>()->camera()->posToCell(pos);
	if (newCell != m_cell)
	{
		m_cell = newCell;
		updateCell();
	}
}

void GameCursor::updatePanel()
{
	m_inPanel = windowScreenPos().y > Engine::Instance().options<GameOptions>()->panel()->getBottomValue();
	Engine::Instance().options<GameOptions>()->panel()->updateCursor();
}

sf::Vector2f GameCursor::windowScreenPos() const
{
	const sf::Vector2i pixelPos = sf::Mouse::getPosition(*Engine::Instance().window());
	const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																		   *Engine::Instance().window()->view());
	return pos;
}

void GameCursor::updateCell()
{
	const sf::Vector2f pos = sf::Vector2f(Engine::Instance().options<GameOptions>()->tileSize().x * m_cell.x,
								  Engine::Instance().options<GameOptions>()->tileSize().y * m_cell.y) -
			sf::Vector2f(abilityHotsPot.x * Engine::Instance().options<GameOptions>()->mapTileSize().x,
					 abilityHotsPot.y * Engine::Instance().options<GameOptions>()->mapTileSize().y);
	if (m_state == TOWER)
	{
		const bool canCreate = Engine::Instance().options<GameOptions>()->level()->canAddTower(
					sf::Vector2i(m_cell.x * 2, m_cell.y * 2), towerType == TOWER_TYPES::POWER);
		towerRadius.setFillColor(canCreate ? TOWER_AREA_COLOR : INACTIVE_TOWER_AREA_COLOR);
		towerRect.setFillColor(canCreate ? PowerTower::POWER_TOWER_AREA_COLOR : INACTIVE_TOWER_AREA_COLOR);
	}
	setPos(pos);
	towerSprite.setPosition(pos);
	abilityRect.setPosition(pos);
	towerRadius.setPosition(pos);	
	towerRadius.setOrigin(towerRadius.getRadius() - GameOptions::MAP_CELL_SIZE,
						  towerRadius.getRadius() - GameOptions::MAP_CELL_SIZE);

	towerRect.setPosition(pos - Engine::Instance().options<GameOptions>()->tileSize());

	Engine::Instance().options<GameOptions>()->panel()->updateInfo();
}

void GameCursor::updateMousePos()
{
	const sf::Vector2f pos = Engine::Instance().options<GameOptions>()->camera()->cellToPos(m_cell)
			+ sf::Vector2f(Engine::Instance().options<GameOptions>()->tileSize().x/2,
						   Engine::Instance().options<GameOptions>()->tileSize().y/2);

	const sf::Vector2i coords = Engine::Instance().window()->mapCoordsToPixel(pos, *Engine::Instance().options<GameOptions>()->camera()->getView());
	sf::Mouse::setPosition(coords, *Engine::Instance().window());
}

void GameCursor::checkBorders()
{
	const sf::Vector2i pixelPos = sf::Mouse::getPosition(*Engine::Instance().window());
	const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(
				pixelPos, *Engine::Instance().options<GameOptions>()->camera()->getView()) + sf::Vector2f(1, 1);
	const sf::Vector2i cell = sf::Vector2i(pos.x/Engine::Instance().options<GameOptions>()->tileSize().x,
								   pos.y/Engine::Instance().options<GameOptions>()->tileSize().y);

	if (cell.x <= Engine::Instance().options<GameOptions>()->camera()->viewLeftCell() && !m_inPanel)
		moveLeftCursor();
	if (cell.x >= Engine::Instance().options<GameOptions>()->camera()->viewRightCell() && !m_inPanel)
		moveRightCursor();
	if (cell.y <= Engine::Instance().options<GameOptions>()->camera()->viewTopCell())
		moveUpCursor();
	if (cell.y >= Engine::Instance().options<GameOptions>()->camera()->viewBottomCell())
		moveDownCursor();	
}

void GameCursor::moveDownCursor()
{
	if (!canMove(GameCursor::MOVE_DOWN))
		return;

	m_cell.y++;
	if (Engine::Instance().options<GameOptions>()->camera()->viewBottomCell() != m_maxCell.y
			+ Engine::Instance().options<GameOptions>()->panel()->cellsCount()
			&& m_cell.y > Engine::Instance().options<GameOptions>()->camera()->viewCenter().y)
		Engine::Instance().options<GameOptions>()->camera()->moveDownByCell();
	else
		updateMousePos();
	updateCell();
}

void GameCursor::moveUpCursor()
{
	if (!canMove(GameCursor::MOVE_UP))
		return;

	m_cell.y--;
	if (Engine::Instance().options<GameOptions>()->camera()->viewTopCell() != 0 &&
			m_cell.y < Engine::Instance().options<GameOptions>()->camera()->viewCenter().y)
		Engine::Instance().options<GameOptions>()->camera()->moveUpByCell();	
	else
		updateMousePos();

	updateCell();
}

void GameCursor::moveLeftCursor()
{
	if (!canMove(GameCursor::MOVE_LEFT))
		return;

	m_cell.x--;
	if (Engine::Instance().options<GameOptions>()->camera()->viewLeftCell() != 0 &&
			m_cell.x < Engine::Instance().options<GameOptions>()->camera()->viewCenter().x)
		Engine::Instance().options<GameOptions>()->camera()->moveLeftByCell();
	else
		updateMousePos();
	updateCell();
}

void GameCursor::moveRightCursor()
{
	if (!canMove(GameCursor::MOVE_RIGHT))
		return;

	m_cell.x++;
	if (Engine::Instance().options<GameOptions>()->camera()->viewRightCell() != m_maxCell.x &&
			m_cell.x > Engine::Instance().options<GameOptions>()->camera()->viewCenter().x)
		Engine::Instance().options<GameOptions>()->camera()->moveRightByCell();
	else
		updateMousePos();
	updateCell();
}

void GameCursor::setHighlight(bool highlight)
{
	m_highlight = highlight;
}

sf::Vector2i GameCursor::getMaxCell() const
{
	return m_maxCell;
}

