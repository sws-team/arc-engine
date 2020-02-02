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
	addElements();
	updatePos();
	updateCurrentInfo();
}

void ManualWindow::back()
{
	Engine::Instance().setState(Engine::MAIN_MENU);
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
		window->draw(elements.at(MAX_ELEMENTS_COUNT * page + current).descriptionText);
	}
}

void ManualWindow::eventFilter(Event *event)
{
	if (event->type == Event::MouseButtonPressed)
	{

	}
	else if (event->type == Event::MouseMoved)
	{
		current = -1;
		const Vector2f pos = Vector2f(event->mouseMove.x, event->mouseMove.y);
		for (unsigned int i = MAX_ELEMENTS_COUNT * page; i < elements.size(); ++i)
		{
			if (elements.at(i).rect.getGlobalBounds().contains(pos))
			{
				current = i;
				break;
			}
		}
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
		{
			page++;
			if (page >= maxPages)
				page = maxPages;
			current = 0;
			updateCurrentInfo();
		}
			break;
		case Keyboard::Left:
		{
			page--;
			if (page < 0)
				page = 0;
			current = MAX_ELEMENTS_COUNT - 1;
			updateCurrentInfo();
		}
			break;
		default:
			break;
		};
	}
	StateWindow::eventFilter(event);
}

