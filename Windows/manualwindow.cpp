#include "manualwindow.h"
#include "Game/tower.h"
#include "Game/enemy.h"
#include "managers.h"
#include "Game/leveldef.h"
#include "gamemanagers.h"
#include "gamestatemanager.h"
#include "gameoptions.h"
#include "Game/gameability.h"
#include "mainwindow.h"
#include "Game/instructions.h"
#include "Game/gamepanel.h"

const sf::Color ManualWindow::SELECTED_COLOR = sf::Color(45, 45, 45, 96);

ManualWindow::ManualWindow()
	: StateWindow()
	,current(0)
	,page(0)
{	
	setBackground(GAME_TEXTURE::MANUAL_BACKGROUND);

	next.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::NEXT));
	next.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	previous.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::PREVIOUS));
	previous.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	close.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::EXIT));
	close.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	credits.setTexture(Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::CREDITS));
	credits.setScale(Engine::Instance().settingsManager()->getScaleFactor());

	toolTip.setFont(Engine::Instance().fontManager()->font());
	toolTip.setScale(Engine::Instance().settingsManager()->getScaleFactor());
	toolTip.setCharacterSize(Engine::Instance().fontManager()->getCharSize(40));
	toolTip.setOutlineThickness(1.f);
	toolTip.setFillColor(GameOptions::primaryColor);
	toolTip.setOutlineColor(GameOptions::secondaryColor);

	const float charOffsetX = 4 * Engine::Instance().settingsManager()->getScaleFactor().x;
	characterObject = new GameObject(GAME_TEXTURE::CHARACTER_FULL, sf::Vector2f(0, 0), sf::Vector2i(608, 768), 5);
	characterObject->setPos(sf::Vector2f(Engine::Instance().settingsManager()->getResolution().x - characterObject->getSize().x + charOffsetX,
										 Engine::Instance().settingsManager()->getResolution().y - characterObject->getSize().y));
	characterObject->animationSpeed = 50;

	demoFrame.setTexture(&Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::DEMO_FRAME));

	addElements();
	updatePos();
	updateCurrentInfo();
}

ManualWindow::~ManualWindow()
{
	for(Element& element : elements)
		delete element.object;
}

void ManualWindow::back()
{
	Engine::Instance().stateManager()->setState(GameStateManager::MENU);
}

void ManualWindow::update()
{
	if (current != -1)
	{
		const int id = page * MAX_ELEMENTS_COUNT + current;
		if (id < elements.size())
		{
			elements.at(id).object->update();
			if (elements.at(id).elementDemo != nullptr)
				elements.at(id).elementDemo->update();
		}
	}
	characterObject->update();
	StateWindow::update();
}

void ManualWindow::paint(sf::RenderWindow *window)
{
	drawBackground(window);
	const int startValue = MAX_ELEMENTS_COUNT * page;
	for (unsigned int i = startValue; i < startValue + MAX_ELEMENTS_COUNT; ++i)
	{
		if (i >= elements.size())
			break;
		const Element element = elements.at(i);
		window->draw(element.nameFrame);
		window->draw(element.icon);
		window->draw(element.titleText);
	}
	if (current != -1)
	{
		window->draw(currentRect);

		const int id = page * MAX_ELEMENTS_COUNT + current;
		if (id < elements.size())
		{
			window->draw(elements.at(id).nameText);
			window->draw(elements.at(id).descriptionText);
			elements.at(id).object->draw(window);
			if (elements.at(id).elementDemo != nullptr)
				elements.at(id).elementDemo->draw(window);
		}
	}
	window->draw(demoFrame);

	window->draw(previous);
	window->draw(next);
	window->draw(credits);
	window->draw(close);
	window->draw(toolTip);
	characterObject->draw(window);
}

