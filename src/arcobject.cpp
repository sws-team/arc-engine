#include "arcobject.h"
#include "engine.h"
#include "managers.h"
#include "arcaction.h"

ArcObject::ArcObject(const std::string &name, ArcEngine::OBJECT_TYPE type)
	: m_name(name)
	,m_type(type)
{
	scaleFactor = SCALE_FACTOR;
}

ArcObject::~ArcObject()
{
	for(ArcObject* child : childs)
		delete child;
	for(ArcAction* action : actions)
		delete action;
}

void ArcObject::draw(sf::RenderTarget * const target)
{
	for(ArcObject* child : childs)
		child->draw(target);
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

void ArcObject::setPos(const sf::Vector2f &coords)
{
	setPos(coords.x, coords.y);
	for(ArcObject* child : childs)
		child->setPos(sf::Vector2f(m_x, m_y) + coords);
}

void ArcObject::setPos(float x, float y)
{
	m_x = x; m_y = y;
}

void ArcObject::setOrigin(const sf::Vector2f &coords)
{
	setOrigin(coords.x, coords.y);
	for(ArcObject* child : childs)
		child->setOrigin(sf::Vector2f(m_originX, m_originY) + coords);
}

void ArcObject::setOrigin(float x, float y)
{
	m_originX = x; m_originY = y;
}

void ArcObject::setScale(const sf::Vector2f &scale)
{
	setScale(scale.x, scale.y);
	for(ArcObject* child : childs)
		child->setScale(sf::Vector2f(m_scaleX * scale.x, m_scaleY * scale.y));
}

void ArcObject::setScale(float x, float y)
{
	m_scaleX = x, m_scaleY = y;
}

void ArcObject::setSize(const sf::Vector2f &size)
{
	setSize(size.x, size.y);
}

void ArcObject::setSize(float x, float y)
{
	m_width = x, m_height = y;
}

float ArcObject::width() const
{
	return m_width;
}
