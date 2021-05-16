#ifndef BUTTON_H
#define BUTTON_H

#include "widget.h"

class Button : public Widget
{
public:
	Button();

	void draw(sf::RenderTarget *target) override;
	void event(sf::Event *event) override;
	void update() override;

	void setCallback(std::function<void()> callback);
	void setText(const sf::String &text);

	void setTextCharacterSize(unsigned int size);

	void setScale(const sf::Vector2f& scaleFactor);
	void setRotation(float angle);

	void setFillColor(const sf::Color &fillColor);
	void setBorderColor(const sf::Color &borderColor);
	void setTextColor(const sf::Color &textColor);
	void setCharacterSize(unsigned int characterSize);

	void setTexture(const sf::Texture *texture);
	void setButtonColor(const sf::Color& color);

	void setEnabled(bool enabled);

	bool isEnabled() const;

private:
	sf::RectangleShape rect;
	sf::Text m_text;
	std::function<void()> m_callback;
	sf::RectangleShape currentRect;
	bool hovered;
	unsigned int m_characterSize;
	sf::Color color;
	bool m_enabled;
};

#endif // BUTTON_H