void ManualWindow::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::MouseButtonPressed)
	{
		const sf::Vector2i pixelPos = sf::Vector2i(event->mouseButton.x, event->mouseButton.y);
		const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																			   *Engine::Instance().window()->view());
		if (next.getGlobalBounds().contains(pos))
		{
			Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
			nextPage();
		}
		if (previous.getGlobalBounds().contains(pos))
		{
			Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
			previousPage();
		}
		if (credits.getGlobalBounds().contains(pos))
		{
			Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
			return Engine::Instance().stateManager()->setState(StateManager::ABOUT);
		}
		if (close.getGlobalBounds().contains(pos))
		{
			Engine::Instance().soundManager()->playOnce(SoundManager::CLICK);
			back();
		}
	}
	else if (event->type == sf::Event::MouseMoved)
	{
		current = -1;
		const sf::Vector2i pixelPos = sf::Vector2i(event->mouseMove.x, event->mouseMove.y);
		const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos,
																			   *Engine::Instance().window()->view());
		const int startValue = MAX_ELEMENTS_COUNT * page;
		for (unsigned int i = startValue; i < startValue + MAX_ELEMENTS_COUNT; ++i)
		{
			if (i >= elements.size())
				break;
			if (elements.at(i).nameFrame.getGlobalBounds().contains(pos))
			{
				Engine::Instance().soundManager()->playOnce(SoundManager::HOVER);
				current = i - startValue;
				break;
			}
		}
		toolTip.setString(sf::String());

		next.setColor(sf::Color::White);
		if (next.getGlobalBounds().contains(pos))
		{
			next.setColor(SELECTED_COLOR);
			toolTip.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::NEXT));
			Engine::Instance().soundManager()->playOnce(SoundManager::HOVER);
		}

		previous.setColor(sf::Color::White);
		if (previous.getGlobalBounds().contains(pos))
		{
			previous.setColor(SELECTED_COLOR);
			toolTip.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::PREVIOUS));
			Engine::Instance().soundManager()->playOnce(SoundManager::HOVER);
		}

		credits.setColor(sf::Color::White);
		if (credits.getGlobalBounds().contains(pos))
		{
			credits.setColor(SELECTED_COLOR);
			toolTip.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::CREDITS));
			Engine::Instance().soundManager()->playOnce(SoundManager::HOVER);
		}

		close.setColor(sf::Color::White);
		if (close.getGlobalBounds().contains(pos))
		{
			close.setColor(SELECTED_COLOR);
			toolTip.setString(Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::BACK));
			Engine::Instance().soundManager()->playOnce(SoundManager::HOVER);
		}

		updateCurrentInfo();
	}
	else if (event->type == sf::Event::KeyPressed)
	{
		switch (event->key.code)
		{
		case sf::Keyboard::Up:
		{
			current--;
			if (current < 0)
				current = 0;
			updateCurrentInfo();
		}
			break;
		case sf::Keyboard::Down:
		{
			current++;
			if (current == MAX_ELEMENTS_COUNT)
				current = MAX_ELEMENTS_COUNT - 1;
			updateCurrentInfo();
		}
			break;
		case sf::Keyboard::Right:
			nextPage();
			break;
		case sf::Keyboard::Left:
			previousPage();
			break;
		default:
			break;
		};
	}
	StateWindow::eventFilter(event);
}

