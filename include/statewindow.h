#ifndef STATEWINDOW_H
#define STATEWINDOW_H

#include "stdheader.h"
#include "engine.h"
#include "enginedef.h"

#include "Swoosh/Activity.h"

class StateWindow : public swoosh::Activity
{
public:
	StateWindow(swoosh::ActivityController& controller);
	virtual ~StateWindow();

	virtual void onStart() override;
	virtual void onLeave() override;
	virtual void onExit() override;
	virtual void onEnter() override;
	virtual void onResume() override;
	virtual void onEnd() override;
	virtual void onUpdate(double elapsed) override;
	virtual void onDraw(sf::RenderTexture& surface) override;
	virtual void eventFilter(sf::Event* event);

	void setBackground(TextureType type);

protected:
	void drawBackground(sf::RenderTarget *target);
	sf::Sprite background;
	virtual void back();
};

#endif // STATEWINDOW_H
