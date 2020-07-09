#ifndef CHOOSELIST_H
#define CHOOSELIST_H

#include "widget.h"

class ChooseList : public Widget
{
public:
	ChooseList();

	void draw(sf::RenderTarget *target) override;
	void event(sf::Event *event) override;
	void update() override;

	void setList(const std::vector<sf::String>& list);
	void addItem(const sf::String& item);
	void setCurrent(int n);
	void setCurrent(const sf::String& text);
	sf::String currentText() const;
	void setScale(const sf::Vector2f& scaleFactor);

	void setCharacterSize(unsigned int characterSize);

	void setTextColor(const sf::Color &textColor);

	void setFillColor(const sf::Color &fillColor);

	void setBorderColor(const sf::Color &borderColor);

	void setCurrentColor(const sf::Color &currentColor);

	void setMaxCount(int maxCount);

private:
	struct ChooseValue
	{
		sf::RectangleShape rect;
		sf::Text text;
	};
	std::vector<ChooseValue> rects;
	std::vector<sf::String> m_list;
	void updateList();
	int current;
	unsigned int m_characterSize;

	sf::Color m_textColor;
	sf::Color m_fillColor;
	sf::Color m_borderColor;
	sf::Color m_currentColor;

	sf::RectangleShape currentRect;
	bool hovered;

	sf::RectangleShape previous;
	sf::RectangleShape next;
	int startValue;
	int m_maxCount;
	void updateCurrentColors();
};

#endif // CHOOSELIST_H