void ManualWindow::updatePos()
{
	const sf::Vector2f scaleFactor = Engine::Instance().settingsManager()->getScaleFactor();

	const float TITLE_OFFSET = 12 * scaleFactor.y;
	const float ICON_WIDTH = Engine::Instance().options<GameOptions>()->tileSize().x;
	const float RECT_WIDTH = 464 * Engine::Instance().settingsManager()->getScaleFactor().x;
	const float RECT_HEIGHT = 128 * Engine::Instance().settingsManager()->getScaleFactor().y;

	const float ICON_X_OFFSET = 32 * scaleFactor.x;
	const float ICON_Y_OFFSET = 32 * scaleFactor.y;

	const sf::Vector2f FRAME_OFFSET_SCALED = sf::Vector2f(FRAME_OFFSET * scaleFactor.x,
														  FRAME_OFFSET * scaleFactor.y);
	const sf::Vector2f FRAME_OFFSET_SCALED_X2 = sf::Vector2f(FRAME_OFFSET_SCALED.x * 2,
														  FRAME_OFFSET_SCALED.y * 2);

	const float rectsHeight = 240  * scaleFactor.y;
	const sf::Vector2f texturePos = sf::Vector2f(656 * scaleFactor.x,
												 96 * scaleFactor.y);
	const sf::Vector2f demoFrameSize = sf::Vector2f(FRAME_OFFSET_SCALED.x * 4,
													FRAME_OFFSET_SCALED.y * 4) + sf::Vector2f(320 * scaleFactor.x,
																			 rectsHeight);
	const sf::Vector2f textureSize = FRAME_OFFSET_SCALED_X2 + sf::Vector2f(318 * scaleFactor.x,
																		   rectsHeight);
	const sf::Vector2f demoPos = sf::Vector2f(1042 * scaleFactor.x,
											  96 * scaleFactor.y) - FRAME_OFFSET_SCALED;

	const sf::Vector2f textPos = sf::Vector2f(684 * scaleFactor.x,
											  424 * scaleFactor.y);

	const sf::Vector2f buttonsPos = sf::Vector2f(1600 * scaleFactor.x,
												 96 * scaleFactor.y);

	demoFrame.setSize(demoFrameSize);
	demoFrame.setPosition(demoPos);

	currentRect.setFillColor(SELECTED_COLOR);
	currentRect.setSize(sf::Vector2f(RECT_WIDTH, RECT_HEIGHT));

	previous.setPosition(buttonsPos);
	next.setPosition(buttonsPos + sf::Vector2f(ICON_WIDTH, 0));
	credits.setPosition(buttonsPos + sf::Vector2f(ICON_WIDTH*2, 0));
	close.setPosition(buttonsPos + sf::Vector2f(ICON_WIDTH*3, 0));
	toolTip.setPosition(buttonsPos + sf::Vector2f(0, ICON_Y_OFFSET));

	sf::Vector2f pos = sf::Vector2f(ICON_X_OFFSET * 2, ICON_Y_OFFSET * 2);

	int count = 0;
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		Element& element = elements.at(i);

		element.init();
		element.icon.setScale(scaleFactor);
		element.update();

		element.nameFrame.setPosition(pos);
		element.nameFrame.setSize(sf::Vector2f(RECT_WIDTH, RECT_HEIGHT));

		element.titleText.setScale(scaleFactor);
		element.titleText.setPosition(pos + sf::Vector2f(ICON_X_OFFSET, ICON_Y_OFFSET) +
									  sf::Vector2f(ICON_WIDTH + ICON_X_OFFSET, 0));

		element.icon.setPosition(pos + sf::Vector2f(ICON_X_OFFSET, ICON_Y_OFFSET));



		element.object->setPos(texturePos + sf::Vector2f(textureSize.x/2 - element.object->sprite.getGlobalBounds().width/2,
														 textureSize.y/2 - element.object->sprite.getGlobalBounds().height/2));

		element.nameText.setScale(scaleFactor);
		element.nameText.setPosition(textPos);

		element.descriptionText.setScale(scaleFactor);
		element.descriptionText.setPosition(element.nameText.getPosition() +
											sf::Vector2f(0, element.nameText.getGlobalBounds().height + TITLE_OFFSET));

		if (element.elementDemo != nullptr)
		{
			const float kx = 362.f / 320.f;
			const float ky = 282.f / 240.f;
			element.elementDemo->sprite.scale(kx, ky);
			element.elementDemo->setPos(FRAME_OFFSET_SCALED +
										sf::Vector2f(demoFrame.getGlobalBounds().left,
													 demoFrame.getGlobalBounds().top));
		}

		pos.y += ICON_Y_OFFSET + RECT_HEIGHT;

		count++;
		if (count == MAX_ELEMENTS_COUNT)
		{
			pos = sf::Vector2f(ICON_X_OFFSET * 2, ICON_Y_OFFSET * 2);
			count = 0;
		}
	}
	maxPages = elements.size()/MAX_ELEMENTS_COUNT;
}

