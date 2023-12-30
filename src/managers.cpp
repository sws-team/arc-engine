#include "managers.h"
#include "enginedef.h"
#include <ArcEngine>
#include <ArcDebug>
#include <mainwindow.h>
#include "../src/Scenes/introscene.h"
#include "../src/Scenes/aboutscene.h"
#include "../src/Scenes/closescene.h"
#include <gameplatform.h>
#include <ArcAction>
#include <arcvariant.h>

#ifdef SFML_SYSTEM_WINDOWS
#include <Windows.h>
#include <shlobj.h>
#endif
#ifdef SFML_SYSTEM_LINUX
#include <X11/Xlib.h>
#endif

#include <cstring>
#include "SFML/SFMLFactory.h"

namespace ArcEngine {
	ArcObject *findChild(const std::string &name) {
		ArcScene *scene = Engine::Instance().sceneManager()->currentScene();
		return scene->findChild(name, true);
	}

	ArcObject *findChildPath(const std::string &path) {
		ArcScene *scene = Engine::Instance().sceneManager()->currentScene();
		return scene->findChildPath(path);
	}
}

void Manager::reset()
{

}

//==================SETTINS MANAGER===================

const sf::Vector2f SettingsManager::defaultResolution = sf::Vector2f(1920.f, 1080.f);
const std::string SettingsManager::settingsFilePath = std::string("settings.ini");

SettingsManager::SettingsManager()
	: Manager()
	,soundLevel(100)
	,musicLevel(100)
	,fullscreen(
#ifdef ARC_DEBUG
	false
#else
	true
#endif
	)
	,shaders(true)
{
	saveDataFileName = settingsFilePath;
	SettingsManager::reset();
}

