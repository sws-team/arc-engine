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
				float animationSpeed,
				TextureType demoTextureType = -1);

		Element(TextureType texture,
				TranslationType name,
				ACTION_STATE type,
				TextureType demoTextureType = -1);

		Element(TextureType texture,
				TranslationType name,
				TOWER_TYPES towerType,
				int frameCount,
				float animationSpeed,
				TextureType demoTextureType = -1);

		Element(TextureType texture,
				TranslationType name,
				Instructions::MAP_EFFECTS type,
				int frameCount,
				float animationSpeed,
				TextureType demoTextureType = -1);

		TextureType texture;
		TranslationType name;
		ElementType type;

		EnemiesFactory::EnemyInfo enemyInfo;
		TOWER_TYPES towerType;
		ACTION_STATE abilityType;
		Instructions::MAP_EFFECTS effectType;

		sf::Sprite icon;
		GameObject *object;
		sf::Text titleText;
		sf::Text descriptionText;
		sf::Text nameText;
		int frameCount;
		float animationSpeed;
		TextureType demoTextureType;

		void init();
		void update();

		GameObject *elementDemo;
		sf::RectangleShape nameFrame;
	};

	std::vector<Element> elements;

	void updatePos();

	void addElements();

	sf::RectangleShape currentRect;
	int current;
	void updateCurrentInfo();

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
	static const sf::Color CURRENT_COLOR;
	GameObject *characterObject;

	static constexpr float FRAME_OFFSET = 21;
	sf::RectangleShape demoFrame;
};

#endif // MANUALWINDOW_H
