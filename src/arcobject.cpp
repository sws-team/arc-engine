#include "arcobject.h"
#include "engine.h"
#include "managers.h"
#include "arcaction.h"

ArcObject::ArcObject(const std::string &name)
	: m_name(name)
	,m_type(ArcEngine::OBJECT)
{
	scaleFactor = Engine::Instance().settingsManager()->getScaleFactor();
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
	debugOriginPos.color = sf::Color::White;
#endif
}

ArcObject::~ArcObject()
{
	for(ArcObject* child : m_childs)
		delete child;
	for(ArcAction* action : m_actions)
		delete action;
}

void ArcObject::init()
{
	setPos(m_x, m_y);
	setOrigin(m_originX, m_originY);
	setScale(m_scaleX, m_scaleY);
	setRotation(m_angle);
	for(ArcObject* child : m_childs)
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

bool ArcObject::hasChild(ArcObject *object, bool recursively) const
{
	for(ArcObject* child : m_childs) {
		if (child == object)
			return true;
		if (recursively) {
			if (child->hasChild(object, recursively))
				return true;
		}
	}
	return false;
}

ArcObject *ArcObject::findChild(const std::string &name, bool recursively)
{
	for(ArcObject* object : m_childs) {
		if (object->name() == name)
			return object;
		if (recursively) {
			if (ArcObject* childObj = object->findChild(name, recursively); childObj != nullptr)
				return childObj;
		}
	}
	return nullptr;
}

ArcObject *ArcObject::parent()
{
	return m_parent;
}

std::vector<ArcObject *> ArcObject::childs() const
{
	return m_childs;
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
	debugOriginPos.position = scaledGlobalPos();
#endif
	for(ArcObject* child : m_childs)
		child->updatePos();
}

void ArcObject::updateScale()
{
#ifdef ARC_DEBUG
	debugRect.setScale(scaledGlobalScale());
#endif
	for(ArcObject* child : m_childs)
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

void ArcObject::drawChilds(sf::RenderTarget * const target)
{
	for(ArcObject* child : m_childs)
		child->paint(target);
}

void ArcObject::updateTransform()
{
	m_transform = sf::Transform();
	std::function<void(ArcObject* object, sf::Transform *transform)> parentTransform = nullptr;
	parentTransform = [&parentTransform](ArcObject* object, sf::Transform *transform) {
		ArcObject* parent = object->parent();
		if (parent != nullptr)
			parentTransform(parent, transform);
		transform->rotate(object->rotation(), object->scaledGlobalPos());
	};
	parentTransform(this, &m_transform);
}

#ifdef ARC_DEBUG
void ArcObject::setDebugRectColor(const sf::Color &color)
{
	debugRect.setOutlineColor(color);
	debugCenter.setOutlineColor(color);
	debugOrigin.color = color;
	debugOriginPos.color = color;
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
		target->draw(debugRect, m_transform);
		target->draw(debugCenter, m_transform);
		target->draw(&debugOrigin, 1, sf::Points, m_transform);
		target->draw(&debugOriginPos, 1, sf::Points, m_transform);

		target->draw(debugRect, m_transform);
	}
#endif
	drawChilds(target);
}

void ArcObject::update()
{
	for(ArcObject* child : m_childs)
		child->update();

	for (auto it = m_actions.begin(); it != m_actions.end();) {
		ArcAction* action = *it;
		if (action->isCompleted()) {
			delete action;
			it = m_actions.erase(it);
		}
		else {
			action->update();
			++it;
		}
	}
}

void ArcObject::addChild(ArcObject *object)
{
	object->setParent(this);
	object->init();
	m_childs.push_back(object);
}

void ArcObject::addAction(ArcAction *action)
{
	m_actions.push_back(action);
}

void ArcObject::insertChild(int pos, ArcObject *object)
{
	object->setParent(this);
	object->init();
	m_childs.insert(m_childs.begin() + pos, object);
}

void ArcObject::removeChild(ArcObject *object)
{
	if (auto it = std::find(m_childs.begin(), m_childs.end(), object); it != m_childs.end()) {
		m_childs.erase(it);
	}
}

void ArcObject::removeChild(const std::string &name)
{
	if (ArcObject *child = findChild(name, false)) {
		removeChild(child);
	}
}

void ArcObject::removeAction(ArcAction *action)
{
	if (auto it = std::find(m_actions.begin(), m_actions.end(), action); it != m_actions.end()) {
		m_actions.erase(it);
	}
}

bool ArcObject::eventFilter(sf::Event *event)
{
	for(ArcObject* child : m_childs) {
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

sf::Vector2f ArcObject::globalPos() const
{
	if (m_parent == nullptr)
		return pos();

	sf::Vector2f gPos = pos();
	gPos.x *= m_parent->m_scaleX;
	gPos.y *= m_parent->m_scaleY;

	return m_parent->globalPos() + gPos;
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
	return sf::Vector2f(m_originX * m_width, m_originY * m_height);
}

void ArcObject::setEnabled(bool enabled)
{
	m_enabled = enabled;
}

void ArcObject::setCentered()
{
	setCenteredOrigin();
	setPos(pos() + sf::Vector2f(m_width * center, m_height * center));
}

void ArcObject::setCenteredOrigin()
{
	setOrigin(center, center);
}

void ArcObject::setData(const std::string &name, const std::any &value)
{
	m_data.insert(std::pair<std::string, std::any>(name, value));
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

sf::Vector2f ArcObject::scaledSize() const
{
	const sf::Vector2f gScale = globalScale();
	return sf::Vector2f(m_width * gScale.x, m_height * gScale.y);
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

void ArcObject::setX(float x)
{
	setPos(x, m_y);
}

void ArcObject::setY(float y)
{
	setPos(m_x, y);
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
	if (m_angle == angle)
		return;
	m_angle = angle;
	std::function<void(ArcObject *obj)> updateFunc = nullptr;
	updateFunc = [&updateFunc](ArcObject *obj) {
		obj->updateTransform();
		for(ArcObject *child : obj->m_childs)
			updateFunc(child);
	};
	updateFunc(this);
}

float ArcObject::width() const
{
	return m_width;
}