void ManualWindow::addElements()
{
	//towers
	elements.push_back(Element(GAME_TEXTURE::TOWER_BASE,
							   GAME_TRANSLATION::TOWER_BASE,
							   BASE,
							   TowersFactory::getFrameCount(BASE),
							   TowersFactory::getAnimationSpeed(BASE),
							   GAME_TEXTURE::SHOW_BASE_TOWER));

	elements.push_back(Element(GAME_TEXTURE::TOWER_POWER,
							   GAME_TRANSLATION::TOWER_POWER,
							   POWER,
							   TowersFactory::getFrameCount(POWER),
							   TowersFactory::getAnimationSpeed(POWER),
							   GAME_TEXTURE::SHOW_FREEZE_TOWER));

	elements.push_back(Element(GAME_TEXTURE::TOWER_LASER,
							   GAME_TRANSLATION::TOWER_LASER,
							   LASER,
							   TowersFactory::getFrameCount(LASER),
							   TowersFactory::getAnimationSpeed(LASER)));

	elements.push_back(Element(GAME_TEXTURE::TOWER_FREEZE,
							   GAME_TRANSLATION::TOWER_FREEZE,
							   FREEZE,
							   TowersFactory::getFrameCount(FREEZE),
							   TowersFactory::getAnimationSpeed(FREEZE)));

	elements.push_back(Element(GAME_TEXTURE::TOWER_ROCKET,
							   GAME_TRANSLATION::TOWER_ROCKET,
							   ROCKET,
							   TowersFactory::getFrameCount(ROCKET),
							   TowersFactory::getAnimationSpeed(ROCKET)));

	elements.push_back(Element(GAME_TEXTURE::TOWER_IMPROVED,
							   GAME_TRANSLATION::TOWER_IMPROVED,
							   IMPROVED,
							   TowersFactory::getFrameCount(IMPROVED),
							   TowersFactory::getAnimationSpeed(IMPROVED)));
	//abilities
	elements.push_back(Element(GAME_TEXTURE::ABILITY_BOMB,
							   GAME_TRANSLATION::ABILITY_BOMB,
							   ACTION_STATE::ABILITY_BOMB,
							   GAME_TEXTURE::SHOW_BOMB_ABILITY));

	elements.push_back(Element(GAME_TEXTURE::ABILITY_FREEZE_BOMB,
							   GAME_TRANSLATION::ABILITY_FREEZE_BOMB,
							   ACTION_STATE::ABILITY_FREEZE_BOMB,
							   GAME_TEXTURE::SHOW_FREEZE_BOMB_ABILITY));

	elements.push_back(Element(GAME_TEXTURE::ABILITY_ACID,
							   GAME_TRANSLATION::ABILITY_ACID,
							   ACTION_STATE::ABILITY_ACID));

	elements.push_back(Element(GAME_TEXTURE::ABILITY_INCREASE_TOWER_DAMAGE,
							   GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE,
							   ACTION_STATE::ABILITY_INCREASE_TOWER_DAMAGE));

	elements.push_back(Element(GAME_TEXTURE::ABILITY_INCREASE_TOWER_ATTACK_SPEED,
							   GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED,
							   ACTION_STATE::ABILITY_INCREASE_TOWER_ATTACK_SPEED));

	elements.push_back(Element(GAME_TEXTURE::ABILITY_TIME_STOP,
							   GAME_TRANSLATION::ABILITY_STOP,
							   ACTION_STATE::ABILITY_STOP));

	//enemies
	elements.push_back(Element(GAME_TEXTURE::ENEMY_INFANTRY,
							   GAME_TRANSLATION::INFANTRY,
							   EnemiesFactory::getEnemyInfo(INFANTRY),
							   EnemiesFactory::getFrameCount(INFANTRY),
							   EnemiesFactory::getAnimationSpeed(INFANTRY)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_WAR_VEHICLE,
							   GAME_TRANSLATION::WAR_VEHICLE,
							   EnemiesFactory::getEnemyInfo(WAR_VEHICLE),
							   EnemiesFactory::getFrameCount(WAR_VEHICLE),
							   EnemiesFactory::getAnimationSpeed(WAR_VEHICLE)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_TRICYCLE,
							   GAME_TRANSLATION::TRICYCLE,
							   EnemiesFactory::getEnemyInfo(TRICYCLE),
							   EnemiesFactory::getFrameCount(TRICYCLE),
							   EnemiesFactory::getAnimationSpeed(TRICYCLE)));
#ifndef DEMO_VERSION
	elements.push_back(Element(GAME_TEXTURE::ENEMY_TANK,
							   GAME_TRANSLATION::TANK,
							   EnemiesFactory::getEnemyInfo(TANK),
							   EnemiesFactory::getFrameCount(TANK),
							   EnemiesFactory::getAnimationSpeed(TANK)));
#endif
	elements.push_back(Element(GAME_TEXTURE::ENEMY_MECHSPIDER,
							   GAME_TRANSLATION::MECHSPIDER,
							   EnemiesFactory::getEnemyInfo(MECHSPIDER),
							   EnemiesFactory::getFrameCount(MECHSPIDER),
							   EnemiesFactory::getAnimationSpeed(MECHSPIDER)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_TUATARA,
							   GAME_TRANSLATION::TUATARA,
							   EnemiesFactory::getEnemyInfo(TUATARA),
							   EnemiesFactory::getFrameCount(TUATARA),
							   EnemiesFactory::getAnimationSpeed(TUATARA)
							   ));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_GIANT_SLUG,
							   GAME_TRANSLATION::GIANT_SLUG,
							   EnemiesFactory::getEnemyInfo(GIANT_SLUG),
							   EnemiesFactory::getFrameCount(GIANT_SLUG),
							   EnemiesFactory::getAnimationSpeed(GIANT_SLUG)));
