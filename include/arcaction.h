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
};

class FunctionAction : public ArcAction
{
public:
	FunctionAction(float time);

	void process(float progress) override;

	void setFunc(const std::function<void(float)>& func);

private:
	std::function<void(float)> processFunc = nullptr;
};

class FadeAction : public ActionWithObject
{
public:
	FadeAction(float time, ArcObject *object, sf::Uint8 targetAlpha);

	void process(float progress) override;

	void setTargetAlpha(sf::Uint8 targetAlpha);

	virtual sf::Uint8 alpha(float progress) const = 0;

protected:
	sf::Uint8 m_targetAlpha;
	static constexpr sf::Uint8 MAX_ALPHA = 255;
};

class FadeInAction : public FadeAction
{
public:
	FadeInAction(float time, ArcObject *object);

	sf::Uint8 alpha(float progress) const;
};

class FadeOutAction : public FadeAction
{
public:
	FadeOutAction(float time, ArcObject *object);

	sf::Uint8 alpha(float progress) const;
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
	void finished() override;

	void process(float progress) override;
};

#endif // ARCACTION_H
