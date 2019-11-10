#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "stdheader.h"
#include "Game/gamedrawable.h"

class Instructions : public GameDrawable
{
public:
	Instructions();

	void draw(RenderTarget *const target) override;
	void update() override;

	bool isActive() const;
	void next();
	void skip();

private:
	RectangleShape shadowRect;

	enum STATES
	{
		START,

		TOWERS,
		ABILITIES,
		MONEY,
		HEALTH,
		ENERGY,
		REMOVE,
		UPGRADE,

		FINISHED
	};
	STATES m_state;
	void changeState(STATES state);

	RectangleShape textRect;
	Text text;
	RectangleShape targetRect;
};

#endif // INSTRUCTIONS_H