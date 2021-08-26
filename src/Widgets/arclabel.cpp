#include "arclabel.h"
#include "engine.h"
#include "managers.h"

ArcLabel::ArcLabel(const std::string& name)
	: ArcObject(name, ArcEngine::LABEL)
{
	watch(scaleFactor.x);
	m_text.setScale(SCALE_FACTOR);
	m_text.setFont(Engine::Instance().fontManager()->font());
}

void ArcLabel::draw(sf::RenderTarget * const target)
{
	ArcObject::draw(target);
	target->draw(m_text);
}

void ArcLabel::setPos(float x, float y)
{
	ArcObject::setPos(x, y);
	m_text.setPosition(x, y);
}

void ArcLabel::setOrigin(float x, float y)
{
	ArcObject::setOrigin(x, y);
	m_text.setOrigin(x, y);
}

void ArcLabel::setScale(float x, float y)
{
	ArcObject::setScale(x, y);
	m_text.setScale(x, y);
}

void ArcLabel::setColor(const sf::Color &color)
{
	m_text.setFillColor(color);
}

sf::Color ArcLabel::color() const
{
	return m_text.getFillColor();
}

void ArcLabel::setFontSize(unsigned int size)
{
	m_text.setCharacterSize(size);
}

unsigned int ArcLabel::fontSize() const
{
	return m_text.getCharacterSize();
}

void ArcLabel::setTextBorderSize(float size)
{
	m_text.setOutlineThickness(size);
}

float ArcLabel::textBorderSize() const
{
	return m_text.getOutlineThickness();
}

void ArcLabel::setTextBorderColor(const sf::Color &color)
{
	m_text.setOutlineColor(color);
}

sf::Color ArcLabel::textBorderColor() const
{
	return m_text.getOutlineColor();
}

void ArcLabel::setText(const std::string &text)
{
	m_text.setString(sf::String(text));
}

std::string ArcLabel::text() const
{
	return m_text.getString().toAnsiString();
}

