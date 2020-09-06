#include "aboutwindow.h"
#include "managers.h"

const sf::Vector2f AboutWindow::RECT_SIZE = sf::Vector2f(900, 600);

AboutWindow::AboutWindow(swoosh::ActivityController &controller)
	: StateWindow(controller)
	,m_backState(Options::MENU)
	,m_characterSize(40)
	,m_color(sf::Color::White)
	,m_borderColor(sf::Color::Black)
	,started(false)
{
	setBackground(TexturesManager::ABOUT_BACKGROUND);

	const sf::Vector2f rectSize = sf::Vector2f(Engine::Instance().settingsManager()->getScaleFactor().x * RECT_SIZE.x,
									   Engine::Instance().settingsManager()->getScaleFactor().y * RECT_SIZE.y);
	rect.setSize(rectSize);
	rect.setFillColor(sf::Color(16, 32, 64, 128));
	rect.setPosition(Engine::Instance().settingsManager()->getResolution().x/2 - rectSize.x/2,
					 Engine::Instance().settingsManager()->getResolution().y/2 - rectSize.y/2);

}

void AboutWindow::onStart()
{
	Engine::Instance().soundManager()->startBackgroundSound(SoundManager::CREDITS_MUSIC);
}

void AboutWindow::onDraw(sf::RenderTexture &surface)
{
	drawBackground(&surface);

	surface.draw(rect);
	for(const Creator& creator : creators)
	{
		if (creator.visible)
			surface.draw(creator.text);
	}
}

void AboutWindow::onUpdate(double elapsed)
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
			creator.visible = true;
		else
			creator.visible = false;
	}
	bool notFinished = false;
	for(const Creator& creator : creators)
	{
		if (creator.visible && !started)
			started = true;
		notFinished = notFinished || creator.visible;
	}
	if (!notFinished && started)
		back();
}

void AboutWindow::back()
{
	Engine::Instance().soundManager()->setMusicLooped(true);
	Engine::Instance().soundManager()->startBackgroundSound(SoundManager::MAIN_MENU_MUSIC);
	Engine::Instance().getOptions()->changeState(m_backState);
}

void AboutWindow::addString(const sf::String &str)
{
	creators.push_back(Creator(str, m_characterSize));
}

void AboutWindow::addStrings(const std::vector<sf::String> &strs)
{
	for(const sf::String& str : strs)
		addString(str);

	const sf::Vector2f rectSize = sf::Vector2f(Engine::Instance().settingsManager()->getScaleFactor().x * RECT_SIZE.x,
									   Engine::Instance().settingsManager()->getScaleFactor().y * RECT_SIZE.y);
	const float x = rect.getPosition().x + rectSize.x/2;
	float y = rect.getPosition().y + rectSize.y;
	const float yOffset = 20 * Engine::Instance().settingsManager()->getScaleFactor().y;
	for(Creator& creator : creators)
	{		
		creator.text.setPosition(x - creator.text.getGlobalBounds().width/2, y);
		y += creator.text.getGlobalBounds().height;
		y += yOffset;
		creator.text.setFillColor(m_color);
		creator.text.setOutlineColor(m_borderColor);
	}
}

void AboutWindow::setBackState(GameState backState)
{
	m_backState = backState;
}

void AboutWindow::setCharacterSize(unsigned int characterSize)
{
	m_characterSize = characterSize;
}

void AboutWindow::setColor(const sf::Color &color)
{
	m_color = color;
}

void AboutWindow::setBorderColor(const sf::Color &color)
{
	m_borderColor = color;
}

AboutWindow::Creator::Creator(const sf::String &str, const unsigned int charSize)
{
	text.setFont(Engine::Instance().fontManager()->font());
	text.setCharacterSize(Engine::Instance().fontManager()->getCharSize(charSize));
	text.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	text.setOutlineThickness(2);
	text.setString(str);
	visible = false;
}
