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
		ArcObject *object = nullptr;
		bool isBlocked = false;
	};
	struct Grid {
		inline bool operator()(unsigned x, unsigned y) const;
		std::vector<std::vector<Rect>> grid;
		unsigned cells = 10;

		float cellWidth = 0;
		float cellHeight = 0;
	};
	Grid grid;
	Grid makeGrid(const Grid& grid, const ArcObject* object) const;

	void checkGrid();
};

#endif // NAVIGATIONMAP_H
