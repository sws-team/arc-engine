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
	void paint(RenderWindow *window) override;
	void eventFilter(Event *event) override;

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
				const String& description);

		Element(TextureType texture,
				TranslationType name,
				TOWER_TYPES towerType);

		TextureType texture;
		TranslationType name;
		ElementType type;

		EnemiesFactory::EnemyInfo enemyInfo;
		TOWER_TYPES towerType;
		String abilityInfo;

		RectangleShape rect;
		Sprite icon;
		GameObject *object;
		Text nameText;
		Text descriptionText;

		void init();
		void update();
	};

	vector<Element> elements;

	void updatePos();

	void addElements();

	RectangleShape currentRect;
	int current;
	void updateCurrentInfo();
	RectangleShape infoRect;

	int page;
	static constexpr unsigned int MAX_ELEMENTS_COUNT = 6;
	int maxPages;

	Sprite next;
	Sprite previous;
	Sprite close;
	Sprite credits;
	Text toolTip;

	void nextPage();
	void previousPage();

	static const Color SELECTED_COLOR;
};

#endif // MANUALWINDOW_H
