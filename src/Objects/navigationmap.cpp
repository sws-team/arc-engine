#include "navigationmap.h"
#include "collisions.h"
#include "jps.hh"

class NavigationMap::Private
{
public:
	NavigationMap *q = nullptr;

	struct Rect {
		sf::FloatRect rect;
		ArcObject* object = nullptr;
		bool isBlocked = false;
		bool isStatic = false;
	};
	struct Grid {
		Grid() = default;
		inline bool operator()(unsigned x, unsigned y) const;
		std::vector<std::vector<Rect>> grid;
		unsigned cells = 10;

		float cellWidth = 0;
		float cellHeight = 0;
	};
	Grid grid;
	Grid cachedGrid;

	sf::Thread *thread = nullptr;
	std::atomic<bool> processing = false;
	std::atomic<bool> active = true;
	sf::Int32 checkTime = 75;
	sf::Mutex mutex;

	struct ElementData {
		ELEMENT_TYPE type = STATIC_OBJECT;
		bool checked = false;
	};
	std::map<ArcObject*, ElementData> elements;
	std::vector<std::pair<ArcObject*, ELEMENT_TYPE>> queuedObjects;
	std::vector<sf::FloatRect> freeZones;
	bool autoUpdateGrid = false;

	void cache();
	void updateGrid();
	void fillGrid(Grid* grid, const ArcObject* object) const;
	void processUpdating();
	void startProcessing();
	void stopProcessing();

	void addElements();
};

NavigationMap::NavigationMap(const std::string &name)
	: ArcObject(name)
	, d(new Private)
{
	setType(ArcEngine::NAVIGATION_MAP);
	d->q = this;
}

NavigationMap::~NavigationMap()
{
	if (d->autoUpdateGrid)
		d->stopProcessing();
	delete d;
}

unsigned NavigationMap::cells() const
{
	return d->cachedGrid.cells;
}

void NavigationMap::setCells(unsigned cells)
{
	if (d->grid.cells == cells)
		return;
	d->grid.cells = cells;
	d->cachedGrid.cells = cells;
	d->updateGrid();
}

void NavigationMap::updatePos()
{
	ArcObject::updatePos();
	d->updateGrid();
}

void NavigationMap::updateScale()
{
	ArcObject::updateScale();
	d->updateGrid();
}

void NavigationMap::updateOrigin()
{
	ArcObject::updateOrigin();
	d->updateGrid();
}

void NavigationMap::updateSize()
{
	ArcObject::updateSize();
	d->updateGrid();
}

void NavigationMap::draw(sf::RenderTarget * const target)
{
#ifdef ARC_DEBUG
	if (!debug) {
		ArcObject::draw(target);
		return;
	}
	for(const std::vector<Private::Rect>& rects : d->cachedGrid.grid) {
		for(const Private::Rect& rect : rects) {
			sf::RectangleShape rectangle;
			rectangle.setSize(rect.rect.getSize());
			rectangle.setPosition(rect.rect.getPosition());
			rectangle.setFillColor(rect.isBlocked ? sf::Color::Red : sf::Color::Green);
			rectangle.setOutlineThickness(1.f);
			rectangle.setOutlineColor(sf::Color::White);
			target->draw(rectangle, m_transform);
		}
	}
	for(const sf::FloatRect& rect : d->freeZones) {
		sf::RectangleShape rectangle;
		rectangle.setSize(rect.getSize());
		rectangle.setPosition(rect.getPosition());
		rectangle.setFillColor(sf::Color(25, 100, 200, 100));
		rectangle.setOutlineThickness(1.f);
		rectangle.setOutlineColor(sf::Color::White);
		target->draw(rectangle, m_transform);
	}
#endif
	ArcObject::draw(target);
}

std::vector<sf::Vector2f> NavigationMap::findPath(ArcObject *object, const sf::Vector2f &targetPos)
{
	JPS::PathVector path;
	const sf::Vector2u objectCell = cell(object->pos());
	const sf::Vector2u targetCell = cell(targetPos);
	d->mutex.lock();
	Private::Grid objectGrid = d->cachedGrid;
	d->mutex.unlock();
	d->fillGrid(&objectGrid, object);
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
	result.x = static_cast<unsigned>(pos.x / d->cachedGrid.cellWidth);
	result.y = static_cast<unsigned>(pos.y / d->cachedGrid.cellHeight);
	return result;
}

sf::Vector2f NavigationMap::pos(const sf::Vector2u &cell) const
{
	sf::Vector2f result;
	result.x = cell.x * d->cachedGrid.cellWidth + d->cachedGrid.cellWidth/2;
	result.y = cell.y * d->cachedGrid.cellHeight + d->cachedGrid.cellHeight/2;
	return result;
}

void NavigationMap::addChild(ArcObject *object)
{
	addElement(object, STATIC_OBJECT);
}

void NavigationMap::addElement(ArcObject *object, ELEMENT_TYPE type)
{
	object->setParent(this);
	d->queuedObjects.push_back(std::pair<ArcObject*, ELEMENT_TYPE>(object, type));
}

void NavigationMap::addFreeZone(const sf::FloatRect &rect)
{
	d->freeZones.push_back(rect);
}

