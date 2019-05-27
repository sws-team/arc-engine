#include "engine.h"
#include "Windows/introwindow.h"
#include "Windows/mainmenu.h"
#include "Windows/aboutwindow.h"
#include "Windows/settingswindow.h"
#include "Windows/closewindow.h"
#include "Windows/loadgamewindow.h"
#include "gamewindow.h"

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
	case IN_GAME:
		stateWindow = new GameWindow();
		break;
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
