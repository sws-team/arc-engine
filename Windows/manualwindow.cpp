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
//			elements.at(id).elementDemo->update();
		}
	}
	StateWindow::update();
}

void ManualWindow::paint(sf::RenderWindow *window)
{
	drawBackground(window);
	window->draw(infoRect);
	const int startValue = MAX_ELEMENTS_COUNT * page;
	for (unsigned int i = startValue; i < startValue + MAX_ELEMENTS_COUNT; ++i)
	{
		if (i >= elements.size())
			break;
		const Element element = elements.at(i);
		window->draw(element.rect);
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
//			elements.at(id).elementDemo->draw(window);
		}
	}
	window->draw(previous);
	window->draw(next);
	window->draw(credits);
	window->draw(close);
	window->draw(toolTip);
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
			if (elements.at(i).rect.getGlobalBounds().contains(pos))
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
	const float ICON_WIDTH = Engine::Instance().options<GameOptions>()->tileSize().x;
	const float RECT_WIDTH = 512 * Engine::Instance().settingsManager()->getScaleFactor().x;
	const float RECT_HEIGHT = 128 * Engine::Instance().settingsManager()->getScaleFactor().y;
	const float ICON_X_OFFSET = 32 * Engine::Instance().settingsManager()->getScaleFactor().x;
	const float ICON_Y_OFFSET = 32 * Engine::Instance().settingsManager()->getScaleFactor().y;

	currentRect.setFillColor(SELECTED_COLOR);
	currentRect.setSize(sf::Vector2f(RECT_WIDTH, RECT_HEIGHT));
	currentRect.setOutlineThickness(1);
	currentRect.setOutlineColor(sf::Color::Black);

	infoRect.setFillColor(sf::Color(45, 45, 45, 128));
	infoRect.setSize(sf::Vector2f(RECT_WIDTH * 2,
							  RECT_HEIGHT * MAX_ELEMENTS_COUNT + ICON_Y_OFFSET * (MAX_ELEMENTS_COUNT - 1)));
	infoRect.setOutlineThickness(1);
	infoRect.setOutlineColor(sf::Color::Black);
	infoRect.setPosition(ICON_X_OFFSET * 3 + RECT_WIDTH, ICON_Y_OFFSET * 2);

	previous.setPosition(sf::Vector2f(infoRect.getGlobalBounds().left + infoRect.getGlobalBounds().width,
							  infoRect.getGlobalBounds().top) + sf::Vector2f(ICON_X_OFFSET, 0));

	next.setPosition(sf::Vector2f(infoRect.getGlobalBounds().left + infoRect.getGlobalBounds().width,
							  infoRect.getGlobalBounds().top) + sf::Vector2f(ICON_X_OFFSET + ICON_WIDTH, 0));

	credits.setPosition(sf::Vector2f(infoRect.getGlobalBounds().left + infoRect.getGlobalBounds().width,
							   infoRect.getGlobalBounds().top) + sf::Vector2f(ICON_X_OFFSET + ICON_WIDTH*2, 0));

	close.setPosition(sf::Vector2f(infoRect.getGlobalBounds().left + infoRect.getGlobalBounds().width,
							   infoRect.getGlobalBounds().top) + sf::Vector2f(ICON_X_OFFSET + ICON_WIDTH*3, 0));

	toolTip.setPosition(sf::Vector2f(infoRect.getGlobalBounds().left + infoRect.getGlobalBounds().width,
								 infoRect.getGlobalBounds().top) + sf::Vector2f(ICON_X_OFFSET, ICON_Y_OFFSET));


	const sf::Vector2f scaleFactor = Engine::Instance().settingsManager()->getScaleFactor();
	sf::Vector2f pos = sf::Vector2f(ICON_X_OFFSET * 2, ICON_Y_OFFSET * 2);

	int count = 0;
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		Element& element = elements.at(i);

		element.init();
		element.icon.setScale(scaleFactor);
		element.update();

		element.rect.setPosition(pos);
		element.rect.setSize(sf::Vector2f(RECT_WIDTH, RECT_HEIGHT));

		element.icon.setPosition(pos + sf::Vector2f(ICON_X_OFFSET, ICON_Y_OFFSET));

		element.object->setPos(sf::Vector2f(infoRect.getGlobalBounds().left,
													 infoRect.getGlobalBounds().top) +
											sf::Vector2f(ICON_X_OFFSET, ICON_Y_OFFSET));

		element.nameText.setScale(scaleFactor);
		element.nameText.setPosition(element.object->pos() +
									 sf::Vector2f(element.object->getSize().x + ICON_X_OFFSET, 0));

		element.descriptionText.setScale(scaleFactor);
		element.descriptionText.setPosition(element.nameText.getPosition() +
											sf::Vector2f(0, element.nameText.getGlobalBounds().height + ICON_Y_OFFSET));

		element.titleText.setScale(scaleFactor);
		element.titleText.setPosition(pos + sf::Vector2f(ICON_X_OFFSET, ICON_Y_OFFSET) +
									 sf::Vector2f(ICON_WIDTH + ICON_X_OFFSET, 0));

