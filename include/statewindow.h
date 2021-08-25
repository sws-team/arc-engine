#ifndef STATEWINDOW_H
#define STATEWINDOW_H

#include "arcobject.h"

class ArcWindow : public ArcObject
{
public:
	ArcWindow();
	virtual ~ArcWindow();

	virtual void init();
	void setBackground(TextureType type);

	bool eventFilter(sf::Event *event) override;
protected:
	virtual void back();
};

#endif // STATEWINDOW_H
