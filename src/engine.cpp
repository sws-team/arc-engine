#include "engine.h"
#include "managers.h"
#include "mainwindow.h"

Engine &Engine::Instance()
{
	static Engine singleton;
	return singleton;
}

Engine::Engine() :
  p_stateManager(nullptr)
  ,p_options(nullptr)
{
	p_fontManager = new FontManager();
	p_soundManager = new SoundManager();
	p_translationsManager = new TranslationsManager();
	p_texturesManager = new TexturesManager();
	p_settingsManager = new SettingsManager();
	p_filesManager = new FilesManager();
	p_globalVariables = new GlobalVariables();
	p_shadersManager = new ShadersManager();
	p_window = nullptr;
	reset();
}

void Engine::clearInstance()
{
	p_soundManager->clearAll();
}

void Engine::setStateManager(StateManager *manager)
{
	p_stateManager = manager;
}

void Engine::setOptions(Options *manager)
{
	p_options = manager;
}

TexturesManager *Engine::texturesManager()
{
	return p_texturesManager;
}

TranslationsManager *Engine::translationsManager()
{
	return p_translationsManager;
}

StateManager *Engine::stateManager()
{
	return p_stateManager;
}

SoundManager *Engine::soundManager()
{
	return p_soundManager;
}

SettingsManager *Engine::settingsManager()
{
	return p_settingsManager;
}

FontManager *Engine::fontManager()
{
	return p_fontManager;
}

FilesManager *Engine::filesManager()
{
	return p_filesManager;
}

GlobalVariables *Engine::globalVariables()
{
	return p_globalVariables;
}

ShadersManager *Engine::shadersManager()
{
	return p_shadersManager;
}

Options *Engine::getOptions()
{
	return p_options;
}

void Engine::setWindow(MainWindow *window)
{
	p_window = window;
}

MainWindow *Engine::window()
{
	return p_window;
}

sf::RenderWindow *Engine::renderWindow()
{
	return static_cast<sf::RenderWindow*>(p_window);
}

sf::String Engine::EngineName()
{
	return sf::String("ArcEngine");
}

sf::String Engine::EngineVersion()
{
	return sf::String(ENGINE_VERSION);
}

std::string Engine::assetsPath()
{
#if defined(SFML_SYSTEM_WINDOWS) || \
	defined (SFML_SYSTEM_LINUX) || \
	defined (SFML_SYSTEM_MACOS)
#ifdef ASSETS_PATH
	return ASSETS_PATH;
#endif
#elif defined(SFML_SYSTEM_ANDROID)
#elif defided(SFML_SYSTEM_IOS)
#endif
	return std::string();
}

void Engine::reset()
{
	p_texturesManager->reset();
	p_translationsManager->reset();
//	p_stateManager->reset();
}

