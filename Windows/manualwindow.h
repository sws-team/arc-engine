#ifndef MANUALWINDOW_H
#define MANUALWINDOW_H

#include "statewindow.h"
#include "enginedef.h"
#include "gameobject.h"
#include "Game/instructions.h"

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
			E_Enemy,
			E_Effect,
		};

		Element(TextureType texture,
				TranslationType name,
				const EnemiesFactory::EnemyInfo& info,
				int frameCount,
				float animationSpeed);

		Element(TextureType texture,
				TranslationType name,
				ACTION_STATE type);

		Element(TextureType texture,
				TranslationType name,
				TOWER_TYPES towerType,
				int frameCount,
				float animationSpeed);

		Element(TextureType texture,
				TranslationType name,
				Instructions::MAP_EFFECTS type,
				int frameCount,
				float animationSpeed);

		TextureType texture;
		TranslationType name;
		ElementType type;

		EnemiesFactory::EnemyInfo enemyInfo;
		TOWER_TYPES towerType;
		ACTION_STATE abilityType;
		Instructions::MAP_EFFECTS effectType;

		sf::RectangleShape rect;
		sf::Sprite icon;
		GameObject *object;
		sf::Text nameText;
		sf::Text descriptionText;
		int frameCount;
		float animationSpeed;

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
