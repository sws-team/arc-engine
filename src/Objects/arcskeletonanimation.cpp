#include "arcskeletonanimation.h"

#include "SFML/SFMLArmatureDisplay.h"
#include "SFML/SFMLEventDispatcher.h"
#include "SFML/SFMLArmatureProxy.h"

ArcSkeletonAnimation::ArcSkeletonAnimation(const std::string &name)
	: ArcObject(name)
{
	setType(ArcEngine::SKELETON_ANIMATION);
	display = new dragonBones::SFMLArmatureDisplay(name);
	animation = display->getAnimation();
	armatureProxy = display->getArmatureProxy();
//	for(const auto &animation_name : animation->getAnimationNames())
//		watch(animation_name);

	dragonBones::SFMLEventDispatcher *dispatcher = display->getEventDispatcher();
	//	dispatcher->addDBEventListener(dragonBones::EventObject::LOOP_COMPLETE, std::bind<void>(&ArcSkeletonAnimation::animationCompleted, this));
}

ArcSkeletonAnimation::~ArcSkeletonAnimation()
{
	delete display;
}

void ArcSkeletonAnimation::draw(sf::RenderTarget * const target)
{
	ArcObject::draw(target);
	target->draw(*display);
}

void ArcSkeletonAnimation::setColor(const sf::Color &color)
{
	m_color = color;
	armatureProxy->setColor(color);
}

sf::Color ArcSkeletonAnimation::color() const
{
	return m_color;
}

void ArcSkeletonAnimation::changeAnimation(const std::string &animationName)
{
	currentAnimation = animationName;
	if (currentAnimation != animation->getLastAnimationName())
		currentState = animation->play(currentAnimation);
}

void ArcSkeletonAnimation::updatePos()
{
	display->setPosition(scaledGlobalPos());
	ArcObject::updatePos();
}

void ArcSkeletonAnimation::updateScale()
{
	ArcObject::updateScale();
}

void ArcSkeletonAnimation::animationCompleted(dragonBones::EventObject *event)
{

}
