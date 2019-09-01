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
    {"1440x900", Vector2i(1440, 900)},
	{"1280x1024", Vector2i(1280, 1024)},
	{"1024x768", Vector2i(1024, 768)},
	{"1280x720", Vector2i(1280, 720)},
	{"1600x900", Vector2i(1600, 900)}
};

SettingsWindow::SettingsWindow()
	: StateWindow()
{
	setBackground(RESOURCES::SETTINGS_BACKGROUND);

    const int startPosX = 50;
    int posX = startPosX;
    int posY = 100;

	lbl_sound = new Text();
	lbl_sound->setFont(GlobalVariables::Instance().font());
	lbl_sound->setCharacterSize(24);
	lbl_sound->setString("Sound");
    lbl_sound->setPosition(posX, posY);

	posX += 100;

	soundScale = new ValueScale();
	soundScale->setSize(Vector2f(100, -20));
	soundScale->setPos(posX, posY);
	soundScale->setValue(34);

    posY += 100;
	posX = startPosX;

	lbl_music = new Text();
	lbl_music->setFont(GlobalVariables::Instance().font());
	lbl_music->setCharacterSize(24);
	lbl_music->setString("Music");
    lbl_music->setPosition(posX, posY);

	posX += 100;

	musicScale = new ValueScale();
	musicScale->setSize(Vector2f(100, -20));
	musicScale->setPos(posX, posY);
	musicScale->setValue(34);

    posX = startPosX;
    posY += 100;

	lbl_fullscreen = new Text();
	lbl_fullscreen->setFont(GlobalVariables::Instance().font());
	lbl_fullscreen->setCharacterSize(24);
	lbl_fullscreen->setString("Fullscreen");
	lbl_fullscreen->setPosition(posX, posY);

	posX += 100;

	cbx_fullscreen = new CheckBox();
	cbx_fullscreen->setSize(Vector2f(32, 32));
	cbx_fullscreen->setPos(posX, posY);

    posX = startPosX;
    posY += 100;

	lbl_resolution = new Text();
	lbl_resolution->setFont(GlobalVariables::Instance().font());
	lbl_resolution->setCharacterSize(24);
	lbl_resolution->setString("Resolution");
	lbl_resolution->setPosition(posX, posY);

	posX += 100;

	resolutions = new ChooseList();
	resolutions->setPos(posX, posY);
	resolutions->setSize(96, 32);

	String currentResolution;
	for (auto it = resolutionsMap.begin(); it != resolutionsMap.end(); ++it)
	{
		if (it->second == Settings::Instance().getResolution())
			currentResolution = it->first;
		resolutions->addItem(it->first);
	}
	resolutions->update();

    posX = startPosX;
    posY += 100;

	lbl_language = new Text();
	lbl_language->setFont(GlobalVariables::Instance().font());
	lbl_language->setCharacterSize(24);
	lbl_language->setString("Language");
	lbl_language->setPosition(posX, posY);

	posX += 200;

	languages = new ChooseList();
	languages->setPos(posX, posY);
	languages->setSize(96, 32);

	const vector<wstring> languanges = Language::Instance().getAvaliableLanguageNames();
	for(const wstring& langName : languanges)
		languages->addItem(langName);

	languages->update();

	posX = startPosX;
	posY += 100;

	button_accept = new Button();
	button_accept->setSize(Vector2f(64, 32));
	button_accept->setPos(posX, posY);
	button_accept->setCallback(bind(&SettingsWindow::accept, this));
	button_accept->setText("Accept");

	cbx_fullscreen->setChecked(Settings::Instance().getFullscreen());
	soundScale->setValue(Settings::Instance().getSoundLevel());
	musicScale->setValue(Settings::Instance().getMusicLevel());
	resolutions->setCurrent(currentResolution);
	languages->setCurrent(Language::Instance().getCurrentLanguageName());
}

SettingsWindow::~SettingsWindow()
{
	delete lbl_sound;
	delete soundScale;
	delete lbl_music;
	delete musicScale;
	delete lbl_fullscreen;
	delete cbx_fullscreen;
	delete lbl_resolution;
	delete resolutions;
	delete lbl_language;
	delete languages;
	delete button_accept;
}

void SettingsWindow::paint(RenderWindow *window)
{
	drawBackground(window);

	window->draw(*lbl_sound);
	soundScale->draw(window);

	window->draw(*lbl_music);
	musicScale->draw(window);

	window->draw(*lbl_fullscreen);
	cbx_fullscreen->draw(window);

	window->draw(*lbl_resolution);
	resolutions->draw(window);

	window->draw(*lbl_language);
	languages->draw(window);

	button_accept->draw(window);
}

void SettingsWindow::eventFilter(Event *event)
{
	soundScale->event(event);
	musicScale->event(event);
	cbx_fullscreen->event(event);
	resolutions->event(event);
	languages->event(event);
	button_accept->event(event);
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
