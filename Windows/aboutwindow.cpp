#include "aboutwindow.h"
#include "Game/Audio/soundcontroller.h"
#include "settings.h"
#include "globalvariables.h"

AboutWindow::AboutWindow()
	: StateWindow()
{
	setBackground(RESOURCES::ABOUT_BACKGROUND);

	const Vector2f rectSize = Vector2f(Settings::Instance().getScaleFactor().x * 640,
									   Settings::Instance().getScaleFactor().y * 480);
	rect.setSize(rectSize);
	rect.setFillColor(Color(16, 32, 64, 128));
	rect.setPosition(Settings::Instance().getResolution().x/2 - rectSize.x/2,
					 Settings::Instance().getResolution().y/2 - rectSize.y/2);

	creators.push_back(Creator("Developers"));
	creators.push_back(Creator("FIO"));
	creators.push_back(Creator("Tesytr"));
	creators.push_back(Creator("FIP"));
	creators.push_back(Creator("Desineg"));
	creators.push_back(Creator("fhjfghjghj"));

	const float x = rect.getPosition().x + rectSize.x/2;
	float y = rect.getPosition().y + rectSize.y;
	const float yOffset = 20 * Settings::Instance().getScaleFactor().y;
	for(Creator& creator : creators)
	{
		creator.text.setPosition(x - creator.text.getGlobalBounds().width/2, y);
		y += creator.text.getGlobalBounds().height;
		y += yOffset;
	}
}

void AboutWindow::init()
{
	SoundController::Instance().startBackgroundSound(ABOUT_SOUND_FILE);
}

void AboutWindow::paint(RenderWindow *window)
{
	drawBackground(window);

	window->draw(rect);
	for(const Creator& creator : creators)
	{
		if (creator.visible)
			window->draw(creator.text);
	}
}

void AboutWindow::back()
{
	Engine::Instance().setState(Engine::MANUAL);
}

void AboutWindow::update()
{
	if (timer.check(CREDITS_SPEED))
	{
		for(Creator& creator : creators)
		{
			creator.text.setPosition(creator.text.getPosition().x,
									 creator.text.getPosition().y - CREDITS_STEP);
		}
	}
	for(Creator& creator : creators)
	{
		if (rect.getGlobalBounds().intersects(creator.text.getGlobalBounds()))
		{
			creator.visible = true;
		}
		else
			creator.visible = false;
	}
}

AboutWindow::Creator::Creator(const string &str)
{
	text.setFont(GlobalVariables::Instance().font());
	text.setCharacterSize(40);
	text.setScale(Settings::Instance().getScaleFactor());
	text.setFillColor(Color::Red);
	text.setOutlineColor(Color::Blue);
	text.setOutlineThickness(2);
	text.setString(str);
	visible = false;
}
