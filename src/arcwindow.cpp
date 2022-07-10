#include "arcwindow.h"
#include "engine.h"
#include "managers.h"

ArcWindow::ArcWindow(const std::string &name)
	: ArcSprite(name)
{
//	bg = new ArcSprite("background");
//	bg->setSize(SettingsManager::defaultResolution);
//	addChild(bg);
}

ArcWindow::~ArcWindow()
{

}

void ArcWindow::init()
{
	ArcObject::initTransform();
}

void ArcWindow::deinit()
{

}

bool ArcWindow::eventFilter(sf::Event *event)
{
	if (canEscClose) {
		if (event->type == sf::Event::KeyPressed) {
			if (event->key.code == sf::Keyboard::Escape) {
				close();
				return true;
			}
		}
	}
	const bool result = ArcObject::eventFilter(event);
	if (result)
		return true;
	return modal;
}

void ArcWindow::setModal(bool modal)
{
	this->modal = modal;
}

void ArcWindow::setUnique(bool unique)
{
	this->unique = unique;
}

void ArcWindow::setEscCloseable(bool close)
{
	canEscClose = close;
}

bool ArcWindow::isModal() const
{
	return modal;
}

bool ArcWindow::isUnique() const
{
	return unique;
}

bool ArcWindow::isEscCloseable() const
{
	return canEscClose;
}

void ArcWindow::close()
{
	Engine::Instance().windowsManager()->closeWindow(this);
}

WindowType ArcWindow::type() const
{
	return m_type;
}

void ArcWindow::setType(const WindowType &type)
{
	m_type = type;
}


