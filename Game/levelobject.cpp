#include "levelobject.h"
#include "engine.h"
#include "managers.h"
#include "gamemanagers.h"

LevelObject::LevelObject(const TextureType &texture_id,
						 const sf::Vector2f &startPos,
						 const sf::Vector2i &frameSize,
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
	addShader(OBJECTS::SMOKE);
	iTime.restart();

//	mapShader.setParameter("iResolution",
//						   Vector2f(Engine::Instance().settingsManager()->getResolution().x,
//									Engine::Instance().settingsManager()->getResolution().y));
//	mapShader.setParameter("texture", sf::Shader::CurrentTexture);
}

sf::Shader *ShadersFactory::getShader(const OBJECTS::SHADER_TYPES type)
{
	if (!Engine::Instance().settingsManager()->getShaders())
		return nullptr;
//	if (find(shaders.begin(), shaders.end(), type) != shaders.end())
	if (shaders.count(type))
		return shaders.at(type);
	return nullptr;
}

void ShadersFactory::update()
{
//	mapShader.setParameter("iTime", flameClock.getElapsedTime().asSeconds());
//	mapShader.setParameter("iMouse",
//						   Vector2f(static_cast<float>(Mouse::getPosition().x)/Engine::Instance().settingsManager()->getResolution().x,
//									static_cast<float>(Mouse::getPosition().y)/Engine::Instance().settingsManager()->getResolution().y));
	if (timer.check(300))
	{
		int a = rand() % 2;
		int b = rand() % 2;
		sf::Shader *movingShader = shaders.at(OBJECTS::MOVING);
		movingShader->setUniform("wave_amplitude", sf::Vector2f(a, b));
		movingShader->setUniform("wave_phase", timer.getElapsedMilliseconds());
	}

	sf::Shader *waveShader = shaders.at(OBJECTS::WAVE);
	waveShader->setUniform("time", iTime.getElapsedTime().asSeconds());

	sf::Shader *smokeShader = shaders.at(OBJECTS::SMOKE);
	smokeShader->setUniform("time", iTime.getElapsedTime().asSeconds());
}

void ShadersFactory::addShader(OBJECTS::SHADER_TYPES type)
{
	sf::Shader *shader = createShader(type);
	shaders.insert(std::pair<OBJECTS::SHADER_TYPES, sf::Shader*>(type, shader));
}

sf::Shader* ShadersFactory::createShader(OBJECTS::SHADER_TYPES type)
{
	sf::Shader *shader = nullptr;

	switch (type)
	{
	case OBJECTS::MOVING:
	{
		shader = new sf::Shader();
		if (!shader->loadFromMemory(Engine::Instance().shadersManager()->getData(GAME_SHADERS::MOVING),
									sf::Shader::Vertex))
		{
			sf::err() << "Failed to load shader" << std::endl;
			delete shader;
			shader = nullptr;
		}
	}
		break;
	case OBJECTS::WAVE:
	{
		shader = new sf::Shader();
		if (!shader->loadFromMemory(Engine::Instance().shadersManager()->getData(GAME_SHADERS::WAVE),
									sf::Shader::Fragment))
		{
			sf::err() << "Failed to load shader" << std::endl;
			delete shader;
			shader = nullptr;
			break;
		}
		shader->setUniform("currentTexture", sf::Shader::CurrentTexture);
		shader->setUniform("resolution", sf::Vector2f(Engine::Instance().settingsManager()->getResolution().x,
												   Engine::Instance().settingsManager()->getResolution().y));
	}
		break;
	case OBJECTS::SMOKE:
	{

		shader = new sf::Shader();
		if (!shader->loadFromMemory(Engine::Instance().shadersManager()->getData(GAME_SHADERS::SMOKE),
									sf::Shader::Fragment))
		{
			sf::err() << "Failed to load shader" << std::endl;
			delete shader;
			shader = nullptr;
			break;
		}
		shader->setUniform("resolution", sf::Vector2f(Engine::Instance().settingsManager()->getResolution().x,
												   Engine::Instance().settingsManager()->getResolution().y));

	}
		break;
	default:
		break;
	}
	return shader;
}
