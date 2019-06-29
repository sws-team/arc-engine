#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include "stdheader.h"
#include "textures_types.h"

class ResourcesManager
{
public:
	static ResourcesManager &Instance();

	Texture &getTexture(RESOURCES::TEXTURE_TYPE type);
private:
	ResourcesManager();

	map<RESOURCES::TEXTURE_TYPE, Texture> m_textures;

	void loadTextures();

	void addTexture(RESOURCES::TEXTURE_TYPE type, const string& path);
};

#endif // RESOURCESMANAGER_H
