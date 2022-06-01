#ifndef ARCLABEL_H
#define ARCLABEL_H

#include "arcobject.h"
#include "arcproperties.h"

class ArcLabel : public ArcObject,
		public ColorProperty,
		public BorderColorProperty,
		public BorderSizeProperty
{
public:
	ArcLabel(const std::string& name);

	void draw(sf::RenderTarget *const target) override;

	void setColor(const sf::Color &color) override;
	void setBorderColor(const sf::Color &color) override;
	void setBorderSize(float size) override;
	void setFont(FontType fontType);
	void setAlign(const sf::Vector2f &align);
	void setAlign(float x, float y);
	void setFontSize(unsigned int size);
	void setAutoSize(bool autoSize);
	static void setGlobalTextOffset(const sf::Vector2f& offset);
	void setMaxLines(unsigned int maxLines);

	void setText(const std::string& text);
	void setText(TranslationType id);
	void setTextFormatted(const sf::String& text, const std::vector<std::string>& args);
	void setTextFormatted(TranslationType id, const std::vector<std::string>& args);
	std::string text() const;

#ifdef ARC_DEBUG
	bool showLabelDebug = false;
#endif

	bool autoSize() const;
	unsigned int fontSize() const;
	sf::Vector2f align() const;
	unsigned int maxLines() const;

protected:
	void updatePos() override;
	void updateScale() override;
	void updateOrigin() override;
	void updateSize() override;
private:
	sf::Text m_text;
	bool m_autoSize = false;
	unsigned int m_fontSize = 0;
	unsigned int m_maxLines = 1;
	sf::Vector2f sizeModifier = sf::Vector2f(1.f, 1.f);
	static sf::Vector2f globalTextOffset;
	sf::Vector2f m_align = sf::Vector2f(0.5f, 0.5f);
	void updateAutoSize();
	void setLabelText(const sf::String& text);
	sf::String m_str;

	struct {
		sf::String str;
		float width = 0.f;
	} lastMultilineSearch;
};

#endif // ARCLABEL_H
