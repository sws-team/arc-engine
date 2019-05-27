#include "savedgame.h"
#include "json/json.h"
#include "globalvariables.h"
#include "Game/level.h"
#include "Crypto/crypto.h"
#include "settings.h"
#include "globalvariables.h"

#include <tinyxml.h>
#include <tinydir.h>
#include <base64.h>

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

bool SavedGameLoader::loadMap(const String &fileName)
{
	FILE* file = nullptr;
	errno_t err = _wfopen_s(&file, fileName.toWideString().c_str(), L"rb");
	if( file != nullptr && err )
		return false;

	TiXmlDocument doc;
	if (!doc.LoadFile(file))
	{
		std::cout << "Loading map failed." << std::endl;
		return false;
	}
	Map *gameMap = new Map();

	TiXmlElement *mapElement = doc.FirstChildElement("map");

	gameMap->width = atoi(mapElement->Attribute("width"));
	gameMap->height = atoi(mapElement->Attribute("height"));

	TiXmlElement *propertiesElement = mapElement->FirstChildElement("properties");
	if (propertiesElement != nullptr)
	{
		TiXmlElement *prop = propertiesElement->FirstChildElement("property");
		while (prop)
		{
			const string propertyName = prop->Attribute("name");
			const string propertyValue = prop->Attribute("value");

			if (propertyName == "name")
				gameMap->name = propertyValue;
			else if (propertyName == "playerCount")
				gameMap->playersCount = stoi(propertyValue);
			else if (propertyName == "description")
				gameMap->decription = propertyValue;
			else if (propertyName == "icon")
			{
				string imgString;
				Base64::Decode(propertyValue, &imgString);
				gameMap->icon.loadFromMemory(imgString.c_str(), imgString.size());
			}

			prop = prop->NextSiblingElement("property");
		}
	}
	TiXmlElement *tilesetElement = mapElement->FirstChildElement("tileset");
	int firstTileID = atoi(tilesetElement->Attribute("firstgid"));
	TiXmlElement *tilePropertiesElement = tilesetElement->FirstChildElement("tile");
	while (tilePropertiesElement)
	{
		Tile::TileProperties tileProperty;
		const int tileId = 1 + atoi(tilePropertiesElement->Attribute("id"));
		TiXmlElement *properties = tilePropertiesElement->FirstChildElement("properties");
		if (properties != nullptr)
		{
			TiXmlElement *prop = properties->FirstChildElement("property");

			while (prop)
			{
				const string propertyName = prop->Attribute("name");
				const string propertyValue = prop->Attribute("value");
				if (propertyName == "defenceBonus")
				{
					tileProperty.defenceBonus = stof(propertyValue);
				}
				else if (propertyName == "solid")
				{
					tileProperty.isSolid = true;
				}
				prop = prop->NextSiblingElement("property");
			}
		}
		gameMap->tileProperties.insert(make_pair(tileId, tileProperty));
		tilePropertiesElement = tilePropertiesElement->NextSiblingElement("tile");
	}

	// source - путь до картинки в контейнере image
	TiXmlElement *image = tilesetElement->FirstChildElement("image");
	const string imagePath = "tiles.png";/*string(path.begin(), path.end()) +"/"+ string(image->Attribute("source"));*/
	Image img;
	if (!img.loadFromFile(imagePath))
	{
		std::cout << "Failed to load tile sheet." << std::endl;
		//FIXME memory leak
		return false;
	}

	img.createMaskFromColor(sf::Color(255, 255, 255));//для маски цвета.сейчас нет маски
	gameMap->tilesetImage.loadFromImage(img);
	gameMap->tilesetImage.setSmooth(false);//сглаживание

	// получаем количество столбцов и строк тайлсета
	const int columns = gameMap->tilesetImage.getSize().x / GlobalVariables::CELL_SIZE;
	const int rows = gameMap->tilesetImage.getSize().y / GlobalVariables::CELL_SIZE;

	// вектор из прямоугольников изображений (TextureRect)
	vector<sf::Rect<int>> subRects;

	for (int y = 0; y < rows; y++)
		for (int x = 0; x < columns; x++)
		{
			sf::Rect<int> rect;

			rect.top = y * GlobalVariables::CELL_SIZE;
			rect.height = GlobalVariables::CELL_SIZE;
			rect.left = x * GlobalVariables::CELL_SIZE;
			rect.width = GlobalVariables::CELL_SIZE;

			subRects.push_back(rect);
		}

	// работа со слоями
	TiXmlElement *layerElement = mapElement->FirstChildElement("layer");
	while (layerElement)
	{
		Layer layer;

		// если присутствует opacity, то задаем прозрачность слоя, иначе он полностью непрозрачен
		if (layerElement->Attribute("opacity") != nullptr)
		{
			float opacity = strtod(layerElement->Attribute("opacity"), nullptr);
			layer.opacity = 255 * opacity;
		}
		else
		{
			layer.opacity = 255;
		}

		//  контейнер <data>
		TiXmlElement *layerDataElement;
		layerDataElement = layerElement->FirstChildElement("data");

		if (layerDataElement == nullptr)
		{
			std::cout << "Bad map. No layer information found." << std::endl;
		}

		//  контейнер <tile> - описание тайлов каждого слоя
		TiXmlElement *tileElement;
		tileElement = layerDataElement->FirstChildElement("tile");

		if (tileElement == nullptr)
		{
			std::cout << "Bad map. No tile information found." << std::endl;
			return false;
		}

		int x = 0;
		int y = 0;

		while (tileElement)
		{
			int tileGID = atoi(tileElement->Attribute("gid"));
			int subRectToUse = tileGID - firstTileID;

			// Устанавливаем TextureRect каждого тайла
			if (subRectToUse >= 0)
			{
				Sprite sprite;
				sprite.setTexture(gameMap->tilesetImage);
				sprite.setTextureRect(subRects[subRectToUse]);
				sprite.setPosition(x * GlobalVariables::CELL_SIZE * Settings::Instance().getScaleFactor().x, y * GlobalVariables::CELL_SIZE * Settings::Instance().getScaleFactor().y);
				sprite.setColor(sf::Color(255, 255, 255, layer.opacity));
				sprite.scale(Settings::Instance().getScaleFactor());

				Tile tile;
				tile.sprite = sprite;
				tile.id = tileGID;
				tile.pos = Vector2i(x, y);

				layer.tiles.push_back(tile);

				if (gameMap->tileSprites.find(tileGID) == gameMap->tileSprites.end())
					gameMap->tileSprites.insert(pair<int, Sprite>(tileGID, sprite));
			}

			tileElement = tileElement->NextSiblingElement("tile");

			x++;
			if (x >= gameMap->width)
			{
				x = 0;
				y++;
				if (y >= gameMap->height)
					y = 0;
			}
		}

		gameMap->layers.push_back(layer);

		layerElement = layerElement->NextSiblingElement("layer");
	}

	// работа с объектами
	TiXmlElement *objectGroupElement;

	// если есть слои объектов
	/*
	if (mapElement->FirstChildElement("objectgroup") != nullptr)
	{
		objectGroupElement = mapElement->FirstChildElement("objectgroup");
		while (objectGroupElement)
		{
			//  контейнер <object>
			TiXmlElement *objectElement;
			objectElement = objectGroupElement->FirstChildElement("object");

			while (objectElement)
			{
				// получаем все данные - тип, имя, позиция, и тд
				string objectType;
				if (objectElement->Attribute("type") != nullptr)
				{
					objectType = objectElement->Attribute("type");
				}
				std::string objectName;
				if (objectElement->Attribute("name") != nullptr)
				{
					objectName = objectElement->Attribute("name");
				}
				int x = atoi(objectElement->Attribute("x"));
				int y = atoi(objectElement->Attribute("y"));

				int width = 0, height = 0;

				Sprite sprite;
				sprite.setTexture(gameMap.tilesetImage);
				sprite.setTextureRect(sf::Rect<int>(0, 0, 0, 0));
				sprite.setPosition(x, y);

				if (objectElement->Attribute("width") != nullptr)
				{
					width = atoi(objectElement->Attribute("width"));
					height = atoi(objectElement->Attribute("height"));
				}
				else
				{
					width = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].width;
					height = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].height;
					sprite.setTextureRect(subRects[atoi(objectElement->Attribute("gid")) - firstTileID]);
				}

//				Building *object = new Building();
//				object->setName(objectName);
//				//                object.type = objectType;
//				object->setSprite(sprite);

//				Vector2f objectPos;
//				objectPos.y = y;
//				objectPos.x = x;
//				object->setPos(objectPos);

				// "переменные" объекта
				TiXmlElement *properties;
				properties = objectElement->FirstChildElement("properties");
				if (properties != nullptr)
				{
					TiXmlElement *prop = properties->FirstChildElement("property");
					if (prop != nullptr)
					{
						while (prop)
						{
							const string propertyName = prop->Attribute("name");
							const string propertyValue = prop->Attribute("value");
//							if (propertyName == "number")
//								object->setNumber(atoi(propertyValue.c_str()));
//							else if (propertyName == "type")
//								object->setBuildingType(static_cast<Building::BUILDINGS_TYPE>(atoi(propertyValue.c_str())));
//							else if(propertyName == "id")
//								object->setId(atoi(propertyValue.c_str()));

							prop = prop->NextSiblingElement("property");
						}
					}
				}

//				gameMap->buildings.push_back(object);

				objectElement = objectElement->NextSiblingElement("object");
			}
			objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
		}
	}
	else
		cout << "No object layers found..." << endl;
*/
	maps.push_back(gameMap);

	fclose(file);

	return true;
}

void SavedGameLoader::loadMaps(const String &path)
{
	tinydir_dir dir;
	const int result = tinydir_open(&dir, path.toWideString().c_str());
	while (dir.has_next)
	{
		tinydir_file file;
		tinydir_readfile(&dir, &file);
		if (!file.is_dir)
			loadMap(file.path);

		tinydir_next(&dir);
	}
	tinydir_close(&dir);
}

unsigned int SavedGame::playersCount() const
{
	return playersData.size();
}
