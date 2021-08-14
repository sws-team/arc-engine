#include "gameobject.h"
#include "engine.h"
#include "managers.h"

//============ANIMATION=================

Animation::Animation() :
	GameDrawable()
  ,frameCount(1)
  ,currentFrame(-1)
  ,animationSpeed(300)
  ,row(0)
  ,callback(nullptr)
  ,cycled(false)
  ,rowCount(0)
  ,loop(false)
  ,lastTimer(0)
  ,shader(nullptr)
  ,paused(false)
{

}

Animation::~Animation()
{

}

void Animation::update()
{
	if (timer.check(animationSpeed))
	{
		if(!nextFrame() && callback != nullptr)
			return;
	}
}

void Animation::setShader(sf::Shader *shader)
{
	this->shader = shader;
}

void Animation::pause()
{
	paused = true;
}

void Animation::resume()
{
	paused = false;
}

void Animation::updateTextureRect()
{
	sprite.setTextureRect(sf::IntRect(currentFrame * size.x, row * size.y, size.x, size.y));
}

void Animation::draw(sf::RenderTarget * const target)
{
	if (shader == nullptr)
		target->draw(sprite);
	else
		target->draw(sprite, shader);
}

void Animation::setTextureId(const TextureType &texture_id)
{
	sprite.setTexture(Engine::Instance().texturesManager()->getTexture(texture_id));
	sprite.scale(Engine::Instance().settingsManager()->getScaleFactor());
	nextFrame();
}

bool Animation::nextFrame()
{
	if(paused)
		return true;
	currentFrame++;
	if (currentFrame >= frameCount)
	{		
		if (cycled)
		{
			row++;
			if (row >= rowCount)
			{
				if (!loop)
				{
					finish();
					return false;
				}
				row = 0;
			}
		}
		else
		{
			if (!loop)
			{
				finish();
				return false;
			}
		}
		currentFrame = 0;
	}
	updateTextureRect();
	return true;
}

void Animation::finish()
{
	if (callback != nullptr)
	{
		callback(this);
		return;
	}
}

//============GAME OBJECT=================

GameObject::GameObject(const TextureType &texture_id,
					   const sf::Vector2f &startPos,
					   const sf::Vector2i &frameSize,
					   const int fCount)
	: Animation()
	,visible(true)
{
	this->size = frameSize;
	this->frameCount = fCount;
	this->animationSpeed = 200;
	this->loop = true;
	setTextureId(texture_id);
	setPos(startPos);
}

GameObject::~GameObject()
{

}

void GameObject::draw(sf::RenderTarget * const target)
{
	if (!visible)
		return;
	Animation::draw(target);
}

sf::Sprite GameObject::getSprite() const
{
	return sprite;
}

void GameObject::setSprite(const sf::Sprite &sprite)
{
	this->sprite = sprite;
	currentFrame = -1;
	nextFrame();
}

void GameObject::setSpriteColor(const sf::Color &color)
{
	sprite.setColor(color);
}

void GameObject::move(float dx, float dy)
{
	move(sf::Vector2f(dx, dy));
}

void GameObject::move(const sf::Vector2f &d)
{
	setPos(sprite.getPosition() + d);
}

void GameObject::setPos(const sf::Vector2f &pos)
{
	sprite.setPosition(pos);
}

sf::Vector2f GameObject::pos() const
{
	return sprite.getPosition();
}

void GameObject::scale(float value)
{
	sprite.scale(value, value);
}

sf::Vector2f GameObject::getSize() const
{
	return sf::Vector2f(size.x * Engine::Instance().settingsManager()->getScaleFactor().x,
					size.y * Engine::Instance().settingsManager()->getScaleFactor().y);
}

sf::FloatRect GameObject::gameRect() const
{
	return sf::FloatRect(pos(), getSize());
}

sf::Sprite &GameObject::getModifiableSprite()
{
	return sprite;
}

sf::Vector2f GameObject::getCenter() const
{
	return sf::Vector2f(this->pos().x + sprite.getGlobalBounds().width/2,
						this->pos().y + sprite.getGlobalBounds().height/2);
}

void GameObject::setRotation(float angle)
{
	sprite.setRotation(angle);
}

float GameObject::rotation() const
{
	return sprite.getRotation();
}

void GameObject::hide()
{
	visible = false;
}

void GameObject::show()
{
	visible = true;
}

void GameObject::setVisible(bool visible)
{
	visible?show():hide();
}

