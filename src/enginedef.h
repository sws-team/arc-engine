#ifndef ENGINEDEF_H
#define ENGINEDEF_H

#include "stdheader.h"

typedef int SceneType;
typedef int TextureType;
typedef int TranslationType;
typedef int SoundType;
typedef int MusicType;
typedef int FileType;
typedef int ShaderType;
typedef int FontType;
typedef int WindowType;
typedef int NotificationType;

namespace ArcEngine
{
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
		PATH,
		CHECKBOX,
		SCROLL_AREA,
		ZOOM_VIEW,
		LINE,
	};

	static constexpr int undefType = -1;
	static constexpr float FRAME_TIME = 1.0f / 60.0f;
	static const sf::Color GrayColor = sf::Color(133, 133, 133);
	static const sf::Color HoverColor = sf::Color(32, 32, 32, 96);
	static const sf::Color DisabledColor = sf::Color(96, 96, 96, 96);
	static const int CRYPTO_VALUE = 12;
	static const std::string saveFileName = std::string("saves.sav");
	static constexpr float MSEC = 1000;
	static const sf::String separator = ": ";
	static const sf::String endline = "\n";
	static constexpr float DEFAULT_WINDOW_FADE_TIME = 2000;
	static const sf::String NAME = "ArcEngine";
	static const sf::String VERSION = ENGINE_VERSION;
	static const sf::String GAME_VERSION = APP_VERSION;
	static const sf::String GAME_NAME = APP_NAME;
}

//forward classes
class ArcObject;
class ArcButton;
class ArcSprite;
class ArcLabel;
class ArcClippedSprite;
class ArcRect;
class ArcVariant;
class ArcLine;
class ArcPolygon;

typedef std::function<void(const std::string&, ArcObject *, const std::vector<ArcVariant>&)> NotificationCallbackType;

#endif // ENGINEDEF_H
