#ifndef MENU_H
#define MENU_H

#include "statewindow.h"

class Menu : public StateWindow
{
public:
	Menu();

	virtual void paint(sf::RenderWindow *window) override;
	virtual void eventFilter(sf::Event* event) override;
	virtual void back() override;

	void setPos(const sf::Vector2f &pos);
	void setColor(const sf::Color &color);
	void setCurrentColor(const sf::Color &currentColor);
	void setCharacterSize(unsigned int characterSize);
	void setOutlineColor(const sf::Color &color);

	sf::Color getColor() const;
	sf::Color getCurrentColor() const;
	sf::Color getBorderColor() const;
	unsigned int getCharacterSize() const;
	sf::Vector2f getPos() const;

	void setMaxMenu(int maxMenu);
	void setTextYOffset(float textYOffset);

protected:
	virtual void accept() = 0;
	virtual void closeEvent() = 0;
	int currentMenu;
	void addItem(const sf::String &str);
	std::vector<sf::Text> menus;

	virtual int getMenuAtPos(const sf::Vector2f &point) const;
	virtual void updateColor();
private:
	void menuUp();
	void menuDown();

	sf::Vector2f m_pos;
	sf::Color m_color;
	sf::Color m_currentColor;
	sf::Color m_borderColor;

	static constexpr float DEFAULT_Y_OFFSET = 30;
	float yPos;
	unsigned int m_characterSize;
	int m_maxMenu;
	float m_textYOffset;
};

#endif // MENU_H
