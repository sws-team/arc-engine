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

class CustomWidgets
{
public:
	static ArcObject* createLoadingIcon();
private:
	CustomWidgets();
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

#endif // CUSTOMWIDGETS_H
