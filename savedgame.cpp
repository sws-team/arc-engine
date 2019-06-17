#include "savedgame.h"
#include "json/json.h"

#include "Crypto/crypto.h"
#include "globalvariables.h"

//const string SAVE_FILE_IDENTIFIER = "#SAVE_FILE_IDENTIFIER";

SavedGameLoader::SavedGameLoader()
{
	saveFileName = GlobalVariables::Instance().applicationPath().toAnsiString() + string("/saves");
}

SavedGameLoader &SavedGameLoader::Instance()
{
	static SavedGameLoader singleton;
	return singleton;
}

void SavedGameLoader::save()
{
	ofstream stream(saveFileName, std::ofstream::out);

	Json::Value obj;
/*
	for(const SavedGame& save : saves)
	{
		Json::Value jsonSave;
//		jsonSave["name"] = save.name;
		Json::Value jsonData;

		for(const GameState& gameState : save.playersData)
		{
			Json::Value jsonGameState;

			Json::Value jsonCurrentWeapons;

			for(auto currentWeapon : gameState.current_weapons)
			{
				Json::Value jsonCurrentWeapon;
				jsonCurrentWeapon["type"] = currentWeapon.first;
				jsonCurrentWeapon["ammo"] = currentWeapon.second;
				jsonCurrentWeapons.append(jsonCurrentWeapon);
			}
			jsonGameState["current_weapons"] = jsonCurrentWeapons;
			jsonGameState["spaceShip"] = gameState.spaceShip;
			jsonGameState["artifact"] = gameState.artifact;

			Json::Value jsonCrew;
			for(const CHARACTERS& character : gameState.crew)
				jsonCrew.append(static_cast<int>(character));
			jsonGameState["crew"] = jsonCrew;

			Json::Value jsonArtifacts;
			for(const ARTIFACT_TYPES& artifact : gameState.artifacts)
				jsonArtifacts.append(static_cast<int>(artifact));
			jsonGameState["artifacts"] = jsonArtifacts;

			Json::Value jsonCharacteristics;
			jsonCharacteristics["attack_speed"] = gameState.characteristics.attack_speed;
			jsonCharacteristics["crit_chance"] = gameState.characteristics.crit_chance;
			jsonCharacteristics["crit_damage"] = gameState.characteristics.crit_damage;
			jsonCharacteristics["reflection"] = gameState.characteristics.reflection;
			jsonCharacteristics["regen_rate"] = gameState.characteristics.regen_rate;
			jsonCharacteristics["regen_energy_rate"] = gameState.characteristics.regen_energy_rate;
			jsonCharacteristics["health"] = gameState.characteristics.health;
			jsonCharacteristics["energy"] = gameState.characteristics.energy;
			jsonCharacteristics["shield"] = gameState.characteristics.shield;
			jsonCharacteristics["ability_cost"] = gameState.characteristics.ability_cost;
			jsonCharacteristics["unused_points"] = gameState.characteristics.unused_points;
			jsonGameState["characteristics"] = jsonCharacteristics;

			jsonGameState["money"] = gameState.money;

			jsonData.append(jsonGameState);
		}

		jsonSave["data"] = jsonData;
		Json::Value completedMissions;
		for(const unsigned int n : save.completedLevels)
			completedMissions.append(n);
		jsonSave["completed_missions"] = completedMissions;

		obj.append(jsonSave);
	}
*/
//	Json::FastWriter fast;
	Json::StyledWriter styled;

//	const string str = fast.write(obj);
	const string str = styled.write(obj);
	const string encodedStr = Crypto::encode(str);
	stream << encodedStr;
	stream.close();

//	Json::StyledStreamWriter styledStream;
//	styledStream.write(ostream, obj);
}

void SavedGameLoader::load()
{
	ifstream stream(saveFileName, std::ifstream::in);
	string str((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
	const string decodedStr = Crypto::decode(str);

	Json::Reader reader;
	Json::Value obj;
	if (!reader.parse(decodedStr, obj))
	{
		cout << "cant read saves"<<saveFileName << endl;
		return;
	}
	/*
	for (unsigned int i = 0; i < obj.size(); i++)
	{
		SavedGame savedGame;
//		savedGame.name = obj[i]["name"].asString();

		const Json::Value& completedMissions = obj[i]["completed_missions"];

		for (unsigned int k = 0; k < completedMissions.size(); ++k)
			savedGame.completedLevels.push_back(completedMissions[k].asUInt());

		const Json::Value& data = obj[i]["data"];
		for (unsigned int j = 0; j < data.size(); j++)
		{
			GameState gameState;
			const Json::Value& current_weapons = data[j]["current_weapons"];
			for (unsigned int k = 0; k < current_weapons.size(); k++)
			{
				const WEAPON_TYPES type = static_cast<WEAPON_TYPES>(current_weapons[k]["type"].asInt());
				const Int64 ammo = current_weapons[k]["ammo"].asLargestInt();
				gameState.current_weapons.insert(pair<WEAPON_TYPES, Int64>(type, ammo));
			}

			gameState.spaceShip = static_cast<SPACESHIP_TYPES>(data[j]["spaceShip"].asInt());
			gameState.artifact = static_cast<ARTIFACT_TYPES>(data[j]["artifact"].asInt());

			const Json::Value& crew = data[j]["crew"];
			for (unsigned int k = 0; k < crew.size(); k++)
				gameState.crew.push_back(static_cast<CHARACTERS>(crew[k].asInt()));

			const Json::Value& artifacts = data[j]["artifacts"];
			for (unsigned int k = 0; k < artifacts.size(); k++)
				gameState.artifacts.push_back(static_cast<ARTIFACT_TYPES>(artifacts[k].asInt()));

			const Json::Value& characteristics = data[j]["characteristics"];
			gameState.characteristics.attack_speed = characteristics["attack_speed"].asFloat();
			gameState.characteristics.crit_chance = characteristics["crit_chance"].asFloat();
			gameState.characteristics.crit_damage = characteristics["crit_damage"].asFloat();
			gameState.characteristics.reflection = characteristics["reflection"].asFloat();
			gameState.characteristics.regen_rate = characteristics["regen_rate"].asFloat();
			gameState.characteristics.regen_energy_rate = characteristics["regen_energy_rate"].asFloat();
			gameState.characteristics.health = characteristics["health"].asFloat();
			gameState.characteristics.energy = characteristics["energy"].asFloat();
			gameState.characteristics.shield = characteristics["shield"].asFloat();
			gameState.characteristics.ability_cost = characteristics["ability_cost"].asFloat();
			gameState.characteristics.unused_points = characteristics["unused_points"].asInt();

			gameState.money = data[j]["money"].asFloat();

			savedGame.playersData.push_back(gameState);
		}
		saves.push_back(savedGame);
	}*/
}

vector<SavedGame> SavedGameLoader::savedGames() const
{
	return saves;
}

void SavedGameLoader::addSaves(const SavedGame &saveGame)
{
	saves.push_back(saveGame);
}

void SavedGameLoader::addCompletedLevel(const unsigned int level)
{
	saves.at(currentSave).completedLevels.push_back(level);
}

void SavedGameLoader::setCurrentSave(const unsigned int n)
{
	currentSave = n;
}

SavedGame SavedGameLoader::getSavedGame() const
{
	return saves.at(currentSave);
}

void SavedGameLoader::setCurrentMission(const unsigned int n)
{
	currentMission = n;
}

unsigned int SavedGameLoader::getCurrentMission() const
{
	return currentMission;
}
