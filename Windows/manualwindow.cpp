#include "manualwindow.h"
#include "Game/tower.h"
#include "Game/enemy.h"
#include "managers.h"
#include "Game/leveldef.h"
#include "gamemanagers.h"
#include "gamestatemanager.h"
#include "gameoptions.h"
#include "Game/gameability.h"

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
	toolTip.setCharacterSize(40);
	toolTip.setOutlineThickness(1.f);
	toolTip.setFillColor(sf::Color::Red);
	toolTip.setOutlineColor(sf::Color::Blue);

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
			elements.at(id).object->update();
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
		window->draw(element.nameText);
	}
	if (current != -1)
	{
		window->draw(currentRect);

		const int id = page * MAX_ELEMENTS_COUNT + current;
		if (id < elements.size())
		{
			window->draw(elements.at(id).descriptionText);
			elements.at(id).object->draw(window);
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
		const sf::Vector2f pos = sf::Vector2f(event->mouseButton.x, event->mouseButton.y);

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
		const sf::Vector2f pos = sf::Vector2f(event->mouseMove.x, event->mouseMove.y);
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


		element.descriptionText.setScale(scaleFactor);
		element.descriptionText.setPosition(element.object->pos() +
											sf::Vector2f(element.object->getSize().x + ICON_X_OFFSET, 0));

		element.nameText.setScale(scaleFactor);
		element.nameText.setPosition(pos + sf::Vector2f(ICON_X_OFFSET, ICON_Y_OFFSET) +
									 sf::Vector2f(ICON_WIDTH + ICON_X_OFFSET, 0));

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
							   BASE));

	elements.push_back(Element(GAME_TEXTURE::TOWER_POWER,
							   GAME_TRANSLATION::TOWER_POWER,
							   POWER));

	elements.push_back(Element(GAME_TEXTURE::TOWER_LASER,
							   GAME_TRANSLATION::TOWER_LASER,
							   LASER));

	elements.push_back(Element(GAME_TEXTURE::TOWER_FREEZE,
							   GAME_TRANSLATION::TOWER_FREEZE,
							   FREEZE));

	elements.push_back(Element(GAME_TEXTURE::TOWER_ROCKET,
							   GAME_TRANSLATION::TOWER_ROCKET,
							   ROCKET));

	elements.push_back(Element(GAME_TEXTURE::TOWER_IMPROVED,
							   GAME_TRANSLATION::TOWER_IMPROVED,
							   IMPROVED));
	//abilities
	elements.push_back(Element(GAME_TEXTURE::ABILITY_BOMB,
							   GAME_TRANSLATION::ABILITY_BOMB,
							   ACTION_STATE::ABILITY_BOMB));

	elements.push_back(Element(GAME_TEXTURE::ABILITY_FREEZE_BOMB,
							   GAME_TRANSLATION::ABILITY_FREEZE_BOMB,
							   ACTION_STATE::ABILITY_FREEZE_BOMB));

	elements.push_back(Element(GAME_TEXTURE::ABILITY_ACID,
							   GAME_TRANSLATION::ABILITY_ACID,
							   ACTION_STATE::ABILITY_VENOM));

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
							   EnemiesFactory::getEnemyInfo(INFANTRY)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_CAR,
							   GAME_TRANSLATION::CAR,
							   EnemiesFactory::getEnemyInfo(CAR)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_TRICYCLE,
							   GAME_TRANSLATION::TRICYCLE,
							   EnemiesFactory::getEnemyInfo(TRICYCLE)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_TANK,
							   GAME_TRANSLATION::TANK,
							   EnemiesFactory::getEnemyInfo(TANK)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_SPIDER,
							   GAME_TRANSLATION::SPIDER,
							   EnemiesFactory::getEnemyInfo(SPIDER)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_HELICOPTER,
							   GAME_TRANSLATION::HELICOPTER,
							   EnemiesFactory::getEnemyInfo(MID_FAST)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_AIRCARRIER,
							   GAME_TRANSLATION::AIRCARRIER,
							   EnemiesFactory::getEnemyInfo(BIG_SLOW)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_BIG_TANK,
							   GAME_TRANSLATION::BIG_TANK,
							   EnemiesFactory::getEnemyInfo(BIG_TANK)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_PLANE,
							   GAME_TRANSLATION::PLANE,
							   EnemiesFactory::getEnemyInfo(ANOTHER_ENEMY)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_REPAIR,
							   GAME_TRANSLATION::REPAIR,
							   EnemiesFactory::getEnemyInfo(REPAIR_ENEMY)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_SHIELD,
							   GAME_TRANSLATION::SHELL,
							   EnemiesFactory::getEnemyInfo(SHIELD_ENEMY)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_TELEPORT,
							   GAME_TRANSLATION::TELEPORT,
							   EnemiesFactory::getEnemyInfo(TELEPORT_ENEMY)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_SELF_HEAL,
							   GAME_TRANSLATION::SELF_HEAL,
							   EnemiesFactory::getEnemyInfo(SELFHEAL_ENEMY)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_TRACTOR,
							   GAME_TRANSLATION::DOWN_TOWER,
							   EnemiesFactory::getEnemyInfo(TRACTOR)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_ANT,
							   GAME_TRANSLATION::ANT,
							   EnemiesFactory::getEnemyInfo(SMALL_NEXT)));

	elements.push_back(Element(GAME_TEXTURE::ENEMY_COW,
							   GAME_TRANSLATION::COW,
							   EnemiesFactory::getEnemyInfo(SPAWN_ENEMY)));
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
							   const EnemiesFactory::EnemyInfo& info)
	: 	texture(texture)
	,name(name)
	,type(E_Enemy)
	,enemyInfo(info)
{

}

