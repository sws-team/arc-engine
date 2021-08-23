#ifndef ARCOBJECT_H
#define ARCOBJECT_H

#include "engine.h"
#include "stdheader.h"

class ArcObject
{
public:
	ArcObject(const std::string& name = std::string());
	virtual ~ArcObject();
	virtual void draw(sf::RenderTarget *const target);
	virtual void update();

	//getters
	std::string name() const;
	sf::Vector2f size() const;
	float width() const;
	float height() const;
	sf::Vector2f scale() const;

	//setters
	void setPos(const sf::Vector2f& coords);
	virtual void setPos(float x, float y);

	void setOrigin(const sf::Vector2f& coords);
	virtual void setOrigin(float x, float y);

	void setScale(const sf::Vector2f& scale);
	virtual void setScale(float x, float y);


protected:
	virtual bool eventFilter(sf::Event *event);

	std::vector<ArcObject*> childs;
private:
	//transform
	friend class ArcDebug;
	std::string m_name;
	float m_x = 0.f;
	float m_y = 0.f;
	float m_angle = 0.f;
	float m_scaleX = 1.f;
	float m_scaleY = 1.f;
	float m_originX = 0.f;
	float m_originY = 0.f;
	float m_width = 0.f;
	float m_height = 0.f;
};

#endif // ARCOBJECT_H
