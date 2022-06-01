#ifndef ARCACTION_H
#define ARCACTION_H

#include "enginedef.h"
#include "timer.h"
#include <queue>

class ArcObject;
class ArcAction
{
public:
	ArcAction(float time);
	virtual ~ArcAction() = default;

	void update();
	virtual void started();
	virtual void process(float progress);
	virtual void finished();

	void setCompletedFunc(const std::function<void()>& func);

	bool isCompleted() const;

protected:
	float m_time = 0;
	std::function<void()> completedFunc = nullptr;
	Timer timer;
private:
	bool m_started = false;
	bool m_completed = false;
};

class ActionWithObject : public ArcAction
{
public:
	ActionWithObject(float time, ArcObject *object = nullptr);
protected:
	ArcObject *m_object = nullptr;
};

class GroupAction : public ArcAction
{
public:
	GroupAction();
	~GroupAction() override;

	void addAction(ArcAction *action);

	void process(float) override;

private:
	std::vector<ArcAction*> actions;
};

class OrderAction : public ArcAction
{
public:
	OrderAction();
	~OrderAction() override;

	void addAction(ArcAction *action);

	void started() override;
	void process(float) override;

private:
	std::queue<ArcAction*> actions;
	ArcAction *currentAction = nullptr;
	void nextAction();
};

class FunctionAction : public ArcAction
{
public:
	FunctionAction(float time);

	void process(float progress) override;

	void setFunc(const std::function<bool(float)>& func);

private:
	std::function<bool(float)> processFunc = nullptr;
};

class FadeAction : public ActionWithObject
{
public:
	FadeAction(float time, ArcObject *object, float targetAlpha);

	void process(float progress) override;
	void setTargetAlpha(sf::Uint8 targetAlpha);
	void finished() override;

	virtual float alpha(float progress) const = 0;

protected:
	float m_targetAlpha;
	static constexpr float MAX_ALPHA = 1.f;
};

class FadeInAction : public FadeAction
{
public:
	FadeInAction(float time, ArcObject *object);

	float alpha(float progress) const;
};

class FadeOutAction : public FadeAction
{
public:
	FadeOutAction(float time, ArcObject *object);

	float alpha(float progress) const;
};

class RepeatAction : public ArcAction
{
	RepeatAction(ArcAction *action);
	~RepeatAction() override;

	void process(float progress) override;
	void finished() override;

protected:
	ArcAction *m_action = nullptr;
};

class ChangePosAction : public ActionWithObject
{
public:
	ChangePosAction(float time, ArcObject *object, const sf::Vector2f& targetPos);

	void started() override;
	void process(float progress) override;
	void finished() override;

protected:
	sf::Vector2f m_targetPos;
	sf::Vector2f m_startPos;
};

class MoveAction : public ChangePosAction
{
public:
	MoveAction(float time, ArcObject *object, const sf::Vector2f& movePos);

	void process(float progress) override;
	void finished() override;
};

class WayPointsMoveAction : public ActionWithObject
{
public:
	WayPointsMoveAction(float time, ArcObject *object);

	void setGetWayPointsFunc(const std::function<std::vector<sf::Vector2f> (ArcObject *)> &func);
	void setPoints(const std::vector<sf::Vector2f> &positions);

	void setResetTime(float time);

	void process(float progress) override;
	void started() override;
	void finished() override;

	virtual bool step();

protected:
	bool resetPath(bool run = true);
	bool nextPosition(bool run = true);
	void end();
	Timer resetTimer;
	float resetTime = -1;
	sf::Clock clock;
	float commonTime = -1;

	std::vector<sf::Vector2f> positions;
	std::vector<float> intervals;
	unsigned currentIndex = 0;

	sf::Vector2f m_nextPos;
	sf::Vector2f m_startPos;
private:
	std::function<std::vector<sf::Vector2f>(ArcObject*)> getWayPointsFunc = nullptr;
};

class NavigationMap;
class NavigationAction : public WayPointsMoveAction
{
public:
	NavigationAction(float time, ArcObject *object, const sf::Vector2f& targetPos, NavigationMap* navMap);

	void setNavigationMap(NavigationMap *navMap);

private:
	NavigationMap *navMap = nullptr;
};

class PathObject;
class PathMoveAction : public WayPointsMoveAction
{
	PathMoveAction(float time, ArcObject *object, PathObject* pathObject);

	void setPathObject(PathObject *pathObject);

private:
	PathObject *pathObject =  nullptr;
};

class ChangeScaleAction : public ActionWithObject
{
public:
	ChangeScaleAction(float time, ArcObject *object, const sf::Vector2f& scale);
	ChangeScaleAction(float time, ArcObject *object, float scale);

	void started() override;
	void process(float progress) override;
	void finished() override;
private:
	sf::Vector2f m_startScale;
	sf::Vector2f m_targetScale;
	std::pair<bool, bool> increase = {true, true};
};

class TypeTextAction : public ActionWithObject
{
public:
	TypeTextAction(float time, ArcObject *object, const sf::String& text);
	TypeTextAction(float time, ArcObject *object);

	void process(float progress) override;
	void finished() override;

private:
	sf::String m_text;
};

#endif // ARCACTION_H
