#ifndef LEVELOBJECT_H
#define LEVELOBJECT_H

#include "gameobject.h"
#include "objects_types.h"

class LevelObject : public GameObject
{
public:
	LevelObject(const TextureType& texture_id,
			   const Vector2f& startPos,
			   const Vector2i& frameSize,
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

	Shader *getShader(const OBJECTS::SHADER_TYPES type);
	void update();
private:
	map<OBJECTS::SHADER_TYPES, Shader*> shaders;
	void addShader(OBJECTS::SHADER_TYPES type);
	Timer timer;
	Shader* createShader(OBJECTS::SHADER_TYPES type);

	Clock iTime;
};

#endif // LEVELOBJECT_H
