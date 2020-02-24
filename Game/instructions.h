#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "stdheader.h"
#include "gameobject.h"

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
		PROGRESS,

		FINISHED
	};
	STATES m_state;
	void changeState(STATES state);

	Sprite textSprite;
	Text text;
	RectangleShape targetRect;

	constexpr static int INSTRUCTIONS_OFFSET = 350;
};

#endif // INSTRUCTIONS_H
