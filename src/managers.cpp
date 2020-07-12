#include "managers.h"
#include "enginedef.h"
#include "engine.h"

#include "mainwindow.h"
#include "Windows/introwindow.h"
#include "Windows/aboutwindow.h"
#include "Windows/closewindow.h"
#include "Windows/settingswindow.h"

#ifdef OS_WIN
#include "windows.h"
#include <shlobj.h>
#endif
#ifdef OS_UNIX
#include <X11/Xlib.h>
#endif
#ifdef OS_MAC
#include <CoreGraphics/CoreGraphics.h>
#endif

#include <cstring>


void Manager::reset()
{

}

//==================SETTINS MANAGER===================
const sf::Vector2i SettingsManager::defaultResolution = sf::Vector2i(1920, 1080);

SettingsManager::SettingsManager():
	resolution(defaultResolution)
  ,soundLevel(100)
  ,musicLevel(100)
  ,fullscreen(true)
  ,shaders(true)
{
	reset();
}

void SettingsManager::reset()
{
#ifdef OS_ANDROID
	const sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
	resolution.x = videoMode.width;
	resolution.y = videoMode.height;
#else
	resolution = defaultResolution;
#endif
}

float SettingsManager::getMusicLevel() const
{
	return musicLevel;
}

void SettingsManager::setMusicLevel(float value)
{
	musicLevel = value;
}

sf::Vector2f SettingsManager::getScaleFactor() const
{
	return sf::Vector2f(static_cast<float>(resolution.x)/defaultResolution.x,
					static_cast<float>(resolution.y)/defaultResolution.y);
}

bool SettingsManager::getShaders() const
{
    return shaders;
}

void SettingsManager::setShaders(bool value)
{
    shaders = value;
}

bool SettingsManager::getFullscreen() const
{
    return fullscreen;
}

void SettingsManager::setFullscreen(bool value)
{
    fullscreen = value;
}

float SettingsManager::getSoundLevel() const
{
	return soundLevel;
}

void SettingsManager::setSoundLevel(float value)
{
	soundLevel = value;
}

sf::Vector2i SettingsManager::getResolution() const
{
	return resolution;
}

sf::Vector2f SettingsManager::getResolutionF() const
{
	return sf::Vector2f(resolution.x, resolution.y);
}

void SettingsManager::setResolution(const sf::Vector2i &value)
{
	resolution = value;
}
//==================TRANSLATIONS MANAGER===================

TranslationsManager::TranslationsManager()
{

}

void TranslationsManager::addTranslation(const std::string &name, const std::map<int, sf::String> &translations)
{
	langs.insert(std::pair<std::string, std::map<int, sf::String> >(name, translations));
}

sf::String TranslationsManager::translate(TranslationType id) const
{
	return langs.at(currentLangName).at(id);
}

std::vector<sf::String> TranslationsManager::getAvaliableLanguageNames() const
{
	std::vector<sf::String> langsNames;
	for(const auto& it : langs)
		langsNames.push_back(it.second.at(LANGUAGE_ID));
	return langsNames;
}

void TranslationsManager::setCurrentLanguage(const std::string &lang)
{
	if (langs.count(lang) == 0)
	{
		currentLangName = "english";
		return;
	}
	currentLangName = lang;
}

void TranslationsManager::setCurrentLanguageByName(const sf::String &name)
{
	for(const auto& it : langs)
	{
		if (it.second.at(LANGUAGE_ID) == name)
		{
			setCurrentLanguage(it.first);
			break;
		}
	}
}

sf::String TranslationsManager::currentLanguageName() const
{
	return langs.at(currentLangName).at(LANGUAGE_ID);
}

std::string TranslationsManager::currentLanguage() const
{
	return currentLangName;
}

//==================TEXTURES MANAGER===================
TexturesManager::TexturesManager()
{

}

sf::Texture &TexturesManager::getTexture(TextureType type)
{
	return m_textures.at(type);
}

void TexturesManager::addTexture(const TextureType type, const std::string &path)
{
	sf::Texture texture;
	if (!texture.loadFromFile(path))
		return;
	m_textures.insert(std::pair<int, sf::Texture>(type, texture));
}

