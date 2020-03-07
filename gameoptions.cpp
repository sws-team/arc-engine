#include "gameoptions.h"

#include "Windows/gamewindow.h"
#include "picosha2.h"

#include "Game/map.h"
#include "Game/level.h"
#include "Game/gamepanel.h"
#include "Game/camera.h"
#include "Game/gamecursor.h"
#include "Game/level.h"
#include "Game/instructions.h"
#include "controller.h"
#include "Game/levelobject.h"
#include "Game/achievements.h"
#include "enginedef.h"
#include "gamemanagers.h"
#include "gameplatform.h"
#include "gamemanagers.h"

#include "json/json.h"
#include <tinyxml.h>

#ifdef STEAM_API
#include "steam_api.h"
#endif

#ifdef OS_WIN
FILE *fmemopen (void *buf, size_t size)
{
	FILE *f;
	f = tmpfile();
	fwrite(buf, 1, size, f);
	rewind(f);
	return f;
}
#endif

GameOptions::GameOptions() :
	m_camera(nullptr)
  ,m_cursor(nullptr)
  ,m_level(nullptr)
  ,m_panel(nullptr)
  ,m_controller(nullptr)
  ,m_instructions(nullptr)
  ,m_mission(0)
  ,m_gameSpeed(1)
  ,m_difficult(1)
{
	reset();
}

Map *GameOptions::findMapByNumber(unsigned int num)
{
	for(Map *currentMap : maps)
	{
		if (currentMap->number == num)
			return currentMap;
	}
	return nullptr;
}

unsigned int GameOptions::getMission() const
{
	return m_mission;
}

void GameOptions::setMission(unsigned int mission)
{
	m_mission = mission;
}

Map *GameOptions::getMap(unsigned int mission)
{
	return findMapByNumber(mission + 1);
}

void GameOptions::save()
{
	Json::Value obj;
	for(const CompletedMission& save : m_save)
	{
		Json::Value jsonMission;
		jsonMission["mission"] = save.number;
		jsonMission["rating"] = save.stars;
		obj.append(jsonMission);
	}

//	Json::FastWriter fast;
	Json::StyledWriter styled;

//	const string str = fast.write(obj);
	const std::string str = styled.write(obj);
	const std::string encodedStr = GlobalVariables::encode(str);

	GamePlatform::Instance().saveFile(EngineDefs::saveFileName, encodedStr);
}

void GameOptions::load()
{
	std::string str = GamePlatform::Instance().readFile(EngineDefs::saveFileName);
	const std::string decodedStr = GlobalVariables::decode(str);

	Json::Reader reader;
	Json::Value obj;
	if (!reader.parse(decodedStr, obj))
	{
		std::cout << "Can't read saves: "<< EngineDefs::saveFileName << std::endl;
		return;
	}
	m_save.clear();
	for (unsigned int i = 0; i < obj.size(); i++)
	{
		const Json::Value& jsonSave = obj[i];
		CompletedMission compeletedMission;
		compeletedMission.number = jsonSave["mission"].asUInt();
		compeletedMission.stars = jsonSave["rating"].asUInt();

		m_save.push_back(compeletedMission);
	}
}

void GameOptions::setMissionFinished(unsigned int n, unsigned int rating)
{
	CompletedMission completedMission;
	completedMission.number = n;
	completedMission.stars = rating;
	if (find(m_save.begin(), m_save.end(), completedMission) == m_save.end())
		m_save.push_back(completedMission);
	else
		m_save[n].stars = rating;
}

std::vector<GameOptions::CompletedMission> GameOptions::getCompletedMissions() const
{
	return m_save;
}

unsigned int GameOptions::maxCompletedLevel() const
{
	unsigned int max = 0;
	for(const CompletedMission& completedMission : m_save)
	{
		if (completedMission.number > max)
			max = completedMission.number;
	}
	return max;
}

std::map<int, Tile::TileProperties> GameOptions::getTileProperties() const
{
	return tileProperties;
}

