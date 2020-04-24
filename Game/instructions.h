#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "stdheader.h"
#include "gameobject.h"

class Instructions : public GameDrawable
{
public:
	Instructions();

	void init();

	void draw(sf::RenderTarget *const target) override;
	void update() override;

	bool isActive() const;
	void next();
	void skip();

private:
	sf::RectangleShape shadowRect;

	enum STATES
	{
		START,

		TOWERS,
		ABILITIES,
		MONEY,
		HEALTH,
		PROGRESS,

		FINISHED
	};
	STATES m_state;
	void changeState(STATES state);

	sf::Text text;
	sf::RectangleShape targetRect;

	constexpr static int INSTRUCTIONS_OFFSET = 350;

	sf::Sprite character;
	sf::RectangleShape bottom;
	sf::RectangleShape top;
	sf::RectangleShape left;
	sf::RectangleShape right;
	sf::RectangleShape textRext;
};

#endif // INSTRUCTIONS_H
