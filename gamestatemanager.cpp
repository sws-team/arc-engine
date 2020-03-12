#include "gamestatemanager.h"

#include "Windows/manualwindow.h"
#include "Windows/gamewindow.h"
#include "Windows/choosemissionwindow.h"
#include "Windows/mainmenu.h"
#include "aboutwindow.h"

GameStateManager::GameStateManager()
{

}

StateWindow *GameStateManager::createState(const GameState state)
{
	StateWindow *stateWindow = StateManager::createState(state);
	if (stateWindow != nullptr)
	{
		if (state == StateManager::ABOUT)
			static_cast<AboutWindow*>(stateWindow)->setBackState(MANUAL);
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
	default:
		break;
	}
	return stateWindow;
}
