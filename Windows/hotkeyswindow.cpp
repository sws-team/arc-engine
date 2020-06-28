#include "hotkeyswindow.h"
#include "gamemanagers.h"
#include "gameoptions.h"
#include "gamestatemanager.h"
#include "managers.h"

HotkeysWindow::HotkeysWindow()
	: StateWindow()
{
	setBackground(GAME_TEXTURE::MISSON_BACKGROUND);

	headerText.setFont(Engine::Instance().fontManager()->font());
	headerText.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	headerText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(80));
	headerText.setOutlineThickness(1.f);
	headerText.setFillColor(GameOptions::alternativePrimaryColor);
	headerText.setOutlineColor(GameOptions::secondaryColor);

	text.setFont(Engine::Instance().fontManager()->font());
	text.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	text.setCharacterSize(Engine::Instance().fontManager()->getCharSize(32));
	text.setOutlineThickness(1.f);
	text.setFillColor(GameOptions::primaryColor);
	text.setOutlineColor(GameOptions::secondaryColor);

	rect.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::HOTKEYS_FRAME));
	rect.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	headerText.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::HOTKEYS));
	text.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ALL_HOTKEYS_TEXT));

	const float offset = 32 * Engine::Instance().settingsManager()->getScaleFactor().y;

	rect.setPosition(Engine::Instance().settingsManager()->getResolution().x/2 - rect.getGlobalBounds().width/2,
					 Engine::Instance().settingsManager()->getResolution().y/2 - rect.getGlobalBounds().height/2);

	headerText.setPosition(Engine::Instance().settingsManager()->getResolution().x/2 - headerText.getGlobalBounds().width/2,
						   rect.getPosition().y + offset);

	text.setPosition(rect.getPosition().x + offset * 2,
					 rect.getPosition().y + offset + headerText.getGlobalBounds().height + offset);
}

HotkeysWindow::~HotkeysWindow()
{

}

void HotkeysWindow::back()
{
	Engine::Instance().stateManager()->setState(GameStateManager::MANUAL);
}

void HotkeysWindow::paint(sf::RenderWindow *window)
{
	drawBackground(window);
	window->draw(rect);
	window->draw(headerText);
	window->draw(text);
}
