#include "settingswindow.h"
#include "engine.h"
#include "managers.h"
#include "Widgets/valuescale.h"
#include "Widgets/checkbox.h"
#include "Widgets/button.h"
#include "Widgets/chooselist.h"

const sf::Color chooseListFillColor = sf::Color(215,226,237, 200);
SettingsWindow::SettingsWindow()
	: StateWindow()
	,currentFocus(SOUND)
	,m_quite(false)
	,m_drawRects(true)
	,m_showShaders(false)
{
	setBackground(TexturesManager::SETTINGS_BACKGROUND);

	const sf::Vector2f settingsSize = sf::Vector2f(Engine::Instance().settingsManager()->getResolution().x * 0.7f,
												   Engine::Instance().settingsManager()->getResolution().y * 0.7f);
	sf::Vector2f pos = sf::Vector2f((Engine::Instance().settingsManager()->getResolution().x - settingsSize.x)/2,
									(Engine::Instance().settingsManager()->getResolution().y - settingsSize.y)/2);

	const sf::Vector2f scaleFactor = Engine::Instance().settingsManager()->getScaleFactor();
	const int blocksCount = 3;
	const float settingsModifierY = 0.25f;
	const sf::Vector2f blockSize = sf::Vector2f(settingsSize.x * 0.9f, settingsSize.y * settingsModifierY);
	const unsigned int characterSize = 38;
	const float border = 6 * scaleFactor.x;

	const float borderXOffset = settingsSize.x * 0.1f/2;
	const float borderYOffset = settingsSize.y * (1 - settingsModifierY*blocksCount)/blocksCount + scaleFactor.y * 20;

	const sf::Vector2f blockOffset = sf::Vector2f(blockSize.x/10, blockSize.y/5);
	const int secondOffset = 4;

	const float SCALE_WIDTH = 200 * scaleFactor.x;
	const float SCALE_HEIGHT = 40 * scaleFactor.y;
	const int defaultAudioLevel = 30;
	const sf::Vector2f chooseListButtonSize = sf::Vector2f(120 * scaleFactor.x,
														   32 * scaleFactor.y);
	const sf::Vector2f checkBoxSize = sf::Vector2f(32, 32);
	const unsigned int chooseListCharacterSize = 24;

	//colors
	const sf::Color borderColor = sf::Color(31,36,42, 100);
	const sf::Color mainRectColor = sf::Color(46,54,63, 50);
	const sf::Color subRectColor = sf::Color(62,73,84, 75);
	const sf::Color valueScaleColorActive = sf::Color(93,109,127);
	const sf::Color valueScaleColorInactive = sf::Color(185,204,224);
	const sf::Color textColor = sf::Color(235,240,246, 200);
	const sf::Color checkBoxBorderColor = sf::Color(31,36,42, 200);
	const sf::Color checkboxColor = sf::Color(77,91,106, 200);
	const sf::Color chooseListCurrentColor = sf::Color(15,18,21, 150);
	const sf::Color chooseListBorderColor = sf::Color(46,54,63, 100);
	const sf::Color chooseListTextColor = sf::Color(62,73,84, 200);
	const sf::Color buttonTextColor = chooseListTextColor;
	const sf::Color buttonFillColor = chooseListFillColor;
	const sf::Color buttonBorderColor = chooseListBorderColor;


	const float settingsLabelOffset = 32 * scaleFactor.y;
	lbl_settings.setFont(Engine::Instance().fontManager()->font());
	lbl_settings.setCharacterSize(Engine::Instance().fontManager()->getCharSize(characterSize * 3));
	lbl_settings.setStyle(sf::Text::Bold);
	lbl_settings.setFillColor(textColor);
	lbl_settings.setOutlineThickness(1);
	lbl_settings.setOutlineColor(sf::Color::Black);
	lbl_settings.setString(Engine::Instance().translationsManager()->translate(TranslationsManager::SETTINGS));
	lbl_settings.setScale(scaleFactor);
	lbl_settings.setPosition(pos - sf::Vector2f(0, lbl_settings.getGlobalBounds().height + border + settingsLabelOffset));

	settingsRect.setSize(settingsSize);
	settingsRect.setFillColor(mainRectColor);
	settingsRect.setOutlineThickness(border);
	settingsRect.setOutlineColor(borderColor);
	settingsRect.setPosition(pos);

	focusSprite.setTexture(Engine::Instance().texturesManager()->getTexture(TexturesManager::FOCUS_ICON));
	focusSprite.setScale(scaleFactor);

	quiteSprite.setTexture(Engine::Instance().texturesManager()->getTexture(TexturesManager::QUIET_ICON));
	quiteSprite.setScale(scaleFactor);

	audioSprite.setTexture(Engine::Instance().texturesManager()->getTexture(TexturesManager::AUDIO_ICON));
	audioSprite.setScale(scaleFactor);

	//audio
	pos += sf::Vector2f(borderXOffset, borderYOffset);

	audioRect.setSize(blockSize);
	audioRect.setFillColor(subRectColor);
	audioRect.setOutlineThickness(border);
	audioRect.setOutlineColor(borderColor);
	audioRect.setPosition(pos);

	//sound
	lbl_sound.setFont(Engine::Instance().fontManager()->font());
	lbl_sound.setCharacterSize(Engine::Instance().fontManager()->getCharSize(characterSize));
	lbl_sound.setFillColor(textColor);
	lbl_sound.setOutlineThickness(1);
	lbl_sound.setOutlineColor(sf::Color::Black);
	lbl_sound.setScale(scaleFactor);
	lbl_sound.setString(Engine::Instance().translationsManager()->translate(TranslationsManager::SOUND));
	lbl_sound.setPosition(pos + blockOffset);

	soundScale = new ValueScale();
	soundScale->setSize(sf::Vector2f(SCALE_WIDTH, -SCALE_HEIGHT));
	soundScale->setValue(defaultAudioLevel);
	soundScale->setColorActive(valueScaleColorActive);
	soundScale->setColorInactive(valueScaleColorInactive);
	soundScale->setPos(pos + sf::Vector2f(blockOffset.x * secondOffset, blockOffset.y + SCALE_HEIGHT/2 +
										  lbl_sound.getGlobalBounds().height - SCALE_HEIGHT/2));

	const float audioSpriteOffset = 100 * scaleFactor.x;
	audioSprite.setPosition(soundScale->pos().x + SCALE_WIDTH + audioSpriteOffset,
							soundScale->pos().y - audioSprite.getGlobalBounds().height/2);
	quiteSprite.setPosition(audioSprite.getPosition());

	//music
	lbl_music.setFont(Engine::Instance().fontManager()->font());
	lbl_music.setCharacterSize(Engine::Instance().fontManager()->getCharSize(characterSize));
	lbl_music.setFillColor(textColor);
	lbl_music.setOutlineThickness(1);
	lbl_music.setOutlineColor(sf::Color::Black);
	lbl_music.setScale(scaleFactor);
	lbl_music.setString(Engine::Instance().translationsManager()->translate(TranslationsManager::MUSIC));
	lbl_music.setPosition(pos + sf::Vector2f(blockOffset.x,
											 blockOffset.y * 2 + lbl_sound.getGlobalBounds().height));

	musicScale = new ValueScale();
	musicScale->setSize(sf::Vector2f(SCALE_WIDTH, -SCALE_HEIGHT));
	musicScale->setValue(defaultAudioLevel);
	musicScale->setColorActive(valueScaleColorActive);
	musicScale->setColorInactive(valueScaleColorInactive);
	musicScale->setPos(pos + sf::Vector2f(blockOffset.x * secondOffset, blockOffset.y * 2 + SCALE_HEIGHT/2 +
										  lbl_sound.getGlobalBounds().height +
										  lbl_music.getGlobalBounds().height - SCALE_HEIGHT/2));

	//video
	pos += sf::Vector2f(0, blockSize.y + borderYOffset/2);

	videoRect.setSize(blockSize);
	videoRect.setFillColor(subRectColor);
	videoRect.setOutlineThickness(border);
	videoRect.setOutlineColor(borderColor);
	videoRect.setPosition(pos);

	//fullscreen
	lbl_fullscreen.setFont(Engine::Instance().fontManager()->font());
	lbl_fullscreen.setCharacterSize(Engine::Instance().fontManager()->getCharSize(characterSize));
	lbl_fullscreen.setFillColor(textColor);
	lbl_fullscreen.setOutlineThickness(1);
	lbl_fullscreen.setOutlineColor(sf::Color::Black);
	lbl_fullscreen.setScale(scaleFactor);
	lbl_fullscreen.setString(Engine::Instance().translationsManager()->translate(TranslationsManager::FULLSCREEN));
	lbl_fullscreen.setPosition(pos + blockOffset);

	cbx_fullscreen = new CheckBox();
	cbx_fullscreen->setCheckedColor(checkboxColor);
	cbx_fullscreen->setBorderColor(checkBoxBorderColor);
	cbx_fullscreen->setSize(checkBoxSize);
	cbx_fullscreen->setPos(pos + sf::Vector2f(blockOffset.x * secondOffset, blockOffset.y +
											  lbl_fullscreen.getGlobalBounds().height - cbx_fullscreen->size().y/2));

	//resolutions
	lbl_resolution.setFont(Engine::Instance().fontManager()->font());
	lbl_resolution.setCharacterSize(Engine::Instance().fontManager()->getCharSize(characterSize));
	lbl_resolution.setFillColor(textColor);
	lbl_resolution.setOutlineThickness(1);
	lbl_resolution.setOutlineColor(sf::Color::Black);
	lbl_resolution.setScale(scaleFactor);
	lbl_resolution.setString(Engine::Instance().translationsManager()->translate(TranslationsManager::RESOLUTION));
	lbl_resolution.setPosition(pos + sf::Vector2f(blockOffset.x,
												  blockOffset.y * 2 + lbl_fullscreen.getGlobalBounds().height));

	resolutions = new ChooseList();
	resolutions->setFillColor(chooseListFillColor);
	resolutions->setBorderColor(chooseListBorderColor);
	resolutions->setCurrentColor(chooseListCurrentColor);
	resolutions->setCharacterSize(chooseListCharacterSize);
	resolutions->setTextColor(chooseListTextColor);
	sf::String currentResolution;
	const std::vector<sf::VideoMode> videoModes = sf::VideoMode::getFullscreenModes();
	for(const sf::VideoMode& videoMode : videoModes)
	{
		const sf::Vector2i resolution = sf::Vector2i(videoMode.width, videoMode.height);
		std::pair<sf::String, sf::Vector2i> v;
		v.first = std::to_string(resolution.x) + "x" + std::to_string(resolution.y);
		v.second = resolution;
		resolutionsMap.insert(v);
		if (v.second == Engine::Instance().settingsManager()->getResolution())
			currentResolution = v.first;
		resolutions->addItem(v.first);
	}
#if 0
	resolutionsMap = {
		{"1920x1080", sf::Vector2i(1920, 1080)},
		{"1360x768", sf::Vector2i(1360, 768)},
		{"1280x720", sf::Vector2i(1280, 720)},
		{"1600x900", sf::Vector2i(1600, 900)},
		{"2560x1440", sf::Vector2i(2560, 1440)}
	};
	for (auto it = resolutionsMap.begin(); it != resolutionsMap.end(); ++it)
	{
		if (it->second == Engine::Instance().settingsManager()->getResolution())
			currentResolution = it->first;
		resolutions->addItem(it->first);
	}
#endif
	resolutions->setScale(scaleFactor);
	resolutions->setSize(chooseListButtonSize);
	resolutions->setPos(pos + sf::Vector2f(blockOffset.x * secondOffset, blockOffset.y * 2 +
										   lbl_fullscreen.getGlobalBounds().height +
										   lbl_resolution.getGlobalBounds().height - resolutions->size().y));
	resolutions->update();

	//language
	pos += sf::Vector2f(0, blockSize.y + borderYOffset/2);
	miscRect.setSize(blockSize);
	miscRect.setFillColor(subRectColor);
	miscRect.setOutlineThickness(border);
	miscRect.setOutlineColor(borderColor);
	miscRect.setPosition(pos);

	lbl_language.setFont(Engine::Instance().fontManager()->font());
	lbl_language.setCharacterSize(Engine::Instance().fontManager()->getCharSize(characterSize));
	lbl_language.setFillColor(textColor);
	lbl_language.setOutlineThickness(1);
	lbl_language.setOutlineColor(sf::Color::Black);
	lbl_language.setScale(scaleFactor);
	lbl_language.setString(Engine::Instance().translationsManager()->translate(TranslationsManager::LANGUAGE));
	lbl_language.setPosition(pos + blockOffset);

	languages = new ChooseList();
	languages->setCharacterSize(chooseListCharacterSize);
	languages->setFillColor(chooseListFillColor);
	languages->setBorderColor(chooseListBorderColor);
	languages->setCurrentColor(chooseListCurrentColor);
	languages->setTextColor(chooseListTextColor);
	const std::vector<sf::String> languanges = Engine::Instance().translationsManager()->getAvaliableLanguageNames();
	for(const sf::String& langName : languanges)
		languages->addItem(langName);
	languages->setScale(scaleFactor);
	languages->setSize(chooseListButtonSize);
	languages->setPos(pos + sf::Vector2f(blockOffset.x * secondOffset, blockOffset.y +
										 lbl_language.getGlobalBounds().height - languages->size().y/2));
	languages->update();


	//shaders
	lbl_shaders.setFont(Engine::Instance().fontManager()->font());
	lbl_shaders.setCharacterSize(Engine::Instance().fontManager()->getCharSize(characterSize));
	lbl_shaders.setFillColor(textColor);
	lbl_shaders.setOutlineThickness(1);
	lbl_shaders.setOutlineColor(sf::Color::Black);
	lbl_shaders.setScale(scaleFactor);
	lbl_shaders.setString(Engine::Instance().translationsManager()->translate(TranslationsManager::SHADERS));
	lbl_shaders.setPosition(pos + sf::Vector2f(blockOffset.x,
											   blockOffset.y * 2 + lbl_language.getGlobalBounds().height));

	cbx_shaders = new CheckBox();
	cbx_shaders->setCheckedColor(checkboxColor);
	cbx_shaders->setBorderColor(checkBoxBorderColor);
	cbx_shaders->setSize(checkBoxSize);
	cbx_shaders->setPos(pos + sf::Vector2f(blockOffset.x * secondOffset,
										   blockOffset.y * 2 + lbl_language.getGlobalBounds().height +
										   lbl_shaders.getGlobalBounds().height - cbx_shaders->size().y/2));

	//buttons
	const float buttonOffset = border + 50 * scaleFactor.y;
	const sf::Vector2f buttonSize = sf::Vector2f(16 * scaleFactor.x,
												 16 * scaleFactor.y);
	const sf::Vector2f bottomRight = settingsRect.getPosition() + settingsSize;
	const float buttonsCharacterSize = 28;

	button_accept = new Button();
	button_accept->setSize(buttonSize);
	button_accept->setFillColor(buttonFillColor);
	button_accept->setBorderColor(buttonBorderColor);
	button_accept->setTextColor(buttonTextColor);
	button_accept->setCallback(std::bind(&SettingsWindow::accept, this));
	button_accept->setText(Engine::Instance().translationsManager()->translate(TranslationsManager::ACCEPT));
	button_accept->setTextCharacterSize(buttonsCharacterSize);
	button_accept->setScale(scaleFactor);
	button_accept->setPos(bottomRight - sf::Vector2f(buttonSize.x, -buttonOffset));

	const sf::Vector2f bottomLeft = settingsRect.getPosition() + sf::Vector2f(0, settingsSize.y);
	button_cancel = new Button();
	button_cancel->setSize(buttonSize);
	button_cancel->setFillColor(buttonFillColor);
	button_cancel->setBorderColor(buttonBorderColor);
	button_cancel->setTextColor(buttonTextColor);
	button_cancel->setCallback(std::bind(&SettingsWindow::back, this));

	button_cancel->setText(Engine::Instance().translationsManager()->translate(TranslationsManager::CANCEL));
	button_cancel->setScale(scaleFactor);
	button_cancel->setTextCharacterSize(buttonsCharacterSize);
	button_cancel->setPos(bottomLeft + sf::Vector2f(0, buttonOffset));

	cbx_fullscreen->setChecked(Engine::Instance().settingsManager()->getFullscreen());
	soundScale->setValue(Engine::Instance().settingsManager()->getSoundLevel());
	musicScale->setValue(Engine::Instance().settingsManager()->getMusicLevel());
	resolutions->setCurrent(currentResolution);
	languages->setCurrent(Engine::Instance().translationsManager()->currentLanguageName());
	cbx_shaders->setChecked(Engine::Instance().settingsManager()->getShaders());

	updateFocus();
}

