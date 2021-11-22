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
	if (!m_started) {
		m_started = true;
		timer.reset();
		started();
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

void ArcAction::started()
{

}

void ArcAction::process(float progress)
{

}

void ArcAction::finished()
{
	if (completedFunc != nullptr)
		completedFunc();
	m_completed = true;
}

void ArcAction::setCompletedFunc(const std::function<void ()> &func)
{
	completedFunc = func;
}

bool ArcAction::isCompleted() const
{
	return m_completed;
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

ChangePosAction::ChangePosAction(float time, ArcObject *object, const sf::Vector2f &targetPos)
	: ArcAction(time, object)
	,m_targetPos(targetPos)
{

}

void ChangePosAction::started()
{
	m_startPos = m_object->pos();
}

void ChangePosAction::process(float progress)
{
	const float targetX = m_startPos.x + (m_targetPos.x - m_startPos.x) * progress;
	const float targetY = m_startPos.y + (m_targetPos.y - m_startPos.y) * progress;
	m_object->setPos(targetX, targetY);
}

void ChangePosAction::finished()
{
	m_object->setPos(m_targetPos);
	ArcAction::finished();
}

MoveAction::MoveAction(float time, ArcObject *object, const sf::Vector2f &movePos)
	: ChangePosAction(time, object, movePos)
{

}


void MoveAction::process(float progress)
{
	const float targetX = m_startPos.x + m_targetPos.x * progress;
	const float targetY = m_startPos.y + m_targetPos.y * progress;
	m_object->setPos(targetX, targetY);
}