LevelObject *GameOptions::createObject(OBJECTS::OBJECT_TYPES type, const sf::Vector2f& pos)
{
	LevelObject *object = nullptr;

	switch (type)
	{
	case OBJECTS::TREE:
		object = new LevelObject(GAME_TEXTURE::OBJECT_TREE, pos, sf::Vector2i(64, 64), 4);
		break;
//	case OBJECTS::RIVER:
//		object = new LevelObject(RESOURCES::OBJECT_RIVER, pos, Vector2i(480, 1088), 1);
//		break;
	default:
		break;
	}

	return object;
}

float GameOptions::gameSpeed() const
{
	return m_gameSpeed;
}

void GameOptions::setNormalSpeed()
{
	setSpeed(1.f);
}

void GameOptions::setFastSpeed()
{
	setSpeed(0.5f);
}

void GameOptions::setExtraFastSpeed()
{
	setSpeed(0.1f);
}

Camera *GameOptions::camera() const
{
	return m_camera;
}

GameCursor *GameOptions::cursor() const
{
	return m_cursor;
}

Level *GameOptions::level()
{
	return m_level;
}

GamePanel *GameOptions::panel()
{
	return m_panel;
}

Controller *GameOptions::controller()
{
	return m_controller;
}

Instructions *GameOptions::instructions()
{
	return m_instructions;
}

sf::Vector2f GameOptions::tileSize() const
{
	return sf::Vector2f(Engine::Instance().settingsManager()->getScaleFactor().x * CELL_SIZE,
					Engine::Instance().settingsManager()->getScaleFactor().y * CELL_SIZE);
}

sf::Vector2f GameOptions::mapTileSize() const
{
	return sf::Vector2f(Engine::Instance().settingsManager()->getScaleFactor().x * MAP_CELL_SIZE,
						Engine::Instance().settingsManager()->getScaleFactor().y * MAP_CELL_SIZE);
}

bool GameOptions::verifyChecksum()
{
	std::ifstream stream(GameManagers::resourcesFileName);
	const std::string src = std::string((std::istreambuf_iterator<char>(stream)),
										std::istreambuf_iterator<char>());
	const std::string checksum = picosha2::hash256_hex_string(src);
	stream.close();
//	if (checksum != GameManagers::checksum)
//		return false;
	return true;
}

void GameOptions::reset()
{
	if (m_camera != nullptr)
		delete m_camera;
	if (m_cursor != nullptr)
		delete m_cursor;
	if (m_panel != nullptr)
		delete m_panel;
	if (m_level != nullptr)
		delete m_level;
	if (m_controller != nullptr)
		delete m_controller;
	if (m_instructions != nullptr)
		delete m_instructions;

	m_camera = new Camera();
	m_cursor = new GameCursor();
	m_panel = new GamePanel();
	m_level = new Level();
	m_controller = new Controller();
	m_instructions = new Instructions();
}

unsigned int GameOptions::missionsCount() const
{
	return maps.size();
}

