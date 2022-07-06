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
	if (modal)
		return true;

	return ArcObject::eventFilter(event);
}

void ArcWindow::setModal(bool modal)
{
	this->modal = modal;
}

bool ArcWindow::isModal() const
{
	return modal;
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


