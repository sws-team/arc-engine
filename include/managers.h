#ifndef MANAGERS_H
#define MANAGERS_H

#include "stdheader.h"
#include "enginedef.h"
#include <sstream>

class StateWindow;
class ArcDebug;
class ArcObject;

class Manager
{
public:
	virtual void reset();
};

#define SCALE_FACTOR Engine::Instance().settingsManager()->getScaleFactor()
#define DEF_SCALE_FACTOR const sf::Vector2f scaleFactor = SCALE_FACTOR;
#define RESOLUTIONF Engine::Instance().settingsManager()->getResolutionF()
#define DEBUG_OBJECT(x) Engine::Instance().getOptions()->debugObject(x);
#define PLAY_SOUND(x) Engine::Instance().soundManager()->playOnce(x);
#define SCREEN_SIZE Engine::Instance().settingsManager()->getResolutionF()

class SettingsManager : public Manager
{
public:
	SettingsManager();

	void reset() override;

	sf::Vector2i getResolution() const;
	sf::Vector2f getResolutionF() const;
	void setResolution(const sf::Vector2i &value);

	float getSoundLevel() const;
	void setSoundLevel(float value);

	bool getFullscreen() const;
	void setFullscreen(bool value);

	float getMusicLevel() const;
	void setMusicLevel(float value);

	bool getShaders() const;
	void setShaders(bool value);

	sf::Vector2f getScaleFactor() const;

	static const sf::Vector2i defaultResolution;

private:
	sf::Vector2i resolution;
	float soundLevel;
	float musicLevel;
	bool fullscreen;
	bool shaders;
};

#define TR(x) Engine::Instance().translationsManager()->translate(x)

class TranslationsManager : public Manager
{
public:
	TranslationsManager();

	void addTranslation(const std::string& name, const std::map<int, sf::String> &translations);

	sf::String translate(TranslationType id) const;
	std::vector<sf::String> getAvaliableLanguageNames() const;
	void setCurrentLanguage(const std::string& lang);
	void setCurrentLanguageByName(const sf::String& name);
	sf::String currentLanguageName() const;
	std::string currentLanguage() const;

	enum DEFAULT_TR_TEXTS
	{
		LANGUAGE_ID,

		SETTINGS,
		SOUND,
		MUSIC,
		FULLSCREEN,
		RESOLUTION,
		LANGUAGE,
		SHADERS,
		ACCEPT,
		CANCEL,

		USER_TRANSLATION_ID,
	};

private:
	std::map<std::string, std::map<TranslationType, sf::String> > langs;
	std::string currentLangName;

};

#define GET_TEXTURE(x) Engine::Instance().texturesManager()->getTexture(x)

class TexturesManager : public Manager
{
public:
	TexturesManager();

	sf::Texture &getTexture(TextureType type);
	void addTexture(const TextureType type, const sf::Texture& texture);
	void addTexture(const TextureType type, const std::string &path);
	void addTexture(const TextureType type, const char *data, const size_t size);

	//defaultTextures
	enum DEFAULT_TEXTURE_ID
	{
		ICON,
		CURSOR,
		CLOSE_BACKGROUND,
		ABOUT_BACKGROUND,
		INTRO_BACKGROUND,
		LOADING_BACKGROUND,

		CHOOSE_LIST_NEXT,
		CHOOSE_LIST_PREVIOUS,

		USER_TEXTURE_ID
	};

private:
	std::map<TextureType, sf::Texture> m_textures;
};


class StateManager : public Manager
{
public:
	StateManager();

	GameState getState() const;
	void setState(const GameState state);
	virtual StateWindow *createState(const GameState state);
	enum STATE
	{
		UNKNOWN,
		INTRO,
		LOADING,
		MENU,
		SETTINGS,
		ABOUT,
		CLOSING,
		EXIT,

		USER_STATE,
	};
private:
	GameState m_state;
};

class SoundManager : public Manager
{
public:
	SoundManager();
	~SoundManager();

