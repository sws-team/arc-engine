#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include "enginedef.h"
#include "engine.h"
#include "managers.h"

class ResourcesManager
{
public:
	ResourcesManager()
	{

	}

	static void addTexture(TextureType type, const std::string& data)
	{
		Engine::Instance().texturesManager()->addTexture(type, data.c_str(), data.size());
	}

	static void addSound(SoundType type, const std::string& data)
	{
		Engine::Instance().soundManager()->addSound(type, data.c_str(), data.size());
	}

	static void addMusic(MusicType type, const std::string& data)
	{
		Engine::Instance().soundManager()->addMusic(type, data.c_str(), data.size());
	}

	static void addFile(FileType type, const std::string& data)
	{
		Engine::Instance().filesManager()->addFile(type, data.c_str(), data.size());
	}

	static void addShader(ShaderType type, const std::string& data)
	{
		Engine::Instance().shadersManager()->addShader(type, data);
	}
};

#endif // RESOURCESMANAGER_H
