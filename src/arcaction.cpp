#include "arcaction.h"
#include "arcobject.h"
#include "navigationmap.h"
#include "customwidgets.h"

ArcAction::ArcAction(float time)
	: m_time(time)
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
	for (auto it = actions.begin(); it != actions.end();) {
		ArcAction* action = *it;
		if (action->isCompleted()) {
			delete action;
			it = actions.erase(it);
		}
		else {
			action->update();
			++it;
		}
	}
	if (actions.empty())
		finished();
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
	currentAction = nullptr;
	if (!actions.empty())
		nextAction();
}

void OrderAction::process(float)
{
	auto checkFinished = [this]() {
		if (actions.empty() && currentAction == nullptr) {
			finished();
			return true;
		}
		return false;
	};
	if (checkFinished())
		return;
	currentAction->update();
	if (currentAction->isCompleted()) {
		delete currentAction;
		currentAction = nullptr;
		if (!checkFinished())
			nextAction();
	}
}

void OrderAction::nextAction()
{
	currentAction = actions.front();
	actions.pop();
}

FunctionAction::FunctionAction(float time)
	: ArcAction(time)
{

}

void FunctionAction::process(float progress)
{
	if (processFunc == nullptr)
		return;
	if (processFunc(progress))
		finished();
}

void FunctionAction::setFunc(const std::function<bool (float)> &func)
{
	processFunc = func;
}

FadeAction::FadeAction(float time, ArcObject *object, float targetAlpha)
	: ActionWithObject(time, object)
	,m_targetAlpha(targetAlpha)
{

}

void FadeAction::process(float progress)
{
	m_object->setAlpha(alpha(progress));
}

void FadeAction::setTargetAlpha(sf::Uint8 targetAlpha)
{
	m_targetAlpha = targetAlpha;
}

void FadeAction::finished()
{
	m_object->setAlpha(m_targetAlpha);
	ArcAction::finished();
}

FadeInAction::FadeInAction(float time, ArcObject *object)
	: FadeAction(time, object, MAX_ALPHA)
{
	object->setAlpha(0.f);
}

float FadeInAction::alpha(float progress) const
{
	return m_targetAlpha * progress;
}

FadeOutAction::FadeOutAction(float time, ArcObject *object)
	: FadeAction(time, object, 0.f)
{
	object->setAlpha(MAX_ALPHA);
}

float FadeOutAction::alpha(float progress) const
{
	return m_targetAlpha + MAX_ALPHA - progress;
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
	ActionWithObject::finished();
}

MoveAction::MoveAction(float time, ArcObject *object, const sf::Vector2f &movePos)
	: ChangePosAction(time, object, movePos)
{

}

void MoveAction::finished()
{
	m_object->setPos(m_startPos + m_targetPos);
	ChangePosAction::finished();
}

void MoveAction::process(float progress)
{
	const float targetX = m_startPos.x + m_targetPos.x * progress;
	const float targetY = m_startPos.y + m_targetPos.y * progress;
	m_object->setPos(targetX, targetY);
}

WayPointsMoveAction::WayPointsMoveAction(float time, ArcObject *object)
	: ActionWithObject(-1, object)
	,commonTime(time)
{

}

void WayPointsMoveAction::setGetWayPointsFunc(
		const std::function<std::vector<sf::Vector2f> (ArcObject *)> &func)
{
	getWayPointsFunc = func;
}

void WayPointsMoveAction::setPoints(const std::vector<sf::Vector2f> &positions)
{
	getWayPointsFunc = [positions](ArcObject *) {
		return positions;
	};
}

void WayPointsMoveAction::setResetTime(float time)
{
	resetTime = time;
}

void WayPointsMoveAction::process(float progress)
{
	const float x = m_startPos.x + (m_nextPos.x - m_startPos.x) * progress;
	const float y = m_startPos.y + (m_nextPos.y - m_startPos.y) * progress;
	m_object->setPos(x, y);
}

