#include "arcwindow.h"
#include "engine.h"
#include "managers.h"
#include "arcsprite.h"

ArcWindow::ArcWindow(const std::string &name)
	: ArcObject(name)
{
	setType(ArcEngine::OBJECT);
	DEBUG_OBJECT(this);
}

ArcWindow::~ArcWindow()
{
	DEBUG_OBJECT(nullptr);
}

bool ArcWindow::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::Closed)
		this->back();
	if (event->type == sf::Event::MouseButtonPressed)
	{
		if (event->mouseButton.button == sf::Mouse::Right)
			back();
	}
	else if (event->type == sf::Event::KeyPressed)
	{
		if (event->key.code == sf::Keyboard::Escape)
			this->back();
	}
	else if (event->type == sf::Event::JoystickButtonPressed)
	{
		if (event->joystickButton.button == ArcEngine::KEY_ESCAPE)
			this->back();
	}
	return ArcObject::eventFilter(event);
}

void ArcWindow::setBackground(TextureType type)
{
	bg = new ArcSprite("background");
	bg->::ArcObject::setSize(sf::Vector2f(SettingsManager::defaultResolution.x,
										  SettingsManager::defaultResolution.y));
	bg->setTexture(type);
	addChild(bg);
}

void ArcWindow::back()
{

}

