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

	/// grid cells count
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

	void checkGrid();
	void setAutoUpdateGrid(bool autoUpdate);

protected:
	void update() override;

	void updatePos() override;
	void updateScale() override;
	void updateOrigin() override;
	void updateSize() override;

private:
	class Private;
	Private *d = nullptr;
};

#endif // NAVIGATIONMAP_H