void TexturesManager::addTexture(const TextureType type, const char *data, const size_t size)
{
	sf::Texture texture;
	void *textureData = malloc(size);
	std::memcpy(textureData,(void*)data, size);
	if (!texture.loadFromMemory(textureData, size))
		return;
	m_textures.insert(std::pair<int, sf::Texture>(type, texture));
}

//==================STATE MANAGER===================
StateManager::StateManager()
{
	m_state = INTRO;
}

GameState StateManager::getState() const
{
	return m_state;
}

void StateManager::setState(const int state)
{
	m_state = state;
}

StateWindow *StateManager::createState(const GameState state)
{
	StateWindow *stateWindow = nullptr;
	switch (state)
	{
	case INTRO:
		stateWindow = new IntroWindow();	
		break;
	case SETTINGS:
		stateWindow = new SettingsWindow();
		break;
	case ABOUT:
		stateWindow = new AboutWindow();
		break;
	case CLOSING:
		stateWindow = new class CloseWindow();
		break;
	default:
		break;
	}
	return stateWindow;
}

SoundManager::SoundManager()
	: currentMusic(nullptr)
	,m_quiet(false)
{

}

SoundManager::~SoundManager()
{
	clearAll();
}

void SoundManager::clearAll()
{
	endBackgroundSound();
	for(auto it : musics)
	{
		it.second->stop();
		delete it.second;
	}
	for(auto it : sounds)
	{
		it.second.sound->stop();
		delete it.second.sound;
		delete it.second.buffer;
	}
}

void SoundManager::addSound(const SoundType type, const std::string &path)
{
	SFX sfx;
	sfx.buffer = new sf::SoundBuffer;
	sfx.sound = new sf::Sound(*sfx.buffer);
	if(!sfx.buffer->loadFromFile(path))
	{
		delete sfx.buffer;
		delete sfx.sound;
		return;
	}
	sounds.insert(std::pair<SoundType, SFX>(type, sfx));
}

void SoundManager::addSound(const SoundType type, const char *data, const size_t size)
{
	SFX sfx;
	sfx.buffer = new sf::SoundBuffer;
	sfx.sound = new sf::Sound(*sfx.buffer);
	void *soundData = malloc(size);
	std::memcpy(soundData,(void*)data, size);
	if(!sfx.buffer->loadFromMemory(soundData, size))
	{
		delete sfx.buffer;
		delete sfx.sound;
		free(soundData);
		return;
	}
	sounds.insert(std::pair<SoundType, SFX>(type, sfx));
}

void SoundManager::addMusic(const MusicType type, const std::string &path)
{
	sf::Music *music = new sf::Music;
	music->setLoop(true);
	const bool ok = music->openFromFile(path);
	if (!ok)
		return;
	musics.insert(std::pair<MusicType, sf::Music*>(type, music));
}

void SoundManager::addMusic(const MusicType type, const char *data, const size_t size)
{
	sf::Music *music = new sf::Music;
	music->setLoop(true);
	void *musicData = malloc(size);
	std::memcpy(musicData,(void*)data, size);
	const bool ok = music->openFromMemory(musicData, size);
	if (!ok)
		return;
	musics.insert(std::pair<MusicType, sf::Music*>(type, music));
}

void SoundManager::startBackgroundSound(const MusicType type)
{
	if (m_quiet)
		return;
	auto it = musics.find(type);
	if (it == musics.end())
		return;

	if (it->second == currentMusic)
		return;
	endBackgroundSound();

	currentMusic = it->second;
	currentMusic->setVolume(Engine::Instance().settingsManager()->getMusicLevel());

	it->second->play();
}

void SoundManager::endBackgroundSound()
{
	if (currentMusic == nullptr)
		return;
	currentMusic->stop();
}

void SoundManager::playOnce(const SoundType type)
{
	play(type, false);
}

void SoundManager::playLooped(const SoundType type)
{
	play(type, true);
}

void SoundManager::stop(const SoundType type)
{
	auto it = sounds.find(type);
	if (it != sounds.end())
		sounds[type].sound->stop();
}

void SoundManager::stopAllSounds()
{
	for(auto snd : sounds)
		if (snd.second.sound->getLoop())
			snd.second.sound->stop();
}

