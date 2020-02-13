#include "levelobject.h"
#include "Engine/engine.h"
#include "ResourcesManager/resourcesmanager.h"
#include "settings.h"

LevelObject::LevelObject(const RESOURCES::TEXTURE_TYPE &texture_id,
						 const Vector2f &startPos,
						 const Vector2i &frameSize,
						 const int fCount)
	: GameObject(texture_id, startPos, frameSize, fCount)
	,m_layer(-1)
{

}

int LevelObject::layer() const
{
	return m_layer;
}

void LevelObject::setLayer(int layer)
{
	m_layer = layer;
}

ShadersFactory::ShadersFactory()
{
	addShader(OBJECTS::MOVING);
	addShader(OBJECTS::WAVE);
	iTime.restart();

//	mapShader.setParameter("iResolution",
//						   Vector2f(Settings::Instance().getResolution().x,
//									Settings::Instance().getResolution().y));
//	mapShader.setParameter("texture", sf::Shader::CurrentTexture);
}

Shader *ShadersFactory::getShader(const OBJECTS::SHADER_TYPES type)
{
//	if (find(shaders.begin(), shaders.end(), type) != shaders.end())
	if (shaders.count(type))
		return shaders.at(type);
	return nullptr;
}

void ShadersFactory::update()
{
//	mapShader.setParameter("iTime", flameClock.getElapsedTime().asSeconds());
//	mapShader.setParameter("iMouse",
//						   Vector2f(static_cast<float>(Mouse::getPosition().x)/Settings::Instance().getResolution().x,
//									static_cast<float>(Mouse::getPosition().y)/Settings::Instance().getResolution().y));
	if (timer.check(300))
	{
		int a = rand() % 2;
		int b = rand() % 2;
		Shader *movingShader = shaders.at(OBJECTS::MOVING);
		movingShader->setUniform("wave_amplitude", Vector2f(a, b));
		movingShader->setUniform("wave_phase", timer.getElapsedMilliseconds());
	}

	Shader *waveShader = shaders.at(OBJECTS::WAVE);
	waveShader->setUniform("time", iTime.getElapsedTime().asSeconds());
}

void ShadersFactory::addShader(OBJECTS::SHADER_TYPES type)
{
	Shader *shader = createShader(type);
	shaders.insert(pair<OBJECTS::SHADER_TYPES, Shader*>(type, shader));
}

Shader* ShadersFactory::createShader(OBJECTS::SHADER_TYPES type)
{
	Shader *shader = nullptr;

	switch (type)
	{
	case OBJECTS::MOVING:
	{
		shader = new Shader();
		if (!shader->loadFromFile("shaders/moving.vs", Shader::Vertex))
		{
			err() << "Failed to load shader" << std::endl;
			delete shader;
			shader = nullptr;
		}
	}
		break;
	case OBJECTS::WAVE:
	{
		shader = new Shader();
		if (!shader->loadFromFile("shaders/wave.fs", Shader::Fragment))
		{
			err() << "Failed to load shader" << std::endl;
			delete shader;
			shader = nullptr;
			break;
		}
		shader->setUniform("currentTexture", Shader::CurrentTexture);
		shader->setUniform("resolution", Vector2f(Settings::Instance().getResolution().x,
												   Settings::Instance().getResolution().y));
	}
		break;
	default:
		break;
	}
	return shader;
}
