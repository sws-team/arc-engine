#include "engine.h"
#include "managers.h"
#include "mainwindow.h"

Engine &Engine::Instance()
{
	static Engine singleton;
	return singleton;
}

Engine::Engine()
{
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	p_fontManager = new FontManager();
	p_soundManager = new SoundManager();
	p_translationsManager = new TranslationsManager();
	p_texturesManager = new TexturesManager();
	p_settingsManager = new SettingsManager();
	p_filesManager = new FilesManager();
	p_shadersManager = new ShadersManager();
	p_resourceManager = new ResourcesManager();
	p_window = nullptr;
	reset();
}

void Engine::clearInstance()
{
	p_soundManager->clearAll();
	p_options->clear();
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

ShadersManager *Engine::shadersManager()
{
	return p_shadersManager;
}

ResourcesManager *Engine::resourceManager()
{
	return p_resourceManager;
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

std::string Engine::appVersion() const
{
	return m_appVersion;
}

void Engine::setAppVersion(const std::string &appVersion)
{
	m_appVersion = appVersion;
}

sf::String Engine::appName() const
{
	return m_appName;
}

void Engine::setAppName(const sf::String &appName)
{
	m_appName = appName;
}

void Engine::reset()
{
	p_texturesManager->reset();
	p_translationsManager->reset();
//	p_stateManager->reset();
}

