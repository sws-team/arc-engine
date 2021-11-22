#include "arcaction.h"
#include "arcobject.h"
#include "arcsprite.h"
#include "arclabel.h"

ArcAction::ArcAction(float time)
	:m_time(time)
{

}

void ArcAction::update()
{
	if (!m_started) {
		m_started = true;
		timer.reset();
		started();
	}
	if (m_time == -1) {
		process(-1);
		return;
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

ActionWithObject::ActionWithObject(float time, ArcObject *object)
	: ArcAction(time)
	,m_object(object)
{

}

GroupAction::GroupAction()
	: ArcAction(-1)
{

}

GroupAction::~GroupAction()
{
	for(ArcAction *action : actions)
		delete action;
}

void GroupAction::addAction(ArcAction *action)
{
	actions.emplace_back(action);
}

void GroupAction::process(float)
{
	for(ArcAction *action : actions)
		action->update();
}

OrderAction::OrderAction()
	: ArcAction(-1)
{

}

OrderAction::~OrderAction()
{
	while (!actions.empty()) {
		delete currentAction;
		actions.pop();
		currentAction = actions.front();
	}
}

void OrderAction::addAction(ArcAction *action)
{
	actions.push(action);
}

void OrderAction::started()
{
	if (!actions.empty())
		currentAction = actions.front();
}

void OrderAction::process(float)
{
	if (actions.empty()) {
		finished();
		return;
	}
	currentAction->update();
	if (currentAction->isCompleted()) {
		delete currentAction;
		actions.pop();
		currentAction = actions.front();
	}
}

FunctionAction::FunctionAction(float time)
	: ArcAction(time)
{

}

void FunctionAction::process(float progress)
{
	if (processFunc == nullptr)
		return;
	processFunc(progress);
}

void FunctionAction::setFunc(const std::function<void (float)> &func)
{
	processFunc = func;
}

FadeAction::FadeAction(float time, ArcObject *object, sf::Uint8 targetAlpha)
	: ActionWithObject(time, object)
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

RepeatAction::RepeatAction(ArcAction *action)
	: ArcAction(-1)
	,m_action(action)
{

}

RepeatAction::~RepeatAction()
{
	if (m_action != nullptr)
		delete m_action;
}

void RepeatAction::process(float progress)
{
	if (m_action != nullptr)
		m_action->process(progress);
}

void RepeatAction::finished()
{
	timer.reset();
}

ChangePosAction::ChangePosAction(float time, ArcObject *object, const sf::Vector2f &targetPos)
	: ActionWithObject(time, object)
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

void MoveAction::finished()
{
	m_object->setPos(m_startPos + m_targetPos);
	ArcAction::finished();
}


void MoveAction::process(float progress)
{
	const float targetX = m_startPos.x + m_targetPos.x * progress;
	const float targetY = m_startPos.y + m_targetPos.y * progress;
	m_object->setPos(targetX, targetY);
}

