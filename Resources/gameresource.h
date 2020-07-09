#ifndef GAMERESOURCE_H
#define GAMERESOURCE_H

#include <string>
#include <vector>

namespace GameResource
{
enum RESOURCE_TYPE
{
	UNKNOWN,
	TEXTURE,
	SOUND,
	MUSIC,
	FONT,
	OTHER
};

struct Resource
{
	RESOURCE_TYPE type;
	std::string data;// as base64
	std::string name;
	std::string description;

	static std::vector<GameResource::Resource> loadResources(const std::string& fileName);
	static bool saveResources(const std::string& fileName, const std::vector<GameResource::Resource>& resources);
};

};

#endif // GAMERESOURCE_H