SettingsWindow::~SettingsWindow()
{
	delete soundScale;
	delete musicScale;
	delete cbx_fullscreen;
	delete resolutions;
	delete languages;
	delete cbx_shaders;
	delete button_accept;
	delete button_cancel;
}

void SettingsWindow::paint(sf::RenderWindow *window)
{
	drawBackground(window);

	if (m_drawRects)
		window->draw(settingsRect);
	window->draw(lbl_settings);

	if (m_drawRects)
		window->draw(audioRect);

	window->draw(lbl_sound);
	soundScale->draw(window);
	window->draw(lbl_music);
	musicScale->draw(window);

	window->draw(audioSprite);
	if (m_quite)
		window->draw(quiteSprite);

	if (m_drawRects)
		window->draw(videoRect);

	window->draw(lbl_fullscreen);
	cbx_fullscreen->draw(window);
	window->draw(lbl_resolution);
	resolutions->draw(window);

	if (m_drawRects)
		window->draw(miscRect);

	window->draw(lbl_language);
	languages->draw(window);
	if (m_showShaders)
	{
		window->draw(lbl_shaders);
		cbx_shaders->draw(window);
	}

	button_accept->draw(window);
	button_cancel->draw(window);

	window->draw(focusSprite);
}

void SettingsWindow::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::KeyPressed)
	{
		if (event->key.code == sf::Keyboard::Down)
			currentFocus = static_cast<FOCUSES>(currentFocus + 1);
		if (event->key.code == sf::Keyboard::Up)
			currentFocus = static_cast<FOCUSES>(currentFocus - 1);

		updateFocus();
	}
	else if (event->type == sf::Event::JoystickMoved)
	{
		if (event->joystickMove.axis == sf::Joystick::Y)
		{
			if (event->joystickMove.position > 50)
				currentFocus = static_cast<FOCUSES>(currentFocus + 1);
			else if (event->joystickMove.position < -50)
				currentFocus = static_cast<FOCUSES>(currentFocus - 1);
		}
		updateFocus();
	}
	else if (event->type == sf::Event::MouseButtonPressed ||
			 event->type == sf::Event::MouseButtonReleased ||
			 event->type == sf::Event::MouseMoved)
	{
		soundScale->event(event);
		musicScale->event(event);
		cbx_fullscreen->event(event);
		resolutions->event(event);
		languages->event(event);
		if(m_showShaders)
			cbx_shaders->event(event);
		button_cancel->event(event);
		button_accept->event(event);

		switch (event->type)
		{
		case sf::Event::MouseMoved:
		{
			const sf::Vector2f pos = sf::Vector2f(event->mouseMove.x, event->mouseMove.y);
			audioSprite.setColor(sf::Color::White);
			quiteSprite.setColor(sf::Color::White);
			if (audioSprite.getGlobalBounds().contains(pos))
			{
				audioSprite.setColor(EngineDefs::GrayColor);
				quiteSprite.setColor(EngineDefs::GrayColor);
			}
		}
			break;
		case sf::Event::MouseButtonReleased:
		{
			if (event->mouseButton.button == sf::Mouse::Left)
			{
				const sf::Vector2f pos = sf::Vector2f(event->mouseButton.x, event->mouseButton.y);
				if (audioSprite.getGlobalBounds().contains(pos))
					m_quite = !m_quite;
			}
		}
			break;
		default:
			break;
		}

		StateWindow::eventFilter(event);
		return;
	}

	switch (currentFocus)
	{
	case SOUND:
		soundScale->event(event);
		break;
	case MUSIC:
		musicScale->event(event);
		break;
	case QUITE:
	{
		if (event->type == sf::Event::KeyPressed)
			if (event->key.code == sf::Keyboard::Enter)
				m_quite = !m_quite;
	}
		break;
	case FULLSCREEN:
		cbx_fullscreen->event(event);
		break;
	case RESOLUTION:
		resolutions->event(event);
		break;
	case LANGUAGE:
		languages->event(event);
		break;
	case SHADERS:
		cbx_shaders->event(event);
		break;
	case BUTTON_CANCEL:
		button_cancel->event(event);
		break;
	case BUTTON_ACCEPT:
		button_accept->event(event);
		break;
	default:
		break;
	}
	StateWindow::eventFilter(event);
}

