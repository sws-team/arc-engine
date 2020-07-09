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

	void paint(sf::RenderWindow *window) override;
	void eventFilter(sf::Event* event) override;
	void back() override;
	void init() override;
	void update() override;

	void setDrawRects(bool drawRects);
	void setShowShaders(bool showShaders);

private:
	void accept();

	sf::Text lbl_settings;

	sf::Text lbl_sound;
	ValueScale *soundScale;

	sf::Text lbl_music;
	ValueScale *musicScale;

	sf::Text lbl_fullscreen;
	CheckBox *cbx_fullscreen;

	sf::Text lbl_resolution;
	ChooseList *resolutions;

	sf::Text lbl_language;
	ChooseList *languages;

	Button *button_accept;
	Button *button_cancel;

	sf::Text lbl_shaders;
	CheckBox *cbx_shaders;

	sf::RectangleShape settingsRect;
	sf::RectangleShape audioRect;
	sf::RectangleShape videoRect;
	sf::RectangleShape miscRect;

	enum FOCUSES
	{
		SOUND,
		MUSIC,
		QUITE,
		FULLSCREEN,
		RESOLUTION,
		LANGUAGE,
		SHADERS,
		BUTTON_CANCEL,
		BUTTON_ACCEPT
	};

	FOCUSES currentFocus;
	sf::Sprite focusSprite;
	sf::Sprite quiteSprite;
	sf::Sprite audioSprite;
	void updateFocus();
	bool m_quite;
	std::map<sf::String, sf::Vector2i> resolutionsMap;
	bool m_drawRects;

	void checkIsChanged();
	bool m_showShaders;
};

#endif // SETTINGSWINDOW_H