bool GameOptions::loadMap(int id)
{
	const FilesManager::OtherFile oFile = Engine::Instance().filesManager()->getData(id);
	FILE* file = fmemopen(oFile.data, oFile.size);

	if(file == nullptr)
		return false;

	TiXmlDocument doc;
	if (!doc.LoadFile(file))
	{
		std::cout << "Loading map failed." << std::endl;
		return false;
	}
	Map *gameMap = new Map();
	gameMap->number = id;

	TiXmlElement *mapElement = doc.FirstChildElement("map");

	gameMap->width = atoi(mapElement->Attribute("width"));
	gameMap->height = atoi(mapElement->Attribute("height"));

	TiXmlElement *propertiesElement = mapElement->FirstChildElement("properties");
	if (propertiesElement != nullptr)
	{
		TiXmlElement *prop = propertiesElement->FirstChildElement("property");
		while (prop)
		{
			const std::string propertyName = prop->Attribute("name");
			const std::string propertyValue = prop->Attribute("value");

			if (propertyName == "life")
				gameMap->life = stod(propertyValue);
			else if (propertyName == "money")
				gameMap->money = stod(propertyValue);

			else if (propertyName == "smoke")
				gameMap->smoke.enabled = static_cast<bool>(stoi(propertyValue));
			else if (propertyName == "smoke_count")
				gameMap->smoke.count = stoi(propertyValue);
			else if (propertyName == "smoke_duration")
				gameMap->smoke.duration = stod(propertyValue) * EngineDefs::MSEC;
			else if (propertyName == "smoke_time")
				gameMap->smoke.time = stod(propertyValue) * EngineDefs::MSEC;

			else if (propertyName == "regress")
				gameMap->regress.enabled = static_cast<bool>(stoi(propertyValue));
			else if (propertyName == "regress_count")
				gameMap->regress.count = stoi(propertyValue);
			else if (propertyName == "regress_duration")
				gameMap->regress.duration = stod(propertyValue) * EngineDefs::MSEC;
			else if (propertyName == "regress_time")
				gameMap->regress.time = stod(propertyValue) * EngineDefs::MSEC;

			else if (propertyName == "moneyDrain")
				gameMap->moneyDrain.enabled = static_cast<bool>(stoi(propertyValue));
			else if (propertyName == "moneyDrain_count")
				gameMap->moneyDrain.count = stoi(propertyValue);
			else if (propertyName == "moneyDrain_duration")
				gameMap->moneyDrain.duration = stod(propertyValue) * EngineDefs::MSEC;
			else if (propertyName == "moneyDrain_time")
				gameMap->moneyDrain.time = stod(propertyValue) * EngineDefs::MSEC;

			else if (propertyName == "explosions")
				gameMap->explosions.enabled = static_cast<bool>(stoi(propertyValue));
			else if (propertyName == "explosions_count")
				gameMap->explosions.count = stoi(propertyValue);
			else if (propertyName == "explosions_duration")
				gameMap->explosions.duration = stod(propertyValue) * EngineDefs::MSEC;
			else if (propertyName == "explosions_time")
				gameMap->explosions.time = stod(propertyValue) * EngineDefs::MSEC;


			prop = prop->NextSiblingElement("property");
		}
	}
	const int firstTileID = 1;

	// получаем количество столбцов и строк тайлсета
	const int columns = tilesetImage.getSize().x / MAP_CELL_SIZE;
	const int rows = tilesetImage.getSize().y / MAP_CELL_SIZE;

	// вектор из прямоугольников изображений (TextureRect)
	std::vector<sf::Rect<int>> subRects;
	for (int y = 0; y < rows; y++)
		for (int x = 0; x < columns; x++)
		{
			sf::Rect<int> rect;

			rect.top = y * MAP_CELL_SIZE;
			rect.height = MAP_CELL_SIZE;
			rect.left = x * MAP_CELL_SIZE;
			rect.width = MAP_CELL_SIZE;

			subRects.push_back(rect);
		}

	//tiles
	int layersCount = 0;
	TiXmlElement *layerElement = mapElement->FirstChildElement("layer");
	while (layerElement)
	{
		Layer layer;
		layer.visibility = true;
		layer.opacity = 255.f;

		// если присутствует opacity, то задаем прозрачность слоя, иначе он полностью непрозрачен
		if (layerElement->Attribute("opacity") != nullptr)
		{
			const float opacity = strtod(layerElement->Attribute("opacity"), nullptr);
			layer.opacity *= opacity;
		}

		const std::string layerName = std::string(layerElement->Attribute("name"));
		if (layerName == std::string("directions"))
			gameMap->directionsLayer = layersCount;
		else if (layerName == std::string("moving"))
			gameMap->movingLayer = layersCount;
		else if (layerName == std::string("water"))
			gameMap->waterLayer = layersCount;

		layersCount++;

		if (layerElement->Attribute("visible") != nullptr)
			layer.visibility = static_cast<bool>(atoi(layerElement->Attribute("visible")));

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
			const char* gidStr = tileElement->Attribute("gid");
			if (gidStr != nullptr)
			{
				const int tileGID = atoi(gidStr);
				const int subRectToUse = tileGID - firstTileID;
				if (subRectToUse >= 0 && !subRects.empty())
				{
					sf::Sprite sprite;
					sprite.setTexture(tilesetImage);
					sprite.setTextureRect(subRects[subRectToUse]);
					sprite.setPosition(x * MAP_CELL_SIZE * Engine::Instance().settingsManager()->getScaleFactor().x,
									   y * MAP_CELL_SIZE * Engine::Instance().settingsManager()->getScaleFactor().y);
					sprite.setColor(sf::Color(255, 255, 255, layer.opacity));
					sprite.scale(Engine::Instance().settingsManager()->getScaleFactor());

					Tile tile;
					tile.sprite = sprite;
					tile.id = tileGID;
					tile.cell = sf::Vector2i(x, y);
					layer.tiles.push_back(tile);

	//				if (gameMap->tileSprites.find(tileGID) == gameMap->tileSprites.end())
	//					gameMap->tileSprites.insert(pair<int, Sprite>(tileGID, sprite));
				}
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

	//objects
	TiXmlElement *objectGroupElement;
	if (mapElement->FirstChildElement("objectgroup") != nullptr)
	{
		objectGroupElement = mapElement->FirstChildElement("objectgroup");
		while (objectGroupElement)
		{
			TiXmlElement *objectElement;
			objectElement = objectGroupElement->FirstChildElement("object");

			while (objectElement)
			{
				std::string objectType;
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

				if (objectName == "spawn")
				{
					gameMap->spawnPos.x = x;
					gameMap->spawnPos.y = y;
				}

				int width = 0;
				int height = 0;
				if (objectElement->Attribute("width") != nullptr)
				{
					width = atoi(objectElement->Attribute("width"));
					height = atoi(objectElement->Attribute("height"));
				}
				else
				{
					width = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].width;
					height = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].height;
				}
				if (objectName == "end")
				{
					gameMap->endRect.left = atof(objectElement->Attribute("x"));
					gameMap->endRect.top = atof(objectElement->Attribute("y"));
					gameMap->endRect.width = width;
					gameMap->endRect.height = height;
				}

				TiXmlElement *properties;
				properties = objectElement->FirstChildElement("properties");
				if (properties != nullptr)
				{
					TiXmlElement *prop = properties->FirstChildElement("property");
					if (prop != nullptr)
					{
						Map::MapObject object;
						object.pos = sf::Vector2f(x, y);
						object.type = OBJECTS::UNKNOWN;
						object.layer = -1;
						object.shader_type = OBJECTS::UNDEFINED;
						while (prop)
						{
							const std::string propertyName = prop->Attribute("name");
							const std::string propertyValue = prop->Attribute("value");

							if (propertyName == "direction" && objectName == "spawn")
								gameMap->spawnDirection = static_cast<Map::MOVE_DIRECTIONS>(atoi(propertyValue.c_str()));

							if (propertyName == "id")
								object.type = static_cast<OBJECTS::OBJECT_TYPES>(stoi(propertyValue));
							if (propertyName == "layer")
								object.layer = stoi(propertyValue);
							if (propertyName == "shader_id")
								object.shader_type = static_cast<OBJECTS::SHADER_TYPES>(stoi(propertyValue));

							prop = prop->NextSiblingElement("property");
						}
						gameMap->objects.push_back(object);
					}
				}

				//				gameMap->buildings.push_back(object);

				objectElement = objectElement->NextSiblingElement("object");
			}
			objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
		}
	}
//	else
//		cout << "No object layers found..." << endl;

	maps.push_back(gameMap);
	fclose(file);
	return true;
}

