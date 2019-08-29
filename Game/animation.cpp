#include "animation.h"
#include "globalvariables.h"
#include "settings.h"
#include "ResourcesManager/resourcesmanager.h"

Animation::Animation() :
	GameDrawable()
  ,frameCount(1)
  ,currentFrame(-1)
  ,animationSpeed(300)
  ,row(0)
  ,callback(nullptr)
  ,loop(false)
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
