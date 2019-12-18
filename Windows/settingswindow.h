#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "statewindow.h"

class ValueScale;
class CheckBox;
class Button;
class ChooseList;

class SettingsWindow : public StateWindow
{
public:
    SettingsWindow();
	~SettingsWindow() override;

	void paint(RenderWindow *window) override;
	void eventFilter(Event* event) override;
	void back() override;

private:

	static const map<String, Vector2i> resolutionsMap;

	void accept();

	Text lbl_settings;

	Text lbl_sound;
	ValueScale *soundScale;

	Text lbl_music;
	ValueScale *musicScale;

	Text lbl_fullscreen;
	CheckBox *cbx_fullscreen;

	Text lbl_resolution;
	ChooseList *resolutions;

	Text lbl_language;
	ChooseList *languages;

	Button *button_accept;
	Button *button_cancel;

	RectangleShape settingsRect;

	Text lbl_audio;
	RectangleShape audioRect;

	Text lbl_video;
	RectangleShape videoRect;

	Text lbl_misc;
	RectangleShape miscRect;

	enum FOCUSES
	{
		SOUND,
		MUSIC,
		FULLSCREEN,
		RESOLUTION,
		LANGUAGE,
		BUTTON_CANCEL,
		BUTTON_ACCEPT
	};

	FOCUSES currentFocus;
	Sprite focusRect;
	void updateFocus();
};

#endif // SETTINGSWINDOW_H
