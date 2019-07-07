#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "stdheader.h"
#include "gamedrawable.h"
#include "effects.h"

class GameObject : public Animation
{
public:
	GameObject(const RESOURCES::TEXTURE_TYPE& texture_id,
			   const Vector2f& startPos,
			   const Vector2i& frameSize,
			   const int frameCount = 3);
	virtual ~GameObject() override;

	Sprite getSprite() const;
    void setSprite(const Sprite &sprite);

    void setSpriteColor(const Color& color);	

	void move(float dx, float dy);
	void move(const Vector2f& d);
	void setPos(const Vector2f &pos);
	Vector2f pos() const;

	Vector2f getSize() const;

	FloatRect gameRect() const;

	bool getMovable() const;
	void setMovable(bool movable);

	Sprite &getModifiableSprite();

	Vector2f getCenter() const;

private:
	bool m_movable;
};

#endif // GAMEOBJECT_H
