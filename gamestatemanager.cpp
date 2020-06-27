#include "gamestatemanager.h"
#include "gamemanagers.h"
#include "Windows/manualwindow.h"
#include "Windows/gamewindow.h"
#include "Windows/choosemissionwindow.h"
#include "Windows/mainmenu.h"
#include "aboutwindow.h"
#include "settingswindow.h"
#include "gameoptions.h"
#include "Windows/hotkeyswindow.h"

GameStateManager::GameStateManager()
{

}

StateWindow *GameStateManager::createState(const GameState state)
{
	StateWindow *stateWindow = StateManager::createState(state);
	if (stateWindow != nullptr)
	{
		if (state == StateManager::ABOUT)
		{
			AboutWindow *aboutWindow = static_cast<AboutWindow*>(stateWindow);
			aboutWindow->setColor(GameOptions::primaryColor);
			aboutWindow->setBorderColor(GameOptions::secondaryColor);
			aboutWindow->setBackState(MANUAL);
			aboutWindow->addStrings(GameManagers::creators());
		}
		else if (state == StateManager::SETTINGS)
		{
			SettingsWindow* settingsWindow = static_cast<SettingsWindow*>(stateWindow);
			settingsWindow->setDrawRects(false);
			settingsWindow->setShowShaders(true);
		}
		return stateWindow;
	}
	switch (state)
	{
	case MENU:
		stateWindow = new MainMenu();
		break;
	case MANUAL:
		stateWindow = new ManualWindow();
		break;
	case CHOOSE_MISSION:
		stateWindow = new ChooseMissionWindow();
		break;
	case IN_GAME:
		stateWindow = new GameWindow();
		break;
	case HOTKEYS:
		stateWindow = new HotkeysWindow();
		break;
	default:
		break;
	}
	return stateWindow;
}
