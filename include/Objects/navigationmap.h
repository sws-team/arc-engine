#ifndef NAVIGATIONMAP_H
#define NAVIGATIONMAP_H

#include "arcobject.h"

class NavigationMap : public ArcObject
{
public:
	NavigationMap(const std::string& name);
	~NavigationMap() override;

	enum ELEMENT_TYPE {
		STATIC_OBJECT,
		DYNAMIC_OBJECT,
		NON_SOLID
	};

#ifdef ARC_DEBUG
	bool debug = true;
#endif

	unsigned cells() const;
	void setCells(unsigned cells);

	void draw(sf::RenderTarget *const target) override;

	std::vector<sf::Vector2f> findPath(ArcObject *object, const sf::Vector2f& targetPos);
	sf::Vector2u cell(const sf::Vector2f& pos) const;
	sf::Vector2f pos(const sf::Vector2u& cell) const;

	void addChild(ArcObject* object) override;
//	void removeChild(ArcObject* object) override;
	void addElement(ArcObject* object, ELEMENT_TYPE type);

	void addFreeZone(const sf::FloatRect& rect);

protected:
	void update() override;

	void updatePos() override;
	void updateScale() override;
	void updateOrigin() override;
	void updateSize() override;

private:
	void updateGrid();
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
	void cache();
	void fillGrid(Grid* grid, const ArcObject* object) const;

	void checkGrid();

	sf::Thread *thread = nullptr;
	std::atomic<bool> processing = false;
	std::atomic<bool> active = true;
	sf::Int32 checkTime = 75;
	struct ElementData {
		ELEMENT_TYPE type = STATIC_OBJECT;
		bool checked = false;
	};
	sf::Mutex mutex;
	std::map<ArcObject*, ElementData> elements;
	std::vector<std::pair<ArcObject*, ELEMENT_TYPE>> queuedObjects;
	void addElements();
	std::vector<sf::FloatRect> freeZones;
};

#endif // NAVIGATIONMAP_H
