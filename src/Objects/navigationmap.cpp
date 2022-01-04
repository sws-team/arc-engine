#include "navigationmap.h"
#include "collisions.h"
#include "jps.hh"

NavigationMap::NavigationMap(const std::string &name)
	: ArcObject(name)
{
	setType(ArcEngine::NAVIGATION_MAP);
}

unsigned NavigationMap::cells() const
{
	return grid.cells;
}

void NavigationMap::setCells(unsigned cells)
{
	if (grid.cells == cells)
		return;
	grid.cells = cells;
	updateGrid();
}

void NavigationMap::updatePos()
{
	ArcObject::updatePos();
	updateGrid();
}

void NavigationMap::updateScale()
{
	ArcObject::updateScale();
	updateGrid();
}

void NavigationMap::updateOrigin()
{
	ArcObject::updateOrigin();
	updateGrid();
}

void NavigationMap::updateSize()
{
	ArcObject::updateSize();
	updateGrid();
}

void NavigationMap::draw(sf::RenderTarget * const target)
{
#ifdef ARC_DEBUG
	if (!debug)
#endif
	{
		ArcObject::draw(target);
		return;
	}
	for(const std::vector<Rect>& rects : grid.grid) {
		for(const Rect& rect : rects) {
			sf::RectangleShape rectangle;
			rectangle.setSize(rect.rect.getSize());
			rectangle.setPosition(rect.rect.getPosition());
			rectangle.setFillColor(rect.isBlocked ? sf::Color::Red : sf::Color::Green);
			rectangle.setOutlineThickness(1.f);
			rectangle.setOutlineColor(sf::Color::White);
			target->draw(rectangle, m_transform);
		}
	}

	ArcObject::draw(target);
}

std::vector<sf::Vector2f> NavigationMap::findPath(ArcObject *object, const sf::Vector2f &targetPos) const
{
	const sf::Vector2u objectCell = cell(object->pos());
	const sf::Vector2u targetCell = cell(targetPos);
	JPS::PathVector path;
	const Grid objectGrid = makeGrid(grid, object);
	JPS::findPath(path, objectGrid, objectCell.x, objectCell.y,
									 targetCell.x, targetCell.y, 1u);
	std::vector<sf::Vector2f> result;
	for(const JPS::Position& pos : path) {
		result.push_back(this->pos(sf::Vector2u(pos.x, pos.y)));
	}
	return result;
}

sf::Vector2u NavigationMap::cell(const sf::Vector2f &pos) const
{
	sf::Vector2u result;
	result.x = static_cast<unsigned>(pos.x / grid.cellWidth);
	result.y = static_cast<unsigned>(pos.y / grid.cellHeight);
	return result;
}

sf::Vector2f NavigationMap::pos(const sf::Vector2u &cell) const
{
	sf::Vector2f result;
	result.x = cell.x * grid.cellWidth + grid.cellWidth/2;
	result.y = cell.y * grid.cellHeight + grid.cellHeight/2;
	return result;
}

void NavigationMap::update()
{
	ArcObject::update();
	for(std::vector<Rect>& rects : grid.grid) {
		for(Rect& rect : rects) {
			rect.isBlocked = false;
			rect.object = nullptr;
		}
	}
	for(ArcObject *child : m_childs) {
		for(std::vector<Rect>& rects : grid.grid) {
			for(Rect& rect : rects) {
				if (rect.isBlocked)
					continue;
				rect.isBlocked = Intersection::intersects(child, rect.rect);
				if (rect.isBlocked)
					rect.object = child;
			}
		}
	}
}

void NavigationMap::updateGrid()
{
	grid.grid.clear();
	const float width = size().x;
	const float height = size().y;

	grid.cellWidth = width / static_cast<float>(grid.cells);
	grid.cellHeight = height / static_cast<float>(grid.cells);

	const float cellWidth = grid.cellWidth * scaledGlobalScale().x;
	const float cellHeight = grid.cellHeight * scaledGlobalScale().y;

	const sf::Vector2f startPos = ArcObject::scaledGlobalPos();
	sf::Vector2f pos = startPos;
	for (unsigned row = 0; row < grid.cells; ++row) {
		std::vector<Rect> rects;
		for (unsigned column = 0; column < grid.cells; ++column) {
			Rect rect;
			rect.rect = sf::FloatRect(pos.x, pos.y, cellWidth, cellHeight);
			rects.push_back(rect);
			pos.x += cellWidth;
		}
		grid.grid.push_back(rects);
		pos.x = startPos.x;
		pos.y += cellHeight;
	}
}

NavigationMap::Grid NavigationMap::makeGrid(const NavigationMap::Grid &grid, const ArcObject *object) const
{
	std::vector<sf::Vector2u> blocked;
	Grid result = grid;
	for (unsigned row = 0; row < result.grid.size(); ++row) {
		std::vector<Rect>& rects = result.grid[row];
		for (unsigned column = 0; column < rects.size(); ++column) {
			Rect& rect = rects[column];
			if (rect.object == object)
				rect.isBlocked = false;
			if (rect.isBlocked)
				blocked.push_back(sf::Vector2u(row, column));
		}
	}

	const sf::Vector2u objectSize = cell(object->size());
	sf::Vector2u objectObstacleSize;
	objectObstacleSize.x = static_cast<unsigned>(std::ceil(static_cast<float>(objectSize.y)/2.f));
	objectObstacleSize.y = static_cast<unsigned>(std::ceil(static_cast<float>(objectSize.x)/2.f));
	for(const sf::Vector2u& cell : blocked) {
		const unsigned startRow = std::max(0u, cell.x - objectObstacleSize.x);
		const unsigned endRow = std::min(static_cast<unsigned>(result.grid.size()), cell.x + objectObstacleSize.x);
		for (unsigned kRow = startRow; kRow < endRow; ++kRow) {
			std::vector<Rect>& kRects = result.grid[kRow];
			const unsigned startColumn = std::max(0u, cell.y - objectObstacleSize.y);
			const unsigned endColumn = std::min(static_cast<unsigned>(kRects.size()), cell.y + objectObstacleSize.y);
			for (unsigned kColumn = startColumn; kColumn < endColumn; ++kColumn) {
				Rect& kRect = kRects[kColumn];
				kRect.isBlocked = true;
			}
		}
	}

	for(std::vector<Rect>& rects : result.grid) {
		for(Rect& rect : rects) {
			if (rect.object == object)
				rect.isBlocked = false;
		}
	}
	return result;
}

bool NavigationMap::Grid::operator()(unsigned x, unsigned y) const
{
	if(x >= cells || y >= cells)
		return false;

	return !grid.at(y).at(x).isBlocked;
}
