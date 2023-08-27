#include "aboutscene.h"
#include <ArcEngine>
#include <managers.h>

const sf::Vector2f AboutScene::RECT_SIZE = sf::Vector2f(900, 600);

AboutScene::AboutScene()
	: ArcScene("AboutScene")
	,m_backState(SceneManager::MENU)
	,m_characterSize(40)
	,m_color(sf::Color::White)
	,m_borderColor(sf::Color::Black)
	,started(false)
{
	setBackground(TexturesManager::ABOUT_BACKGROUND);

	static constexpr float CREDITS_SPEED = 50;
	moveTextTimer.setInterval(CREDITS_SPEED);
	moveTextTimer.setCallback(std::bind(&AboutScene::moveText, this));

	const sf::Vector2f rectSize = sf::Vector2f(Engine::Instance().settingsManager()->getScaleFactor().x * RECT_SIZE.x,
									   Engine::Instance().settingsManager()->getScaleFactor().y * RECT_SIZE.y);
	rect.setSize(rectSize);
	rect.setFillColor(sf::Color(16, 32, 64, 128));
	rect.setPosition(Engine::Instance().settingsManager()->getResolution().x/2 - rectSize.x/2,
					 Engine::Instance().settingsManager()->getResolution().y/2 - rectSize.y/2);
}

void AboutScene::init()
{
	ArcScene::init();
	Engine::Instance().soundManager()->startBackgroundSound(SoundManager::CREDITS_MUSIC);
	moveTextTimer.start();
}

void AboutScene::draw(sf::RenderTarget *const target)
{
	ArcScene::draw(target);
	target->draw(rect);
	for(const Creator& creator : creators)
	{
		if (creator.visible)
			target->draw(creator.text);
	}
}

bool AboutScene::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::Closed) {
		back();
		return true;
	}
	else if (event->type == sf::Event::KeyPressed) {
		if (event->key.code == sf::Keyboard::Escape) {
			back();
			return true;
		}
	}
	return ArcScene::eventFilter(event);
}

void AboutScene::deinit()
{
	Engine::Instance().soundManager()->setMusicLooped(true);
	Engine::Instance().soundManager()->startBackgroundSound(SoundManager::MAIN_MENU_MUSIC);
}

void AboutScene::update()
{
	for(Creator& creator : creators) {
		if (rect.getGlobalBounds().intersects(creator.text.getGlobalBounds()))
			creator.visible = true;
		else
			creator.visible = false;
	}
	bool notFinished = false;
	for(const Creator& creator : creators) {
		if (creator.visible && !started)
			started = true;
		notFinished = notFinished || creator.visible;
	}
	if (!notFinished && started)
		back();
}

void AboutScene::addString(const sf::String &str)
{
	creators.push_back(Creator(str, m_characterSize));
}

void AboutScene::addStrings(const std::vector<sf::String> &strs)
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

void AboutScene::setBackState(SceneType backState)
{
	m_backState = backState;
}

void AboutScene::setCharacterSize(unsigned int characterSize)
{
	m_characterSize = characterSize;
}

void AboutScene::setColor(const sf::Color &color)
{
	m_color = color;
}

void AboutScene::setBorderColor(const sf::Color &color)
{
	m_borderColor = color;
}

void AboutScene::back()
{
	CHANGE_SCENE(m_backState);
}

void AboutScene::moveText()
{
	static constexpr float CREDITS_STEP = 3;
	for(Creator& creator : creators) {
		creator.text.setPosition(creator.text.getPosition().x,
								 creator.text.getPosition().y - CREDITS_STEP);
	}
}

AboutScene::Creator::Creator(const sf::String &str, const unsigned int charSize)
{
	text.setFont(Engine::Instance().fontManager()->font());
	text.setCharacterSize(Engine::Instance().fontManager()->getCharSize(charSize));
	text.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	text.setOutlineThickness(2);
	text.setString(str);
	visible = false;
}
