#ifndef ARCWINDOW_H
#define ARCWINDOW_H

#include "arcobject.h"

class ArcWindow : public ArcObject
{
public:
	ArcWindow(const std::string& name = "Window");

	virtual void init();
	void setBackground(TextureType type);

	bool eventFilter(sf::Event *event) override;
protected:
	virtual void back();

	class ArcSprite *bg = nullptr;
};

#endif // ARCWINDOW_H
