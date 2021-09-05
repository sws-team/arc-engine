#include "arcobject.h"
#include "engine.h"
#include "managers.h"
#include "arcaction.h"

ArcObject::ArcObject(const std::string &name)
	: m_name(name)
{
	scaleFactor = SCALE_FACTOR;
#ifdef ARC_DEBUG
	debugRect.setFillColor(sf::Color::Transparent);
	debugRect.setOutlineColor(sf::Color::White);
	debugRect.setOutlineThickness(1.f);

	debugCenter.setFillColor(sf::Color::Transparent);
	debugCenter.setOutlineColor(sf::Color::White);
	debugCenter.setOutlineThickness(1.f);
	debugCenter.setRadius(2.f);
	debugCenter.setOrigin(debugCenter.getRadius(), debugCenter.getRadius());

	debugOrigin.color = sf::Color::White;
#endif
}

ArcObject::~ArcObject()
{
	for(ArcObject* child : childs)
		delete child;
	for(ArcAction* action : actions)
		delete action;
}

void ArcObject::init()
{
	setPos(m_x, m_y);
	setOrigin(m_originX, m_originY);
	setScale(m_scaleX, m_scaleY);
	setRotation(m_angle);
	for(ArcObject* child : childs)
		child->init();
}

void ArcObject::paint(sf::RenderTarget * const target)
{
	if (!m_enabled)
		return;
	draw(target);
}

void ArcObject::process()
{
	if (!m_enabled)
		return;
	update();
}

bool ArcObject::event(sf::Event *event)
{
	if (!m_enabled)
		return true;
	return eventFilter(event);
}

void ArcObject::setParent(ArcObject *parent)
{
	m_parent = parent;
}

void ArcObject::updatePos()
{
#ifdef ARC_DEBUG
	debugRect.setPosition(scaledGlobalPos());
	debugCenter.setPosition(scaledGlobalPos());
	debugOrigin.position = scaledGlobalPos();
#endif
	for(ArcObject* child : childs)
		child->updatePos();
}

void ArcObject::updateScale()
{
#ifdef ARC_DEBUG
	debugRect.setScale(scaledGlobalScale());
#endif
	for(ArcObject* child : childs)
		child->updateScale();
}

void ArcObject::updateOrigin()
{
#ifdef ARC_DEBUG
	debugRect.setOrigin(globalOrigin());
#endif
}

void ArcObject::updateSize()
{
#ifdef ARC_DEBUG
	debugRect.setSize(size());
#endif
}

#ifdef ARC_DEBUG
void ArcObject::setDebugRectColor(const sf::Color &color)
{
	debugRect.setOutlineColor(color);
	debugCenter.setOutlineColor(color);
	debugOrigin.color = color;
}

sf::Color ArcObject::debugRectColor() const
{
	return debugRect.getOutlineColor();
}

void ArcObject::setDebugRectLineSize(float size)
{
	debugRect.setOutlineThickness(size);
	debugCenter.setOutlineThickness(size);
}

float ArcObject::debugRectLineSize() const
{
	return debugRect.getOutlineThickness();
}
#endif

void ArcObject::draw(sf::RenderTarget * const target)
{
#ifdef ARC_DEBUG
	if (drawDebugRect) {
		target->draw(debugRect);
		target->draw(debugCenter);
		target->draw(&debugOrigin, 1, sf::Points);
	}
#endif
	for(ArcObject* child : childs)
		child->paint(target);
}

void ArcObject::update()
{
	for(ArcObject* child : childs)
		child->update();

	for(ArcAction* action : actions)
		action->update();
}

void ArcObject::addChild(ArcObject *object)
{
	object->setParent(this);
	object->init();
	childs.push_back(object);
}

void ArcObject::addAction(ArcAction *action)
{
	actions.push_back(action);
}

bool ArcObject::eventFilter(sf::Event *event)
{
	for(ArcObject* child : childs) {
		if (child->eventFilter(event))
			break;
	}
	return false;
}

void ArcObject::setName(const std::string &name)
{
	m_name = name;
}

