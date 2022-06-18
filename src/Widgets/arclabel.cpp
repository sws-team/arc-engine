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

void ArcLabel::setText(const sf::String &text)
{
	setLabelText(text);
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

void ArcLabel::setTextFormatted(TranslationType id, const std::vector<std::string> &args)
{
	setTextFormatted(TR(id), args);
}

sf::String ArcLabel::text() const
{
	return m_str;
}

void ArcLabel::updatePos()
{
	sf::Vector2f textOffset = globalTextOffset;

	float w = width();
	float h = height();
	if (enabledScaleFactor) {
		textOffset.x *= scaleFactor.x;
		textOffset.y *= scaleFactor.y;

		w *= scaleFactor.x;
		h *= scaleFactor.y;
	}
	w -= m_text.getGlobalBounds().width;
	h -= m_text.getGlobalBounds().height;

	sf::Vector2f textPos;
	textPos.x = w * m_align.x;
	textPos.y = h * m_align.y;

	m_text.setPosition(scaledGlobalPos() + textPos + textOffset);
	ArcObject::updatePos();
}

void ArcLabel::updateScale()
{
	m_text.setScale(scaledGlobalScale());
	ArcObject::updateScale();
}

void ArcLabel::updateOrigin()
{
	m_text.setOrigin(globalOrigin());
	ArcObject::updateOrigin();
}

void ArcLabel::updateSize()
{
	if (m_autoSize)
		updateAutoSize();
	ArcObject::updateSize();
	updatePos();
}

unsigned int ArcLabel::maxLines() const
{
	return m_maxLines;
}

void ArcLabel::setMaxLines(unsigned int maxLines)
{
	m_maxLines = maxLines;
	setLabelText(m_str);
}

sf::Vector2f ArcLabel::align() const
{
	return m_align;
}

void ArcLabel::setAlign(const sf::Vector2f &align)
{
	m_align = align;
	updatePos();
}

void ArcLabel::setAlign(float x, float y)
{
	setAlign(sf::Vector2f(x, y));
}

void ArcLabel::updateAutoSize()
{
	if (m_autoSize) {
		if (m_maxLines > 1) {
			if (lastMultilineSearch.str == m_str && lastMultilineSearch.width == width()) {
				return;
			}

			{
				m_text.setString(m_str);
				if (m_text.getLocalBounds().width <= width()) {
					return;
				}
			}

			auto getSeparators = [](const sf::String& str, char ch) -> std::vector<size_t> {
				std::vector<size_t> spaces;
				size_t pos = 0;
				while(true) {
					const size_t space = str.find(ch, pos);
					if (space == sf::String::InvalidPos)
						break;
					pos = space + 1;
					spaces.push_back(space);
				}
				return spaces;
			};

			struct SetTextResult {
				float width = 0;
				sf::String text;
				size_t linesCount = 0;
				int pos = -1;
			};
			auto getWidth = [this](const size_t space, const sf::String& text, char ch) -> SetTextResult {
				SetTextResult result;
				result.text = text;
				result.text.replace(space, 1, ch);
				m_text.setString(result.text);
				result.width = m_text.getLocalBounds().width;
				return result;
			};

			sf::String resultText = m_str;
			SetTextResult minResult;
			minResult.width = RESOLUTIONF.x;

			std::vector<size_t> spaces = getSeparators(m_str, ' ');
			const size_t lineCount = std::min(static_cast<size_t>(m_maxLines - 1), spaces.size());
			for (size_t line = 0; line < lineCount; ++line) {
				if (m_text.getLocalBounds().width <= size().x) {
					break;
				}
				for (int pos = spaces.size() - 1; pos >= 0; --pos) {
					const size_t space = spaces.at(pos);
					const SetTextResult setTextResult = getWidth(space, resultText, '\n');
					if (setTextResult.width <= minResult.width) {
						minResult = setTextResult;
						m_text.setString(minResult.text);
						if (m_text.getLocalBounds().width <= size().x) {
							break;
						}
					}
				}
				resultText = minResult.text;
			}

			spaces = getSeparators(minResult.text, '\n');
			for (unsigned pos = 0; pos < spaces.size(); ++pos) {
				const size_t space = spaces.at(pos);
				const SetTextResult setTextResult = getWidth(space, resultText, ' ');
				if (setTextResult.width <= minResult.width) {
					minResult = setTextResult;
					m_text.setString(minResult.text);
					if (m_text.getLocalBounds().width <= size().x) {
						break;
					}
				}
			}
			resultText = minResult.text;
			m_text.setString(resultText);
			lastMultilineSearch.str = m_str;
			lastMultilineSearch.width = width();
		}
		else {
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
	}
	else {
		setSize(m_text.getGlobalBounds().width /scaleFactor.x,
				m_text.getGlobalBounds().height /scaleFactor.y);
	}
}

void ArcLabel::setLabelText(const sf::String &text)
{
	if (m_str == text)
		return;
	m_str = text;
	m_text.setString(text);
	updateAutoSize();
	updatePos();
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