bool GameOptions::loadTiles()
{
	tileProperties.clear();

	const FilesManager::OtherFile oFile = Engine::Instance().filesManager()->getData(GAME_FILES::TILES);
	FILE* file = fmemopen(oFile.data, oFile.size);

	if(file == nullptr)
		return false;

	TiXmlDocument doc;
	if (!doc.LoadFile(file))
	{
		std::cout << "Loading map failed." << std::endl;
		fclose(file);
		return false;
	}
	fclose(file);

	TiXmlElement *tilesetElement = doc.FirstChildElement("tileset");
//	int firstTileID = atoi(tilesetElement->Attribute("firstgid"));

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
				const std::string propertyName = prop->Attribute("name");
				const std::string propertyValue = prop->Attribute("value");
				if (propertyName == "direction")
					tileProperty.direction = stoi(propertyValue);
				if (propertyName == "alternate_direction1")
					tileProperty.alternate_direction1 = stoi(propertyValue);
				if (propertyName == "alternate_direction2")
					tileProperty.alternate_direction2 = stoi(propertyValue);

				prop = prop->NextSiblingElement("property");
			}
		}
		tileProperties.insert(std::make_pair(tileId, tileProperty));
		tilePropertiesElement = tilePropertiesElement->NextSiblingElement("tile");
	}


	// source - путь до картинки в контейнере image
	TiXmlElement *image = tilesetElement->FirstChildElement("image");
	const std::string imagePath = std::string(image->Attribute("source"));
	sf::Image img;
	if (!img.loadFromFile(imagePath))
	{
		std::cout << "Failed to load tile sheet." << std::endl;
		return false;
	}

	img.createMaskFromColor(sf::Color(255, 255, 255));//для маски цвета.сейчас нет маски
	tilesetImage.loadFromImage(img);
	tilesetImage.setSmooth(false);
	return true;
}

