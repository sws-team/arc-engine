#ifndef MANAGERS_H
#define MANAGERS_H

#include "stdheader.h"
#include "engine.h"
#include "enginedef.h"
#include <ArcTimer>
#include "arcwindow.h"
#include "arcscene.h"
#include "arcvariant.h"

class ArcWindow;
class ArcDebug;
class ArcObject;
class MainWindow;
class DebugSection;

namespace ArcEngine {
ArcObject *findChild(const std::string& name);
ArcObject *findChildPath(const std::string& path);
}

class Manager
{
public:
	virtual void reset();
};

#define RESOLUTIONF Engine::Instance().settingsManager()->getResolutionF()
#define DEBUG_OBJECT(x) Engine::Instance().getOptions()->debugObject(x);
#define PLAY_SOUND(x) Engine::Instance().soundManager()->playOnce(x);
#define STOP_SOUND(x) Engine::Instance().soundManager()->stop(x);
#define CHANGE_SCENE(x) Engine::Instance().sceneManager()->setSceneType(x);
#define NOTIFY(x, y) Engine::Instance().notificationManager()->notify(x, y);

#define SCENE_MANAGER Engine::Instance().sceneManager()

#define RESOLUTION SettingsManager::defaultResolution

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

	static const sf::Vector2f defaultResolution;
	static const std::string settingsFilePath;

	std::string saveGameSettings() const;
	void loadGameSettings(const std::string& data);

	void save();
	void load();

private:
	sf::Vector2i resolution;
	float soundLevel;
	float musicLevel;
	bool fullscreen;
	bool shaders;
	std::string saveDataFileName;
};

#define TR(x) Engine::Instance().translationsManager()->translate(x)

class TranslationsManager : public Manager
{
public:
	TranslationsManager();

	void addTranslation(const std::string& name, const std::map<int, sf::String> &translations);

	sf::String translate(TranslationType id) const;
	sf::String translate(TranslationType id, const std::string& language) const;
	std::vector<sf::String> getAvaliableLanguageNames() const;
	void setCurrentLanguage(const std::string& lang);
	void setCurrentLanguageByName(const sf::String& name);
	sf::String currentLanguageName() const;
	std::string currentLanguage() const;

	enum DEFAULT_TR_TEXTS
	{
		LANGUAGE_ID,

		USER_TRANSLATION_ID,
	};

private:
	std::map<std::string, std::map<TranslationType, sf::String> > langs;
	std::string currentLangName;

};

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

class SceneManager : public Manager
{
public:
	SceneManager();

	ArcScene *currentScene() const;
	void setCurrentScene(ArcScene *scene);

	SceneType currentSceneType() const;
	void setSceneType(const SceneType type);

	enum SCENE
	{
		UNKNOWN,
		INTRO,
		LOADING,
		MENU,
		SETTINGS,
		ABOUT,
		CLOSING,
		EXIT,
		GAME,

		USER_SCENE,
	};
	typedef std::function<ArcScene*()> Creator;

	void addScene(SceneType type, const Creator& creator);
	template<class T> void addScene(SceneType type) {
		addScene(type, std::bind(&ArcScene::create<T>));
	}
	ArcScene *createScene(SceneType type) const;
protected:
private:
	SceneType m_type = UNKNOWN;
	ArcScene *m_scene = nullptr;
	std::unordered_map<SceneType, Creator> scenes;
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
	void addDebugSection(DebugSection *section);
	void removeDebugSection(DebugSection *section);

	void addNotificationCallback(const std::function<void(const std::string&, const ArcVariant&)>& callback);

	virtual void globalCallbacks();
	virtual void clear();
	virtual bool globalEventFilter(sf::Event* event);
	virtual void globalDraw(sf::RenderTarget *target);
	virtual void globalNotifications(const std::string &name, const ArcVariant &value);

	bool isResourcesLoaded() const;
	void setResourcesLoaded(bool loaded);

protected:
	MainWindow *mw = nullptr;
	ArcDebug *debug = nullptr;
	bool m_resourcesLoaded;

	std::vector<std::function<void(const std::string&, const ArcVariant&)>> notificationCallbacks;
};

class FilesManager : public Manager
{
public:
	struct GameFile
	{
		char *data = nullptr;
		size_t size  = 0;
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

namespace dragonBones {
class SFMLFactory;
}

class ResourcesManager : public Manager
{
public:
	ResourcesManager();

	static const std::string texturesFileName;
	static const std::string soundsFileName;
	static const std::string resourcesFileName;
	static const std::string preloadFileName;

	void update();

	static void addTexture(TextureType type, const std::string& data);
	static void addSound(SoundType type, const std::string& data);
	static void addMusic(MusicType type, const std::string& data);
	static void addFont(FontType type, const std::string& data);
	static void addFile(FileType type, const std::string& data);
	static void addShader(ShaderType type, const std::string& data);

	void loadSkeletonData(FileType atlasType, FileType skeletonType, TextureType textureType);

	static std::string encode(const std::string& str);
	static std::string decode(const std::string& str);
private:
	dragonBones::SFMLFactory* skeletonAnimationFactory = nullptr;
	ArcTimer timer;
};

class NotificationManager : public Manager
{
public:
	NotificationManager();

	enum NOTIFICATION_TYPE
	{
		UNKNOWN,
		//scenes
		SCENE_CHANGED,
		//windows
		WINDOW_OPENING,
		WINDOW_OPENED,
		WINDOW_CLOSING,
		WINDOW_CLOSED,
		//button
		BUTTON_CLICKED,

		ACTION_STARTED,
		ACTION_FINISHED,
	};

	void notify(NotificationType type, const ArcVariant& value);
	void notify(const std::string& name, const ArcVariant &value);

	std::optional<std::string> notificationName(NotificationType type) const;

protected:
	std::optional<int> addCallback(NotificationType type, const CallbackType& callback);
	std::optional<int> addCallback(const std::string& name, const CallbackType& callback);
	void removeCallback(int id);

private:
	friend class ArcObject;
	struct NotificationData {
		CallbackType callback;
		int id = -1;
	};
	int counter = 0;
	std::unordered_map<std::string, std::vector<NotificationData> > callbacks;

	const static std::unordered_map<NOTIFICATION_TYPE, std::string> NOTIFICATIONS;
};

class WindowsManager : public Manager
{
public:
	WindowsManager();

	typedef std::function<ArcWindow*()> Creator;

	template<class T> void showWindow(WindowType type) {
		NOTIFY(NotificationManager::NOTIFICATION_TYPE::WINDOW_OPENING, type);
		opening.push(std::make_pair(type, std::bind(&ArcWindow::create<T>)));
	}
	void closeWindow(ArcWindow *window);

	bool isWindowOpened(WindowType type) const;
	ArcWindow *getWindow(WindowType type) const;

	void update();

private:
	constexpr static float ANIMATION_TIME = 100.f;

	std::queue<std::pair<WindowType, Creator>> opening;
	std::queue<ArcWindow*> closing;
	std::unordered_set<ArcWindow*> opened;

	void removeWindow(ArcWindow* window);
};


#endif // MANAGERS_H
