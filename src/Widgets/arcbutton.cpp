#include "arcbutton.h"
#include <ArcEngine>
#include "managers.h"
#include "collisions.h"
#include "mainwindow.h"
#include <ArcVariant>
#include <ArcEngineUtils>

ArcButton::ArcButton(const std::string &name)
	: ArcSprite(name)
{
	setType(ArcEngine::BUTTON);
	hoverRect.setFillColor(sf::Color::Transparent);
}

void ArcButton::draw(sf::RenderTarget * const target)
{
	ArcSprite::draw(target);
	if (m_hoverType == HOVER_TYPE::RECT_HOVER)
		target->draw(hoverRect, m_transform);
}

void ArcButton::setCallback(const std::function<void ()> &func)
{
	m_callback = func;
}

bool ArcButton::eventFilter(sf::Event *event)
{
	if (!m_clickable)
		return false;
	if (m_callback == nullptr)
		return false;
	if (event->type == sf::Event::MouseButtonReleased) {
		const sf::Vector2f pos = PIXEL_TO_POS(event->mouseButton.x, event->mouseButton.y);
		if (Intersection::contains(this, pos)) {
			if (event->mouseButton.button == sf::Mouse::Left) {
				click();
				return m_opaqueClicks;
			}
			else if (event->mouseButton.button == sf::Mouse::Right) {
//				if (m_menu != nullptr)
//					m_menu();
			}
		}
	}
	else if (m_hoverType != HOVER_TYPE::NONE && event->type == sf::Event::MouseMoved) {
		const sf::Vector2f pos = PIXEL_TO_POS(event->mouseMove.x, event->mouseMove.y);
		bool hover = false;
		hoverRect.setFillColor(sf::Color::Transparent);
		if (Intersection::contains(this, pos)) {
			hover = true;
			hoverRect.setFillColor(ArcEngine::HoverColor);
		}
		this->hover(hover);
	}
//	else if (event->type == sf::Event::KeyPressed) {
//		if (event->key.code == sf::Keyboard::Return)
//			m_callback();
//	}
//	else if (event->type == sf::Event::JoystickButtonPressed) {
//		if (event->joystickButton.button == ArcEngine::KEY_ACCEPT)
//			m_callback();
//	}
	return ArcSprite::eventFilter(event);
}

void ArcButton::updatePos()
{
	hoverRect.setPosition(scaledGlobalPos());
	ArcSprite::updatePos();
}

void ArcButton::updateScale()
{
	hoverRect.setScale(scaledGlobalScale());
	ArcSprite::updateScale();
}

void ArcButton::updateOrigin()
{
	hoverRect.setOrigin(globalOrigin());
	ArcSprite::updateOrigin();
}

void ArcButton::updateSize()
{
	hoverRect.setSize(size());
	ArcSprite::updateSize();
}

void ArcButton::setOpaqueClicks(bool opaqueClicks)
{
	m_opaqueClicks = opaqueClicks;
}

void ArcButton::click()
{
	NOTIFICATION_MANAGER->notify(NotificationManager::BUTTON_CLICKED, this);
	PLAY_SOUND(SoundManager::CLICK);
	if (m_callback != nullptr)
		m_callback();
}

void ArcButton::hover(bool update)
{
	if (update && !hovered) {
		hovered = true;
		PLAY_SOUND(SoundManager::HOVER);
		if (m_hoverType == HOVER_TYPE::HOVER) {
			lastAlpha = alpha();
			setAlpha(0.3f);
		}
		else if (m_hoverType == HOVER_TYPE::SCALE) {
			lastScale = scale();
			setScale(1.3f, 1.3f);
		}
	}
	if (!update && hovered) {
		if (m_hoverType == HOVER_TYPE::HOVER)
			setAlpha(lastAlpha);
		else if (m_hoverType == HOVER_TYPE::SCALE)
			setScale(lastScale);
	}
	hovered = update;
}

ArcButton::HOVER_TYPE ArcButton::hoverType() const
{
	return m_hoverType;
}

void ArcButton::setHoverType(const HOVER_TYPE &hoverType)
{
	m_hoverType = hoverType;
}

void ArcButton::setClickable(bool enabled)
{
	m_clickable = enabled;
}

bool ArcButton::isClickable() const
{
	return m_clickable;
}
