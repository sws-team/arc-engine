#ifndef ARCSKELETONANIMATION_H
#define ARCSKELETONANIMATION_H

#include "arcobject.h"

namespace dragonBones{
class SFMLArmatureDisplay;
class Animation;
class AnimationState;
class EventObject;
class SFMLArmatureProxy;
}

class ArcSkeletonAnimation : public ArcObject
{
public:
	ArcSkeletonAnimation(const std::string& name);

	void draw(sf::RenderTarget *const target) override;

	void setColor(const sf::Color& color);
	sf::Color color() const;

protected:
	void changeAnimation(const std::string& animationName);

	void setPos(float x, float y) override;
//	void setOrigin(float x, float y) override;
//	void setScale(float x, float y) override;
//	void setSize(float x, float y) override;

private:
	dragonBones::SFMLArmatureDisplay *display = nullptr;
	dragonBones::Animation* animation = nullptr;
	dragonBones::AnimationState *currentState = nullptr;
	dragonBones::SFMLArmatureProxy *armatureProxy = nullptr;

	std::string currentAnimation;
	sf::Color m_color = sf::Color::White;
	void animationCompleted(dragonBones::EventObject* event);
};

#endif // ARCSKELETONANIMATION_H
