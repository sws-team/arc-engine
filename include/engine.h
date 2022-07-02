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
class NotificationManager;
class WindowsManager;

class Engine
{
public:
	static Engine &Instance();
	void clearInstance();

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
	NotificationManager *notificationManager();
	WindowsManager *windowsManager();

	template<typename Class> Class castedStateManager() {
		return static_cast<Class>(p_stateManager);
	}
	template<typename Class> Class* options() {
		return static_cast<Class*>(p_options);
	}
	Options *getOptions();

	void reset();

	void setWindow(MainWindow *window);
	MainWindow *window();
	sf::RenderWindow *renderWindow();

	static std::string assetsPath();

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
	NotificationManager *p_notificationManager = nullptr;
	WindowsManager *p_windowsManager = nullptr;
	Options *p_options = nullptr;

	MainWindow *p_window = nullptr;
};

#endif // ENGINE_H
