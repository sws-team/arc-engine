#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include "arcscene.h"
#include "timer.h"

class AboutWindow : public ArcScene
{
public:
	AboutWindow();

	void init() override;
	void draw(sf::RenderTarget *const target) override;
	void back() override;
	void update() override;

	void addString(const sf::String& str);
	void addStrings(const std::vector<sf::String>& strs);

	void setBackState(GameState backState);

	void setCharacterSize(unsigned int characterSize);

	void setColor(const sf::Color& color);
	void setBorderColor(const sf::Color& color);

private:
	struct Creator
	{
		Creator(const sf::String &str, const unsigned int charSize);
		sf::Text text;
		bool visible;
	};
	std::vector<Creator> creators;

	sf::RectangleShape rect;
	Timer timer;
	static constexpr float CREDITS_SPEED = 50;
	static constexpr float CREDITS_STEP = 3;
	GameState m_backState;
	static const sf::Vector2f RECT_SIZE;
	unsigned int m_characterSize;
	sf::Color m_color;
	sf::Color m_borderColor;
	bool started;
};

#endif // ABOUTWINDOW_H
