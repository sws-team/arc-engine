#include "settingswindow.h"
#include "settings.h"
#include "Game/Audio/soundcontroller.h"
#include "Translations/language.h"
#include "Engine/engine.h"
#include "Widgets/valuescale.h"
#include "Widgets/checkbox.h"
#include "Widgets/button.h"
#include "Widgets/chooselist.h"
#include "globalvariables.h"

const map<String, Vector2i> SettingsWindow::resolutionsMap = {
    {"1920x1080", Vector2i(1920, 1080)},
	{"1360x768", Vector2i(1360, 768)},
	{"1280x1024", Vector2i(1280, 1024)},
	{"1024x768", Vector2i(1024, 768)},
	{"1280x720", Vector2i(1280, 720)},
	{"1600x900", Vector2i(1600, 900)}
};

SettingsWindow::SettingsWindow()
	: StateWindow()
{
	setBackground(RESOURCES::SETTINGS_BACKGROUND);

	const Vector2f settingsSize = Vector2f(Settings::Instance().getResolution().x * 0.7f,
										   Settings::Instance().getResolution().y * 0.7f);
	Vector2f pos = Vector2f((Settings::Instance().getResolution().x - settingsSize.x)/2,
				   (Settings::Instance().getResolution().y - settingsSize.y)/2);

	const int blocksCount = 3;
	const float settingsModifierY = 0.25f;
	const Vector2f blockSize = Vector2f(settingsSize.x * 0.9f, settingsSize.y * settingsModifierY);
	const unsigned int characterSize = 30;
	const float border = 6 * Settings::Instance().getScaleFactor().x;

	const float borderXOffset = settingsSize.x * 0.1f/2;
	const float borderYOffset = settingsSize.y * (1 - settingsModifierY*blocksCount)/blocksCount;

	const Vector2f blockOffset = Vector2f(blockSize.x/10, blockSize.y/5);
	const int secondOffset = 4;

	const float SCALE_WIDTH = 200 * Settings::Instance().getScaleFactor().x;
	const float SCALE_HEIGHT = 40 * Settings::Instance().getScaleFactor().y;
	const int defaultAudioLevel = 30;
	const Vector2f chooseListButtonSize = Vector2f(96 * Settings::Instance().getScaleFactor().x, 32 * Settings::Instance().getScaleFactor().y);
	const Vector2f checkBoxSize = Vector2f(32 * Settings::Instance().getScaleFactor().x, 32 * Settings::Instance().getScaleFactor().y);
	const unsigned int chooseListCharacterSize = 24;

	lbl_settings.setFont(GlobalVariables::Instance().font());
	lbl_settings.setCharacterSize(characterSize * 4);
	lbl_settings.setStyle(Text::Bold);
	lbl_settings.setFillColor(Color::Red);
	lbl_settings.setString(Language::Instance().translate(Language::SETTINGS));
	lbl_settings.setScale(Settings::Instance().getScaleFactor());
	lbl_settings.setPosition(pos - Vector2f(0, lbl_settings.getGlobalBounds().height * 2 + border));

	settingsRect.setSize(settingsSize);
	settingsRect.setFillColor(Color::Transparent);
	settingsRect.setOutlineThickness(border);
	settingsRect.setOutlineColor(Color::Red);
	settingsRect.setPosition(pos);


	//audio
	pos += Vector2f(borderXOffset, borderYOffset);

	audioRect.setSize(blockSize);
	audioRect.setFillColor(Color::Transparent);
	audioRect.setOutlineThickness(border);
	audioRect.setOutlineColor(Color::Blue);
	audioRect.setPosition(pos);

	lbl_audio.setFont(GlobalVariables::Instance().font());
	lbl_audio.setCharacterSize(characterSize);
	lbl_audio.setStyle(Text::Bold);
	lbl_audio.setString(Language::Instance().translate(Language::AUDIO));
	lbl_audio.setScale(Settings::Instance().getScaleFactor());
	lbl_audio.setPosition(pos + Vector2f(blockOffset.x/2, 0));

	//sound
	lbl_sound.setFont(GlobalVariables::Instance().font());
	lbl_sound.setCharacterSize(characterSize);
	lbl_sound.setScale(Settings::Instance().getScaleFactor());
	lbl_sound.setString(Language::Instance().translate(Language::SOUND));
	lbl_sound.setPosition(pos + blockOffset + Vector2f(0, lbl_audio.getGlobalBounds().height));

	soundScale = new ValueScale();
	soundScale->setSize(Vector2f(SCALE_WIDTH, -SCALE_HEIGHT));
	soundScale->setValue(defaultAudioLevel);
	soundScale->setPos(pos + Vector2f(blockOffset.x * secondOffset, blockOffset.y + SCALE_HEIGHT/2 +
									  lbl_sound.getGlobalBounds().height + lbl_audio.getGlobalBounds().height));

	//music
	lbl_music.setFont(GlobalVariables::Instance().font());
	lbl_music.setCharacterSize(characterSize);
	lbl_music.setScale(Settings::Instance().getScaleFactor());
	lbl_music.setString(Language::Instance().translate(Language::MUSIC));
	lbl_music.setPosition(pos + Vector2f(blockOffset.x, blockOffset.y * 2 + lbl_sound.getGlobalBounds().height + lbl_audio.getGlobalBounds().height));

	musicScale = new ValueScale();
	musicScale->setSize(Vector2f(SCALE_WIDTH, -SCALE_HEIGHT));
	musicScale->setValue(defaultAudioLevel);
	musicScale->setPos(pos + Vector2f(blockOffset.x * secondOffset, blockOffset.y * 2 + SCALE_HEIGHT/2 +
									  lbl_sound.getGlobalBounds().height + lbl_audio.getGlobalBounds().height + lbl_music.getGlobalBounds().height));

	//video
	pos += Vector2f(0, blockSize.y + borderYOffset/2);

	videoRect.setSize(blockSize);
	videoRect.setFillColor(Color::Transparent);
	videoRect.setOutlineThickness(border);
	videoRect.setOutlineColor(Color::Green);
	videoRect.setPosition(pos);

	lbl_video.setFont(GlobalVariables::Instance().font());
	lbl_video.setCharacterSize(characterSize);
	lbl_video.setScale(Settings::Instance().getScaleFactor());
	lbl_video.setString(Language::Instance().translate(Language::VIDEO));
	lbl_video.setStyle(Text::Bold);
	lbl_video.setPosition(pos + Vector2f(blockOffset.x/2, 0));

	//fullscreen
	lbl_fullscreen.setFont(GlobalVariables::Instance().font());
	lbl_fullscreen.setCharacterSize(characterSize);
	lbl_fullscreen.setScale(Settings::Instance().getScaleFactor());
	lbl_fullscreen.setString(Language::Instance().translate(Language::FULLSCREEN));
	lbl_fullscreen.setPosition(pos + blockOffset + Vector2f(0, lbl_video.getGlobalBounds().height));

	cbx_fullscreen = new CheckBox();
	cbx_fullscreen->setSize(checkBoxSize);
	cbx_fullscreen->setPos(pos + Vector2f(blockOffset.x * secondOffset, blockOffset.y +
										  lbl_fullscreen.getGlobalBounds().height + lbl_video.getGlobalBounds().height));

	//resolutions
	lbl_resolution.setFont(GlobalVariables::Instance().font());
	lbl_resolution.setCharacterSize(characterSize);
	lbl_resolution.setScale(Settings::Instance().getScaleFactor());
	lbl_resolution.setString(Language::Instance().translate(Language::RESOLUTION));
	lbl_resolution.setPosition(pos + Vector2f(blockOffset.x, blockOffset.y * 2 + lbl_fullscreen.getGlobalBounds().height + lbl_video.getGlobalBounds().height));

	resolutions = new ChooseList();
	resolutions->setCharacterSize(chooseListCharacterSize);
	String currentResolution;
	for (auto it = resolutionsMap.begin(); it != resolutionsMap.end(); ++it)
	{
		if (it->second == Settings::Instance().getResolution())
			currentResolution = it->first;
		resolutions->addItem(it->first);
	}
	resolutions->setScale(Settings::Instance().getScaleFactor());
	resolutions->setPos(pos + Vector2f(blockOffset.x * secondOffset, blockOffset.y * 2 +
									   lbl_fullscreen.getGlobalBounds().height + lbl_video.getGlobalBounds().height + lbl_resolution.getGlobalBounds().height));
	resolutions->setSize(chooseListButtonSize);
	resolutions->update();

	//language
	pos += Vector2f(0, blockSize.y + borderYOffset/2);
	miscRect.setSize(blockSize);
	miscRect.setFillColor(Color::Transparent);
	miscRect.setOutlineThickness(border);
	miscRect.setOutlineColor(Color::Magenta);
	miscRect.setPosition(pos);

	lbl_misc.setFont(GlobalVariables::Instance().font());
	lbl_misc.setCharacterSize(characterSize);
	lbl_misc.setScale(Settings::Instance().getScaleFactor());
	lbl_misc.setString(Language::Instance().translate(Language::MISC));
	lbl_misc.setStyle(Text::Bold);
	lbl_misc.setPosition(pos + Vector2f(blockOffset.x/2, 0));

	lbl_language.setFont(GlobalVariables::Instance().font());
	lbl_language.setCharacterSize(characterSize);
	lbl_language.setScale(Settings::Instance().getScaleFactor());
	lbl_language.setString(Language::Instance().translate(Language::LANGUAGE));
	lbl_language.setPosition(pos + blockOffset + Vector2f(0, lbl_audio.getGlobalBounds().height));

	languages = new ChooseList();
	languages->setCharacterSize(chooseListCharacterSize);
	const vector<wstring> languanges = Language::Instance().getAvaliableLanguageNames();
	for(const wstring& langName : languanges)
		languages->addItem(langName);
	languages->setScale(Settings::Instance().getScaleFactor());
	languages->setPos(pos + Vector2f(blockOffset.x * secondOffset, blockOffset.y +
									 lbl_language.getGlobalBounds().height + lbl_audio.getGlobalBounds().height));
	languages->setSize(chooseListButtonSize);
	languages->update();

	//buttons
	const float buttonOffset = border + 50 * Settings::Instance().getScaleFactor().y;
	const Vector2f buttonSize = Vector2f(128 * Settings::Instance().getScaleFactor().x, 32 * Settings::Instance().getScaleFactor().y);
	const Vector2f bottomRight = settingsRect.getPosition() + settingsSize;
	button_accept = new Button();
	button_accept->setSize(buttonSize);
	button_accept->setCallback(bind(&SettingsWindow::accept, this));
	button_accept->setText(Language::Instance().translate(Language::ACCEPT));
	button_accept->setTextCharacterSize(characterSize);
	button_accept->setScale(Settings::Instance().getScaleFactor());
	button_accept->setPos(bottomRight - Vector2f(buttonSize.x, -buttonOffset));

	const Vector2f bottomLeft = settingsRect.getPosition() + Vector2f(0, settingsSize.y);
	button_cancel = new Button();
	button_cancel->setSize(buttonSize);
	button_cancel->setCallback(bind(&SettingsWindow::back, this));
	button_cancel->setText(Language::Instance().translate(Language::CANCEL));
	button_cancel->setScale(Settings::Instance().getScaleFactor());
	button_cancel->setTextCharacterSize(characterSize);
	button_cancel->setPos(bottomLeft + Vector2f(0, buttonOffset));

	cbx_fullscreen->setChecked(Settings::Instance().getFullscreen());
	soundScale->setValue(Settings::Instance().getSoundLevel());
	musicScale->setValue(Settings::Instance().getMusicLevel());
	resolutions->setCurrent(currentResolution);
	languages->setCurrent(Language::Instance().getCurrentLanguageName());
}

