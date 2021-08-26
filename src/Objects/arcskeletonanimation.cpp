#include "arcskeletonanimation.h"

#include "SFML/SFMLArmatureDisplay.h"
#include "SFML/SFMLEventDispatcher.h"
#include "SFML/SFMLArmatureProxy.h"

ArcSkeletonAnimation::ArcSkeletonAnimation(const std::string &name)
	: ArcObject(name)
{
	display = new dragonBones::SFMLArmatureDisplay(name);
	animation = display->getAnimation();
	armatureProxy = display->getArmatureProxy();
//	for(const auto &animation_name : animation->getAnimationNames())
//		watch(animation_name);

	dragonBones::SFMLEventDispatcher *dispatcher = display->getEventDispatcher();
//	dispatcher->addDBEventListener(dragonBones::EventObject::LOOP_COMPLETE, std::bind<void>(&ArcSkeletonAnimation::animationCompleted, this));
}

void ArcSkeletonAnimation::draw(sf::RenderTarget * const target)
{
	ArcObject::draw(target);
	target->draw(*display);
}

void ArcSkeletonAnimation::setPos(float x, float y)
{
	ArcObject::setPos(x, y);
	display->setPosition(sf::Vector2f(x, y));
}

void ArcSkeletonAnimation::setColor(const sf::Color &color)
{
	armatureProxy->setColor(color);
}

void ArcSkeletonAnimation::changeAnimation(const std::string &animationName)
{
	currentAnimation = animationName;
	if (currentAnimation != animation->getLastAnimationName())
		currentState = animation->play(currentAnimation);
}

void ArcSkeletonAnimation::animationCompleted(dragonBones::EventObject *event)
{

}
