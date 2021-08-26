#ifndef ARCDEBUG_H
#define ARCDEBUG_H

#include "stdheader.h"
#include "enginedef.h"
#include "timer.h"

class ArcObject;

class ArcDebug
{
public:
	ArcDebug();
	void setObject(ArcObject* object);
	void draw(sf::RenderTarget *target);
	void init();
	void clear();
	void update();
	void eventFilter(sf::Event* event);
private:
	ArcObject *object = nullptr;
	sf::Clock clock;

	struct {
		sf::Clock clock;
		int value = 0;
		Timer updateTimer;
		const float UPDATE_TIME = 150.f;
		const float STANDARD = 60.f;
	} FPS;

	void drawFrame();
	static void drawObject(ArcObject* obj);
	static void drawObjectProperties(ArcObject* obj);
	static std::string typeToName(ArcEngine::OBJECT_TYPE type);
};

#endif // ARCDEBUG_H
