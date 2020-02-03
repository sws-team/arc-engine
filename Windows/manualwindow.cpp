#include "manualwindow.h"
#include "settings.h"
#include "ResourcesManager/resourcesmanager.h"
#include "globalvariables.h"
#include "Game/Level/tower.h"
#include "Game/Level/enemy.h"
#include "Game/leveldef.h"

ManualWindow::ManualWindow()
	: StateWindow()
	,current(0)
	,page(0)
{	
	setBackground(RESOURCES::ABOUT_BACKGROUND);

	next.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::NEXT));
	next.setScale(Settings::Instance().getScaleFactor());
	previous.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::PREVIOUS));
	previous.setScale(Settings::Instance().getScaleFactor());

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
	Engine::Instance().setState(Engine::MAIN_MENU);
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

void ManualWindow::paint(RenderWindow *window)
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
}

void ManualWindow::eventFilter(Event *event)
{
	if (event->type == Event::MouseButtonPressed)
	{
		const Vector2f pos = Vector2f(event->mouseButton.x, event->mouseButton.y);

		if (next.getGlobalBounds().contains(pos))
			nextPage();
		if (previous.getGlobalBounds().contains(pos))
			previousPage();
	}
	else if (event->type == Event::MouseMoved)
	{
		current = -1;
		const Vector2f pos = Vector2f(event->mouseMove.x, event->mouseMove.y);
		const int startValue = MAX_ELEMENTS_COUNT * page;
		for (unsigned int i = startValue; i < startValue + MAX_ELEMENTS_COUNT; ++i)
		{
			if (i >= elements.size())
				break;
			if (elements.at(i).rect.getGlobalBounds().contains(pos))
			{
				current = i - startValue;
				break;
			}
		}
		next.setColor(Color::White);
		if (next.getGlobalBounds().contains(pos))
			next.setColor(Color::Red);

		previous.setColor(Color::White);
		if (previous.getGlobalBounds().contains(pos))
			previous.setColor(Color::Red);

		updateCurrentInfo();
	}
	else if (event->type == Event::KeyPressed)
	{
		switch (event->key.code)
		{
		case Keyboard::Up:
		{
			current--;
			if (current < 0)
				current = 0;
			updateCurrentInfo();
		}
			break;
		case Keyboard::Down:
		{
			current++;
			if (current == MAX_ELEMENTS_COUNT)
				current = MAX_ELEMENTS_COUNT - 1;
			updateCurrentInfo();
		}
			break;
		case Keyboard::Right:
			nextPage();
			break;
		case Keyboard::Left:
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
	const float ICON_WIDTH = GlobalVariables::Instance().tileSize().x;
	const float RECT_WIDTH = 512 * Settings::Instance().getScaleFactor().x;
	const float RECT_HEIGHT = 128 * Settings::Instance().getScaleFactor().y;
	const float ICON_Y_OFFSET = 32 * Settings::Instance().getScaleFactor().y;
	const float ICON_X_OFFSET = 32 * Settings::Instance().getScaleFactor().x;

	currentRect.setFillColor(Color(45, 45, 45, 128));
	currentRect.setSize(Vector2f(RECT_WIDTH, RECT_HEIGHT));
	currentRect.setOutlineThickness(1);
	currentRect.setOutlineColor(Color::Black);

	infoRect.setFillColor(Color(45, 45, 45, 128));
	infoRect.setSize(Vector2f(RECT_WIDTH * 2,
							  RECT_HEIGHT * MAX_ELEMENTS_COUNT + ICON_Y_OFFSET * (MAX_ELEMENTS_COUNT - 1)));
	infoRect.setOutlineThickness(1);
	infoRect.setOutlineColor(Color::Black);
	infoRect.setPosition(ICON_X_OFFSET * 3 + RECT_WIDTH, ICON_Y_OFFSET * 2);
	next.setPosition(Vector2f(infoRect.getGlobalBounds().left + infoRect.getGlobalBounds().width,
							  infoRect.getGlobalBounds().top) + Vector2f(ICON_X_OFFSET + ICON_WIDTH, 0));

	previous.setPosition(Vector2f(infoRect.getGlobalBounds().left + infoRect.getGlobalBounds().width,
							  infoRect.getGlobalBounds().top) + Vector2f(ICON_X_OFFSET, 0));


	const  Vector2f scaleFactor = Settings::Instance().getScaleFactor();
	Vector2f pos = Vector2f(ICON_X_OFFSET * 2, ICON_Y_OFFSET * 2);

	int count = 0;
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		Element& element = elements.at(i);

		element.init();
		element.icon.setScale(scaleFactor);
		element.update();

		element.rect.setPosition(pos);
		element.rect.setSize(Vector2f(RECT_WIDTH, RECT_HEIGHT));

		element.icon.setPosition(pos + Vector2f(ICON_X_OFFSET, ICON_Y_OFFSET));

		element.nameText.setScale(scaleFactor);

		element.descriptionText.setScale(scaleFactor);
		element.descriptionText.setPosition(Vector2f(infoRect.getGlobalBounds().left,
													 infoRect.getGlobalBounds().top) +
											Vector2f(ICON_X_OFFSET, ICON_Y_OFFSET));

		element.nameText.setPosition(pos + Vector2f(ICON_X_OFFSET, ICON_Y_OFFSET) +
									 Vector2f(ICON_WIDTH + ICON_X_OFFSET, 0));

		pos.y += ICON_Y_OFFSET + RECT_HEIGHT;

		count++;
		if (count == MAX_ELEMENTS_COUNT)
		{
			pos = Vector2f(ICON_X_OFFSET * 2, ICON_Y_OFFSET * 2);
			count = 0;
		}
	}
	maxPages = elements.size()/MAX_ELEMENTS_COUNT;
}

void ManualWindow::addElements()
{
	//towers
	elements.push_back(Element(RESOURCES::TOWER_BASE,
							   Language::TOWER_BASE,
							   BASE));

	elements.push_back(Element(RESOURCES::TOWER_POWER,
							   Language::TOWER_POWER,
							   POWER));

	elements.push_back(Element(RESOURCES::TOWER_LASER,
							   Language::TOWER_LASER,
							   LASER));

	elements.push_back(Element(RESOURCES::TOWER_FREEZE,
							   Language::TOWER_FREEZE,
							   FREEZE));

	elements.push_back(Element(RESOURCES::TOWER_ROCKET,
							   Language::TOWER_ROCKET,
							   ROCKET));

	elements.push_back(Element(RESOURCES::TOWER_IMPROVED,
							   Language::TOWER_IMPROVED,
							   IMPROVED));
	//abilities
	elements.push_back(Element(RESOURCES::ABILITY_BOMB,
							   Language::ABILITY_BOMB,
							   Language::Instance().translate(Language::BOMB_ABILITY_DESCRIPTION)));

	elements.push_back(Element(RESOURCES::ABILITY_FREEZE_BOMB,
							   Language::ABILITY_FREEZE_BOMB,
							   Language::Instance().translate(Language::FREEZE_BOMB_ABILITY_DESCRIPTION)));

	elements.push_back(Element(RESOURCES::ABILITY_CARPET_BOMBING,
							   Language::ABILITY_VENOM,
							   Language::Instance().translate(Language::VENOM_ABILITY_DESCRIPTION)));

	elements.push_back(Element(RESOURCES::ABILITY_INCREASE_TOWER_DAMAGE,
							   Language::ABILITY_INCREASE_TOWER_DAMAGE,
							   Language::Instance().translate(Language::INC_DMG_ABILITY_DESCRIPTION)));

	elements.push_back(Element(RESOURCES::ABILITY_INCREASE_TOWER_ATTACK_SPEED,
							   Language::ABILITY_INCREASE_TOWER_ATTACK_SPEED,
							   Language::Instance().translate(Language::INC_AS_ABILITY_DESCRIPTION)));

	elements.push_back(Element(RESOURCES::ABILITY_TIME_STOP,
							   Language::ABILITY_STOP,
							   Language::Instance().translate(Language::STOP_ABILITY_DESCRIPTION)));

	//enemies
	elements.push_back(Element(RESOURCES::ENEMY_SCORPION,
							   Language::SCORPION,
							   EnemiesFactory::getEnemyInfo(SCORPION)));

	elements.push_back(Element(RESOURCES::ENEMY_CAR,
							   Language::CAR,
							   EnemiesFactory::getEnemyInfo(SMALL_MEDIUM)));

	elements.push_back(Element(RESOURCES::ENEMY_TRICYCLE,
							   Language::TRICYCLE,
							   EnemiesFactory::getEnemyInfo(SMALL_FAST)));

	elements.push_back(Element(RESOURCES::ENEMY_TANK,
							   Language::TANK,
							   EnemiesFactory::getEnemyInfo(MID_SLOW)));

	elements.push_back(Element(RESOURCES::ENEMY_SPIDER,
							   Language::SPIDER,
							   EnemiesFactory::getEnemyInfo(SPIDER)));

	elements.push_back(Element(RESOURCES::ENEMY_HELICOPTER,
							   Language::HELICOPTER,
							   EnemiesFactory::getEnemyInfo(MID_FAST)));

	elements.push_back(Element(RESOURCES::ENEMY_AIRCARRIER,
							   Language::AIRCARRIER,
							   EnemiesFactory::getEnemyInfo(BIG_SLOW)));

	elements.push_back(Element(RESOURCES::ENEMY_BIG_TANK,
							   Language::BIG_TANK,
							   EnemiesFactory::getEnemyInfo(BIG_MEDIUM)));

	elements.push_back(Element(RESOURCES::ENEMY_PLANE,
							   Language::PLANE,
							   EnemiesFactory::getEnemyInfo(ANOTHER_ENEMY)));

	elements.push_back(Element(RESOURCES::ENEMY_REPAIR,
							   Language::REPAIR,
							   EnemiesFactory::getEnemyInfo(REPAIR_ENEMY)));

	elements.push_back(Element(RESOURCES::ENEMY_SHELL,
							   Language::SHELL,
							   EnemiesFactory::getEnemyInfo(SHELL_ENEMY)));

	elements.push_back(Element(RESOURCES::ENEMY_TELEPORT,
							   Language::TELEPORT,
							   EnemiesFactory::getEnemyInfo(TELEPORT_ENEMY)));

	elements.push_back(Element(RESOURCES::ENEMY_SELF_HEAL,
							   Language::SELF_HEAL,
							   EnemiesFactory::getEnemyInfo(SELFHEAL_ENEMY)));

	elements.push_back(Element(RESOURCES::ENEMY_DOWN_TOWER,
							   Language::DOWN_TOWER,
							   EnemiesFactory::getEnemyInfo(DOWN_TOWER_ENEMY)));

	elements.push_back(Element(RESOURCES::ENEMY_ANT,
							   Language::ANT,
							   EnemiesFactory::getEnemyInfo(SMALL_NEXT)));

	elements.push_back(Element(RESOURCES::ENEMY_COW,
							   Language::COW,
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

ManualWindow::Element::Element(RESOURCES::TEXTURE_TYPE texture,
							   Language::TR_TEXT name,
							   const EnemiesFactory::EnemyInfo& info)
	: 	texture(texture)
	,name(name)
	,type(E_Enemy)
	,enemyInfo(info)
{

}

ManualWindow::Element::Element(RESOURCES::TEXTURE_TYPE texture,
							   Language::TR_TEXT name,
							   const String &description)
	: 	texture(texture)
	,name(name)
	,type(E_Ability)
	,abilityInfo(description)
{

}

ManualWindow::Element::Element(RESOURCES::TEXTURE_TYPE texture,
							   Language::TR_TEXT name,
							   TOWER_TYPES towerType)
	: 	texture(texture)
	,name(name)
	,type(E_Tower)
	,towerType(towerType)
{

}

void ManualWindow::Element::init()
{
	rect.setFillColor(Color(34, 53, 200, 100));

	nameText.setFont(GlobalVariables::Instance().font());
	nameText.setFillColor(Color::Red);
	nameText.setCharacterSize(36);

	descriptionText.setFont(GlobalVariables::Instance().font());
	descriptionText.setFillColor(Color::Red);
	descriptionText.setCharacterSize(36);
}

void ManualWindow::Element::update()
{
	int frameCount = 1;
	Vector2i frameSize;
	//texture
	icon.setTexture(ResourcesManager::Instance().getTexture(texture));
	//name
	nameText.setString(Language::Instance().translate(name));
	//description
	String description;
	const String separator = ": ";
	const String endline = "\n";
	switch (type)
	{
	case Element::E_Tower:
	{
		frameSize = Vector2i(192, 192);
		icon.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);

		switch (towerType)
		{
		case BASE:
		{
			description = Language::Instance().translate(Language::TOWER_BASE);
			description += endline;
			description += Language::Instance().translate(Language::BASE_TOWER_DESCRIPTION);
		}
			break;
		case POWER:
		{
			description = Language::Instance().translate(Language::TOWER_POWER);
			description += endline;
			description += Language::Instance().translate(Language::POWER_TOWER_DESCRIPTION);
		}
			break;
		case ROCKET:
		{
			description = Language::Instance().translate(Language::TOWER_ROCKET);
			description += endline;
			description += Language::Instance().translate(Language::ROCKET_TOWER_DESCRIPTION);
		}
			break;
		case FREEZE:
		{
			description = Language::Instance().translate(Language::TOWER_FREEZE);
			description += endline;
			description += Language::Instance().translate(Language::FREEZE_TOWER_DESCRIPTION);
		}
			break;
		case LASER:
		{
			description = Language::Instance().translate(Language::TOWER_LASER);
			description += endline;
			description += Language::Instance().translate(Language::LASER_TOWER_DESCRIPTION);
		}
			break;
		case IMPROVED:
		{
			description = Language::Instance().translate(Language::TOWER_IMPROVED);
			description += endline;
			description += Language::Instance().translate(Language::IMPROVED_TOWER_DESCRIPTION);
		}
			break;
		}

		description += endline;

		const TowerStats towerStats = TowersFactory::getTowerStats(towerType);
		const float dps = towerStats.damage / ((towerStats.attackSpeed) * 0.001f);

		description += Language::Instance().translate(Language::DAMAGE_PER_SECOND) + separator + GlobalVariables::to_string_with_precision(dps, 2);
		description += endline;

		description += Language::Instance().translate(Language::RADIUS) + separator +
				GlobalVariables::to_string_with_precision(towerStats.radius, 1);
		description += endline;

		const float cost = towerStats.cost;

		description += Language::Instance().translate(Language::COST);
		description += separator;
		description += GlobalVariables::to_string_with_precision(cost, 2);
	}
		break;
	case Element::E_Ability:
	{
		frameSize = Vector2i(GlobalVariables::CELL_SIZE, GlobalVariables::CELL_SIZE);
		description += Language::Instance().translate(name);
		description += endline;
		description += abilityInfo;
	}
		break;
	case Element::E_Enemy:
	{
		frameCount = 4;
		frameSize = Vector2i(enemyInfo.size.x * GlobalVariables::CELL_SIZE/2,
							 enemyInfo.size.y * GlobalVariables::CELL_SIZE/2);
		icon.setTextureRect(IntRect(0, 0, frameSize.x, frameSize.y));
		if (enemyInfo.size.x == 1)
			icon.scale(2, 2);
		else if (enemyInfo.size.x == 4)
			icon.scale(0.5f, 0.5f);

		description += Language::Instance().translate(name);
		description += endline;
		//speed
		description += Language::Instance().translate(Language::SPEED) + separator;
		if (enemyInfo.stats.speed < 30)
			description += Language::Instance().translate(Language::SLOW);
		else if (enemyInfo.stats.speed < 65)
			description += Language::Instance().translate(Language::NORMAL);
		else
			description += Language::Instance().translate(Language::FAST);
		description += endline;
		//size
		description += Language::Instance().translate(Language::SIZE) + separator;
		if (enemyInfo.size.x == 1)
			description += Language::Instance().translate(Language::SMALL);
		else if (enemyInfo.size.x == 2)
			description += Language::Instance().translate(Language::MID);
		else
			description += Language::Instance().translate(Language::BIG);
		description += endline;
		//health
		description += Language::Instance().translate(Language::HEALTH) + separator +
				GlobalVariables::to_string_with_precision(enemyInfo.stats.health, 1);
		description += endline;

		//armor

		//ability

	}
		break;
	default:
		break;
	}

	object = new GameObject(texture, Vector2f(0,0), frameSize, frameCount);

	descriptionText.setString(description);
}