	void clearAll();

	void addSound(const SoundType type, const std::string& path);
	void addSound(const SoundType type, const char *data, const size_t size);

	void addMusic(const MusicType type, const std::string& path);
	void addMusic(const MusicType type, const char *data, const size_t size);

	void startBackgroundSound(const MusicType type);
	void endBackgroundSound();

	void playOnce(const SoundType type);
	void playLooped(const SoundType type);
	void stop(const SoundType type);
	void stopAllSounds();

	void updateVolume();

	void pauseMusic();
	void unpauseMusic();

	void setMusicLooped(bool loop);
	bool isMusicFinished() const;

	enum DEFAULT_SOUND_ID
	{
		CLICK,
		HOVER,
		INTRO,

		USER_SOUND_ID
	};

	enum DEFAULT_MUSIC_ID
	{
		MAIN_MENU_MUSIC,
		CREDITS_MUSIC,

		USER_MUSIC_ID
	};

	void setQuiet(bool quiet);

	bool quiet() const;

private:
	struct SFX
	{
		sf::SoundBuffer *buffer;
		sf::Sound *sound;
	};
	std::map<SoundType, SFX> sounds;
	std::map<SoundType, sf::Music*> musics;

	sf::Music *currentMusic;

	void play(SoundType type, bool loop);

	bool m_quiet;
};

class FontManager : public Manager
{
public:
	FontManager();

	void addFont(const FontType type, sf::Font *font);
	void addFont(const FontType type, const std::string &path);
	void addFont(const FontType type, const char *data, const size_t size);

	sf::Font &font();
	sf::Font &getFont(FontType type);

	unsigned int getCharSize(const unsigned int size);

	enum DEFAULT_SOUND_ID
	{
		DEFAULT_FONT,
		HIEROGLYPH_FONT,

		USER_FONT_ID,
	};

	void setFontModifier(float fontModifier);

private:
	std::map<FontType, sf::Font*> m_fonts;
	float m_fontModifier;
};

class GlobalVariables : public Manager
{
public:
	GlobalVariables();

	void setApplicationPath(const sf::String &applicationPath);

	std::string saveGameSettings() const;
	void loadGameSettings(const std::string& data);

	static std::string to_string_with_precision(const float a_value, const int n = 6);

	static std::string encode(const std::string& str);
	static std::string decode(const std::string& str);

	std::string appVersion() const;
	void setAppVersion(const std::string &appVersion);

	sf::String appName() const;
	void setAppName(const sf::String &appName);

	int fps() const;
	void setFps(int fps);

private:
	int m_fps;
	std::string m_appVersion;
	sf::String m_appName;
};

class MainWindow;
class Options : public Manager
{
	//classes like level, panel, cursor, camera
public:
	Options();
	virtual ~Options();

	MainWindow *mainWindow();
	void setMainWindow(MainWindow *window);
	virtual void updateWindow();
	void debugObject(ArcObject* object);

	virtual void globalCallbacks();
	virtual void clear();
	virtual void globalEventFilter(sf::Event* event);
	virtual void globalDraw(sf::RenderTarget *target);

	bool isResourcesLoaded() const;
	void setResourcesLoaded(bool loaded);

protected:
	MainWindow *mw = nullptr;
	ArcDebug *debug = nullptr;
	bool m_resourcesLoaded;
};

class FilesManager : public Manager
{
public:
	struct GameFile
	{
		char *data;
		size_t size;
	};
	GameFile getData(FileType type) const;

	void addFile(const FileType type, const std::string &path);
	void addFile(const FileType type, const char *data, const size_t size);

private:
	std::map<FileType, GameFile> m_data;
};

class ShadersManager : public Manager
{
public:
	std::string getData(ShaderType type);
	void addShader(const ShaderType type, const std::string& data);

private:
	std::map<ShaderType, std::string> m_shaders;
};

#endif // MANAGERS_H
