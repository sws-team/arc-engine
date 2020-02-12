#include "engine.h"
#include "Windows/introwindow.h"
#include "Windows/mainmenu.h"
#include "Windows/aboutwindow.h"
#include "Windows/settingswindow.h"
#include "Windows/closewindow.h"
#include "Windows/choosemissionwindow.h"
#include "Windows/manualwindow.h"
#include "gamewindow.h"
#include "globalvariables.h"
#include "settings.h"
#include "Crypto/crypto.h"
#include "Crypto/picosha2.h"
#include "Crypto/checksums.h"

#include "Game/Level/map.h"
#include "Game/Level/level.h"
#include "Game/gamepanel.h"
#include "Game/Level/camera.h"
#include "Game/Level/gamecursor.h"
#include "Game/Level/level.h"
#include "Game/Level/instructions.h"
#include "controller.h"
#include "Game/Level/levelobject.h"

#include "json/json.h"
#include <tinyxml.h>
#include <tinydir.h>

#ifdef STEAM_API
#include "isteamuserstats.h"
#endif

Engine &Engine::Instance()
{
	static Engine singleton;
	return singleton;
}

Engine::Engine() :
	m_camera(nullptr)
  ,m_cursor(nullptr)
  ,m_level(nullptr)
  ,m_panel(nullptr)
  ,m_instructions(nullptr)
  ,m_mission(0)
  ,m_gameSpeed(1)
{
	p_window = nullptr;
	m_state = INTRO;
	reset();
	saveFileName = GlobalVariables::Instance().applicationPath().toAnsiString() + string("/saves");
#ifdef STEAM_API
	SteamUserStats()->RequestCurrentStats();
//	SteamUtils()->GetAppID();
	//	ISteamUserStats *stats = SteamUserStats();
#endif
}

Map *Engine::findMapByNumber(unsigned int num)
{
	const string mapName = "mission" + to_string(num);
	for(Map *currentMap : maps)
	{
		if (currentMap->name == mapName)
			return currentMap;
	}
	return nullptr;
}

#ifdef STEAM_API
void Engine::OnGameOverlayActivated(GameOverlayActivated_t *pCallback)
{
	if (pCallback->m_bActive)
	{
		//pause
	}
	else
	{
		//unpause
	}
}
#endif

unsigned int Engine::getMission() const
{
	return m_mission;
}

void Engine::setMission(unsigned int mission)
{
	m_mission = mission;
}

Map *Engine::getMap(unsigned int mission)
{	
	return findMapByNumber(mission + 1);
}

void Engine::save()
{
	ofstream stream(saveFileName, std::ofstream::out);

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
	const string str = styled.write(obj);
	const string encodedStr = Crypto::encode(str);
	stream << encodedStr;
	stream.close();

//	Json::StyledStreamWriter styledStream;
//	styledStream.write(ostream, obj);
}