void SettingsWindow::back()
{
	Engine::Instance().stateManager()->setState(StateManager::MENU);
}

void SettingsWindow::init()
{
	m_quite = Engine::Instance().soundManager()->quiet();
}

void SettingsWindow::update()
{
	StateWindow::update();
	checkIsChanged();
}

void SettingsWindow::accept()
{
	const bool fullscreen = cbx_fullscreen->isChecked();
	const sf::Vector2i resolution = resolutionsMap.at(resolutions->currentText());
	bool videoModeChanged = false;
	if (Engine::Instance().settingsManager()->getFullscreen() != fullscreen ||
			Engine::Instance().settingsManager()->getResolution() != resolution )
	{
		Engine::Instance().settingsManager()->setResolution(resolution);
		Engine::Instance().settingsManager()->setFullscreen(fullscreen);
		videoModeChanged = true;
	}
	Engine::Instance().settingsManager()->setSoundLevel(soundScale->value());
	Engine::Instance().settingsManager()->setMusicLevel(musicScale->value());
	Engine::Instance().soundManager()->updateVolume();
	Engine::Instance().soundManager()->setQuiet(m_quite);
	if (m_quite)
		Engine::Instance().soundManager()->endBackgroundSound();
	else
	{
		Engine::Instance().soundManager()->setMusicLooped(true);
		Engine::Instance().soundManager()->startBackgroundSound(SoundManager::MAIN_MENU_MUSIC);
	}

	if (m_showShaders)
		Engine::Instance().settingsManager()->setShaders(cbx_shaders->isChecked());

	bool languageChanged = false;
	if (Engine::Instance().translationsManager()->currentLanguageName() != languages->currentText().toWideString())
	{
		Engine::Instance().translationsManager()->setCurrentLanguageByName(languages->currentText().toWideString());
		languageChanged = true;
	}
	Engine::Instance().stateManager()->setState(StateManager::MENU);

	if (languageChanged && !videoModeChanged)
		Engine::Instance().options<Options>()->reset();

	if (videoModeChanged)
		Engine::Instance().options<Options>()->updateWindow();
}

