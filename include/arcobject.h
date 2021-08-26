#ifndef ARCOBJECT_H
#define ARCOBJECT_H

#include "stdheader.h"
#include "enginedef.h"

class ArcAction;

class ArcObject
{
public:
	ArcObject(const std::string& name = std::string(), ArcEngine::OBJECT_TYPE type = ArcEngine::UNDEF);
	virtual ~ArcObject();

	void paint(sf::RenderTarget *const target);
	void process();
	bool event(sf::Event *event);

	void addChild(ArcObject* object);
	void addAction(ArcAction* action);

	//getters
	bool isEnabled() const;
	ArcEngine::OBJECT_TYPE type() const;
	std::string name() const;
	sf::Vector2f size() const;
	float width() const;
	float height() const;
	float scaleX() const;
	float scaleY() const;
	sf::Vector2f scale() const;
	float x() const;
	float y() const;
	sf::Vector2f pos() const;
	float originX() const;
	float originY() const;
	sf::Vector2f origin() const;
	float rotation() const;

	//setters
	void setPos(const sf::Vector2f& coords);
	void setOrigin(const sf::Vector2f& coords);
	void setScale(const sf::Vector2f& scale);
	void setSize(const sf::Vector2f& size);
	virtual void setRotation(float angle);
	void setEnabled(bool enabled);

protected:
	virtual void update();
	virtual void draw(sf::RenderTarget *const target);
	virtual bool eventFilter(sf::Event *event);
	void setName(const std::string& name);
	void setType(ArcEngine::OBJECT_TYPE type);

	virtual void setPos(float x, float y);
	virtual void setOrigin(float x, float y);
	virtual void setScale(float x, float y);
	virtual void setSize(float x, float y);

	std::vector<ArcObject*> childs;
	std::vector<ArcAction*> actions;
	sf::Vector2f scaleFactor;
private:
	//transform
	friend class ArcDebug;
	friend class ArcAction;
	std::string m_name;
	bool m_enabled = true;
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
