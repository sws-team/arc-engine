#include "arclabel.h"
#include "engine.h"
#include "managers.h"

sf::Vector2f ArcLabel::globalTextOffset = sf::Vector2f(0, 0);

ArcLabel::ArcLabel(const std::string& name)
	: ArcObject(name)
{
	m_fontSize = 20;
	setType(ArcEngine::LABEL);
	m_text.setFont(Engine::Instance().fontManager()->font());
	ColorProperty::setObject(this);
	BorderColorProperty::setObject(this);
	BorderSizeProperty::setObject(this);
}

void ArcLabel::draw(sf::RenderTarget * const target)
{
	target->draw(m_text, m_transform);
	ArcObject::draw(target);

#ifdef ARC_DEBUG
	if (showLabelDebug) {
		sf::RectangleShape rect;
		rect.setOutlineThickness(1);
		rect.setOutlineColor(sf::Color::Cyan);
		rect.setFillColor(sf::Color::Transparent);
		rect.setPosition(m_text.getGlobalBounds().left, m_text.getGlobalBounds().top);
		rect.setSize(sf::Vector2f(m_text.getGlobalBounds().width, m_text.getGlobalBounds().height));
		target->draw(rect);
	}
#endif
}

void ArcLabel::setColor(const sf::Color &color)
{
	ColorProperty::setColor(color);
	m_text.setFillColor(actualColor());
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
	BorderSizeProperty::setBorderSize(size);
	m_text.setOutlineThickness(size);
}

void ArcLabel::setFont(FontType fontType)
{
	m_text.setFont(Engine::Instance().fontManager()->getFont(fontType));
}

void ArcLabel::setBorderColor(const sf::Color &color)
{
	BorderColorProperty::setBorderColor(color);
	m_text.setOutlineColor(actualBorderColor());
}

void ArcLabel::setText(const std::string &text)
{
	setLabelText(sf::String(text));
}

void ArcLabel::setText(TranslationType id)
{
	setLabelText(TR(id));
}

void ArcLabel::setTextFormatted(const sf::String &text, const std::vector<std::string> &args)
{
	sf::String copy = text;
	for (unsigned i = 0; i < args.size(); ++i) {
		const std::string placeholder = "%" + std::to_string(i + 1);
		copy.replace(placeholder, sf::String::fromUtf8(args.at(i).begin(), args.at(i).end()));
	}
	setLabelText(copy);
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

sf::Vector2f ArcLabel::textOffset() const
{
	return m_textOffset;
}

void ArcLabel::setTextOffset(const sf::Vector2f &textOffset)
{
	m_textOffset = textOffset;
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

void ArcLabel::setLabelText(const sf::String &text)
{
	m_text.setString(text);
	updateAutoSize();
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

void ArcLabel::setGlobalTextOffset(const sf::Vector2f &offset)
{
	globalTextOffset = offset;
}

sf::Vector2f ArcLabel::scaledGlobalPos() const
{
	sf::Vector2f textOffset;
	if (m_textOffset == sf::Vector2f(0, 0))
		textOffset = globalTextOffset;
	else
		textOffset = m_textOffset;

	if (enabledScaleFactor) {
		textOffset.x *= scaleFactor.x;
		textOffset.y *= scaleFactor.y;
	}

	return ArcObject::scaledGlobalPos() + textOffset;
}

