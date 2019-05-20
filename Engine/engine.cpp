#include "engine.h"
#include "Windows/introwindow.h"
#include "Windows/mainmenu.h"
#include "Windows/aboutwindow.h"
#include "Windows/settingswindow.h"
#include "Windows/closewindow.h"
#include "Windows/loadgamewindow.h"
#include "Windows/newgamewindow.h"
#include "gamewindow.h"
#include "Game/Locations/garagelocation.h"
#include "Game/Locations/shoplocation.h"
#include "Game/Locations/mainlocation.h"

Engine::Engine()
{
	m_state = INTRO;

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
		stateWindow = new CloseWindow();
		break;
	case LOAD_GAME:
		stateWindow = new LoadGameWindow();
		break;
	case NEW_GAME:
		stateWindow = new NewGameWindow();
		break;
	case IN_GAME:
		stateWindow = new GameWindow();
		break;
	case SHOP_LOCATION:
		stateWindow = new ShopLocation();
		break;
	case GARAGE_LOCATION:
		stateWindow = new GarageLocation();
		break;
	case MAIN_LOCATION:
		stateWindow = new MainLocation();
		break;
//	case MISSION:
//		stateWindow = new MissionWindow();
//		break;
	default:
		break;
	}
	return stateWindow;
}

Engine &Engine::Instance()
{
    static Engine singleton;
    return singleton;
}

//void Engine::loadTextures(const map<int, string> &fileNames)
//{
//	for(const pair<int, string>& textureFile : fileNames)
//	{
//		Texture texture;
//		texture.loadFromFile(textureFile.second);
//		m_textures.insert(pair<int, Texture>(textureFile.first, texture));
//	}
//}

//Texture &Engine::getTexture(const int id)
//{
//	return m_textures.at(id);
//}
