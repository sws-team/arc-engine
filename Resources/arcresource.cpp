#include "arcresource.h"

#include <fstream>
#include <iostream>

std::vector<ArcEngine::Resource> ArcEngine::Resource::loadResources(const std::string &fileName)
{
//	std::cout << "LOAD "<< fileName <<std::endl;
	std::vector<ArcEngine::Resource> resources;

	std::ifstream file;
	file.open(fileName);
	if (!file.is_open()) {
		std::cerr << "Error to load file: "<< fileName << std::endl;
		return resources;
	}

	size_t count;
	file >> count;
//	std::cout << "COUNT "<< count <<std::endl;
	for (unsigned int i = 0; i < count; ++i) {
//		std::cout << "NUMBER: "<<i << std::endl;
		uint8_t t;
		file >> t;
		ArcEngine::Resource res;
		res.type = static_cast<RESOURCE_TYPE>(t);
//		std::cout << "Type "<< res.type <<std::endl;
		{
			uint32_t size;
			file >> size;

			char *buffer = new char[size + 1];
			file.read(buffer, size);
			buffer[size] = '\0';
			res.name = std::string(buffer);
			delete[] buffer;
		}
		{
			uint32_t size;
			file >> size;

			char *buffer = new char[size + 1];
			file.read(buffer, size);
			buffer[size] = '\0';
			res.description = std::string(buffer);
			delete[] buffer;
		}
		{
			uint32_t size;
			file >> size;

			char *buffer = new char[size + 1];
			file.read(buffer, size);
			buffer[size] = '\0';
			res.data = std::string(buffer);
			delete[] buffer;
		}
		resources.push_back(res);
	}
	file.close();
	return resources;
}

bool ArcEngine::Resource::saveResources(const std::string &fileName, const std::vector<ArcEngine::Resource>& resources)
{
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open())
		return false;

	file << resources.size() << std::endl;
	for (unsigned int i = 0; i < resources.size(); ++i) {
		const ArcEngine::Resource res = resources.at(i);
		file << static_cast<uint8_t>(res.type);

		file << static_cast<uint32_t>(res.name.size());
		file << res.name;

		file << static_cast<uint32_t>(res.description.size());
		file << res.description;

		file << static_cast<uint32_t>(res.data.size());
		file << res.data;
	}
	file.close();
	return true;
}