void Engine::load()
{
	ifstream stream(saveFileName, std::ifstream::in);
	string str((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
	const string decodedStr = Crypto::decode(str);

	Json::Reader reader;
	Json::Value obj;
	if (!reader.parse(decodedStr, obj))
	{
		cout << "Can't read saves: "<< saveFileName << endl;
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

void Engine::setMissionFinished(unsigned int n, unsigned int rating)
{
	CompletedMission completedMission;
	completedMission.number = n;
	completedMission.stars = rating;
	if (find(m_save.begin(), m_save.end(), completedMission) == m_save.end())
		m_save.push_back(completedMission);
	else
		m_save[n].stars = rating;
}

vector<Engine::CompletedMission> Engine::getCompletedMissions() const
{
	return m_save;
}

unsigned int Engine::maxCompletedLevel() const
{
	unsigned int max = 0;
	for(const CompletedMission& completedMission : m_save)
	{
		if (completedMission.number > max)
			max = completedMission.number;
	}
	return max;
}

void Engine::checkAchievments()
{

}

bool Engine::unlockAchievment(GameAchievements::AchievmentsTypes type)
{
	std::string ID;
	switch (type)
	{
	case GameAchievements::COMPLETE_ALL_LEVELS:
		ID = GameAchievements::COMPLETE_ALL_LEVELS_STR;
		break;
	default:
		return false;
	}
#ifdef STEAM_API
	SteamUserStats()->SetAchievement(ID.data());
	return SteamUserStats()->StoreStats();
#else
	return false;
#endif
}

map<int, Tile::TileProperties> Engine::getTileProperties() const
{
	return tileProperties;
}

LevelObject *Engine::createObject(OBJECTS::OBJECT_TYPES type, const Vector2f& pos)
{
	LevelObject *object = nullptr;

	switch (type)
	{
	case OBJECTS::TREE:
		object = new LevelObject(RESOURCES::OBJECT_TREE, pos, Vector2i(64, 64), 4);
		break;
//	case OBJECTS::RIVER:
//		object = new LevelObject(RESOURCES::OBJECT_RIVER, pos, Vector2i(480, 1088), 1);
//		break;
	default:
		break;
	}

	return object;
}

float Engine::gameSpeed() const
{
	return m_gameSpeed;
}

void Engine::setNormalSpeed()
{
	setSpeed(1.f);
}

void Engine::setFastSpeed()
{
	setSpeed(0.5f);
}

void Engine::setExtraFastSpeed()
{
	setSpeed(0.1f);
}

Camera *Engine::camera() const
{
	return m_camera;
}

GameCursor *Engine::cursor() const
{
	return m_cursor;
}

void Engine::setWindow(RenderWindow *window)
{
	p_window = window;
}

RenderWindow *Engine::window()
{
	return p_window;
}

Level *Engine::level()
{
	return m_level;
}

GamePanel *Engine::panel()
{
	return m_panel;
}

Controller *Engine::controller()
{
	return m_controller;
}

Instructions *Engine::instructions()
{
	return m_instructions;
}

void Engine::reset()
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

unsigned int Engine::missionsCount() const
{
	return maps.size();
}

Engine::GAME_STATE Engine::getState() const
{
	return m_state;
}

void Engine::setState(const GAME_STATE &state)
{
	m_state = state;
}

StateWindow* Engine::createState(const Engine::GAME_STATE &state)
{
	StateWindow *stateWindow = nullptr;
	switch (state)
	{
	case INTRO:
		stateWindow = new IntroWindow();
		break;
	case MAIN_MENU:
		stateWindow = new MainMenu();
		break;
	case OPTIONS:
		stateWindow = new SettingsWindow();
		break;
	case MANUAL:
		stateWindow = new ManualWindow();
		break;
	case ABOUT:
		stateWindow = new AboutWindow();
		break;
	case CLOSING:
		stateWindow = new class CloseWindow();
		break;
	case CHOOSE_MISSION:
		stateWindow = new ChooseMissionWindow();
		break;
	case IN_GAME:
		stateWindow = new GameWindow();
		break;
	default:
		break;
	}
	return stateWindow;
}

bool Engine::loadMap(const String &fileName)
{
#ifdef OS_WIN
	FILE* file = nullptr;
	errno_t err = _wfopen_s(&file, fileName.toWideString().c_str(), L"rb");
#else
	char *charStr = wstringToChar(fileName.toWideString());
	FILE *file = fopen(charStr, "rb");
	int err = 1;
	delete charStr;
#endif
	if( file != nullptr && err )
		return false;

	TiXmlDocument doc;
	if (!doc.LoadFile(file))
	{
		std::cout << "Loading map failed." << std::endl;
		return false;
	}
	Map *gameMap = new Map();
	gameMap->name = fileName.toAnsiString();
	gameMap->name.erase(fileName.getSize() - 4, 4);
	gameMap->name.erase(0, 5);

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
			else if (propertyName == "description")
				gameMap->decription = propertyValue;
			else if (propertyName == "life")
				gameMap->life = stod(propertyValue);
			else if (propertyName == "money")
				gameMap->money = stod(propertyValue);

			else if (propertyName == "smoke")
				gameMap->smoke.enabled = static_cast<bool>(stoi(propertyValue));
			else if (propertyName == "smoke_count")
				gameMap->smoke.count = stoi(propertyValue);
			else if (propertyName == "smoke_duration")
				gameMap->smoke.duration = stod(propertyValue) * MSEC;
			else if (propertyName == "smoke_time")
				gameMap->smoke.time = stod(propertyValue) * MSEC;

			else if (propertyName == "regress")
				gameMap->regress.enabled = static_cast<bool>(stoi(propertyValue));
			else if (propertyName == "regress_count")
				gameMap->regress.count = stoi(propertyValue);
			else if (propertyName == "regress_duration")
				gameMap->regress.duration = stod(propertyValue) * MSEC;
			else if (propertyName == "regress_time")
				gameMap->regress.time = stod(propertyValue) * MSEC;

			else if (propertyName == "moneyDrain")
				gameMap->moneyDrain.enabled = static_cast<bool>(stoi(propertyValue));
			else if (propertyName == "moneyDrain_count")
				gameMap->moneyDrain.count = stoi(propertyValue);
			else if (propertyName == "moneyDrain_duration")
				gameMap->moneyDrain.duration = stod(propertyValue) * MSEC;
			else if (propertyName == "moneyDrain_time")
				gameMap->moneyDrain.time = stod(propertyValue) * MSEC;

			else if (propertyName == "explosions")
				gameMap->explosions.enabled = static_cast<bool>(stoi(propertyValue));
			else if (propertyName == "explosions_count")
				gameMap->explosions.count = stoi(propertyValue);
			else if (propertyName == "explosions_duration")
				gameMap->explosions.duration = stod(propertyValue) * MSEC;
			else if (propertyName == "explosions_time")
				gameMap->explosions.time = stod(propertyValue) * MSEC;


			prop = prop->NextSiblingElement("property");
		}
	}
	const int firstTileID = 1;

	// получаем количество столбцов и строк тайлсета
	const int columns = tilesetImage.getSize().x / GlobalVariables::MAP_CELL_SIZE;
	const int rows = tilesetImage.getSize().y / GlobalVariables::MAP_CELL_SIZE;

	// вектор из прямоугольников изображений (TextureRect)
	vector<sf::Rect<int>> subRects;
	for (int y = 0; y < rows; y++)
		for (int x = 0; x < columns; x++)
		{
			sf::Rect<int> rect;

			rect.top = y * GlobalVariables::MAP_CELL_SIZE;
			rect.height = GlobalVariables::MAP_CELL_SIZE;
			rect.left = x * GlobalVariables::MAP_CELL_SIZE;
			rect.width = GlobalVariables::MAP_CELL_SIZE;

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

		const string layerName = string(layerElement->Attribute("name"));
		if (layerName == string("directions"))
			gameMap->directionsLayer = layersCount;
		else if (layerName == string("moving"))
			gameMap->movingLayer = layersCount;
		else if (layerName == string("water"))
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
			int tileGID = atoi(tileElement->Attribute("gid"));
			int subRectToUse = tileGID - firstTileID;
			// Устанавливаем TextureRect каждого тайла
			if (subRectToUse >= 0 && !subRects.empty())
			{
				Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(subRects[subRectToUse]);
				sprite.setPosition(x * GlobalVariables::MAP_CELL_SIZE * Settings::Instance().getScaleFactor().x,
								   y * GlobalVariables::MAP_CELL_SIZE * Settings::Instance().getScaleFactor().y);
				sprite.setColor(sf::Color(255, 255, 255, layer.opacity));
				sprite.scale(Settings::Instance().getScaleFactor());

				Tile tile;
				tile.sprite = sprite;
				tile.id = tileGID;
				tile.cell = Vector2i(x, y);
				layer.tiles.push_back(tile);

//				if (gameMap->tileSprites.find(tileGID) == gameMap->tileSprites.end())
//					gameMap->tileSprites.insert(pair<int, Sprite>(tileGID, sprite));
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
				string objectName;
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

				// "переменные" объекта
				TiXmlElement *properties;
				properties = objectElement->FirstChildElement("properties");
				if (properties != nullptr)
				{
					TiXmlElement *prop = properties->FirstChildElement("property");
					if (prop != nullptr)
					{
						Map::MapObject object;
						object.pos = Vector2f(x, y);
						object.type = OBJECTS::UNKNOWN;
						object.layer = -1;
						object.shader_type = OBJECTS::UNDEFINED;
						while (prop)
						{
							const string propertyName = prop->Attribute("name");
							const string propertyValue = prop->Attribute("value");

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
	else
		cout << "No object layers found..." << endl;

	maps.push_back(gameMap);
	fclose(file);
	return true;
}

bool Engine::loadTiles(const String &fileName)
{
	tileProperties.clear();

#ifdef OS_WIN
	FILE* file = nullptr;
	errno_t err = _wfopen_s(&file, fileName.toWideString().c_str(), L"rb");
#else
	char *charStr = wstringToChar(fileName.toWideString());
	FILE *file = fopen(charStr, "rb");
	int err = 1;
	delete charStr;
#endif

	if( file != nullptr && err )
		return false;

	TiXmlDocument doc;
	if (!doc.LoadFile(file))
	{
		std::cout << "Loading map failed." << std::endl;
		return false;
	}

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
				const string propertyName = prop->Attribute("name");
				const string propertyValue = prop->Attribute("value");
				if (propertyName == "direction")
					tileProperty.direction = stoi(propertyValue);
				if (propertyName == "alternate_direction1")
					tileProperty.alternate_direction1 = stoi(propertyValue);
				if (propertyName == "alternate_direction2")
					tileProperty.alternate_direction2 = stoi(propertyValue);

				prop = prop->NextSiblingElement("property");
			}
		}
		tileProperties.insert(make_pair(tileId, tileProperty));
		tilePropertiesElement = tilePropertiesElement->NextSiblingElement("tile");
	}


	// source - путь до картинки в контейнере image
	TiXmlElement *image = tilesetElement->FirstChildElement("image");
	const string imagePath = string(image->Attribute("source"));
	Image img;
	if (!img.loadFromFile(imagePath))
	{
		std::cout << "Failed to load tile sheet." << std::endl;
		//FIXME memory leak
		return false;
	}

	img.createMaskFromColor(sf::Color(255, 255, 255));//для маски цвета.сейчас нет маски
	tilesetImage.loadFromImage(img);
	tilesetImage.setSmooth(false);//сглаживание
	fclose(file);
	return true;
}

void Engine::setSpeed(float speed)
{
	m_gameSpeed = speed;
	watch(m_gameSpeed);
}

#ifndef OS_WIN
char *Engine::wstringToChar(const wstring &wStr) const
{
	const wchar_t *input = wStr.c_str();

	// Count required buffer size (plus one for null-terminator).
	size_t size = (wcslen(input) + 1) * sizeof(wchar_t);
	char *buffer = new char[size];

	#ifdef __STDC_LIB_EXT1__
		// wcstombs_s is only guaranteed to be available if __STDC_LIB_EXT1__ is defined
		size_t convertedSize;
		std::wcstombs_s(&convertedSize, buffer, size, input, size);
	#else
		std::wcstombs(buffer, input, size);
	#endif
		return buffer;
}
#endif

void Engine::loadMaps(const String &path)
{
	maps.clear();
	if (!loadTiles("tiles.tsx"))
	{
		std::cout << "Loading tileset failed." << std::endl;
		return;
	}
	tinydir_dir dir;
#ifdef OS_WIN
	const int result = tinydir_open(&dir, path.toWideString().c_str());
#else
	char *charStr = wstringToChar(path.toWideString());
	const int result = tinydir_open(&dir, charStr);
	delete charStr;
#endif
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


bool Engine::checkMaps(const String &path) const
{
	tinydir_dir dir;
#ifdef OS_WIN
	const int result = tinydir_open(&dir, path.toWideString().c_str());
#else
	char *charStr = wstringToChar(path.toWideString());
	const int result = tinydir_open(&dir, charStr);
	delete charStr;
#endif
	int count = 0;
	while (dir.has_next)
	{
		tinydir_file file;
		tinydir_readfile(&dir, &file);
		if (!file.is_dir)
		{
			ifstream stream(String(file.path).toAnsiString());
			const string src = string((std::istreambuf_iterator<char>(stream)),
									  std::istreambuf_iterator<char>());
			const string checksum = picosha2::hash256_hex_string(src);
			stream.close();
//			cout << checksum << endl << count << endl << MISSIONS_CHECKSUM[count] << endl << endl;
//			if (checksum != MISSIONS_CHECKSUM[count])
//			{
//				tinydir_close(&dir);
//				return false;
//			}
			count++;
		}
		tinydir_next(&dir);
	}
	tinydir_close(&dir);
	return true;
}
