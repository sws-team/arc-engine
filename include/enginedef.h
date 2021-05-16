#ifndef ENGINEDEF_H
#define ENGINEDEF_H

#include "stdheader.h"

typedef int GameState;
typedef int TextureType;
typedef int TranslationType;
typedef int SoundType;
typedef int MusicType;
typedef int FileType;
typedef int ShaderType;
typedef int FontType;

namespace EngineDefs
{
	enum JOYSTICK_KEYS
	{
		KEY_ESCAPE,
		KEY_ACCEPT,
	};

	static constexpr float FRAME_TIME = 1.0f / 60.0f;
	static const sf::Color GrayColor = sf::Color(133, 133, 133);
	static const int CRYPTO_VALUE = 12;
	static const std::string settingsFilePath = std::string("settings.ini");
	static const std::string saveFileName = std::string("saves.sav");
	static constexpr float MSEC = 1000;
	static const sf::String separator = ": ";
	static const sf::String endline = "\n";
}

#endif // ENGINEDEF_H
