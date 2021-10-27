#ifndef ARCLABEL_H
#define ARCLABEL_H

#include "arcobject.h"

class ArcLabel : public ArcObject
{
public:
	ArcLabel(const std::string& name);

	void draw(sf::RenderTarget *const target) override;

	void setColor(const sf::Color &color);
	sf::Color color() const;

	void setFontSize(unsigned int size);
	unsigned int fontSize() const;

	void setTextBorderSize(float size);
	float textBorderSize() const;

	void setTextBorderColor(const sf::Color &color);
	sf::Color textBorderColor() const;

	void setText(const std::string& text);
	std::string text() const;

	bool autoSize() const;
	void setAutoSize(bool autoSize);

protected:
	void updatePos() override;
	void updateScale() override;
	void updateOrigin() override;
	void updateSize() override;
private:
	sf::Text m_text;
	bool m_autoSize = false;
	unsigned int m_fontSize = 0;
	sf::Vector2f sizeModifier = sf::Vector2f(1.f, 1.f);
	void updateAutoSize();
};

#endif // ARCLABEL_H
