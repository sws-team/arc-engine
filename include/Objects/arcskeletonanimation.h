#ifndef ARCSKELETONANIMATION_H
#define ARCSKELETONANIMATION_H

#include "arcobject.h"
#include "arcproperties.h"

namespace dragonBones{
class SFMLArmatureDisplay;
class Animation;
class AnimationState;
class EventObject;
class SFMLArmatureProxy;
}

class ArcSkeletonAnimation : public ArcObject, public ColorProperty
{
public:
	ArcSkeletonAnimation(const std::string& name);
	~ArcSkeletonAnimation() override;

	void draw(sf::RenderTarget *const target) override;

	void setColor(const sf::Color& color) override;

protected:
	void changeAnimation(const std::string& animationName);

	void updatePos() override;
	void updateScale() override;
//	void setOrigin(float x, float y) override;
//	void setScale(float x, float y) override;
//	void setSize(float x, float y) override;

private:
	dragonBones::SFMLArmatureDisplay *display = nullptr;
	dragonBones::Animation* animation = nullptr;
	dragonBones::AnimationState *currentState = nullptr;
	dragonBones::SFMLArmatureProxy *armatureProxy = nullptr;

	std::string currentAnimation;
	void animationCompleted(dragonBones::EventObject* event);
};

#endif // ARCSKELETONANIMATION_H