void SettingsManager::reset()
{
#ifdef SFML_SYSTEM_ANDROID
	const sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
	resolution.x = videoMode.width;
	resolution.y = videoMode.height;
#else
#ifdef ARC_DEBUG
	resolution = sf::Vector2i(1366, 768);
#else
	const sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
	resolution = sf::Vector2i(videoMode.width, videoMode.height);
#endif
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

std::string SettingsManager::saveGameSettings() const
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

void SettingsManager::loadGameSettings(const std::string& data)
{
	bool fullscreen = true;
	int width = Engine::Instance().settingsManager()->getResolution().x;
	int height = Engine::Instance().settingsManager()->getResolution().y;
	float soundLevel = 100.f;
	float musicLevel = 50.f;
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

void SettingsManager::save()
{
	const std::string data = this->saveGameSettings();
	GamePlatform::Instance().saveFile(saveDataFileName, data);
}

void SettingsManager::load()
{
	const std::string data = GamePlatform::Instance().readFile(saveDataFileName);
	if (data.empty())
		return;
	this->loadGameSettings(data);
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
	return translate(id, currentLangName);
}

sf::String TranslationsManager::translate(TranslationType id, const std::string &language) const
{
	auto &translations = langs.at(language);
	if (auto it = translations.find(id); it == translations.end()) {
		assert("Missing text!");
		return sf::String();
	}
	return translations.at(id);
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

void TexturesManager::addTexture(const TextureType type, const sf::Texture &texture)
{
	m_textures.insert(std::pair<int, sf::Texture>(type, texture));
}

void TexturesManager::addTexture(const TextureType type, const std::string &path)
{
	sf::Texture texture;
	const std::string fullPath = Engine::assetsPath() + path;
	if (!texture.loadFromFile(fullPath))
	{
		std::cout << "Error to load texture: " << fullPath << std::endl;
		return;
	}
	addTexture(type, texture);
}

void TexturesManager::addTexture(const TextureType type, const char *data, const size_t size)
{
	sf::Texture texture;
	void *textureData = malloc(size);
	std::memcpy(textureData,(void*)data, size);
	if (!texture.loadFromMemory(textureData, size))
		return;
	addTexture(type, texture);
}

//==================STATE MANAGER===================
SceneManager::SceneManager()
{
	m_type = INTRO;

	addScene<IntroScene>(INTRO);
	addScene<AboutScene>(ABOUT);
	addScene<class CloseScene>(CLOSING);
}

ArcScene *SceneManager::currentScene() const
{
	return m_scene;
}

void SceneManager::setCurrentScene(ArcScene *scene)
{
	m_scene = scene;
}

SceneType SceneManager::currentSceneType() const
{
	return m_type;
}

void SceneManager::setSceneType(const SceneType type)
{
	m_type = type;
}

void SceneManager::addScene(SceneType type, const SceneManager::Creator &creator)
{
	scenes.insert(std::make_pair(type, creator));
}

ArcScene *SceneManager::createScene(SceneType type) const
{
	if (auto it = scenes.find(type); it != scenes.end()) {
		return it->second();
	}
	return nullptr;
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
	const std::string fullPath = Engine::assetsPath() + path;
	if(!sfx.buffer->loadFromFile(fullPath))
	{
		std::cout << "Error to load sound: " << fullPath << std::endl;
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
	const std::string fullPath = Engine::assetsPath() + path;
	if (!music->openFromFile(fullPath))
	{
		std::cout << "Error to load music: " << fullPath << std::endl;
		delete music;
		return;
	}

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
	for(const std::pair<SoundType, SFX>& sfx : sounds) {
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
	for(const std::pair<SoundType, SFX>& sfx : sounds) {
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
	const std::string fullPath = Engine::assetsPath() + path;
	if (!font->loadFromFile(fullPath))
	{
		std::cout << "Error to load font: " << fullPath << std::endl;
		delete font;
		return;
	}
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

Options::Options()
	: mw(nullptr)
	,m_resourcesLoaded(false)
{
	debug = new ArcDebug();
}

Options::~Options()
{
	delete debug;
}

MainWindow *Options::mainWindow()
{
	return mw;
}

void Options::setMainWindow(MainWindow *window)
{
	mw = window;
	debug->init();
}

void Options::updateWindow()
{
	const int w = Engine::Instance().Instance().settingsManager()->getResolution().x;
	const int h = Engine::Instance().Instance().settingsManager()->getResolution().y;
	const int style = Engine::Instance().Instance().settingsManager()->getFullscreen()?sf::Style::Fullscreen:sf::Style::Default;

	mw->create(
#ifdef SFML_SYSTEM_ANDROID
				sf::VideoMode::getDesktopMode()
#else
				sf::VideoMode(w, h)
#endif
			   , ArcEngine::GAME_NAME, style
#ifndef SFML_SYSTEM_ANDROID
			   ,sf::ContextSettings(0, 0, 8)
#endif
			   );
	mw->setVerticalSyncEnabled(true);
	Engine::Instance().reset();
	Engine::Instance().window()->updateView();
}

void Options::debugObject(ArcObject *object)
{
	debug->setObject(object);
}

void Options::addDebugSection(DebugSection *section)
{
	debug->addSection(section);
}

void Options::removeDebugSection(DebugSection *section)
{
	debug->removeSection(section);
}

void Options::addNotificationCallback(const std::function<void (const std::string &, const ArcVariant &)> &callback)
{
	notificationCallbacks.push_back(callback);
}

void Options::globalCallbacks()
{
	debug->update();
}

void Options::clear()
{
	debug->clear();
}

bool Options::globalEventFilter(sf::Event *event)
{
	return debug->eventFilter(event);
}

void Options::globalDraw(sf::RenderTarget *target)
{
	debug->draw(target);
}

void Options::globalNotifications(const std::string &name, const ArcVariant &value)
{
	for(const auto &notificationCallback : notificationCallbacks) {
		if (notificationCallback != nullptr)
			notificationCallback(name, value);
	}
}

bool Options::isResourcesLoaded() const
{
	return m_resourcesLoaded;
}

void Options::setResourcesLoaded(bool loaded)
{
	m_resourcesLoaded = loaded;
}

FilesManager::GameFile FilesManager::getData(FileType type) const
{
	return m_data.at(type);
}

void FilesManager::addFile(const FileType type, const std::string &path)
{
	const std::string fullPath = Engine::assetsPath() + path;
	sf::FileInputStream stream;
	if (!stream.open(fullPath))
	{
		std::cout << "Open file failed. - " << fullPath << std::endl;
		return;
	}
	const sf::Int64 size = stream.getSize();
	char *chars = new char[size];
	const sf::Int64 readed = stream.read(chars, size);
	const std::string data = std::string(chars).substr(0, readed);
	delete[] chars;
	if (data.empty())
	{
		std::cout << "Loaded empty file." << std::endl;
		return;
	}
	addFile(type, data.c_str(), data.size());
}

void FilesManager::addFile(const FileType type, const char *data, const size_t size)
{
	char *file = (char*)malloc(size + 1);
	std::memcpy(file, (void*)data, size + 1);
	GameFile gFile;
	gFile.data = file;
	gFile.size = size;
	m_data.insert(std::pair<FileType, GameFile>(type, gFile));
}

std::string ShadersManager::getData(ShaderType type)
{
	return m_shaders.at(type);
}

void ShadersManager::addShader(const ShaderType type, const std::string &data)
{
	m_shaders.insert(std::pair<ShaderType, std::string>(type, data));
}

const std::string ResourcesManager::texturesFileName = "textures.dat";
const std::string ResourcesManager::soundsFileName = "sounds.dat";
const std::string ResourcesManager::preloadFileName = "preload.dat";
const std::string ResourcesManager::resourcesFileName = "resources.dat";

ResourcesManager::ResourcesManager()
{
	skeletonAnimationFactory = new dragonBones::SFMLFactory();
	timer.setInterval(ArcEngine::FRAME_TIME);
	timer.setCallback(std::bind(&dragonBones::SFMLFactory::update,
								skeletonAnimationFactory, ArcEngine::FRAME_TIME));
}

void ResourcesManager::addTexture(TextureType type, const std::string &data)
{
	Engine::Instance().texturesManager()->addTexture(type, data.c_str(), data.size());
}

void ResourcesManager::addSound(SoundType type, const std::string &data)
{
	Engine::Instance().soundManager()->addSound(type, data.c_str(), data.size());
}

void ResourcesManager::addMusic(MusicType type, const std::string &data)
{
	Engine::Instance().soundManager()->addMusic(type, data.c_str(), data.size());
}

void ResourcesManager::addFont(FontType type, const std::string &data)
{
	Engine::Instance().fontManager()->addFont(type, data.c_str(), data.size());
}

void ResourcesManager::addFile(FileType type, const std::string &data)
{
	Engine::Instance().filesManager()->addFile(type, data.c_str(), data.size());
}

void ResourcesManager::addShader(ShaderType type, const std::string &data)
{
	Engine::Instance().shadersManager()->addShader(type, data);
}

void ResourcesManager::loadSkeletonData(FileType atlasType, FileType skeletonType, TextureType textureType)
{
	const float scale = Engine::Instance().settingsManager()->getScaleFactor().x;

	const FilesManager::GameFile atlasFile = Engine::Instance().filesManager()->getData(atlasType);
	const FilesManager::GameFile skeletonFile = Engine::Instance().filesManager()->getData(skeletonType);

	dragonBones::TextureAtlasData *atlas = skeletonAnimationFactory->loadTextureAtlasData(atlasFile.data,
								  &Engine::Instance().texturesManager()->getTexture(textureType));
	if(atlas == nullptr)
		ArcErr() << "Loading atlas failed.";

	dragonBones::DragonBonesData *skeleton = skeletonAnimationFactory->loadDragonBonesData(skeletonFile.data, std::string(), scale);
	if(skeleton == nullptr)
		ArcErr() << "Loading skeleton failed.";
}

std::string ResourcesManager::encode(const std::string &str)
{
	std::string result;
	for (unsigned int i = 0; i < str.size(); ++i)
	{
		const char ch = str.at(i);
		result.push_back(ch + ArcEngine::CRYPTO_VALUE);
	}
	return str;
}

std::string ResourcesManager::decode(const std::string &str)
{
	std::string result;
	for (unsigned int i = 0; i < str.size(); ++i)
	{
		const char ch = str.at(i);
		result.push_back(ch - ArcEngine::CRYPTO_VALUE);
	}
	return str;
}

const std::unordered_map<NotificationManager::NOTIFICATION_TYPE, std::string> NotificationManager::NOTIFICATIONS = {
	{ UNKNOWN, "unknown" },
	{ SCENE_CHANGED, "scene changed" },
	{ WINDOW_OPENING, "window opening" },
	{ WINDOW_OPENED, "window opened" },
	{ WINDOW_CLOSING, "window closing" },
	{ WINDOW_CLOSED, "window closed" },
	{ BUTTON_CLICKED, "button clicked" },
	{ ACTION_STARTED, "action started" },
	{ ACTION_FINISHED, "action finished" },
	{ CHILD_ADDED, "child added" },
	{ DRAG_STARTED, "drag started" },
	{ DRAG_MOVED, "drag moved" },
	{ DRAG_FINISHED, "drag finished" },
};

NotificationManager::NotificationManager()
{

}

void NotificationManager::notify(NotificationType type, const ArcVariant &value)
{
	std::optional<std::string> name = notificationName(type);
	if (name != std::nullopt) {
		notify(name.value(), value);
	}
}

void NotificationManager::notify(const std::string &name, const ArcVariant &value)
{
	if (auto it = callbacks.find(name); it != callbacks.end()) {
		for(const auto& data : (*it).second) {
			data.callback(value);
		}
	}
	Engine::Instance().getOptions()->globalNotifications(name, value);
}

std::optional<std::string> NotificationManager::notificationName(NotificationType type) const
{
	if (auto it = NOTIFICATIONS.find(static_cast<NotificationManager::NOTIFICATION_TYPE>(type)); it != NOTIFICATIONS.end()) {
		return it->second;
	}
	return std::nullopt;
}

std::optional<int> NotificationManager::addCallback(NotificationType type, const CallbackType &callback)
{
	if (auto it = NOTIFICATIONS.find(static_cast<NotificationManager::NOTIFICATION_TYPE>(type)); it != NOTIFICATIONS.end()) {
		return addCallback(it->second, callback);
	}
	return std::nullopt;
}

std::optional<int> NotificationManager::addCallback(const std::string &name, const CallbackType &callback)
{
	const int id = counter++;
	NotificationData data;
	data.id = id;
	data.callback = callback;

	if (auto it = callbacks.find(name); it != callbacks.end())
		(*it).second.emplace_back(data);
	else
		callbacks.insert(std::make_pair(name, std::vector<NotificationData>{ data }));
	return id;
}

void NotificationManager::removeCallback(int id)
{
	for(auto& [key, value] : callbacks) {
		for (auto it = value.begin(); it != value.end();) {
			NotificationData& data = *(it);
			if (data.id == id) {
				it = value.erase(it);
				continue;
			}
			++it;
		}
	}
}

WindowsManager::WindowsManager()
{

}

void WindowsManager::closeWindow(ArcWindow *window)
{
	NOTIFY(NotificationManager::NOTIFICATION_TYPE::WINDOW_CLOSING, window->type());
	closing.push(window);
}

bool WindowsManager::isWindowOpened(WindowType type) const
{
	ArcWindow *window = getWindow(type);
	return window != nullptr;
}

ArcWindow *WindowsManager::getWindow(WindowType type) const
{
	const auto findType = [type](ArcWindow* window) {
		return window->type() == type;
	};
	if (auto it = std::find_if(opened.begin(), opened.end(), findType); it != opened.end())
		return *it;
	return nullptr;
}

void WindowsManager::update()
{
	if (!closing.empty()) {
		ArcWindow *window = closing.front();
		closing.pop();
		GroupAction *action = new GroupAction();
		ArcAction *scaleAction = new ChangeScaleAction(ANIMATION_TIME, window, sf::Vector2f(0.f, 0.f));
		ArcAction *fadeAction = new FadeOutAction(ANIMATION_TIME, window);
		action->addAction(scaleAction);
		action->addAction(fadeAction);
		action->setCompletedFunc(std::bind(&WindowsManager::removeWindow, this, window));
		window->addAction(action);
	} else if(!opening.empty()) {
		const auto wnd = opening.front();
		if(auto it = std::find_if(opened.begin(), opened.end(), [type = wnd.first](ArcWindow* window) {
			return window->type() == type && window->isUnique();
		}); it == opened.end()) {
			const Creator creator = wnd.second;
			ArcWindow *window = creator();
			Engine::Instance().sceneManager()->currentScene()->addChild(window);
			window->setType(wnd.first);
			window->init();
			window->setScale(0, 0);
			GroupAction *action = new GroupAction();
			ArcAction *scaleAction = new ChangeScaleAction(ANIMATION_TIME, window, sf::Vector2f(1.f, 1.f));
			ArcAction *fadeAction = new FadeInAction(ANIMATION_TIME, window);
			action->addAction(scaleAction);
			action->addAction(fadeAction);
			window->addAction(action);
			action->setCompletedFunc(std::bind(static_cast<void(NotificationManager::*)(NotificationType, const ArcVariant&)>(&NotificationManager::notify),
											   Engine::Instance().notificationManager(),
											   NotificationManager::NOTIFICATION_TYPE::WINDOW_OPENED,
											   wnd.first));
			opened.insert(window);
		}
		opening.pop();
	}
}

void WindowsManager::removeWindow(ArcWindow *window)
{
	window->deinit();
	window->destroy();
	opened.erase(window);
	NOTIFY(NotificationManager::NOTIFICATION_TYPE::WINDOW_CLOSED, window->type());
}