void SoundManager::play(SoundType type, bool loop)
{
	if (m_quiet)
		return;
	auto it = sounds.find(type);
	if (it == sounds.end())
		return;

	it->second.sound->setLoop(loop);
	it->second.sound->setVolume(Engine::Instance().settingsManager()->getSoundLevel());
	it->second.sound->play();
}

bool SoundManager::quiet() const
{
	return m_quiet;
}

void SoundManager::setQuiet(bool quiet)
{
	m_quiet = quiet;
}

void SoundManager::updateVolume()
{
	for(const std::pair<MusicType, sf::Music*>& music : musics)
		music.second->setVolume(Engine::Instance().settingsManager()->getMusicLevel());
	for(const std::pair<SoundType, SFX>& sfx : sounds)
		sfx.second.sound->setVolume(Engine::Instance().settingsManager()->getSoundLevel());
}

void SoundManager::pauseMusic()
{
	if (currentMusic != nullptr)
		currentMusic->pause();
	for(const std::pair<SoundType, SFX>& sfx : sounds)
	{
		if (sfx.second.sound->getStatus() == sf::SoundSource::Playing)
			sfx.second.sound->pause();
	}
}

void SoundManager::unpauseMusic()
{
	if (m_quiet)
		return;
	if (currentMusic != nullptr)
		currentMusic->play();
	for(const std::pair<SoundType, SFX>& sfx : sounds)
	{
		if (sfx.second.sound->getStatus() == sf::SoundSource::Paused)
			sfx.second.sound->play();
	}
}

void SoundManager::setMusicLooped(bool loop)
{
	if (currentMusic != nullptr)
		currentMusic->setLoop(loop);
}

bool SoundManager::isMusicFinished() const
{
	if (currentMusic == nullptr)
		return true;
	return currentMusic->getStatus() == sf::Sound::Stopped;
}

FontManager::FontManager()
	: m_fontModifier(1.f)
{

}

void FontManager::addFont(const FontType type, sf::Font* font)
{
	m_fonts.insert(std::pair<FontType, sf::Font*>(type, font));
}

void FontManager::addFont(const FontType type, const std::string &path)
{
	sf::Font *font = new sf::Font();
	if (!font->loadFromFile(path))
		return;
	addFont(type, font);
}

void FontManager::addFont(const FontType type, const char *data, const size_t size)
{
	sf::Font *font = new sf::Font();
	void *fontData = malloc(size);
	std::memcpy(fontData,(void*)data, size);
	if (!font->loadFromMemory(fontData, size))
		return;
	addFont(type, font);
}

sf::Font &FontManager::font()
{
	if (Engine::Instance().translationsManager()->currentLanguage() == "schinese")
		return getFont(HIEROGLYPH_FONT);
	return getFont(DEFAULT_FONT);
}

sf::Font &FontManager::getFont(FontType type)
{
	return *m_fonts.at(type);
}

unsigned int FontManager::getCharSize(const unsigned int size)
{
	return static_cast<int>(floor(size * m_fontModifier));
}

void FontManager::setFontModifier(float fontModifier)
{
	m_fontModifier = fontModifier;
}

GlobalVariables::GlobalVariables() :
	m_fps(false)
{

}


sf::Vector2i GlobalVariables::getScreenResolution() const
{
	const sf::VideoMode defaultMode = sf::VideoMode::getDesktopMode();
	sf::Vector2i resolution;
	resolution.x = defaultMode.width;
	resolution.y = defaultMode.height;
	return resolution;
}

void GlobalVariables::switchFPS()
{
	m_fps = !m_fps;
}

bool GlobalVariables::isEnabledFPS() const
{
	return m_fps;
}

std::string GlobalVariables::saveGameSettings() const
{
	std::stringstream stream;
	stream << Engine::Instance().settingsManager()->getFullscreen() << std::endl
		   << Engine::Instance().settingsManager()->getResolution().x << std::endl
		   << Engine::Instance().settingsManager()->getResolution().y << std::endl
		   << Engine::Instance().settingsManager()->getSoundLevel() << std::endl
		   << Engine::Instance().settingsManager()->getMusicLevel() << std::endl
		   << Engine::Instance().soundManager()->quiet() << std::endl
		   << Engine::Instance().translationsManager()->currentLanguage() << std::endl
		   << Engine::Instance().settingsManager()->getShaders() << std::endl
		 ;
	return std::string(stream.str());
}

