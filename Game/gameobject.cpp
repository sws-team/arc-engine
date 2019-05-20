#include "gameobject.h"
#include "globalvariables.h"

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
	if (!m_movable)
		return;
	setPos(sprite.getPosition() + Vector2f(dx, dy));
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
	return Vector2f(size.x, size.y);
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

