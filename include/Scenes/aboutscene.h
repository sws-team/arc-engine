#ifndef ABOUTSCENE_H
#define ABOUTSCENE_H

#include "arcscene.h"
#include <ArcTimer>

class AboutScene : public ArcScene
{
public:
	AboutScene();

	void init() override;
	void draw(sf::RenderTarget *const target) override;
	bool eventFilter(sf::Event *event) override;
	void deinit() override;
	void update() override;

	void addString(const sf::String& str);
	void addStrings(const std::vector<sf::String>& strs);

	void setBackState(SceneType backState);

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
	ArcTimer timer;
	static constexpr float CREDITS_SPEED = 50;
	static constexpr float CREDITS_STEP = 3;
	SceneType m_backState;
	static const sf::Vector2f RECT_SIZE;
	unsigned int m_characterSize;
	sf::Color m_color;
	sf::Color m_borderColor;
	bool started;

	void back();
};

#endif // ABOUTSCENE_H
