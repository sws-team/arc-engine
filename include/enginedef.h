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

namespace ArcEngine
{
	enum JOYSTICK_KEY
	{
		KEY_ESCAPE,
		KEY_ACCEPT,
	};
	enum OBJECT_TYPE
	{
		UNDEF,
		OBJECT,
		SPRITE,
		LABEL,
		BUTTON,
		ANIMATED_SPRITE,
		SKELETON_ANIMATION,
		LAYOUT,
		POLYGON,
		RECT,
		NAVIGATION_MAP,
	};

	static constexpr float FRAME_TIME = 1.0f / 60.0f;
	static const sf::Color GrayColor = sf::Color(133, 133, 133);
	static const sf::Color HoverColor = sf::Color(32, 32, 32, 96);
	static const sf::Color DisabledColor = sf::Color(96, 96, 96, 96);
	static const int CRYPTO_VALUE = 12;
	static const std::string settingsFilePath = std::string("settings.ini");
	static const std::string saveFileName = std::string("saves.sav");
	static constexpr float MSEC = 1000;
	static const sf::String separator = ": ";
	static const sf::String endline = "\n";
	static constexpr float DEFAULT_WINDOW_FADE_TIME = 2000;
}

//forward classes
class ArcObject;
class ArcButton;
class ArcSprite;
class ArcLabel;
class ArcClippedSprite;
class ArcRect;

#endif // ENGINEDEF_H
