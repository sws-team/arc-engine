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

	sf::Sprite textSprite;
	sf::Text text;
	sf::RectangleShape targetRect;

	constexpr static int INSTRUCTIONS_OFFSET = 350;
};

#endif // INSTRUCTIONS_H
