#ifndef MENU_H
#define MENU_H

#include "arcwindow.h"

class Menu : public ArcWindow
{
public:
	Menu();

	bool eventFilter(sf::Event* event) override;
	void draw(sf::RenderTarget *const target) override;
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

	enum MENU_ALIGN
	{
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT
	};

	void setAlign(const MENU_ALIGN &align);

protected:
	virtual void accept() = 0;
	virtual void closeEvent() = 0;
	int currentMenu;
	void addItem(const sf::String &str, float k = 1.f);
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
	MENU_ALIGN m_align;
};

#endif // MENU_H