ManualWindow::Element::Element(TextureType texture,
							   TranslationType name,
							   ACTION_STATE type)
	: 	texture(texture)
	,name(name)
	,type(E_Ability)
	,abilityType(type)
{

}

ManualWindow::Element::Element(TextureType texture,
							   TranslationType name,
							   TOWER_TYPES towerType)
	: 	texture(texture)
	,name(name)
	,type(E_Tower)
	,towerType(towerType)
{

}

void ManualWindow::Element::init()
{
	rect.setFillColor(sf::Color(34, 53, 200, 100));

	nameText.setFont(Engine::Instance().fontManager()->font());
	nameText.setFillColor(sf::Color::Red);
	nameText.setCharacterSize(36);

	descriptionText.setFont(Engine::Instance().fontManager()->font());
	descriptionText.setFillColor(sf::Color::Red);
	descriptionText.setCharacterSize(36);
}

void ManualWindow::Element::update()
{
	int frameCount = 1;
	sf::Vector2i frameSize;
	//texture
	icon.setTexture(Engine::Instance().texturesManager()->getTexture(texture));
	//name
	nameText.setString(Engine::Instance().translationsManager()->translate(name));
	//description
	sf::String description;
	const sf::String separator = ": ";
	const sf::String endline = "\n";
	switch (type)
	{
	case Element::E_Tower:
	{
		frameSize = sf::Vector2i(192, 192);
		icon.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);

		switch (towerType)
		{
		case BASE:
		{
			description = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_BASE);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY);
			description += separator;
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::BASE_TOWER_ABILITY);
		}
			break;
		case POWER:
		{
			description = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_POWER);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::POWER_TOWER_DESCRIPTION);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::POWER_TOWER_TRAIT);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY);
			description += separator;
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::POWER_TOWER_ABILITY);
		}
			break;
		case ROCKET:
		{
			description = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_ROCKET);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY);
			description += separator;
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ROCKET_TOWER_ABILITY);
		}
			break;
		case FREEZE:
		{
			description = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_FREEZE);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY);
			description += separator;
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::FREEZE_TOWER_ABILITY);
		}
			break;
		case LASER:
		{
			description = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_LASER);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY);
			description += separator;
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::LASER_TOWER_ABILITY);
		}
			break;
		case IMPROVED:
		{
			description = Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::TOWER_IMPROVED);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY);
			description += separator;
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY);
		}
			break;
		}

		description += endline;

		const TowerStats towerStats = TowersFactory::getTowerStats(towerType);
		const float dps = towerStats.damage / ((towerStats.attackSpeed) * 0.001f);

		description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DAMAGE_PER_SECOND) + separator + GlobalVariables::to_string_with_precision(dps, 2);
		description += endline;

		description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::RADIUS) + separator +
				GlobalVariables::to_string_with_precision(towerStats.radius, 1);
		description += endline;

		const float cost = towerStats.cost;

		description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COST);
		description += separator;
		description += GlobalVariables::to_string_with_precision(cost, 2);
	}
		break;
	case Element::E_Ability:
	{		
		frameSize = sf::Vector2i(GameOptions::CELL_SIZE, GameOptions::CELL_SIZE);
		description += Engine::Instance().translationsManager()->translate(name);
		description += endline;
		switch (abilityType)
		{
		case ACTION_STATE::ABILITY_BOMB:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COOLDOWN_TIME);
			description += separator;
			description += std::to_string(BombAbility::BOMB_ABILITY_COOLDOWN/EngineDefs::MSEC);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DAMAGE_ATTRIBUTE);
			description += separator;
			description += std::to_string(BombAbility::BOMB_ABILITY_DAMAGE);
			break;
		case ACTION_STATE::ABILITY_FREEZE_BOMB:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COOLDOWN_TIME);
			description += separator;
			description += std::to_string(FreezeBombAbility::FREEZE_ABILITY_COOLDOWN/EngineDefs::MSEC);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DURATION_ATTRIBUTE);
			description += separator;
			description += std::to_string(FreezeBombAbility::FREEZE_ABILITY_DURATION/EngineDefs::MSEC);
			break;
		case ACTION_STATE::ABILITY_VENOM:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COOLDOWN_TIME);
			description += separator;
			description += std::to_string(VenomAbility::VENOM_ABLITY_COOLDOWN/EngineDefs::MSEC);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DAMAGE_ATTRIBUTE);
			description += separator;
			description += std::to_string(VenomAbility::VENOM_DAMAGE_COUNT * VenomAbility::VENOM_DAMAGE);
			break;
		case ACTION_STATE::ABILITY_INCREASE_TOWER_DAMAGE:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COOLDOWN_TIME);
			description += separator;
			description += std::to_string(IncreaseTowerDamageAbility::INCREASE_DAMAGE_ABILITY_COOLDOWN/EngineDefs::MSEC);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DURATION_ATTRIBUTE);
			description += separator;
			description += std::to_string(IncreaseTowerDamageAbility::INCREASE_DAMAGE_ABILITY_DURATION/EngineDefs::MSEC);
			break;
		case ACTION_STATE::ABILITY_INCREASE_TOWER_ATTACK_SPEED:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COOLDOWN_TIME);
			description += separator;
			description += std::to_string(IncreaseTowerAttackSpeedAbility::INCREASE_ATTACK_SPEED_ABILITY_COOLDOWN/EngineDefs::MSEC);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DURATION_ATTRIBUTE);
			description += separator;
			description += std::to_string(IncreaseTowerAttackSpeedAbility::INCREASE_ATTACK_SPEED_ABILITY_DURATION/EngineDefs::MSEC);
			break;
		case ACTION_STATE::ABILITY_STOP:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::COOLDOWN_TIME);
			description += separator;
			description += std::to_string(StopAbility::STOP_ABILITY_COOLDOWN/EngineDefs::MSEC);
			description += endline;
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::DURATION_ATTRIBUTE);
			description += separator;
			description += std::to_string(StopAbility::STOP_ABILITY_DURATION/EngineDefs::MSEC);
			break;
		default:
			break;
		}
	}
		break;
	case Element::E_Enemy:
	{
		frameCount = 4;
		frameSize = sf::Vector2i(Enemy::ENEMY_SCALE * enemyInfo.size.x * GameOptions::MAP_CELL_SIZE,
								 Enemy::ENEMY_SCALE * enemyInfo.size.y * GameOptions::MAP_CELL_SIZE);
		icon.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y));

		const sf::Vector2f k = sf::Vector2f(2.f/static_cast<float>(enemyInfo.size.x),
											2.f/static_cast<float>(enemyInfo.size.y));
		icon.scale(k.x/Enemy::ENEMY_SCALE, k.y/Enemy::ENEMY_SCALE);

		description += Engine::Instance().translationsManager()->translate(name);
		description += endline;
		//speed
		description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::SPEED) + separator;
		if (enemyInfo.stats.speed < 30)
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::SLOW);
		else if (enemyInfo.stats.speed < 65)
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::NORMAL_SPEED);
		else
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::FAST);
		description += endline;
		//size
		description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::SIZE) + separator;
		if (enemyInfo.size.x == 1)
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::SMALL);
		else if (enemyInfo.size.x == 2)
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::MID);
		else
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::BIG);
		description += endline;
		//health
		description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::HEALTH) + separator +
				GlobalVariables::to_string_with_precision(enemyInfo.stats.health, 1);
		description += endline;

		//armor
		description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ARMOR) + separator;
		if (enemyInfo.stats.reflection < 0.1f)
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ARMOR_LIGHT);
		else if (enemyInfo.stats.reflection < 0.3f)
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ARMOR_MEDIUM);
		else
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ARMOR_STRONG);
		description += endline;

		//ability
		description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ABILITY) + separator;
		description += endline;

		switch (enemyInfo.abilityType)
		{
		case EnemiesFactory::EnemyInfo::NONE:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_NONE);
			break;
		case EnemiesFactory::EnemyInfo::RAGE:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_RAGE);
			break;
		case EnemiesFactory::EnemyInfo::SPAWN:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN);
			break;
		case EnemiesFactory::EnemyInfo::TELEPORT:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT);
			break;
		case EnemiesFactory::EnemyInfo::SHELL_NEAR:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR);
			break;
		case EnemiesFactory::EnemyInfo::HEAL_NEAR:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR);
			break;
		case EnemiesFactory::EnemyInfo::SHUTDOWN_TOWER:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER);
			break;
		case EnemiesFactory::EnemyInfo::STRONG:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_STRONG);
			break;
		case EnemiesFactory::EnemyInfo::DOWN_TOWER:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER);
			break;
		case EnemiesFactory::EnemyInfo::SELF_HEAL:
			description += Engine::Instance().translationsManager()->translate(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL);
			break;
		default:
			break;
		}
	}
		break;
	default:
		break;
	}

	object = new GameObject(texture, sf::Vector2f(0,0), frameSize, frameCount);

	descriptionText.setString(description);
}