#ifndef DEMO_VERSION
	elements.push_back(Element(GAME_TEXTURE::ENEMY_HEAVY_TANK,
							   GAME_TRANSLATION::HEAVY_TANK,
							   EnemiesFactory::getEnemyInfo(HEAVY_TANK),
							   EnemiesFactory::getFrameCount(HEAVY_TANK),
							   EnemiesFactory::getAnimationSpeed(HEAVY_TANK)));
#endif
	elements.push_back(Element(GAME_TEXTURE::ENEMY_SLUGGY,
							   GAME_TRANSLATION::SLUGGY,
							   EnemiesFactory::getEnemyInfo(SLUGGY),
							   EnemiesFactory::getFrameCount(SLUGGY),
							   EnemiesFactory::getAnimationSpeed(SLUGGY)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_REPAIR,
							   GAME_TRANSLATION::REPAIR,
							   EnemiesFactory::getEnemyInfo(REPAIR_ENEMY),
							   EnemiesFactory::getFrameCount(REPAIR_ENEMY),
							   EnemiesFactory::getAnimationSpeed(REPAIR_ENEMY)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_SHIELD,
							   GAME_TRANSLATION::SHELL,
							   EnemiesFactory::getEnemyInfo(SHIELD_ENEMY),
							   EnemiesFactory::getFrameCount(SHIELD_ENEMY),
							   EnemiesFactory::getAnimationSpeed(SHIELD_ENEMY)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_WALKER,
							   GAME_TRANSLATION::WALKER,
							   EnemiesFactory::getEnemyInfo(WALKER),
							   EnemiesFactory::getFrameCount(WALKER),
							   EnemiesFactory::getAnimationSpeed(WALKER)));
#ifndef DEMO_VERSION
	elements.push_back(Element(GAME_TEXTURE::ENEMY_SELF_HEAL,
							   GAME_TRANSLATION::SELF_HEAL,
							   EnemiesFactory::getEnemyInfo(SELFHEAL_ENEMY),
							   EnemiesFactory::getFrameCount(SELFHEAL_ENEMY),
							   EnemiesFactory::getAnimationSpeed(SELFHEAL_ENEMY)));
