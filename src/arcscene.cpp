#include "arcscene.h"
#include "engine.h"
#include "managers.h"
#include "arcsprite.h"

ArcScene::ArcScene(const std::string &name)
	: ArcObject(name)
{
	setType(ArcEngine::OBJECT);
	DEBUG_OBJECT(this);
}

ArcScene::~ArcScene()
{
	DEBUG_OBJECT(nullptr);
}

void ArcScene::initWindow()
{
	ArcObject::initTransform();
}

bool ArcScene::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::Closed)
		this->back();
	else if (event->type == sf::Event::KeyPressed) {
		if (event->key.code == sf::Keyboard::Escape)
			this->back();
	}
	else if (event->type == sf::Event::JoystickButtonPressed) {
		if (event->joystickButton.button == ArcEngine::KEY_ESCAPE)
			this->back();
	}
	return ArcObject::eventFilter(event);
}

void ArcScene::setBackground(TextureType type)
{
	bg = new ArcSprite("background");
	bg->setSize(sf::Vector2f(SettingsManager::defaultResolution.x,
							 SettingsManager::defaultResolution.y));
	bg->setTexture(type);
	addChild(bg);
}

void ArcScene::back()
{

}

