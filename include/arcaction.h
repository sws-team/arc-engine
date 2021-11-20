#ifndef ARCACTION_H
#define ARCACTION_H

#include "enginedef.h"
#include "timer.h"
#include <queue>

class ArcObject;
class ArcAction
{
public:
	ArcAction(float time, ArcObject *object = nullptr);
	virtual ~ArcAction();

	void update();
	virtual void started();
	virtual void process(float progress);
	virtual void finished();

	void setCompletedFunc(const std::function<void()>& func);

	bool isCompleted() const;

protected:
	ArcObject *m_object;
	float m_time = 0;
	std::function<void()> completedFunc = nullptr;
	Timer timer;
private:
	bool m_started = false;
	bool m_completed = false;
};

class GroupAction : public ArcAction
{
public:
	GroupAction(float time, ArcObject *object = nullptr);

	void addAction(ArcAction *action);

	void process(float progress) override;

private:
	std::queue<ArcAction*> group;
	ArcAction *currentAction = nullptr;
};

class FunctionAction : public ArcAction
{
public:
	FunctionAction(float time, ArcObject *object = nullptr);

	void process(float progress) override;

	void setFunc(const std::function<void(float, ArcObject*)>& func);

private:
	std::function<void(float, ArcObject*)> processFunc = nullptr;
};

class FadeAction : public ArcAction
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
	RepeatAction(float time, ArcObject *object = nullptr);

	void finished() override;
};

class ChangePosAction : public ArcAction
{
public:
	ChangePosAction(float time, ArcObject *object, const sf::Vector2f& targetPos);

	void started() override;
	void process(float progress) override;

protected:
	sf::Vector2f m_targetPos;
	sf::Vector2f m_startPos;
};

class MoveAction : public ChangePosAction
{
public:
	MoveAction(float time, ArcObject *object, const sf::Vector2f& movePos);

	void process(float progress) override;
};

#endif // ARCACTION_H
