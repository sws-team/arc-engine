#include "engine.h"
#include "Windows/introwindow.h"
#include "Windows/mainmenu.h"
#include "Windows/aboutwindow.h"
#include "Windows/settingswindow.h"
#include "Windows/closewindow.h"
#include "Windows/choosemissionwindow.h"
#include "gamewindow.h"
#include "globalvariables.h"
#include "settings.h"
#include "Crypto/crypto.h"

#include "Game/Level/map.h"
#include "Game/Level/level.h"
#include "Game/gamepanel.h"
#include "Game/Level/camera.h"
#include "Game/Level/cursor.h"
#include "Game/Level/level.h""
#include "Game/Level/instructions.h"
#include "controller.h"

#include "json/json.h"
#include <tinyxml.h>
#include <tinydir.h>
#include <base64.h>

#include "isteamuserstats.h"

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
{
	p_window = nullptr;
	m_state = INTRO;
	reset();
	saveFileName = GlobalVariables::Instance().applicationPath().toAnsiString() + string("/saves");
	SteamUserStats()->RequestCurrentStats();
//	SteamUtils()->GetAppID();

//	ISteamUserStats *stats = SteamUserStats();
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
	if (mission == GlobalVariables::SURVIVAL_MODE_ID)
		return maps.at(0);
	return maps.at(mission);
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

float Engine::getStartMoney(const unsigned int n)
{
	switch (n)
	{
	case 128:
		return 5000;
	case 0:
		return 300;
	case 1:
		return 550;
	case 2:
		return 650;
	case 3:
		return 750;
	case 4:
		return 1000;
	case 5:
		return 1100;
	case 6:
		return 1200;
	case 7:
		return 1300;
	case 8:
		return 1400;
	case 9:
		return 2000;
	case 10:
		return 2200;
	case 11:
		return 2400;
	case 12:
		return 2500;
	case 13:
		return 2800;
	case 14:
		return 3000;
	}
	return 0;
}

float Engine::getStartEnergy(const unsigned int n)
{
	switch (n)
	{
	case 128:
		return 1000;
	case 0:
		return 1000;
	case 1:
		return 1000;
	case 2:
		return 1000;
	case 3:
		return 1000;
	case 4:
		return 1000;
	case 5:
		return 1000;
	case 6:
		return 1000;
	case 7:
		return 1000;
	case 8:
		return 1000;
	case 9:
		return 1000;
	case 10:
		return 1000;
	case 11:
		return 1000;
	case 12:
		return 1000;
	case 13:
		return 1000;
	case 14:
		return 1000;
	}
	return 0;
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
	SteamUserStats()->SetAchievement(ID.data());
	return SteamUserStats()->StoreStats();
}

Camera *Engine::camera() const
{
	return m_camera;
}

Cursor *Engine::cursor() const
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
	if (m_instructions != nullptr)
		delete m_instructions;

	m_camera = new Camera();
	m_cursor = new Cursor();
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

				if (propertyName == "direction")
					tileProperty.direction = stoi(propertyValue);
				if (propertyName == "alternate_direction1")
					tileProperty.alternate_direction1 = stoi(propertyValue);
				if (propertyName == "alternate_direction2")
					tileProperty.alternate_direction2 = stoi(propertyValue);

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
	const int columns = gameMap->tilesetImage.getSize().x / GlobalVariables::MAP_CELL_SIZE;
	const int rows = gameMap->tilesetImage.getSize().y / GlobalVariables::MAP_CELL_SIZE;

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
				std::string objectName;
				if (objectElement->Attribute("name") != nullptr)
				{
					objectName = objectElement->Attribute("name");
				}
				int x = atoi(objectElement->Attribute("x"));
				int y = atoi(objectElement->Attribute("y"));

				if (objectName == "spawn")
				{
					gameMap->spawnPos.x = x/GlobalVariables::MAP_CELL_SIZE;
					gameMap->spawnPos.y = y/GlobalVariables::MAP_CELL_SIZE;
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
						while (prop)
						{
							const string propertyName = prop->Attribute("name");
							const string propertyValue = prop->Attribute("value");

							if (objectName == "spawn")
							{
								if (propertyName == "direction")
									gameMap->spawnDirection = static_cast<Map::MOVE_DIRECTIONS>(atoi(propertyValue.c_str()));
							}
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

	maps.push_back(gameMap);

	fclose(file);

	return true;
}

void Engine::loadMaps(const String &path)
{
	maps.clear();
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