#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include "managers.h"

class GameStateManager : public StateManager
{
public:
	GameStateManager();
	StateWindow *createState(const GameState state) override;

	enum GAME_STATE
	{
		UNKNOWN = StateManager::USER_STATE,

		MANUAL,
		CHOOSE_MISSION,
		IN_GAME,
	};
};

#endif // GAMESTATEMANAGER_H