#endif
	elements.push_back(Element(GAME_TEXTURE::ENEMY_TRACTOR,
							   GAME_TRANSLATION::TRACTOR,
							   EnemiesFactory::getEnemyInfo(TRACTOR),
							   EnemiesFactory::getFrameCount(TRACTOR),
							   EnemiesFactory::getAnimationSpeed(TRACTOR)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_WORM,
							   GAME_TRANSLATION::WORM,
							   EnemiesFactory::getEnemyInfo(WORM),
							   EnemiesFactory::getFrameCount(WORM),
							   EnemiesFactory::getAnimationSpeed(WORM)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_BUGSAURUS,
							   GAME_TRANSLATION::BUGSAURUS,
							   EnemiesFactory::getEnemyInfo(BUGSAURUS),
							   EnemiesFactory::getFrameCount(BUGSAURUS),
							   EnemiesFactory::getAnimationSpeed(BUGSAURUS)));
#ifndef DEMO_VERSION
	//map effects
	elements.push_back(Element(GAME_TEXTURE::SMOKE,
							   GAME_TRANSLATION::SMOKE_NAME,
							   Instructions::SMOKE,
							   1,
							   100));

	elements.push_back(Element(GAME_TEXTURE::REGRESS,
							   GAME_TRANSLATION::REGRESS_NAME,
							   Instructions::REGRESS,
							   4,
							   200));

	elements.push_back(Element(GAME_TEXTURE::DRAIN,
							   GAME_TRANSLATION::DRAIN_NAME,
							   Instructions::DRAIN,
							   4,
							   200));

	elements.push_back(Element(GAME_TEXTURE::TOWER_EXPLOSION,
							   GAME_TRANSLATION::TOWER_EXPLOSION_NAME,
							   Instructions::EXPLOSION,
							   4,
							   200));
#endif
}


void ManualWindow::updateCurrentInfo()
{
	if (current == -1)
		return;

	const int id = page * MAX_ELEMENTS_COUNT + current;
	if (id >= elements.size())
		return;
	const Element element = elements.at(id);
	currentRect.setPosition(element.nameFrame.getPosition());
}

void ManualWindow::nextPage()
{
	page++;
	if (page >= maxPages)
		page = maxPages;
	current = 0;
	updateCurrentInfo();
}

void ManualWindow::previousPage()
{
	page--;
	if (page < 0)
		page = 0;
	current = MAX_ELEMENTS_COUNT - 1;
	updateCurrentInfo();
}

ManualWindow::Element::Element(TextureType texture,
							   TranslationType name,
							   const EnemiesFactory::EnemyInfo& info,
							   int frameCount,
							   float animationSpeed,
							   TextureType demoTextureType)
	: texture(texture)
	,name(name)
	,type(E_Enemy)
	,enemyInfo(info)
	,frameCount(frameCount)
	,animationSpeed(animationSpeed)
	,demoTextureType(demoTextureType)
{

}

ManualWindow::Element::Element(TextureType texture,
							   TranslationType name,
							   ACTION_STATE type,
							   TextureType demoTextureType)
	: texture(texture)
	,name(name)
	,type(E_Ability)
	,abilityType(type)
	,frameCount(1)
	,animationSpeed(100)
	,demoTextureType(demoTextureType)
{

}

ManualWindow::Element::Element(TextureType texture,
							   TranslationType name,
							   TOWER_TYPES towerType,
							   int frameCount,
							   float animationSpeed,
							   TextureType demoTextureType)
	: texture(texture)
	,name(name)
	,type(E_Tower)
	,towerType(towerType)
	,frameCount(frameCount)
	,animationSpeed(animationSpeed)
	,demoTextureType(demoTextureType)
{

}

ManualWindow::Element::Element(TextureType texture,
							   TranslationType name,
							   Instructions::MAP_EFFECTS type,
							   int frameCount,
							   float animationSpeed,
							   TextureType demoTextureType)
	: texture(texture)
	,name(name)
	,type(E_Effect)
	,effectType(type)
	,frameCount(frameCount)
	,animationSpeed(animationSpeed)
	,demoTextureType(demoTextureType)
{

}

