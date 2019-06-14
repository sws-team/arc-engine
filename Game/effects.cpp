#include "effects.h"
#include "globalvariables.h"
#include "settings.h"
#include "ResourcesManager/resourcesmanager.h"

Effects::Effects()
{

}

Effects &Effects::Instance()
{
	static Effects instance;
	return instance;
}

void Effects::showAnimation(const RESOURCES::TEXTURE_TYPE& texture_id,
							const Vector2f &pos,
							const Vector2i &size,
							int duration,
							int frameCount,
							int row)
{
	Animation *animation = new Animation();
	animation->row = row;
	animation->animationSpeed = duration;
	animation->size = size;
	animation->callback = std::bind(&Effects::removeAnimation, this, animation);
	animation->frameCount = frameCount;
	animation->setTextureId(texture_id);
	animation->sprite.setPosition(pos);

	animations.push_back(animation);
}

void Effects::draw(RenderTarget * const target)
{
	for(Animation* animation : animations)
		animation->draw(target);
}

void Effects::update()
{
	for(Animation* animation : animations)
		animation->update();
}

void Effects::removeAnimation(Animation *animation)
{
	animations.erase( std::remove( animations.begin(), animations.end(), animation ), animations.end() );
	delete animation;
}

void Effects::moveAnimnations(float dy)
{
	for(Animation* animation : animations)
		animation->sprite.move(0, dy);
}

Animation::Animation() :
	GameDrawable()
  ,loop(false)
  ,frameCount(1)
  ,currentFrame(-1)
  ,animationSpeed(300)
  ,row(0)
  ,callback(nullptr)
  ,lastTimer(0)
  ,shader(nullptr)
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

void Animation::setShader(Shader *shader)
{
	this->shader = shader;
}

void Animation::draw(RenderTarget * const target)
{
	if (shader == nullptr)
		target->draw(sprite, transform);
	else
		target->draw(sprite, shader);
}

void Animation::setTextureId(const RESOURCES::TEXTURE_TYPE &texture_id)
{
	sprite.setTexture(ResourcesManager::Instance().getTexture(texture_id));
	sprite.scale(Settings::Instance().getScaleFactor());
	nextFrame();
}

bool Animation::nextFrame()
{
	currentFrame++;
	if (currentFrame >= frameCount)
	{
		if (!loop)
		{
			stop();
			return false;
		}
		currentFrame = 0;
	}
	sprite.setTextureRect(IntRect(currentFrame * size.x,
									row * size.y,
									size.x,
									size.y
								  ));
//	sprite.rotate(rotation);
	return true;
}

void Animation::stop()
{
	if (callback != nullptr)
	{
		callback(this);
		return;
	}
}
