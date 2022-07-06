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

	bool isCursorVisible() const;
protected:
	virtual void back();

	class ArcSprite *bg = nullptr;
	bool cursorVisible = true;
};

#endif // ARCSCENE_H
