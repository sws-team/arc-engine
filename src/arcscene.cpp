#include "arcscene.h"
#include "managers.h"
#include <ArcSprite>

ArcScene::ArcScene(const std::string &name)
	: ArcObject(name)
{
	setType(ArcEngine::OBJECT);
	setSize(SettingsManager::defaultResolution);
	bg = new ArcSprite("background");
	bg->setSize(SettingsManager::defaultResolution);
	addChild(bg);

#ifdef ARC_DEBUG
	Engine::Instance().debugManager()->setObject(this);
#endif
}

ArcScene::~ArcScene()
{
#ifdef ARC_DEBUG
	Engine::Instance().debugManager()->setObject(nullptr);
#endif
}

void ArcScene::init()
{
	ArcObject::initTransform();
}

void ArcScene::deinit()
{

}

bool ArcScene::eventFilter(sf::Event *event)
{
	return ArcObject::eventFilter(event);
}

bool ArcScene::isCursorVisible() const
{
	return cursorVisible;
}

void ArcScene::removeBackground()
{
	bg->destroy();
	bg = nullptr;
}

void ArcScene::setBackground(TextureType type)
{
	if (bg != nullptr)
		bg->setTexture(type);
}


