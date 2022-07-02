#include "menuscene.h"
#include "engine.h"
#include "managers.h"
#include "enginedef.h"
#include "mainwindow.h"
#include <arclayout.h>
#include <arcbutton.h>

MenuScene::MenuScene(const std::string &name)
	: ArcScene(name)
{
	menuLayout = new ArcLayout("menus");
	menuLayout->setOffsetY(DEFAULT_Y_OFFSET);
	menuLayout->setAutoSize(true);
	bg->addChild(menuLayout);
}

void MenuScene::init()
{
	menuLayout->setGrid(menus.size(), 1);

	for(ArcButton *menu : menus)
		menuLayout->addChild(menu);

	menuLayout->updateLayout();
}

bool MenuScene::eventFilter(sf::Event *event)
{
	switch (event->type)
	{
	case sf::Event::KeyPressed:
	{
		switch (event->key.code)
		{
		case sf::Keyboard::Up:
			menuUp();
			break;
		case sf::Keyboard::Down:
			menuDown();
			break;
		case sf::Keyboard::Return:
			accept();
			break;
		default:
			break;
		}
	}
		break;
	default:
		break;
	}
	return ArcScene::eventFilter(event);
}

void MenuScene::back()
{
	CHANGE_STATE(SceneManager::CLOSING);
}

void MenuScene::addMenu(ArcButton *menu)
{
	menus.push_back(menu);
}

void MenuScene::menuUp()
{
	menus.at(currentMenu)->hover(false);
	if (currentMenu > 0)
		currentMenu--;

	menus.at(currentMenu)->hover(true);
}

void MenuScene::menuDown()
{
	menus.at(currentMenu)->hover(false);
	if (currentMenu < menus.size())
		currentMenu++;
	menus.at(currentMenu)->hover(true);
}

void MenuScene::setMenuPos(const sf::Vector2f &pos)
{
	menuLayout->setPos(pos);
}

void MenuScene::setMenuPos(float x, float y)
{
	setMenuPos(sf::Vector2f(x, y));
}

void MenuScene::accept()
{
	menus.at(currentMenu)->click();
}
