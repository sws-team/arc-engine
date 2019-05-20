#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "statewindow.h"

class SettingsWindow : public StateWindow
{
public:
    SettingsWindow();

//	void init() override;
	void paint(RenderWindow *window) override;
	void eventFilter(Event* event) override;
//	void update() override;
	void back() override;

    Vector2i getResolution() const;
    int getSoundLevel() const;
    int getMusicLevel() const;
    bool getFullscreen() const;
private:
    shared_ptr<tgui::Label> lbl_sound;
    shared_ptr<tgui::Slider> slider_sound;

    shared_ptr<tgui::Label> lbl_music;
    shared_ptr<tgui::Slider> slider_music;

    shared_ptr<tgui::Label> lbl_resolution;
    shared_ptr<tgui::ComboBox> cmb_resolution;
    shared_ptr<tgui::CheckBox> cbx_fullscreen;


	shared_ptr<tgui::ComboBox> cmb_language;
	shared_ptr<tgui::Button> button_ok;

    static const map<String, Vector2i> resolutions;

	tgui::Gui *gui;

	void accept();
};

#endif // SETTINGSWINDOW_H
