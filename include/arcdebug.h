#ifndef ARCDEBUG_H
#define ARCDEBUG_H

#include <enginedef.h>
#include <ArcTimer>

class ArcObject;

class DebugSection
{
public:
	DebugSection(const std::string& name);
	virtual ~DebugSection() = default;

	virtual void draw();

	std::string name() const;

private:
	std::string m_name;
};

class ArcDebug
{
public:
	ArcDebug() = default;
	void setObject(ArcObject* object);
	void draw(sf::RenderTarget *target);
	void init();
	void clear();
	void update();
	bool eventFilter(sf::Event* event);

	void addSection(DebugSection *section);
	void removeSection(DebugSection *section);

private:
	ArcObject *object = nullptr;
	sf::Clock clock;
	bool visible = true;
	std::vector<DebugSection*> sections;

	struct {
		sf::Clock clock;
		float value = 0;
		ArcTimer updateTimer;
	} FPS;

	static ArcObject *selectedObject;
	void drawFrame();
	static void drawObject(ArcObject* obj);
	static void drawObjectProperties(ArcObject* obj);
	static std::string typeToName(ArcEngine::OBJECT_TYPE type);
	static sf::Color typeToColor(ArcEngine::OBJECT_TYPE type);
};

#endif // ARCDEBUG_H
