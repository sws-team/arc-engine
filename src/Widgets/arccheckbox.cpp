#include "arccheckbox.h"
#include <engine.h>
#include <managers.h>

ArcCheckBox::ArcCheckBox(const std::string& name)
	: ArcButton(name)
{
	constexpr float defaultSize = 50.f;
	setSize(defaultSize, defaultSize);
	initTexture();

	tick.setFillColor(ArcEngine::GrayColor);
	setCallback([this]() {
		setChecked(!checked);
	});
}

void ArcCheckBox::setTickTexture(TextureType textureID)
{
	m_tickTextureID = textureID;
	if (textureID == -1)
		return;
	tick.setTexture(&Engine::Instance().texturesManager()->getTexture(textureID), true);
	updateSize();
	updatePos();
}

void ArcCheckBox::setCheckedCallback(const std::function<void (bool)> &callback)
{
	m_checkedCallback = callback;
}

void ArcCheckBox::setChecked(bool checked)
{
	this->checked = checked;
	if (m_checkedCallback != nullptr)
		m_checkedCallback(checked);
}

void ArcCheckBox::draw(sf::RenderTarget * const target)
{
	ArcButton::draw(target);
	if (checked)
		target->draw(tick, m_transform);
}

void ArcCheckBox::updatePos()
{
	if (m_tickTextureID != -1)
		tick.setPosition(scaledGlobalPos());
	else {
		const sf::Vector2f scale = scaledGlobalScale();
		tick.setPosition(scaledGlobalPos() + sf::Vector2f((width() * (1 - tickSize)) * scale.x / 2.f,
														  (height() * (1 - tickSize)) * scale.y /2.f));
	}
	ArcSprite::updatePos();
}

void ArcCheckBox::updateSize()
{
	if (m_tickTextureID != -1)
		tick.setSize(size());
	else
		tick.setSize(sf::Vector2f(width() * tickSize, height() * tickSize));
	ArcButton::updateSize();
}

void ArcCheckBox::updateScale()
{
	tick.setScale(scaledGlobalScale());
	ArcButton::updateScale();
}

void ArcCheckBox::setColor(const sf::Color &color)
{
	ArcButton::setColor(color);
	initTexture();
}

void ArcCheckBox::setTexture(TextureType textureID)
{
	ArcButton::setTexture(textureID);
	initTexture();
}

void ArcCheckBox::initTexture()
{
	if (textureID() == -1) {
		sprite.setFillColor(sf::Color::Transparent);

		setBorderColor(sf::Color::Black);
		setBorderSize(3);
	}
	else {
		sprite.setFillColor(actualColor());

		setBorderColor(sf::Color::Black);
		setBorderSize(0);
	}
}
