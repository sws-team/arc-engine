#ifndef ARCOBJECT_H
#define ARCOBJECT_H

#include "stdheader.h"
#include "enginedef.h"

class ArcObject
{
public:
	ArcObject(const std::string& name = std::string(), ArcEngine::OBJECT_TYPE type = ArcEngine::UNDEF);
	virtual ~ArcObject();

	virtual void draw(sf::RenderTarget *const target);
	virtual void update();

	void addChild(ArcObject* object);

	//getters
	ArcEngine::OBJECT_TYPE type() const;
	std::string name() const;
	sf::Vector2f size() const;
	float width() const;
	float height() const;
	sf::Vector2f scale() const;
	float x() const;
	float y() const;
	sf::Vector2f pos() const;

	//setters
	void setPos(const sf::Vector2f& coords);
	virtual void setPos(float x, float y);

	void setOrigin(const sf::Vector2f& coords);
	virtual void setOrigin(float x, float y);

	void setScale(const sf::Vector2f& scale);
	virtual void setScale(float x, float y);

	void setSize(const sf::Vector2f& size);
	virtual void setSize(float x, float y);

protected:
	virtual bool eventFilter(sf::Event *event);
	void setName(const std::string& name);
	void setType(ArcEngine::OBJECT_TYPE type);

	std::vector<ArcObject*> childs;
	sf::Vector2f scaleFactor;
private:
	//transform
	friend class ArcDebug;
	std::string m_name;
	ArcEngine::OBJECT_TYPE m_type = ArcEngine::UNDEF;
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
