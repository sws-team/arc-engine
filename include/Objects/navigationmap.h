#ifndef NAVIGATIONMAP_H
#define NAVIGATIONMAP_H

#include "arcobject.h"

class NavigationMap : public ArcObject
{
public:
	NavigationMap(const std::string& name);
	~NavigationMap() override;

#ifdef ARC_DEBUG
	bool debug = true;
#endif

	unsigned cells() const;
	void setCells(unsigned cells);

	void draw(sf::RenderTarget *const target) override;

	std::vector<sf::Vector2f> findPath(ArcObject *object, const sf::Vector2f& targetPos) const;
	sf::Vector2u cell(const sf::Vector2f& pos) const;
	sf::Vector2f pos(const sf::Vector2u& cell) const;

	void addElement(ArcObject* object);

protected:
	void update() override;

	void updatePos() override;
	void updateScale() override;
	void updateOrigin() override;
	void updateSize() override;

private:
	void updateGrid();
	void addElements();
	std::vector<ArcObject*> elementsToAdd;
	struct Rect {
		sf::FloatRect rect;
		ArcObject* object = nullptr;
		bool isBlocked = false;
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
	std::vector<std::vector<Rect>> cachedGrid;
	void cache() {
		cachedGrid = grid.grid;
	}
	Grid makeGrid(Grid grid, const ArcObject* object) const;

	void checkGrid();

	sf::Thread *thread = nullptr;
	std::atomic<bool> processing = false;
	std::atomic<bool> active = true;
	sf::Int32 checkTime = 50;
};

#endif // NAVIGATIONMAP_H
