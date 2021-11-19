#include "arclabel.h"
#include "engine.h"
#include "managers.h"

ArcLabel::ArcLabel(const std::string& name)
	: ArcObject(name)
{
	setType(ArcEngine::LABEL);
	m_text.setFont(Engine::Instance().fontManager()->font());
	setColor(sf::Color::White);
	setBorderColor(sf::Color::Black);
	setBorderSize(1.f);
}

void ArcLabel::draw(sf::RenderTarget * const target)
{
	target->draw(m_text, m_transform);
	ArcObject::draw(target);

#if 0
	sf::RectangleShape rect;
	rect.setOutlineThickness(1);
	rect.setOutlineColor(sf::Color::Cyan);
	rect.setFillColor(sf::Color::Transparent);
	rect.setPosition(m_text.getGlobalBounds().left, m_text.getGlobalBounds().top);
	rect.setSize(sf::Vector2f(m_text.getGlobalBounds().width, m_text.getGlobalBounds().height));
	target->draw(rect);
#endif
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
	m_fontSize = size;
	if (!m_autoSize)
		m_text.setCharacterSize(size);
	updateAutoSize();
}

unsigned int ArcLabel::fontSize() const
{
	return m_fontSize;
}

void ArcLabel::setBorderSize(float size)
{
	m_text.setOutlineThickness(size);
}

float ArcLabel::borderSize() const
{
	return m_text.getOutlineThickness();
}

void ArcLabel::setBorderColor(const sf::Color &color)
{
	m_text.setOutlineColor(color);
}

sf::Color ArcLabel::borderColor() const
{
	return m_text.getOutlineColor();
}

void ArcLabel::setText(const std::string &text)
{
	m_text.setString(sf::String(text));
	updateAutoSize();
}

void ArcLabel::setText(TranslationType id)
{
	setText(TR(id));
}

std::string ArcLabel::text() const
{
	return m_text.getString().toAnsiString();
}

void ArcLabel::updatePos()
{
	m_text.setPosition(scaledGlobalPos());
	ArcObject::updatePos();
}

void ArcLabel::updateScale()
{
	m_text.setScale(scaledGlobalScale());
	ArcObject::updateScale();
}

void ArcLabel::updateOrigin()
{
	sf::Vector2f center = sf::Vector2f(size().x/2 * scaledGlobalScale().x - m_text.getGlobalBounds().width/2,
									   size().y/2 * scaledGlobalScale().y - m_text.getGlobalBounds().height);
	m_text.setOrigin(globalOrigin() - center);
	ArcObject::updateOrigin();
}

void ArcLabel::updateSize()
{
	if (m_autoSize)
		updateAutoSize();
	ArcObject::updateSize();
}

void ArcLabel::updateAutoSize()
{
	if (m_autoSize) {
		m_text.setCharacterSize(m_fontSize);
		unsigned actualFontSize = m_fontSize;
		while(true) {
			if (m_text.getLocalBounds().width <= size().x &&
					m_text.getLocalBounds().height <= size().y)
				break;
			if (actualFontSize <= 1)
				break;
			actualFontSize--;
			m_text.setCharacterSize(actualFontSize);
		}
	}
	else {
		setSize(m_text.getGlobalBounds().width, m_text.getGlobalBounds().height);
	}
}

bool ArcLabel::autoSize() const
{
	return m_autoSize;
}

void ArcLabel::setAutoSize(bool autoSize)
{
	m_autoSize = autoSize;
	setFontSize(m_fontSize);
	updateAutoSize();
}