void SettingsWindow::updateFocus()
{
	if (currentFocus < SOUND)
		currentFocus = SOUND;
	if (currentFocus > BUTTON_ACCEPT)
		currentFocus = BUTTON_ACCEPT;

	sf::Vector2f pos;
	switch (currentFocus)
	{
	case SOUND:
		pos = lbl_sound.getPosition();
		break;
	case MUSIC:
		pos = lbl_music.getPosition();
		break;
	case QUITE:
		pos = audioSprite.getPosition();
		break;
	case FULLSCREEN:
		pos = lbl_fullscreen.getPosition();
		break;
	case RESOLUTION:
		pos = lbl_resolution.getPosition();
		break;
	case LANGUAGE:
		pos = lbl_language.getPosition();
		break;
	case BUTTON_CANCEL:
		pos = button_cancel->pos();
		break;
	case BUTTON_ACCEPT:
		pos = button_accept->pos();
		break;
	default:
		break;
	}
	pos.x -= 42 * Engine::Instance().settingsManager()->getScaleFactor().x;
	focusSprite.setPosition(pos);
}

void SettingsWindow::checkIsChanged()
{
	bool canAccept = false;
	bool shadersChanged = false;
	if (m_showShaders)
		shadersChanged = Engine::Instance().settingsManager()->getShaders() != cbx_shaders->isChecked();

	if (shadersChanged ||
			Engine::Instance().settingsManager()->getFullscreen() != cbx_fullscreen->isChecked() ||
			Engine::Instance().settingsManager()->getResolution() != resolutionsMap.at(resolutions->currentText()) ||
			soundScale->value() != Engine::Instance().settingsManager()->getSoundLevel() ||
			musicScale->value() != Engine::Instance().settingsManager()->getMusicLevel() ||
			Engine::Instance().soundManager()->quiet() != m_quite ||
			languages->currentText().toWideString() != Engine::Instance().translationsManager()->currentLanguageName())
	{
		canAccept = true;
	}
	button_accept->setFillColor(canAccept ? chooseListFillColor : Widget::DISABLED_COLOR);
}

void SettingsWindow::setShowShaders(bool showShaders)
{
	m_showShaders = showShaders;
}

void SettingsWindow::setDrawRects(bool drawRects)
{
	m_drawRects = drawRects;
}