void NavigationMap::checkGrid()
{
	for(std::vector<Private::Rect>& rects : d->grid.grid) {
		for(Private::Rect& rect : rects) {
			if (rect.isStatic)
				continue;
			rect.isBlocked = false;
			rect.object = nullptr;
		}
	}
	for(auto& element : d->elements) {
		switch (element.second.type)
		{
		case NON_SOLID:
			continue;
		case STATIC_OBJECT:
		{
			if (element.second.checked)
				break;
		}
		case DYNAMIC_OBJECT:
		{
			for(std::vector<Private::Rect>& rects : d->grid.grid) {
				for(Private::Rect& rect : rects) {
					if (!rect.isBlocked) {
						rect.isBlocked = Intersection::intersects(element.first, rect.rect);
						if (rect.isBlocked) {
							rect.object = element.first;
							rect.isStatic = element.second.type != DYNAMIC_OBJECT;
						}
					}
				}
			}
			element.second.checked = true;
		}
			break;
		default:
			break;
		}
	}
	std::vector<sf::FloatRect> zones = d->freeZones;
	for(const sf::FloatRect& zone : zones) {
		for(std::vector<Private::Rect>& rects : d->grid.grid) {
			for(Private::Rect& rect : rects) {
				if (rect.rect.intersects(zone)) {
					rect.isBlocked = false;
				}
			}
		}
	}
	d->cache();
}

void NavigationMap::setAutoUpdateGrid(bool autoUpdate)
{
	if (d->autoUpdateGrid == autoUpdate)
		return;
	d->autoUpdateGrid = autoUpdate;
	if (d->autoUpdateGrid)
		d->startProcessing();
	else
		d->stopProcessing();
}

void NavigationMap::update()
{
	if (!d->queuedObjects.empty()) {
		d->addElements();
	}
	ArcObject::update();
}

void NavigationMap::Private::cache() {
	sf::Lock lock(mutex);
	cachedGrid = grid;
}

void NavigationMap::Private::updateGrid()
{
	processing.store(false);

	grid.grid.clear();
	const float width = q->size().x;
	const float height = q->size().y;

	grid.cellWidth = width / static_cast<float>(grid.cells);
	grid.cellHeight = height / static_cast<float>(grid.cells);

	const float cellWidth = grid.cellWidth * q->scaledGlobalScale().x;
	const float cellHeight = grid.cellHeight * q->scaledGlobalScale().y;

	const sf::Vector2f startPos = q->ArcObject::scaledGlobalPos();
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
	cache();

	processing.store(true);
}


void NavigationMap::Private::fillGrid(Grid* grid, const ArcObject *object) const
{
	std::vector<sf::Vector2u> blocked;
	for (unsigned row = 0; row < grid->grid.size(); ++row) {
		std::vector<Rect>& rects = grid->grid[row];
		for (unsigned column = 0; column < rects.size(); ++column) {
			Rect& rect = rects[column];
			if (rect.object == object) {
				rect.isBlocked = false;
			}
			if (rect.isBlocked)
				blocked.push_back(sf::Vector2u(row, column));
		}
	}

	const sf::Vector2u objectSize = q->cell(object->size());
	sf::Vector2u objectObstacleSize;
	objectObstacleSize.x = static_cast<unsigned>(std::ceil(static_cast<float>(objectSize.y)/2.f));
	objectObstacleSize.y = static_cast<unsigned>(std::ceil(static_cast<float>(objectSize.x)/2.f));
	for(const sf::Vector2u& cell : blocked) {
		const unsigned startRow = std::max(0u, cell.x - objectObstacleSize.x);
		const unsigned endRow = std::min(static_cast<unsigned>(grid->grid.size()), cell.x + objectObstacleSize.x);
		for (unsigned kRow = startRow; kRow < endRow; ++kRow) {
			std::vector<Rect>& kRects = grid->grid[kRow];
			const unsigned startColumn = std::max(0u, cell.y - objectObstacleSize.y);
			const unsigned endColumn = std::min(static_cast<unsigned>(kRects.size()), cell.y + objectObstacleSize.y);
			for (unsigned kColumn = startColumn; kColumn < endColumn; ++kColumn) {
				Rect& kRect = kRects[kColumn];
				kRect.isBlocked = true;
			}
		}
	}

	for(std::vector<Rect>& rects : grid->grid) {
		for(Rect& rect : rects) {
			if (rect.object == object)
				rect.isBlocked = false;
		}
	}
}

void NavigationMap::Private::processUpdating()
{
	while (active.load()) {
		if (!processing.load())
			continue;
		q->checkGrid();
		sf::sleep(sf::milliseconds(checkTime));
	}
}

void NavigationMap::Private::addElements()
{
	for(const auto& element : queuedObjects) {
		q->ArcObject::addChild(element.first);
		ElementData data;
		data.type = element.second;
		elements.insert(std::pair<ArcObject*, ElementData>(element.first, data));
	}
	queuedObjects.clear();
}

void NavigationMap::Private::startProcessing()
{
	thread = new sf::Thread(std::bind(&NavigationMap::Private::processUpdating, this));
	thread->launch();
}

void NavigationMap::Private::stopProcessing()
{
	if (thread == nullptr)
		return;
	processing.store(false);
	active.store(false);
	thread->wait();
	delete thread;
}

bool NavigationMap::Private::Grid::operator()(unsigned x, unsigned y) const
{
	if(x >= cells || y >= cells)
		return false;

	return !grid.at(y).at(x).isBlocked;
}
