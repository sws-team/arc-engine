#include "arcaction.h"
#include "arcobject.h"
#include "arcsprite.h"
#include "arclabel.h"

ArcAction::ArcAction(float time, ArcObject *object)
	:m_object(object)
	,m_time(time)
{

}

ArcAction::~ArcAction()
{

}

void ArcAction::update()
{
	if (!started) {
		started = true;
		timer.reset();
	}
	if (timer.check(m_time)) {
		finished();
		return;
	}
	float progress = static_cast<float>(timer.getElapsedMilliseconds()) / m_time;
	if (progress > 1.f)
		progress = 1.f;
	process(progress);
}

void ArcAction::process(float progress)
{

}

void ArcAction::finished()
{
	if (completedFunc != nullptr)
		completedFunc();
	completed = true;
}

void ArcAction::setCompletedFunc(const std::function<void ()> &func)
{
	completedFunc = func;
}

bool ArcAction::isCompleted() const
{
	return completed;
}

GroupAction::GroupAction(float time, ArcObject *object)
	: ArcAction(time, object)
{

}

void GroupAction::addAction(ArcAction *action)
{
	group.push(action);
}

void GroupAction::process(float progress)
{
	if (group.empty()) {
		finished();
		return;
	}
	if (currentAction == nullptr) {
		currentAction = group.front();
		group.pop();
	}
	currentAction->process(progress);
}

FunctionAction::FunctionAction(float time, ArcObject *object)
	: ArcAction(time, object)
{

}

void FunctionAction::process(float progress)
{
	if (processFunc == nullptr)
		return;
	processFunc(progress, m_object);
}

void FunctionAction::setFunc(const std::function<void (float, ArcObject *)> &func)
{
	processFunc = func;
}

FadeAction::FadeAction(float time, ArcObject *object, sf::Uint8 targetAlpha)
	: ArcAction(time, object)
	,m_targetAlpha(targetAlpha)
{

}

void FadeAction::process(float progress)
{
	switch (m_object->type())
	{
	case ArcEngine::SPRITE:
	case ArcEngine::BUTTON:
	{
		ArcSprite *sprite = dynamic_cast<ArcSprite*>(m_object);
		sf::Color color = sprite->color();
		color.a = alpha(progress);
		sprite->setColor(color);
	}
		break;
	case ArcEngine::LABEL:
	{
		ArcLabel *label = dynamic_cast<ArcLabel*>(m_object);
		sf::Color color = label->color();
		color.a = alpha(progress);
		label->setColor(color);
	}
		break;
	default:
		break;
	}
}

void FadeAction::setTargetAlpha(sf::Uint8 targetAlpha)
{
	m_targetAlpha = targetAlpha;
}

FadeInAction::FadeInAction(float time, ArcObject *object)
	: FadeAction(time, object, MAX_ALPHA)
{

}

sf::Uint8 FadeInAction::alpha(float progress) const
{
	return static_cast<float>(m_targetAlpha) * progress;
}

FadeOutAction::FadeOutAction(float time, ArcObject *object)
	: FadeAction(time, object, 0)
{

}

sf::Uint8 FadeOutAction::alpha(float progress) const
{
	return MAX_ALPHA - static_cast<float>(MAX_ALPHA) * progress;
}

RepeatAction::RepeatAction(float time, ArcObject *object)
	: ArcAction(time, object)
{

}

void RepeatAction::finished()
{
	timer.reset();
}
