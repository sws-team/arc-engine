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

protected:
	virtual void accept() = 0;
	virtual void closeEvent() = 0;
	int currentMenu;
	void addItem(const sf::String &str);
	std::vector<sf::Text> menus;
private:
	void menuUp();

	void menuDown();

	int getMenuAtPos(const sf::Vector2f &point) const;

	void updateColor();

	sf::Vector2f m_pos;
	sf::Color m_color;
	sf::Color m_currentColor;
	sf::Color m_borderColor;

	static constexpr float Y_OFFSET = 30;
	float yPos;
	unsigned int m_characterSize;
};

#endif // MENU_H