void GameOptions::setSpeed(float speed)
{
	m_gameSpeed = speed;
}

void GameOptions::loadMaps()
{
	maps.clear();
	if (!loadTiles())
	{
		std::cout << "Loading tileset failed." << std::endl;
		return;
	}

	const std::list<GAME_FILES::FILE_ID> missions = {
		GAME_FILES::MISSON_1,
		GAME_FILES::MISSON_2,
		GAME_FILES::MISSON_3,
		GAME_FILES::MISSON_4,
		GAME_FILES::MISSON_5,
		GAME_FILES::MISSON_6,
		GAME_FILES::MISSON_7,
		GAME_FILES::MISSON_8,
		GAME_FILES::MISSON_9,
		GAME_FILES::MISSON_10,
		GAME_FILES::MISSON_11,
		GAME_FILES::MISSON_12,
		GAME_FILES::MISSON_13,
		GAME_FILES::MISSON_14,
		GAME_FILES::MISSON_15
	};
	for(const GAME_FILES::FILE_ID& mission : missions)
		loadMap(mission);
}

void GameOptions::setEasyDifficult()
{
	m_difficult = 2;
}

void GameOptions::setNormalDifficult()
{
	m_difficult = 1;
}

void GameOptions::setHardDifficult()
{
	m_difficult = 0.5;
}

void GameOptions::setDifficult(float value)
{
	m_difficult = value;
}

float GameOptions::difficult() const
{
	return m_difficult;
}

void GameOptions::updateWindow()
{
	Options::updateWindow();
	loadMaps();
}

void GameOptions::loadAchievements()
{
	GamePlatform::Instance().addAchievement(ACHIEVEMENT_COMPLETE_ALL_LEVELS, std::string("COMPLETE_ALL_LEVELS"));
	GamePlatform::Instance().addAchievement(ACHIEVEMENT_COMPLETE_LEVEL_WITHOUT_DAMAGE, std::string("COMPLETE_LEVEL_WITHOUT_DAMAGE"));
	GamePlatform::Instance().addAchievement(ACHIEVEMENT_KILL_100_CARS, std::string("KILL_100_CARS"));


	GamePlatform::Instance().addStat(STAT_CARS_KILLS, std::string("CARS_KILLS"));
}

void GameOptions::globalCallbacks()
{
#ifdef STEAM_API
	SteamAPI_RunCallbacks();
#endif
}