void GlobalVariables::loadGameSettings(const std::string& data)
{
	bool fullscreen = true;
	int width = Engine::Instance().settingsManager()->getResolution().x;
	int height = Engine::Instance().settingsManager()->getResolution().y;
	int soundLevel = 100;
	int musicLevel = 50;
	bool quite = false;
	std::string languageName;
	bool shaders = true;

	std::stringstream stream(data);
	stream >> fullscreen
			>> width
			>> height
			>> soundLevel
			>> musicLevel
			>> quite
			>> languageName
			>> shaders
			;

	if (width < Engine::Instance().settingsManager()->getResolution().x ||
			height < Engine::Instance().settingsManager()->getResolution().y)
		Engine::Instance().settingsManager()->setResolution(sf::Vector2i(width, height));
	Engine::Instance().settingsManager()->setFullscreen(fullscreen);
	Engine::Instance().settingsManager()->setSoundLevel(soundLevel);
	Engine::Instance().settingsManager()->setMusicLevel(musicLevel);
	Engine::Instance().soundManager()->setQuiet(quite);
	Engine::Instance().translationsManager()->setCurrentLanguage(languageName);
	Engine::Instance().settingsManager()->setShaders(shaders);
}

std::string GlobalVariables::encode(const std::string &str)
{
	std::string result;
	for (unsigned int i = 0; i < str.size(); ++i)
	{
		const char ch = str.at(i);
		result.push_back(ch + EngineDefs::CRYPTO_VALUE);
	}
	return str;
}

std::string GlobalVariables::decode(const std::string &str)
{
	std::string result;
	for (unsigned int i = 0; i < str.size(); ++i)
	{
		const char ch = str.at(i);
		result.push_back(ch - EngineDefs::CRYPTO_VALUE);
	}
	return str;
}

std::string GlobalVariables::appVersion() const
{
	return m_appVersion;
}

void GlobalVariables::setAppVersion(const std::string &appVersion)
{
	m_appVersion = appVersion;
}

std::string GlobalVariables::appName() const
{
	return m_appName;
}

void GlobalVariables::setAppName(const std::string &appName)
{
	m_appName = appName;
}

MainWindow *Options::mainWindow()
{
	return mw;
}

void Options::setMainWindow(MainWindow *window)
{
	mw = window;
}

void Options::updateWindow()
{
	mw->create(
#ifdef OS_ANDROID
				sf::VideoMode::getDesktopMode()
#else
				sf::VideoMode(Engine::Instance().Instance().settingsManager()->getResolution().x,
							 Engine::Instance().Instance().settingsManager()->getResolution().y)
#endif
			   ,sf::String(Engine::Instance().globalVariables()->appName()),
			   Engine::Instance().Instance().settingsManager()->getFullscreen()?sf::Style::Fullscreen:sf::Style::Default
#ifndef OS_ANDROID
			   ,sf::ContextSettings(0, 0, 8)
#endif
			   );
	mw->setVerticalSyncEnabled(true);
	Engine::Instance().reset();
//	Engine::Instance().window()->updateView();
}

void Options::globalCallbacks()
{

}

FilesManager::OtherFile FilesManager::getData(FileType type)
{
	return m_data.at(type);
}

void FilesManager::addFile(const FileType type, const std::string &path)
{

}

void FilesManager::addFile(const FileType type, const char *data, const size_t size)
{
	void *file = malloc(size);
	std::memcpy(file, (void*)data, size);
	OtherFile oFile;
	oFile.data = file;
	oFile.size = size;
	m_data.insert(std::pair<FileType, OtherFile>(type, oFile));
}

std::string ShadersManager::getData(ShaderType type)
{
	return m_shaders.at(type);
}

void ShadersManager::addShader(const ShaderType type, const std::string &data)
{
    m_shaders.insert(std::pair<ShaderType, std::string>(type, data));
}

std::string GlobalVariables::to_string_with_precision(const float a_value, const int n)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return std::string(out.str());
}
