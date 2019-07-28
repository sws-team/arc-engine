#include "gameobject.h"
#include "globalvariables.h"
#include "settings.h"

GameObject::GameObject(const RESOURCES::TEXTURE_TYPE& texture_id,
					   const Vector2f &startPos,
					   const Vector2i &frameSize,
					   const int frameCount)
	: Animation()
	,m_movable(true)
{
	this->size = frameSize;
	this->frameCount = frameCount;
	this->animationSpeed = 200;
	this->loop = true;
	setTextureId(texture_id);
	setPos(startPos);
}

GameObject::~GameObject()
{

}

Sprite GameObject::getSprite() const
{
	return sprite;
}

void GameObject::setSprite(const Sprite &sprite)
{
	this->sprite = sprite;
	currentFrame = -1;
	nextFrame();
}

void GameObject::setSpriteColor(const Color &color)
{
	sprite.setColor(color);
}

void GameObject::move(float dx, float dy)
{
	move(Vector2f(dx, dy));
}

void GameObject::move(const Vector2f &d)
{
	if (!m_movable)
		return;
	setPos(sprite.getPosition() + d);
}

void GameObject::setPos(const Vector2f &pos)
{
	sprite.setPosition(pos);
}

Vector2f GameObject::pos() const
{
	return sprite.getPosition();
}

Vector2f GameObject::getSize() const
{
	return Vector2f(size.x * Settings::Instance().getScaleFactor().x, size.y * Settings::Instance().getScaleFactor().y);
}

FloatRect GameObject::gameRect() const
{
	return FloatRect(pos(), getSize());
}

bool GameObject::getMovable() const
{
	return m_movable;
}

void GameObject::setMovable(bool movable)
{
	m_movable = movable;
}

Sprite &GameObject::getModifiableSprite()
{
	return sprite;
}

Vector2f GameObject::getCenter() const
{
	return Vector2f(this->pos().x + sprite.getGlobalBounds().width/2,
					this->pos().y + sprite.getGlobalBounds().height/2);
}

