#ifndef LEVELOBJECT_H
#define LEVELOBJECT_H

#include "gameobject.h"
#include "objects_types.h"

class LevelObject : public GameObject
{
public:
	LevelObject(const TextureType& texture_id,
			   const sf::Vector2f& startPos,
			   const sf::Vector2i& frameSize,
			   const int fCount = 3);


	int layer() const;
	void setLayer(int layer);

private:
	int m_layer;
};

class ShadersFactory
{
public:
	ShadersFactory();

	sf::Shader *getShader(const OBJECTS::SHADER_TYPES type);
	void update();

	static const std::string WAVE_SHADER;
	static const std::string MOVING_SHADER;
	static const std::string SMOKE_SHADER;

private:
	std::map<OBJECTS::SHADER_TYPES, sf::Shader*> shaders;
	void addShader(OBJECTS::SHADER_TYPES type);
	Timer timer;
	sf::Shader* createShader(OBJECTS::SHADER_TYPES type);

	sf::Clock iTime;
};

#endif // LEVELOBJECT_H
