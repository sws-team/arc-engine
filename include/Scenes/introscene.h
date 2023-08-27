#ifndef INTROSCENE_H
#define INTROSCENE_H

#include "arcscene.h"

class IntroScene : public ArcScene
{
public:
	IntroScene();

	void init() override;
	bool eventFilter(sf::Event* event) override;
private:
	void finish();
};

#endif // INTROSCENE_H
