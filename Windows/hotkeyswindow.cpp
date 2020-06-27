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
	headerText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(100));
	headerText.setOutlineThickness(1.f);
	headerText.setFillColor(GameOptions::primaryColor);
	headerText.setOutlineColor(GameOptions::secondaryColor);

	text.setFont(Engine::Instance().fontManager()->font());
	text.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	text.setCharacterSize(Engine::Instance().fontManager()->getCharSize(32));
	text.setOutlineThickness(1.f);
	text.setFillColor(GameOptions::primaryColor);
	text.setOutlineColor(GameOptions::secondaryColor);

	headerText.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::HOTKEYS));

	sf::String str;
	str += "Space - start/skip" + EngineDefs::endline;
	str += "Return - action/next" + EngineDefs::endline;
	str += "Escape - pause/back" + EngineDefs::endline;
	str += "BackSpace - reset camera" + EngineDefs::endline;
	str += "Tab - rotate acid ability" + EngineDefs::endline;
	str += "Q - swap panel" + EngineDefs::endline;
	str += "F - upgrade selected tower" + EngineDefs::endline;
	str += "+ - zoom in" + EngineDefs::endline;
	str += "- - zoom out" + EngineDefs::endline;
	str += "Left - move cursor left" + EngineDefs::endline;
	str += "Right - move cursor right" + EngineDefs::endline;
	str += "Up - move cursor up" + EngineDefs::endline;
	str += "Down - move cursor down" + EngineDefs::endline;
	str += "T - build base tower" + EngineDefs::endline;
	str += "Y - build freeze tower" + EngineDefs::endline;
	str += "U - build rocket tower" + EngineDefs::endline;
	str += "G - build energy tower" + EngineDefs::endline;
	str += "H - build laser tower" + EngineDefs::endline;
	str += "J - build improved tower" + EngineDefs::endline;
	str += "Z - activate bomb ability" + EngineDefs::endline;
	str += "X - activate freeze bomb ability" + EngineDefs::endline;
	str += "C - activate acid ability" + EngineDefs::endline;
	str += "V - activate increase tower damage ability" + EngineDefs::endline;
	str += "B - activate increase tower attack speed ability" + EngineDefs::endline;
	str += "N - activate stop ability" + EngineDefs::endline;
	str += "R, Delete - sell tower" + EngineDefs::endline;
	str += "F3 - show FPS" + EngineDefs::endline;
	text.setString(str);

	const float offset = 32 * Engine::Instance().settingsManager()->getScaleFactor().y;
	const float yOffset = 100 * Engine::Instance().settingsManager()->getScaleFactor().y;
	headerText.setPosition(Engine::Instance().settingsManager()->getResolution().x/2 -
						   headerText.getGlobalBounds().width/2,
						   yOffset);
	text.setPosition(Engine::Instance().settingsManager()->getResolution().x/2 -
					 text.getGlobalBounds().width/2, yOffset + headerText.getGlobalBounds().height + offset);
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
	window->draw(headerText);
	window->draw(text);
}