void ManualWindow::updatePos()
{
	const float RECT_WIDTH = 512 * Settings::Instance().getScaleFactor().x;
	const float RECT_HEIGHT = 128 * Settings::Instance().getScaleFactor().y;
	const float ICON_Y_OFFSET = 32 * Settings::Instance().getScaleFactor().y;
	const float ICON_X_OFFSET = 32 * Settings::Instance().getScaleFactor().x;

	currentRect.setFillColor(Color(45, 45, 45, 128));
	currentRect.setSize(Vector2f(RECT_WIDTH, RECT_HEIGHT));
	currentRect.setOutlineThickness(1);
	currentRect.setOutlineColor(Color::Black);

	infoRect.setFillColor(Color(45, 45, 45, 128));
	infoRect.setSize(Vector2f(RECT_WIDTH * 2, (RECT_HEIGHT + ICON_Y_OFFSET) * MAX_ELEMENTS_COUNT));
	infoRect.setOutlineThickness(1);
	infoRect.setOutlineColor(Color::Black);
	infoRect.setPosition(ICON_X_OFFSET * 3 + RECT_WIDTH, ICON_Y_OFFSET * 2);

	const  Vector2f scaleFactor = Settings::Instance().getScaleFactor();
	Vector2f pos = Vector2f(ICON_X_OFFSET * 2, ICON_Y_OFFSET * 2);

	int count = 0;
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		Element& element = elements.at(i);

		element.rect.setPosition(pos);
		element.rect.setFillColor(Color(34, 53, 200, 100));
		element.rect.setSize(Vector2f(RECT_WIDTH, RECT_HEIGHT));
		element.icon.setTexture(ResourcesManager::Instance().getTexture(element.texture));
		element.icon.setTextureRect(IntRect(0,0,element.size.x, element.size.y));
		element.icon.setPosition(pos + Vector2f(ICON_X_OFFSET, ICON_Y_OFFSET));
		element.icon.setScale(scaleFactor);
		element.nameText.setFont(GlobalVariables::Instance().font());
		element.nameText.setFillColor(Color::Red);
		element.nameText.setCharacterSize(36);
		element.nameText.setScale(scaleFactor);
		element.nameText.setString(Language::Instance().translate(element.name));
		element.nameText.setPosition(pos + Vector2f(ICON_X_OFFSET, ICON_Y_OFFSET) +
									 Vector2f(element.size.x * scaleFactor.x + ICON_X_OFFSET, 0));
		element.descriptionText.setFont(GlobalVariables::Instance().font());
		element.descriptionText.setFillColor(Color::Red);
		element.descriptionText.setCharacterSize(36);
		element.descriptionText.setScale(scaleFactor);
		element.descriptionText.setString(Language::Instance().translate(element.description));
		element.descriptionText.setPosition(Vector2f(infoRect.getGlobalBounds().left,
													 infoRect.getGlobalBounds().top));

		switch (element.type)
		{
		case Element::E_Tower:
			element.icon.scale(Tower::TOWER_SCAlE, Tower::TOWER_SCAlE);
			break;
		case Element::E_Ability:
			break;
		case Element::E_Enemy:
			break;
		default:
			break;
		}

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
	const Vector2i towersSize = Vector2i(192, 192);
	elements.push_back(Element(RESOURCES::TOWER_BASE,
							   Language::TOWER_BASE,
							   towersSize,
							   Element::E_Tower,
							   Language::BASE_TOWER_DESCRIPTION,
							   RESOURCES::TOWER_BASE,
							   Language::TOWER_BASE,
							   RESOURCES::TOWER_BASE));

	elements.push_back(Element(RESOURCES::TOWER_POWER,
							   Language::TOWER_POWER,
							   towersSize,
							   Element::E_Tower,
							   Language::POWER_TOWER_DESCRIPTION,
							   RESOURCES::TOWER_POWER,
							   Language::TOWER_POWER,
							   RESOURCES::TOWER_POWER));

	elements.push_back(Element(RESOURCES::TOWER_LASER,
							   Language::TOWER_LASER,
							   towersSize,
							   Element::E_Tower,
							   Language::LASER_TOWER_DESCRIPTION,
							   RESOURCES::TOWER_LASER,
							   Language::TOWER_LASER,
							   RESOURCES::TOWER_LASER));

	elements.push_back(Element(RESOURCES::TOWER_FREEZE,
							   Language::TOWER_FREEZE,
							   towersSize,
							   Element::E_Tower,
							   Language::FREEZE_TOWER_DESCRIPTION,
							   RESOURCES::TOWER_FREEZE,
							   Language::TOWER_FREEZE,
							   RESOURCES::TOWER_FREEZE));

	elements.push_back(Element(RESOURCES::TOWER_ROCKET,
							   Language::TOWER_ROCKET,
							   towersSize,
							   Element::E_Tower,
							   Language::ROCKET_TOWER_DESCRIPTION,
							   RESOURCES::TOWER_ROCKET,
							   Language::TOWER_ROCKET,
							   RESOURCES::TOWER_ROCKET));

	elements.push_back(Element(RESOURCES::TOWER_IMPROVED,
							   Language::TOWER_IMPROVED,
							   towersSize,
							   Element::E_Tower,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::TOWER_IMPROVED,
							   Language::TOWER_IMPROVED,
							   RESOURCES::TOWER_IMPROVED));
	//abilities
	const Vector2i abilitiesSize = Vector2i(GlobalVariables::CELL_SIZE, GlobalVariables::CELL_SIZE);
	elements.push_back(Element(RESOURCES::ABILITY_BOMB,
							   Language::TOWER_IMPROVED,
							   abilitiesSize,
							   Element::E_Ability,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ABILITY_BOMB,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ABILITY_BOMB));

	elements.push_back(Element(RESOURCES::ABILITY_FREEZE_BOMB,
							   Language::TOWER_IMPROVED,
							   abilitiesSize,
							   Element::E_Ability,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ABILITY_FREEZE_BOMB,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ABILITY_FREEZE_BOMB));

	elements.push_back(Element(RESOURCES::ABILITY_CARPET_BOMBING,
							   Language::TOWER_IMPROVED,
							   abilitiesSize,
							   Element::E_Ability,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ABILITY_CARPET_BOMBING,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ABILITY_CARPET_BOMBING));

	elements.push_back(Element(RESOURCES::ABILITY_INCREASE_TOWER_DAMAGE,
							   Language::TOWER_IMPROVED,
							   abilitiesSize,
							   Element::E_Ability,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ABILITY_INCREASE_TOWER_DAMAGE,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ABILITY_INCREASE_TOWER_DAMAGE));

	elements.push_back(Element(RESOURCES::ABILITY_INCREASE_TOWER_ATTACK_SPEED,
							   Language::TOWER_IMPROVED,
							   abilitiesSize,
							   Element::E_Ability,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ABILITY_INCREASE_TOWER_ATTACK_SPEED,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ABILITY_INCREASE_TOWER_ATTACK_SPEED));

	elements.push_back(Element(RESOURCES::ABILITY_TIME_STOP,
							   Language::TOWER_IMPROVED,
							   abilitiesSize,
							   Element::E_Ability,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ABILITY_TIME_STOP,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ABILITY_TIME_STOP));

	//enemies
	Vector2i enemiesSize = Vector2i(GlobalVariables::CELL_SIZE/2, GlobalVariables::CELL_SIZE/2);
	elements.push_back(Element(RESOURCES::ENEMY_SCORPION,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::SCORPION).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::SCORPION).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_SCORPION,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_SCORPION));

	elements.push_back(Element(RESOURCES::ENEMY_CAR,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::SMALL_MEDIUM).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::SMALL_MEDIUM).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_CAR,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_CAR));

	elements.push_back(Element(RESOURCES::ENEMY_TRICYCLE,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::SMALL_FAST).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::SMALL_FAST).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_TRICYCLE,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_TRICYCLE));

	elements.push_back(Element(RESOURCES::ENEMY_TANK,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::MID_SLOW).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::MID_SLOW).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_TANK,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_TANK));

	elements.push_back(Element(RESOURCES::ENEMY_SPIDER,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::SPIDER).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::SPIDER).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_SPIDER,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_SPIDER));

	elements.push_back(Element(RESOURCES::ENEMY_HELICOPTER,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::MID_FAST).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::MID_FAST).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_HELICOPTER,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_HELICOPTER));

	elements.push_back(Element(RESOURCES::ENEMY_AIRCARRIER,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::BIG_SLOW).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::BIG_SLOW).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_AIRCARRIER,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_AIRCARRIER));

	elements.push_back(Element(RESOURCES::ENEMY_BIG_TANK,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::BIG_MEDIUM).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::BIG_MEDIUM).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_BIG_TANK,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_BIG_TANK));

	elements.push_back(Element(RESOURCES::ENEMY_PLANE,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::ANOTHER_ENEMY).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::ANOTHER_ENEMY).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_PLANE,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_PLANE));

	elements.push_back(Element(RESOURCES::ENEMY_REPAIR,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::REPAIR_ENEMY).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::REPAIR_ENEMY).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_REPAIR,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_REPAIR));

	elements.push_back(Element(RESOURCES::ENEMY_SHELL,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::SHELL_ENEMY).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::SHELL_ENEMY).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_SHELL,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_SHELL));

	elements.push_back(Element(RESOURCES::ENEMY_TELEPORT,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::TELEPORT_ENEMY).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::TELEPORT_ENEMY).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_TELEPORT,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_TELEPORT));

	elements.push_back(Element(RESOURCES::ENEMY_SELF_HEAL,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::SELFHEAL_ENEMY).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::SELFHEAL_ENEMY).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_SELF_HEAL,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_SELF_HEAL));

	elements.push_back(Element(RESOURCES::ENEMY_DOWN_TOWER,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::DOWN_TOWER_ENEMY).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::DOWN_TOWER_ENEMY).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_DOWN_TOWER,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_DOWN_TOWER));

	elements.push_back(Element(RESOURCES::ENEMY_ANT,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::SMALL_NEXT).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::SMALL_NEXT).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_ANT,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_ANT));

	elements.push_back(Element(RESOURCES::ENEMY_COW,
							   Language::TOWER_IMPROVED,
							   Vector2i(
								   EnemiesFactory::enemySize(ENEMY_TYPES::SPAWN_ENEMY).x * enemiesSize.x,
								   EnemiesFactory::enemySize(ENEMY_TYPES::SPAWN_ENEMY).y * enemiesSize.y),
							   Element::E_Enemy,
							   Language::IMPROVED_TOWER_DESCRIPTION,
							   RESOURCES::ENEMY_COW,
							   Language::TOWER_IMPROVED,
							   RESOURCES::ENEMY_COW));
}


void ManualWindow::updateCurrentInfo()
{
	if (current == -1)
		return;

	const int id = page * MAX_ELEMENTS_COUNT + current;
	const Element element = elements.at(id);
	currentRect.setPosition(element.rect.getPosition());
}