SettingsWindow::~SettingsWindow()
{
	delete soundScale;
	delete musicScale;
	delete cbx_fullscreen;
	delete resolutions;
	delete languages;
	delete button_accept;
	delete button_cancel;
}

void SettingsWindow::paint(RenderWindow *window)
{
	drawBackground(window);

	window->draw(settingsRect);
	window->draw(lbl_settings);

	window->draw(audioRect);
	window->draw(lbl_audio);

	window->draw(lbl_sound);
	soundScale->draw(window);
	window->draw(lbl_music);
	musicScale->draw(window);

	window->draw(videoRect);
	window->draw(lbl_video);

	window->draw(lbl_fullscreen);
	cbx_fullscreen->draw(window);
	window->draw(lbl_resolution);
	resolutions->draw(window);

	window->draw(miscRect);
	window->draw(lbl_misc);
	window->draw(lbl_language);
	languages->draw(window);

	button_accept->draw(window);
	button_cancel->draw(window);
}

void SettingsWindow::eventFilter(Event *event)
{
	soundScale->event(event);
	musicScale->event(event);
	cbx_fullscreen->event(event);
	resolutions->event(event);
	languages->event(event);
	button_accept->event(event);
	button_cancel->event(event);

	StateWindow::eventFilter(event);
}

void SettingsWindow::back()
{
	Engine::Instance().setState(Engine::MAIN_MENU);
}

void SettingsWindow::accept()
{
	const bool fullscreen = cbx_fullscreen->isChecked();
	const Vector2i resolution = resolutionsMap.at(resolutions->currentText());
	if (Settings::Instance().getFullscreen() != fullscreen ||
		   Settings::Instance().getResolution() != resolution )
	{
		Settings::Instance().setResolution(resolution);
		Settings::Instance().setFullscreen(fullscreen);
	}
	Settings::Instance().setSoundLevel(soundScale->value());
	Settings::Instance().setMusicLevel(musicScale->value());
	SoundController::Instance().updateVolume();

	Language::Instance().setCurrentLanguageByName(languages->currentText());
	Engine::Instance().setState(Engine::MAIN_MENU);

	Settings::Instance().updateWindow();
}
