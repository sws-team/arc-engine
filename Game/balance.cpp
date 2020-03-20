#include "balance.h"

#include <json/json.h>

Balance::Balance()
{

}

void Balance::load()
{
	std::string fileName = "balance";
	std::ifstream file;
	file.open(fileName);
	if (!file.is_open())
		return;

	std::string data((std::istreambuf_iterator<char>(file)),
					 std::istreambuf_iterator<char>());

	Json::Reader reader;
	Json::Value obj;
	if (!reader.parse(data, obj))
	{
		std::cout << "Can't read balance" << std::endl;
		return;
	}
	for (unsigned int i = 0; i < obj.size(); i++)
	{
		const Json::Value& jsonSave = obj[i];
//		CompletedMission compeletedMission;
//		compeletedMission.number = jsonSave["mission"].asUInt();
//		compeletedMission.stars = jsonSave["rating"].asUInt();

//		m_save.push_back(compeletedMission);
	}
}
