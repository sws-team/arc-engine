#ifndef ARCOBJECT_H
#define ARCOBJECT_H

#include "enginedef.h"
#include <any>
#include <queue>

class ArcAction;

class ArcObject
{
public:
	ArcObject(const std::string& name);
	virtual ~ArcObject();

	void paint(sf::RenderTarget *const target);
	void process();
	bool event(sf::Event *event);

	void destroy();

	bool isDestroyed() const;
	bool hasChild(ArcObject* object, bool recursively = true) const;
	ArcObject *findChild(const std::string& name, bool recursively = true);
	ArcObject *findChildPath(const std::string& path);
	ArcObject *parent() const;
	std::vector<ArcObject*> childs() const;
	std::string path() const;

	void setParent(ArcObject* parent);
	virtual void addChild(ArcObject* object);
	void addAction(ArcAction* action, bool instant = true);
	void insertChild(int pos, ArcObject* object);

	virtual void removeChild(ArcObject* object);
	void removeChild(const std::string& name);
	void removeAction(ArcAction *action);

	std::optional<int> addCallback(const std::string& name, const CallbackType& callback);
	std::optional<int> addCallback(NotificationType type, const CallbackType& callback);
	void removeCallback(const int id);

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
	float alpha() const;

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
	void setAlpha(float alpha);
	void setScaleFactorEnabled(bool enabled);

	void setCentered();
	void setCenteredOrigin();

	//custom data
	void setData(const std::string& name, const ArcVariant& value);
	ArcVariant data(const std::string& name) const;

	virtual void draw(sf::RenderTarget *const target);

protected:
	virtual void initTransform();
	virtual void update();
	virtual bool eventFilter(sf::Event *event);
	void setName(const std::string& name);
	void setType(ArcEngine::OBJECT_TYPE type);

	virtual sf::Vector2f scaledGlobalPos() const;
	sf::Vector2f scaledGlobalScale() const;

	sf::Vector2f globalPos() const;
	sf::Vector2f globalScale(const bool tree = false) const;
	sf::Vector2f globalOrigin() const;
	float globalAlpha() const;
	virtual void updatePos();
	virtual void updateScale();
	virtual void updateOrigin();
	virtual void updateSize();
	void updateAlpha();

	std::vector<ArcObject*> m_childs;
	std::vector<ArcAction*> m_actions;
	ArcObject *m_parent = nullptr;
	sf::Transform m_transform;

	void drawChilds(sf::RenderTarget * const target);
	void updateTransform();

	sf::Vector2f scaleFactor;
	bool enabledScaleFactor = true;
private:
	//base
	friend class ArcDebug;
	friend class Intersection;
	friend class ColorProperty;
	friend class BorderColorProperty;
	friend class ArcLayout;
	friend class ArcScrollArea;
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
	float m_alpha = 1.f;

	std::map<std::string, ArcVariant> m_data;
	static constexpr float center = 0.5f;
	static constexpr float MAX_ALPHA = 255.f;
	bool isDirty = true;
	std::queue<ArcAction*> actionsQueue;
	bool destroyed = false;
	std::vector<int> callbacks;
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
	void updateScaleFactor();
};

#endif // ARCOBJECT_H
