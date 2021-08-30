#include "arcbutton.h"
#include "engine.h"
#include "managers.h"
#include "collisions.h"
#include "mainwindow.h"

const sf::Color ArcButton::HOVERED_COLOR = sf::Color(32, 32, 32, 96);

ArcButton::ArcButton(const std::string &name)
	: ArcSprite(name)
{
	setType(ArcEngine::BUTTON);
	hoverRect.setFillColor(sf::Color::Transparent);
}

void ArcButton::draw(sf::RenderTarget * const target)
{
	ArcSprite::draw(target);
	target->draw(hoverRect);
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
		const sf::Vector2i pixelPos = sf::Vector2i(event->mouseButton.x, event->mouseButton.y);
		const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos, *Engine::Instance().window()->view());

		if (Collision::rectContains(sprite, pos)) {
			if (event->mouseButton.button == sf::Mouse::Left) {
				PLAY_SOUND(SoundManager::CLICK);
				if (m_callback != nullptr)
					m_callback();
			}
			else if (event->mouseButton.button == sf::Mouse::Right) {
//				if (m_menu != nullptr)
//					m_menu();
			}
		}
	}
	else if (event->type == sf::Event::MouseMoved) {
		const sf::Vector2i pixelPos = sf::Vector2i(event->mouseMove.x, event->mouseMove.y);
		const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																			   *Engine::Instance().window()->view());
		bool hover = false;
		hoverRect.setFillColor(sf::Color::Transparent);
		if (Collision::rectContains(sprite, pos)) {
//		if (rect.getGlobalBounds().contains(pos)) {
			hover = true;
			hoverRect.setFillColor(HOVERED_COLOR);
		}
		if (hover && !hovered) {
			hovered = true;
			PLAY_SOUND(SoundManager::HOVER);
		}
		hovered = hover;
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

void ArcButton::setClickable(bool enabled)
{
	m_clickable = enabled;
}

bool ArcButton::isClickable() const
{
	return m_clickable;
}
