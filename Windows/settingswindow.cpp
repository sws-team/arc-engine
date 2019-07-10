#include "settingswindow.h"
#include "settings.h"
#include "Game/Audio/soundcontroller.h"
#include "Translations/language.h"
#include "Engine/engine.h"

#include <TGUI/TGUI.hpp>

const map<String, Vector2i> SettingsWindow::resolutions = {
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

	gui = new tgui::Gui();

    const int startPosX = 50;
    int posX = startPosX;
    int posY = 100;

    lbl_sound = tgui::Label::create("Sound");
    lbl_sound->setPosition(posX, posY);
    gui->add(lbl_sound);
    posX += 200;
    slider_sound = tgui::Slider::create(0, 100);
    slider_sound->setPosition(posX, posY);
    gui->add(slider_sound);

    posY += 100;

    posX = startPosX;
    lbl_music = tgui::Label::create("Music");
    lbl_music->setPosition(posX, posY);
    gui->add(lbl_music);
    posX += 200;
    slider_music = tgui::Slider::create(0, 100);
    slider_music->setPosition(posX, posY);
    gui->add(slider_music);

    posX = startPosX;
    posY += 100;

    cbx_fullscreen = tgui::CheckBox::create("FullScreen");
    cbx_fullscreen->setPosition(posX, posY);
    gui->add(cbx_fullscreen);

    posX = startPosX;
    posY += 100;

    lbl_resolution = tgui::Label::create("Resolution");
    lbl_resolution->setPosition(posX, posY);
    gui->add(lbl_resolution);
    posX += 200;
    cmb_resolution = tgui::ComboBox::create();
    cmb_resolution->setPosition(posX, posY);
    String currentResolution;
    for (auto it = resolutions.begin(); it != resolutions.end(); ++it)
    {
        if (it->second == Settings::Instance().getResolution())
            currentResolution = it->first;
        cmb_resolution->addItem(it->first);
    }
    cmb_resolution->setSelectedItem(currentResolution);
    gui->add(cmb_resolution);

    posX = startPosX;
    posY += 100;

	cmb_language = tgui::ComboBox::create();
	cmb_language->setPosition(posX, posY);

	const vector<pair<wstring, string> > languanges = Language::Instance().getAvaliableLanguages();
	for(const pair<wstring, string>& lang : languanges)
		cmb_language->addItem(lang.first, lang.second);
	cmb_language->setSelectedItem(Language::Instance().getCurrentLanguageName());
	gui->add(cmb_language);

	posX = startPosX;
	posY += 100;

	button_ok = tgui::Button::create();
	button_ok->setPosition(posX, posY);
	button_ok->setText("accept");
	button_ok->connect("pressed", [&]()
	{
		this->accept();
	});
	gui->add(button_ok);
    //=========================SET UP========================

    if(Settings::Instance().getFullscreen())
        cbx_fullscreen->check();
    slider_sound->setValue(Settings::Instance().getSoundLevel());
	slider_music->setValue(Settings::Instance().getMusicLevel());
}

void SettingsWindow::paint(RenderWindow *window)
{
	gui->setWindow(*window);
	drawBackground(window);
	gui->draw();
}

void SettingsWindow::eventFilter(Event *event)
{
	gui->handleEvent(*event);
	StateWindow::eventFilter(event);
}

void SettingsWindow::back()
{
	Engine::Instance().setState(Engine::MAIN_MENU);
}

Vector2i SettingsWindow::getResolution() const
{
    return resolutions.at(cmb_resolution->getSelectedItem());
}

int SettingsWindow::getSoundLevel() const
{
    return slider_sound->getValue();
}

int SettingsWindow::getMusicLevel() const
{
    return slider_music->getValue();
}

bool SettingsWindow::getFullscreen() const
{
	return cbx_fullscreen->isChecked();
}

void SettingsWindow::accept()
{
	if (Settings::Instance().getFullscreen() != this->getFullscreen() ||
		   Settings::Instance().getResolution() != this->getResolution() )
	{
		Settings::Instance().setResolution(this->getResolution());
		Settings::Instance().setFullscreen(this->getFullscreen());
	}
	Settings::Instance().setSoundLevel(this->getSoundLevel());
	Settings::Instance().setMusicLevel(this->getMusicLevel());
	SoundController::Instance().updateVolume();

	Language::Instance().setCurrentLanguage(cmb_language->getSelectedItemId());
	Engine::Instance().setState(Engine::MAIN_MENU);

	Settings::Instance().updateWindow();
}
