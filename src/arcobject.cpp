#include "arcobject.h"
#include "engine.h"
#include "managers.h"
#include <ArcAction>
#include "arcproperties.h"
#include <arcvariant.h>
#include <utils.h>

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
	for(const int id : callbacks)
		Engine::Instance().notificationManager()->removeCallback(id);
	for(ArcObject* child : m_childs)
		delete child;
	for(ArcAction* action : m_actions)
		delete action;
}

void ArcObject::initTransform()
{
	setPos(m_x, m_y);
	setOrigin(m_originX, m_originY);
	setScale(m_scaleX, m_scaleY);
	setRotation(m_angle);
	for(ArcObject* child : m_childs)
		child->initTransform();
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

void ArcObject::destroy()
{
	destroyed = true;
}

bool ArcObject::isDestroyed() const
{
	return destroyed;
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

ArcObject *ArcObject::findChildPath(const std::string &path)
{
	const std::vector<std::string> names = Utils::split(path, "/");
	if (names.empty())
		return nullptr;
	ArcObject *obj = this;
	for (unsigned i = 0; i < names.size(); ++i) {
		const std::string name = names.at(i);
		if (i == 0) {
			if (name != obj->name()) {
				return nullptr;
			}
			continue;
		}
		obj = obj->findChild(name, false);
	}
	return obj;
}

ArcObject *ArcObject::parent() const
{
	return m_parent;
}

std::vector<ArcObject *> ArcObject::childs() const
{
	return m_childs;
}

std::string ArcObject::path() const
{
	std::string p;
	ArcObject *parent = this->parent();
	while(parent != nullptr) {
		p = parent->name() + "/" +p;
		parent = parent->parent();
	}
	p += name();
	return p;
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

void ArcObject::updateAlpha()
{
	if (ColorProperty *property = dynamic_cast<ColorProperty*>(this); property != nullptr)
		property->setColor(property->color());
	if (BorderColorProperty *property = dynamic_cast<BorderColorProperty*>(this); property != nullptr)
		property->setBorderColor(property->borderColor());

	for(ArcObject* child : m_childs)
		child->updateAlpha();
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

void ArcObject::updateScaleFactor()
{
	updateScale();
	updatePos();
	updateTransform();
	for(ArcObject *child : m_childs) {
		child->setScaleFactorEnabled(enabledScaleFactor);
		child->updateScaleFactor();
	}
}

float ArcObject::alpha() const
{
	return m_alpha;
}

void ArcObject::setAlpha(float alpha)
{
	if (m_alpha == alpha)
		return;
	m_alpha = alpha;
	updateAlpha();
}

void ArcObject::setScaleFactorEnabled(bool enabled)
{
	enabledScaleFactor = enabled;
	updateScaleFactor();
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
	}
#endif
	drawChilds(target);
}

void ArcObject::update()
{
	while(!actionsQueue.empty()) {
		ArcAction *action = actionsQueue.front();
		actionsQueue.pop();
		m_actions.push_back(action);
	}

	for (auto it = m_childs.begin(); it != m_childs.end();) {
		ArcObject* child = *it;
		if (child->destroyed) {
			delete child;
			it = m_childs.erase(it);
		}
		else {
			child->update();
			++it;
		}
	}

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
	if (isDirty) {
		updateTransform();
		isDirty = false;
	}
}

void ArcObject::addChild(ArcObject *object)
{
	object->setParent(this);
	object->initTransform();
	m_childs.push_back(object);
}

void ArcObject::addAction(ArcAction *action, bool instant)
{
	if (instant)
		m_actions.push_back(action);
	else
		actionsQueue.push(action);
}

void ArcObject::insertChild(int pos, ArcObject *object)
{
	object->setParent(this);
	object->initTransform();
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

std::optional<int> ArcObject::addCallback(const std::string &name, const CallbackType &callback)
{
	const std::optional<int> id = Engine::Instance().notificationManager()->addCallback(name, callback);
	callbacks.emplace_back(id.value());
	return id;
}

std::optional<int> ArcObject::addCallback(NotificationType type, const CallbackType &callback)
{
	const std::optional<int> id = Engine::Instance().notificationManager()->addCallback(type, callback);
	if (id != std::nullopt) {
		callbacks.emplace_back(id.value());
	}
	return id;
}

void ArcObject::removeCallback(const int id)
{
	Engine::Instance().notificationManager()->removeCallback(id);
	if (auto it = std::find(callbacks.begin(), callbacks.end(), id); it != callbacks.end())
		callbacks.erase(it);
}

bool ArcObject::eventFilter(sf::Event *event)
{
	for (int i = static_cast<int>(m_childs.size()) - 1; i >= 0; --i) {
		if (m_childs.at(i)->eventFilter(event))
			return true;
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
	if (enabledScaleFactor) {
		gPos.x *= scaleFactor.x;
		gPos.y *= scaleFactor.y;
	}
	return gPos;
}

sf::Vector2f ArcObject::scaledGlobalScale() const
{
	sf::Vector2f gScale = globalScale();
	if (enabledScaleFactor) {
		gScale.x *= scaleFactor.x;
		gScale.y *= scaleFactor.y;
	}
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
	const sf::Vector2f gScale = globalScale(true);
	gPos.x *= gScale.x;
	gPos.y *= gScale.y;

	if (!enabledScaleFactor && m_parent->parent() == nullptr)
		return gPos;

	return m_parent->globalPos() + gPos;
}

sf::Vector2f ArcObject::globalScale(const bool tree) const
{
	if (m_parent == nullptr)
		return tree ? sf::Vector2f(1.f, 1.f) : scale();
	sf::Vector2f gScale = m_parent->globalScale();
	if (!tree) {
		gScale.x *= m_scaleX;
		gScale.y *= m_scaleY;
	}
	return gScale;
}

sf::Vector2f ArcObject::globalOrigin() const
{
	return sf::Vector2f(m_originX * m_width, m_originY * m_height);
}

float ArcObject::globalAlpha() const
{
	if (m_parent == nullptr)
		return alpha();
	return alpha() * m_parent->globalAlpha();
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

void ArcObject::setData(const std::string &name, const ArcVariant &value)
{
	m_data.insert(std::pair<std::string, ArcVariant>(name, value));
}

ArcVariant ArcObject::data(const std::string &name) const
{
	if (auto it = m_data.find(name); it != m_data.end())
		return it->second;
	return ArcVariant();
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
	const sf::Vector2f gScale = globalScale(true);
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
	if (m_width == x && m_height == y)
		return;
	m_width = x, m_height = y;
	updateOrigin();
	updateSize();
}

void ArcObject::setWidth(float w)
{
	setSize(w, m_height);
}

void ArcObject::setHeight(float h)
{
	setSize(m_width, h);
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

