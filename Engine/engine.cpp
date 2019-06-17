#include "engine.h"
#include "Windows/introwindow.h"
#include "Windows/mainmenu.h"
#include "Windows/aboutwindow.h"
#include "Windows/settingswindow.h"
#include "Windows/closewindow.h"
#include "Windows/loadgamewindow.h"
#include "gamewindow.h"
#include "globalvariables.h"
#include "Game/Level/level.h"
#include "settings.h"

#include "Game/Level/camera.h"
#include "Game/Level/cursor.h"
#include "Game/Level/level.h"

#include <tinyxml.h>
#include <tinydir.h>
#include <base64.h>

Engine &Engine::Instance()
{
	static Engine singleton;
	return singleton;
}

Engine::Engine()
{
	m_state = INTRO;
	m_camera = new Camera();
	m_cursor = new Cursor();
}

View Engine::getView() const
{
	return m_view;
}

void Engine::setView(const View &view)
{
	m_view = view;
}

Camera *Engine::camera() const
{
	return m_camera;
}

Cursor *Engine::cursor() const
{
	return m_cursor;
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
	case LOAD_GAME:
		stateWindow = new LoadGameWindow();
		break;
	case IN_GAME:
		stateWindow = new GameWindow();
		break;
	default:
		break;
	}
	return stateWindow;
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
				sprite.setPosition(x * GlobalVariables::CELL_SIZE * Settings::Instance().getScaleFactor().x, y * GlobalVariables::CELL_SIZE * Settings::Instance().getScaleFactor().y);
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
					gameMap->spawnPos.x = x/GlobalVariables::CELL_SIZE;
					gameMap->spawnPos.y = y/GlobalVariables::CELL_SIZE;
				}
				if (objectName == "end")
				{
					gameMap->endPos.x = x/GlobalVariables::CELL_SIZE;
					gameMap->endPos.y = y/GlobalVariables::CELL_SIZE;
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
