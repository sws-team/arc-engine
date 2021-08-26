#ifndef ENGINE_H
#define ENGINE_H

#include "stdheader.h"

class TranslationsManager;
class TexturesManager;
class StateManager;
class SoundManager;
class SettingsManager;
class FontManager;
class FilesManager;
class Options;
class ShadersManager;
class MainWindow;
class ResourcesManager;

class Engine
{
public:
	static Engine &Instance();
	void clearInstance();

	void setStateManager(StateManager *manager);
	void setOptions(Options *manager);

	TexturesManager *texturesManager();
	TranslationsManager *translationsManager();
	StateManager *stateManager();
	SoundManager *soundManager();
	SettingsManager *settingsManager();
	FontManager *fontManager();
	FilesManager *filesManager();
	ShadersManager *shadersManager();
	ResourcesManager *resourceManager();

	template<typename Class> Class castedStateManager()
	{
		return static_cast<Class>(p_stateManager);
	}
	template<typename Class> Class* options()
	{
		return static_cast<Class*>(p_options);
	}
	Options *getOptions();

	void reset();

	void setWindow(MainWindow *window);
	MainWindow *window();
	sf::RenderWindow *renderWindow();

	static sf::String EngineName();
	static sf::String EngineVersion();

	static std::string assetsPath();

	std::string appVersion() const;
	void setAppVersion(const std::string &appVersion);

	sf::String appName() const;
	void setAppName(const sf::String &appName);

private:
	Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	TexturesManager *p_texturesManager = nullptr;
	TranslationsManager *p_translationsManager = nullptr;
	StateManager *p_stateManager = nullptr;
	SoundManager *p_soundManager = nullptr;
	SettingsManager *p_settingsManager = nullptr;
	FontManager *p_fontManager = nullptr;
	FilesManager *p_filesManager = nullptr;
	ShadersManager *p_shadersManager = nullptr;
	ResourcesManager *p_resourceManager = nullptr;
	Options *p_options = nullptr;

	MainWindow *p_window = nullptr;

	std::string m_appVersion;
	sf::String m_appName;
};

#endif // ENGINE_H
