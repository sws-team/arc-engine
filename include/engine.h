#ifndef ENGINE_H
#define ENGINE_H

#include "stdheader.h"

class TranslationsManager;
class TexturesManager;
class StateManager;
class SoundManager;
class SettingsManager;
class FontManager;
class GlobalVariables;
class FilesManager;
class Options;
class ShadersManager;
class MainWindow;

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
	GlobalVariables *globalVariables();
	ShadersManager *shadersManager();

	template<typename Class>
	Class castedStateManager()
	{
		return static_cast<Class>(p_stateManager);
	}
	template<typename Class>
        Class* options()
	{
                return static_cast<Class*>(p_options);
	}
	Options *getOptions();

	void reset();

	void setWindow(MainWindow *window);
	MainWindow *window();

	static sf::String EngineName();
	static sf::String EngineVersion();

private:
	Engine();
	Engine(const Engine& root) = delete;
	Engine& operator=(const Engine&) = delete;

	TexturesManager *p_texturesManager;
	TranslationsManager *p_translationsManager;
	StateManager *p_stateManager;
	SoundManager *p_soundManager;
	SettingsManager *p_settingsManager;
	FontManager *p_fontManager;
	FilesManager *p_filesManager;
	GlobalVariables *p_globalVariables;
	ShadersManager *p_shadersManager;
	Options *p_options;

	MainWindow *p_window;
};

#endif // ENGINE_H
