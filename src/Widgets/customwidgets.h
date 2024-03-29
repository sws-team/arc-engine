#ifndef CUSTOMWIDGETS_H
#define CUSTOMWIDGETS_H

#include "arcobject.h"

class CircleScale : public ArcObject
{
public:
	CircleScale(const std::string& name, TextureType texture, unsigned sectors = 100);

	unsigned value() const;
	void setValue(unsigned value);
protected:
	void initTransform() override;
private:
	unsigned m_value = 50;
	unsigned m_sectors = 100;
};

class PathObject : public ArcObject
{
public:
	PathObject(const std::string& name);

	std::vector<sf::Vector2f> path() const;

	void draw(sf::RenderTarget *const target) override;

	void setPath(const std::vector<sf::Vector2f> &path);

private:
#ifdef ARC_DEBUG
	void debug();
	bool m_debug = true;
	float radius = 5;
	sf::Color m_color = sf::Color::Red;
	sf::Color m_pointColor = sf::Color::Red;
	friend class ArcDebug;
#endif
	std::vector<sf::Vector2f> m_path;
};

class ZoomView : public ArcObject
{
public:
	ZoomView(const std::string& name);

	void setView(const sf::Vector2f& pos);
	void setView(const float x, const float y);
	sf::Vector2f viewPos() const;

	void zoomIn();
	void zoomOut();

	void draw(sf::RenderTarget *const target) override;
	bool eventFilter(sf::Event *event) override;

	sf::View *getView();
private:
	sf::View view;
	bool move = false;
	sf::Vector2i startMovePos;
	int zoomRatio = 0;
	float zoomFactor = 1;

	constexpr static int MAX_ZOOM = 20;
	constexpr static float ZOOM_STEP = 0.95f;

	void fitScreen();
};

class GridMagnetizer : public ArcObject
{
public:
	GridMagnetizer(const std::string& name);

	void setGrid(int rows, int columns);
	void setHighlightEnabled(bool enabled);

	bool isHighlightEnabled() const;

	sf::Vector2f posAt(int row, int column) const;
	std::optional<sf::Vector2f> nearestPoint(const sf::Vector2f& pos) const;
protected:
	void updateSize() override;
private:
	void updateGrid();
	void magnetize(const std::string &name, ArcObject *object, const std::vector<ArcVariant> &args);
	void highlight(const std::string &name, ArcObject *object, const std::vector<ArcVariant> &args);

	bool highlightEnabled = false;
	int rows = 0;
	int columns = 0;
	sf::Vector2f cell;

	std::vector<sf::Vector2f> grid;

	ArcRect *highlightRect = nullptr;
};

#endif // CUSTOMWIDGETS_H
