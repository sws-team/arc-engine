#ifndef ARCOBJECT_H
#define ARCOBJECT_H

#include "stdheader.h"
#include "enginedef.h"
#include <any>

class ArcAction;

class ArcObject
{
public:
	ArcObject(const std::string& name);
	virtual ~ArcObject();

	virtual void init();
	void paint(sf::RenderTarget *const target);
	void process();
	bool event(sf::Event *event);

	bool hasChild(ArcObject* object, bool recursively = true) const;
	ArcObject *findChild(const std::string& name, bool recursively = true);
	ArcObject *parent();
	std::vector<ArcObject*> childs() const;

	void setParent(ArcObject* parent);
	void addChild(ArcObject* object);
	void addAction(ArcAction* action);
	void insertChild(int pos, ArcObject* object);

	void removeChild(ArcObject* object);
	void removeChild(const std::string& name);
	void removeAction(ArcAction *action);

	//getters
	bool isEnabled() const;
	ArcEngine::OBJECT_TYPE type() const;
	std::string name() const;
	sf::Vector2f size() const;
	sf::Vector2f scaledSize() const;
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
	void setX(float x);
	void setY(float y);
	void setPos(const sf::Vector2f& coords);
	void setPos(float x, float y);
	void setOrigin(const sf::Vector2f& coords);
	void setOrigin(float x, float y);
	void setScale(const sf::Vector2f& scale);
	void setScale(float x, float y);
	void setSize(const sf::Vector2f& size);
	void setSize(float x, float y);
	void setWidth(float w);
	void setHeight(float h);
	void setRotation(float angle);
	void setEnabled(bool enabled);

	void setCentered();
	void setCenteredOrigin();

	//custom data
	void setData(const std::string& name, const std::any& value);
	template<typename T> T data(const std::string& name)
	{
		if (auto it = m_data.find(name); it != m_data.end())
			return std::any_cast<T>(m_data.at(name));
		return T();
	}

protected:
	virtual void update();
	virtual void draw(sf::RenderTarget *const target);
	virtual bool eventFilter(sf::Event *event);
	void setName(const std::string& name);
	void setType(ArcEngine::OBJECT_TYPE type);

	sf::Vector2f scaledGlobalPos() const;
	sf::Vector2f scaledGlobalScale() const;

	sf::Vector2f globalPos() const;
	sf::Vector2f globalScale() const;
	sf::Vector2f globalOrigin() const;
	virtual void updatePos();
	virtual void updateScale();
	virtual void updateOrigin();
	virtual void updateSize();

	std::vector<ArcObject*> m_childs;
	std::vector<ArcAction*> m_actions;
	sf::Vector2f scaleFactor;
	ArcObject *m_parent = nullptr;
	sf::Transform m_transform;

	void drawChilds(sf::RenderTarget * const target);
	void updateTransform();
private:
	//base
	friend class ArcDebug;
	friend class Intersection;
	bool m_enabled = true;
	std::string m_name;
	ArcEngine::OBJECT_TYPE m_type = ArcEngine::UNDEF;
	//transform
	float m_x = 0.f;
	float m_y = 0.f;
	float m_angle = 0.f;
	float m_scaleX = 1.f;
	float m_scaleY = 1.f;
	float m_originX = 0.f;
	float m_originY = 0.f;
	float m_width = 0.f;
	float m_height = 0.f;

	std::map<std::string, std::any> m_data;
	static constexpr float center = 0.5f;
#ifdef ARC_DEBUG
	bool drawDebugRect = false;
	bool drawOrigin = false;
	sf::RectangleShape debugRect;
	sf::Vertex debugOrigin;
	sf::Vertex debugOriginPos;
	sf::CircleShape debugCenter;
	void setDebugRectColor(const sf::Color& color);
	sf::Color debugRectColor() const;
	void setDebugRectLineSize(float size);
	float debugRectLineSize() const;
#endif
};

#endif // ARCOBJECT_H
