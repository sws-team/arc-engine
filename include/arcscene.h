#ifndef ARCSCENE_H
#define ARCSCENE_H

#include "arcobject.h"

class ArcScene : public ArcObject
{
public:
	ArcScene(const std::string& name = "Scene");
	~ArcScene() override;

	virtual void init();
	virtual void deinit();

	void setBackground(TextureType type);

	bool eventFilter(sf::Event *event) override;
protected:
	virtual void back();

	class ArcSprite *bg = nullptr;
};

#endif // ARCSCENE_H