//		element.elementDemo->setPos(sf::Vector2f(infoRect.getGlobalBounds().left,
//												 element.descriptionText.getGlobalBounds().top +
//												 element.descriptionText.getGlobalBounds().height));

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
							   TowersFactory::getAnimationSpeed(BASE)));

	elements.push_back(Element(GAME_TEXTURE::TOWER_POWER,
							   GAME_TRANSLATION::TOWER_POWER,
							   POWER,
							   TowersFactory::getFrameCount(POWER),
							   TowersFactory::getAnimationSpeed(POWER)));

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
							   ACTION_STATE::ABILITY_BOMB));

	elements.push_back(Element(GAME_TEXTURE::ABILITY_FREEZE_BOMB,
							   GAME_TRANSLATION::ABILITY_FREEZE_BOMB,
							   ACTION_STATE::ABILITY_FREEZE_BOMB));

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

	elements.push_back(Element(GAME_TEXTURE::ENEMY_TANK,
							   GAME_TRANSLATION::TANK,
							   EnemiesFactory::getEnemyInfo(TANK),
							   EnemiesFactory::getFrameCount(TANK),
							   EnemiesFactory::getAnimationSpeed(TANK)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_SPIDER,
							   GAME_TRANSLATION::SPIDER,
							   EnemiesFactory::getEnemyInfo(SPIDER),
							   EnemiesFactory::getFrameCount(SPIDER),
							   EnemiesFactory::getAnimationSpeed(SPIDER)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_HELICOPTER,
							   GAME_TRANSLATION::HELICOPTER,
							   EnemiesFactory::getEnemyInfo(MID_FAST),
							   EnemiesFactory::getFrameCount(MID_FAST),
							   EnemiesFactory::getAnimationSpeed(MID_FAST)
							   ));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_AIRCARRIER,
							   GAME_TRANSLATION::AIRCARRIER,
							   EnemiesFactory::getEnemyInfo(BIG_SLOW),
							   EnemiesFactory::getFrameCount(BIG_SLOW),
							   EnemiesFactory::getAnimationSpeed(BIG_SLOW)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_BIG_TANK,
							   GAME_TRANSLATION::BIG_TANK,
							   EnemiesFactory::getEnemyInfo(BIG_TANK),
							   EnemiesFactory::getFrameCount(BIG_TANK),
							   EnemiesFactory::getAnimationSpeed(BIG_TANK)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_PLANE,
							   GAME_TRANSLATION::PLANE,
							   EnemiesFactory::getEnemyInfo(ANOTHER_ENEMY),
							   EnemiesFactory::getFrameCount(ANOTHER_ENEMY),
							   EnemiesFactory::getAnimationSpeed(ANOTHER_ENEMY)));

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

	elements.push_back(Element(GAME_TEXTURE::ENEMY_TELEPORT,
							   GAME_TRANSLATION::TELEPORT,
							   EnemiesFactory::getEnemyInfo(TELEPORT_ENEMY),
							   EnemiesFactory::getFrameCount(TELEPORT_ENEMY),
							   EnemiesFactory::getAnimationSpeed(TELEPORT_ENEMY)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_SELF_HEAL,
							   GAME_TRANSLATION::SELF_HEAL,
							   EnemiesFactory::getEnemyInfo(SELFHEAL_ENEMY),
							   EnemiesFactory::getFrameCount(SELFHEAL_ENEMY),
							   EnemiesFactory::getAnimationSpeed(SELFHEAL_ENEMY)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_TRACTOR,
							   GAME_TRANSLATION::DOWN_TOWER,
							   EnemiesFactory::getEnemyInfo(TRACTOR),
							   EnemiesFactory::getFrameCount(TRACTOR),
							   EnemiesFactory::getAnimationSpeed(TRACTOR)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_WORM,
							   GAME_TRANSLATION::WORM,
							   EnemiesFactory::getEnemyInfo(WORM),
							   EnemiesFactory::getFrameCount(WORM),
							   EnemiesFactory::getAnimationSpeed(WORM)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_COW,
							   GAME_TRANSLATION::COW,
							   EnemiesFactory::getEnemyInfo(SPAWN_ENEMY),
							   EnemiesFactory::getFrameCount(SPAWN_ENEMY),
							   EnemiesFactory::getAnimationSpeed(SPAWN_ENEMY)));

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
}


