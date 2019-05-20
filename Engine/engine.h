#ifndef ENGINE_H
#define ENGINE_H

#include "stdheader.h"

class Engine
{
public:
	static Engine &Instance();

//	void loadTextures(const map<int, string>& fileNames);

//	Texture &getTexture(const int id);

	enum GAME_STATE
	{
		UNKNNOWN,
		INTRO,
		MAIN_MENU,
		OPTIONS,
		ABOUT,
		CLOSING,
		EXIT,

		LOAD_GAME,
		NEW_GAME,

		MAIN_LOCATION,
		SHOP_LOCATION,
		GARAGE_LOCATION,

		IN_GAME,
	};

	GAME_STATE getState() const;
	void setState(const GAME_STATE &state);

	class StateWindow *createState(const GAME_STATE &state);

private:
	Engine();
	Engine(const Engine& root) = delete;
	Engine& operator=(const Engine&) = delete;

//	map<int, Texture> m_textures;
	GAME_STATE m_state;
};

#endif // ENGINE_H