void ManualWindow::Element::init()
{
//	nameFrame.setFillColor(sf::Color(34, 53, 200, 100));
	nameFrame.setTexture(&Engine::Instance().texturesManager()->getTexture(GAME_TEXTURE::NAME_FRAME));

	titleText.setFont(Engine::Instance().fontManager()->font());
	titleText.setFillColor(GameOptions::primaryColor);
	titleText.setOutlineThickness(1);
	titleText.setOutlineColor(GameOptions::secondaryColor);
	titleText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(30));

	nameText.setFont(Engine::Instance().fontManager()->font());
	nameText.setFillColor(GameOptions::alternativePrimaryColor);
	nameText.setOutlineThickness(1);
	nameText.setOutlineColor(GameOptions::alternativeSecondaryColor);
	nameText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(45));

	descriptionText.setFont(Engine::Instance().fontManager()->font());
	descriptionText.setFillColor(GameOptions::primaryColor);
	descriptionText.setOutlineThickness(1);
	descriptionText.setOutlineColor(GameOptions::secondaryColor);
	descriptionText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(34));
}

void ManualWindow::Element::update()
{
	bool cycled = false;
	int rowCount = 1;
	float objectScale = 1;
	sf::Vector2i frameSize;	
	TextureType objectTexture = texture;
	//texture
	icon.setTexture(Engine::Instance().texturesManager()->getTexture(texture));
	//name
	std::wstring titleStr = Engine::Instance().translationsManager()->translate(name).toWideString();
	titleText.setString(titleStr);
	//description
	sf::String descriptionStr;
	switch (type)
	{
	case Element::E_Tower:
	{
		rowCount = 4;
		cycled = true;
		frameSize = sf::Vector2i(192, 192);
		icon.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);
		icon.setTextureRect(sf::IntRect(0, frameSize.y, frameSize.x, frameSize.y));
		descriptionStr += Instructions::towerInfoText(towerType);
	}
		break;
	case Element::E_Ability:
	{		
		frameSize = sf::Vector2i(GameOptions::CELL_SIZE, GameOptions::CELL_SIZE);		
		descriptionStr += Instructions::abilityInfoText(abilityType);
		switch (abilityType)
		{
		case ABILITY_BOMB:
		{
			animationSpeed = 25;
			frameCount = 8;
			frameSize = sf::Vector2i(3 * GameOptions::CELL_SIZE,
									 3 * GameOptions::CELL_SIZE);
			objectTexture = GAME_TEXTURE::BOMB_EXPLOSION;
		}
			break;
		case ABILITY_FREEZE_BOMB:
		{
			animationSpeed = 25;
			frameCount = 8;
			frameSize = sf::Vector2i(3 * GameOptions::CELL_SIZE,
									 3 * GameOptions::CELL_SIZE);
			objectTexture = GAME_TEXTURE::FREEZE_BOMB_EXPLOSION;
		}
			break;
		case ABILITY_ACID:
			objectTexture = GAME_TEXTURE::ABILITY_ACID;
			break;
		case ABILITY_INCREASE_TOWER_ATTACK_SPEED:

			break;
		case ABILITY_INCREASE_TOWER_DAMAGE:

			break;
		case ABILITY_STOP:

			break;
		default:
			break;
		}
	}
		break;
	case Element::E_Enemy:
	{
		frameSize = sf::Vector2i(Enemy::ENEMY_SCALE * enemyInfo.size.x * GameOptions::MAP_CELL_SIZE,
								 Enemy::ENEMY_SCALE * enemyInfo.size.y * GameOptions::MAP_CELL_SIZE);
		icon.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y));

		const sf::Vector2f k = sf::Vector2f(2.f/static_cast<float>(enemyInfo.size.x),
											2.f/static_cast<float>(enemyInfo.size.y));
		icon.scale(k.x/Enemy::ENEMY_SCALE, k.y/Enemy::ENEMY_SCALE);
		if (enemyInfo.size.x == 4)
			objectScale = 0.5f;

		//speed
		descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::SPEED) + EngineDefs::separator;
		if (enemyInfo.stats.speed < 30)
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::FAST);
		else if (enemyInfo.stats.speed < 65)
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::NORMAL_SPEED);
		else
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::SLOW);
		descriptionStr += EngineDefs::endline;
		//size
		descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::SIZE) + EngineDefs::separator;
		if (enemyInfo.size.x == 1)
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::SMALL);
		else if (enemyInfo.size.x == 2)
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::MID);
		else
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::BIG);
		descriptionStr += EngineDefs::endline;
		//health
		descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::HEALTH) + EngineDefs::separator +
				GlobalVariables::to_string_with_precision(enemyInfo.stats.health, 1);
		descriptionStr += EngineDefs::endline;

		//armor
		descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ARMOR) + EngineDefs::separator;
		if (enemyInfo.stats.reflection < 0.1f)
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ARMOR_LIGHT);
		else if (enemyInfo.stats.reflection < 0.3f)
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ARMOR_MEDIUM);
		else
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ARMOR_STRONG);
		descriptionStr += EngineDefs::endline;

		//ability
		descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY) + EngineDefs::separator;
		descriptionStr += EngineDefs::endline;

		switch (enemyInfo.abilityType)
		{
		case EnemiesFactory::EnemyInfo::RAGE:
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_RAGE);
			break;
		case EnemiesFactory::EnemyInfo::SPAWN:
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN);
			break;
		case EnemiesFactory::EnemyInfo::TELEPORT:
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT);
			break;
		case EnemiesFactory::EnemyInfo::SHELL_NEAR:
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR);
			break;
		case EnemiesFactory::EnemyInfo::HEAL_NEAR:
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR);
			break;
		case EnemiesFactory::EnemyInfo::SHUTDOWN_TOWER:
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER);
			break;
		case EnemiesFactory::EnemyInfo::STRONG:
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_STRONG);
			break;
		case EnemiesFactory::EnemyInfo::DOWN_TOWER:
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER);
			break;
		case EnemiesFactory::EnemyInfo::SELF_HEAL:
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL);
			break;
		case EnemiesFactory::EnemyInfo::FASTER:
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_FASTER);
			break;
		case EnemiesFactory::EnemyInfo::KILL_TOWER:
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER);
			break;
		case EnemiesFactory::EnemyInfo::DOWNGRADE_TOWER:
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER);
			break;
		default:
			descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_NONE);
			break;
		}
	}
		break;
	case Element::E_Effect:
	{
		frameSize = sf::Vector2i(GameOptions::CELL_SIZE, GameOptions::CELL_SIZE);
		switch (effectType)
		{
		case Instructions::SMOKE:
			frameSize = sf::Vector2i(256, 256);
			rowCount = 3;
			break;
		case Instructions::REGRESS:
			rowCount = 3;
			break;
		case Instructions::DRAIN:
			rowCount = 2;
			break;
		case Instructions::EXPLOSION:
			rowCount = 1;
			break;
		default:
			break;
		}
		icon.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y));
		if (effectType == Instructions::SMOKE)
			icon.scale(0.25f, 0.25f);
		cycled = true;
		descriptionStr += Instructions::mapEffectInfoText(effectType);
	}
		break;
	default:
		break;
	}

	object = new GameObject(objectTexture, sf::Vector2f(0,0), frameSize, frameCount);
	object->animationSpeed = animationSpeed;
	object->cycled = cycled;
	object->rowCount = rowCount;
	object->sprite.scale(objectScale, objectScale);

	if (demoTextureType == -1)
		elementDemo = nullptr;
	else
	{
		elementDemo = new GameObject(demoTextureType, sf::Vector2f(0,0), sf::Vector2i(320, 240), 10);
		elementDemo->animationSpeed = 50;
		elementDemo->cycled = true;
		elementDemo->rowCount = 5;
	}
	std::replace(titleStr.begin(), titleStr.end(), '\n', ' ');
	nameText.setString(titleStr);
	descriptionText.setString(descriptionStr);
}