void ArcObject::setType(ArcEngine::OBJECT_TYPE type)
{
	m_type = type;
}

sf::Vector2f ArcObject::scaledGlobalPos() const
{
	sf::Vector2f gPos = globalPos();
	gPos.x *= scaleFactor.x;
	gPos.y *= scaleFactor.y;
	return gPos;
}

sf::Vector2f ArcObject::scaledGlobalScale() const
{
	sf::Vector2f gScale = globalScale();
	gScale.x *= scaleFactor.x;
	gScale.y *= scaleFactor.y;
	return gScale;
}

bool ArcObject::isEnabled() const
{
	return m_enabled;
}

sf::FloatRect ArcObject::rect() const
{
	return sf::FloatRect(m_x, m_y, m_x + m_width, m_y + m_height);
}

sf::Vector2f ArcObject::globalPos() const
{
	if (m_parent == nullptr)
		return pos();
	return m_parent->globalPos() + pos();
}

sf::Vector2f ArcObject::globalScale() const
{
	if (m_parent == nullptr)
		return scale();
	return sf::Vector2f(m_parent->globalScale().x * m_scaleX,
						m_parent->globalScale().y * m_scaleY);
}

sf::Vector2f ArcObject::globalOrigin() const
{
	return sf::Vector2f(m_originX * m_width,
						m_originY * m_height);
}

void ArcObject::setEnabled(bool enabled)
{
	m_enabled = enabled;
}

ArcEngine::OBJECT_TYPE ArcObject::type() const
{
	return m_type;
}

std::string ArcObject::name() const
{
	return m_name;
}

sf::Vector2f ArcObject::size() const
{
	return sf::Vector2f(m_width, m_height);
}

float ArcObject::height() const
{
	return m_height;
}

float ArcObject::scaleX() const
{
	return m_scaleX;
}

float ArcObject::scaleY() const
{
	return m_scaleY;
}

sf::Vector2f ArcObject::scale() const
{
	return sf::Vector2f(m_scaleX, m_scaleY);
}

float ArcObject::x() const
{
	return m_x;
}

float ArcObject::y() const
{
	return m_y;
}

sf::Vector2f ArcObject::pos() const
{
	return sf::Vector2f(m_x, m_y);
}

float ArcObject::originX() const
{
	return m_originX;
}

float ArcObject::originY() const
{
	return m_originY;
}

sf::Vector2f ArcObject::origin() const
{
	return sf::Vector2f(m_originX, m_originY);
}

float ArcObject::rotation() const
{
	return m_angle;
}

void ArcObject::setPos(const sf::Vector2f &coords)
{
	setPos(coords.x, coords.y);
}

void ArcObject::setPos(float x, float y)
{
	m_x = x; m_y = y;
	updatePos();
}

void ArcObject::setOrigin(const sf::Vector2f &coords)
{
	setOrigin(coords.x, coords.y);
}

void ArcObject::setOrigin(float x, float y)
{
	if (x > 1.f)
		m_originX = 1.f;
	else if (x < 0.f)
		m_originX = 0.f;
	else
		m_originX = x;

	if (x > 1.f)
		m_originY = 1.f;
	else if (x < 0.f)
		m_originY = 0.f;
	else
		m_originY = y;

	updateOrigin();
}

void ArcObject::setScale(const sf::Vector2f &scale)
{
	setScale(scale.x, scale.y);
}

void ArcObject::setScale(float x, float y)
{
	m_scaleX = x, m_scaleY = y;
	updateScale();
}

void ArcObject::setSize(const sf::Vector2f &size)
{
	setSize(size.x, size.y);
}

void ArcObject::setSize(float x, float y)
{
	m_width = x, m_height = y;
	updateOrigin();
	updateSize();
}

void ArcObject::setRotation(float angle)
{
	m_angle = angle;
	for(ArcObject* child : childs)
		child->setRotation(angle);
#ifdef ARC_DEBUG
	debugRect.setRotation(angle);
#endif
}

float ArcObject::width() const
{
	return m_width;
}
