#ifndef MANUALWINDOW_H
#define MANUALWINDOW_H

#include "statewindow.h"
#include "enginedef.h"
#include "gameobject.h"

#include "Game/enemy.h"
#include "Game/tower.h"

class ManualWindow : public StateWindow
{
public:
	ManualWindow();
	~ManualWindow() override;

	void back() override;
	void update() override;

protected:
	void paint(sf::RenderWindow *window) override;
	void eventFilter(sf::Event *event) override;

private:

	struct Element
	{
		enum ElementType
		{
			E_Tower,
			E_Ability,
			E_Enemy
		};

		Element(TextureType texture,
				TranslationType name,
				const EnemiesFactory::EnemyInfo& info);

		Element(TextureType texture,
				TranslationType name,
				const sf::String& description);

		Element(TextureType texture,
				TranslationType name,
				TOWER_TYPES towerType);

		TextureType texture;
		TranslationType name;
		ElementType type;

		EnemiesFactory::EnemyInfo enemyInfo;
		TOWER_TYPES towerType;
		sf::String abilityInfo;

		sf::RectangleShape rect;
		sf::Sprite icon;
		GameObject *object;
		sf::Text nameText;
		sf::Text descriptionText;

		void init();
		void update();
	};

	std::vector<Element> elements;

	void updatePos();

	void addElements();

	sf::RectangleShape currentRect;
	int current;
	void updateCurrentInfo();
	sf::RectangleShape infoRect;

	int page;
	static constexpr unsigned int MAX_ELEMENTS_COUNT = 6;
	int maxPages;

	sf::Sprite next;
	sf::Sprite previous;
	sf::Sprite close;
	sf::Sprite credits;
	sf::Text toolTip;

	void nextPage();
	void previousPage();

	static const sf::Color SELECTED_COLOR;
};

#endif // MANUALWINDOW_H