void ManualWindow::updateCurrentInfo()
{
	if (current == -1)
		return;

	const int id = page * MAX_ELEMENTS_COUNT + current;
	if (id >= elements.size())
		return;
	const Element element = elements.at(id);
	currentRect.setPosition(element.rect.getPosition());
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
							   float animationSpeed)
	: texture(texture)
	,name(name)
	,type(E_Enemy)
	,enemyInfo(info)
	,frameCount(frameCount)
	,animationSpeed(animationSpeed)
{

}

ManualWindow::Element::Element(TextureType texture,
							   TranslationType name,
							   ACTION_STATE type)
	: texture(texture)
	,name(name)
	,type(E_Ability)
	,abilityType(type)
	,frameCount(1)
	,animationSpeed(100)
{

}

ManualWindow::Element::Element(TextureType texture,
							   TranslationType name,
							   TOWER_TYPES towerType,
							   int frameCount,
							   float animationSpeed)
	: texture(texture)
	,name(name)
	,type(E_Tower)
	,towerType(towerType)
	,frameCount(frameCount)
	,animationSpeed(animationSpeed)
{

}

ManualWindow::Element::Element(TextureType texture,
							   TranslationType name,
							   Instructions::MAP_EFFECTS type,
							   int frameCount,
							   float animationSpeed)
	: texture(texture)
	,name(name)
	,type(E_Effect)
	,effectType(type)
	,frameCount(frameCount)
	,animationSpeed(animationSpeed)
{

}

void ManualWindow::Element::init()
{
	rect.setFillColor(sf::Color(34, 53, 200, 100));

	titleText.setFont(Engine::Instance().fontManager()->font());
	titleText.setFillColor(GameOptions::primaryColor);
	titleText.setOutlineThickness(1);
	titleText.setOutlineColor(GameOptions::secondaryColor);
	titleText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(30));

	nameText.setFont(Engine::Instance().fontManager()->font());
	nameText.setFillColor(GameOptions::primaryColor);
	nameText.setOutlineThickness(1);
	nameText.setOutlineColor(GameOptions::secondaryColor);
	nameText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(45));

	descriptionText.setFont(Engine::Instance().fontManager()->font());
	descriptionText.setFillColor(GameOptions::primaryColor);
	descriptionText.setOutlineThickness(1);
	descriptionText.setOutlineColor(GameOptions::secondaryColor);
	descriptionText.setCharacterSize(Engine::Instance().fontManager()->getCharSize(35));
}

void ManualWindow::Element::update()
{
	bool cycled = false;
	int rowCount = 1;
	sf::Vector2i frameSize;
	//texture
	icon.setTexture(Engine::Instance().texturesManager()->getTexture(texture));
	//name
	titleText.setString(Engine::Instance().translationsManager()->translate(name));
	//description
	sf::String descriptionStr;
	const sf::String nameStr = Engine::Instance().translationsManager()->translate(name);
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
		descriptionStr += EngineDefs::endline;

		const TowerStats towerStats = Balance::Instance().getTowerStats(towerType);
		const float dps = towerStats.damage / ((towerStats.attackSpeed) * 0.001f);

		descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DAMAGE_PER_SECOND) + EngineDefs::separator + GlobalVariables::to_string_with_precision(dps, 2);
		descriptionStr += EngineDefs::endline;

		descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::RADIUS) + EngineDefs::separator +
				GlobalVariables::to_string_with_precision(towerStats.radius, 1);
		descriptionStr += EngineDefs::endline;

		const float cost = towerStats.cost;

		descriptionStr += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COST);
		descriptionStr += EngineDefs::separator;
		descriptionStr += GlobalVariables::to_string_with_precision(cost, 2);
	}
		break;
	case Element::E_Ability:
	{		
		frameSize = sf::Vector2i(GameOptions::CELL_SIZE, GameOptions::CELL_SIZE);		
		descriptionStr += Instructions::abilityInfoText(abilityType);
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

	object = new GameObject(texture, sf::Vector2f(0,0), frameSize, frameCount);
	object->animationSpeed = animationSpeed;
	object->cycled = cycled;
	object->rowCount = rowCount;

//	elementDemo = new GameObject(GAME_TEXTURE::TEST_TEXTTURE, sf::Vector2f(0,0), sf::Vector2i(320, 240), 10);
//	elementDemo->animationSpeed = 50;
//	elementDemo->cycled = true;
//	elementDemo->rowCount = 17;

	nameText.setString(nameStr);
	descriptionText.setString(descriptionStr);
}