void WayPointsMoveAction::started()
{
	clock.restart();
	resetPath(false);
}

void WayPointsMoveAction::finished()
{
	if (!step()) {
		return;
	}
	if (resetTime != -1 && resetTimer.check(resetTime)) {
		if (!resetPath())
			return;
		resetTimer.reset();
	}
	if (nextPosition())
		timer.reset();
}

bool WayPointsMoveAction::step()
{
	return true;
}

bool WayPointsMoveAction::resetPath(bool run)
{
	positions.clear();
	if (getWayPointsFunc != nullptr) {
		positions = getWayPointsFunc(m_object);
		const sf::Int32 elapsed = clock.getElapsedTime().asMilliseconds();
		const float currentTimeLeft = commonTime - elapsed;

		float path = 0.f;
		std::vector<float> ds;
		for (unsigned i = 0; i < positions.size() - 1; ++i) {
			const sf::Vector2f pos1 = positions[i];
			const sf::Vector2f pos2 = positions[i + 1];
			const float d = sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
			path += d;
			ds.push_back(d);
		}
		intervals.push_back(0);
		for(const float &d : ds) {
			const float time =  currentTimeLeft * d/path;
			intervals.push_back(time);
		}
	}
	if (positions.empty()) {
		end();
		return false;
	}
	currentIndex = 0;
	return nextPosition(run);
}

bool WayPointsMoveAction::nextPosition(bool run)
{
	if (positions.empty() ||
			currentIndex == positions.size()) {
		end();
		return false;
	}
	if (run)
		m_object->setPos(m_nextPos);
	m_startPos = m_object->pos();
	m_nextPos = positions.at(currentIndex);
	m_time = intervals.at(currentIndex);
	if (currentIndex == positions.size()) {
		end();
		return false;
	}
	currentIndex++;
	return true;
}

void WayPointsMoveAction::end()
{
	m_object->setPos(m_nextPos);
	ActionWithObject::finished();
}

NavigationAction::NavigationAction(float time, ArcObject *object,
								   const sf::Vector2f &targetPos,
								   NavigationMap *navMap)
	: WayPointsMoveAction(time, object)
	, navMap(navMap)
{
	setGetWayPointsFunc([this, targetPos](ArcObject *object) {
		return this->navMap->findPath(object, targetPos);
	});
}

void NavigationAction::setNavigationMap(NavigationMap *navMap)
{
	this->navMap = navMap;
}

PathMoveAction::PathMoveAction(float time, ArcObject *object,
							   PathObject *pathObject)
	: WayPointsMoveAction(time, object)
	, pathObject(pathObject)
{
	setGetWayPointsFunc([this](ArcObject *) {
		return this->pathObject->path();
	});
}

void PathMoveAction::setPathObject(PathObject *pathObject)
{
	this->pathObject = pathObject;
}


ChangeScaleAction::ChangeScaleAction(float time, ArcObject *object, const sf::Vector2f &scale)
	: ActionWithObject(time, object)
	,m_targetScale(scale)
{

}

ChangeScaleAction::ChangeScaleAction(float time, ArcObject *object, float scale)
	: ChangeScaleAction(time, object, sf::Vector2f(scale, scale))
{

}

void ChangeScaleAction::started()
{
	m_startScale = m_object->scale();
	if (m_startScale == m_targetScale) {
		ActionWithObject::finished();
		return;
	}
	increase.first = m_startScale.x < m_targetScale.x;
	increase.second = m_startScale.y < m_targetScale.y;
}

void ChangeScaleAction::process(float progress)
{
	const float offsetX = (m_targetScale.x - m_startScale.x) * progress;
	const float offsetY = (m_targetScale.y - m_startScale.y) * progress;

	const float scaleX = m_startScale.x + offsetX;
	const float scaleY = m_startScale.y + offsetY;

	m_object->setScale(scaleX, scaleY);
}

void ChangeScaleAction::finished()
{
	m_object->setScale(m_targetScale);
	ActionWithObject::finished();
}
