#ifndef ARCSOURCE_H
#define ARCSOURCE_H

#include <string>
#include <vector>

namespace ArcEngine
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

	static std::vector<ArcEngine::Resource> loadResources(const std::string& fileName);
	static bool saveResources(const std::string& fileName, const std::vector<ArcEngine::Resource>& resources);
};

};

#endif // ARCRESOURCE_H
