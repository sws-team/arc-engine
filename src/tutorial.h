#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <stdheader.h>
#include <arcobject.h>
#include <arcvariant.h>

#include "rapidjson/document.h"

#ifdef ARC_DEBUG
#include <arcdebug.h>
class TutorialDebug : public DebugSection
{
public:
	TutorialDebug();

	void draw() override;

	class Tutorial *p = nullptr;
};
#endif

//block node(s)
#define BLOCK(x, y) x->block(y);
//add to garbage collector
#define CREATED(x, y, z) x->created(y, z);
//garbage collect all
#define GC_ALL(x) x->garbageCollect();
//remove from garbage collector
#define GC(x, y) x->garbageCollect(y);
// unblock all
#define UNBLOCK(x) x->unblock();

class Tutorial : public ArcObject
{
public:
	Tutorial();
	~Tutorial();

	struct Condition {
		std::string name;
		ArcVariant value;
	};

	struct TutorialStep {
		std::string name;
		Condition condition;
		std::function<void()> func = nullptr;
	};

	struct TutorialEntry {
		std::string name;
		std::deque<TutorialStep> steps;

		bool operator == (const TutorialEntry& other) const {
			return this->name == other.name;
		}
		bool operator == (const std::string& other) const {
			return this->name == other;
		}
	};

	void fill(const std::vector<TutorialEntry>& tutorials);

	rapidjson::Value save(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator);
	void load(const rapidjson::Value& data);

	bool isActive() const;


	//controls
	void block(const std::vector<std::string>& blocking);
	void block(const std::string& blocking);
	void created(const std::string &name, ArcObject *object);
	void garbageCollect(const std::string &name);
	void garbageCollect();
	void unblock();

protected:
	bool eventFilter(sf::Event *event) override;

private:
	std::vector<TutorialEntry> tutorials;
	std::map<std::string, ArcObject*> gc;
	std::vector<std::string> blocking;

	TutorialEntry *currentTutorial = nullptr;

	void nextStep();

	void checkTriggers(const std::string& name, const ArcVariant& value);
	void checkConditions(const std::string& name, const ArcVariant& value);

	std::vector<std::string> completed;

	void completeCurrent();

#ifdef ARC_DEBUG
	friend class TutorialDebug;
	TutorialDebug *debug = nullptr;
#endif
};


#endif // TUTORIAL_H
