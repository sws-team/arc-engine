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

protected:
	void setPos(float x, float y) override;
	void setOrigin(float x, float y) override;
	void setScale(float x, float y) override;
private:
	sf::Text m_text;
};

#endif // ARCLABEL_H
