#ifndef MANUALWINDOW_H
#define MANUALWINDOW_H

#include "statewindow.h"
#include "ResourcesManager/textures_types.h"
#include "Translations/language.h"

#include "Game/Level/enemy.h"
#include "Game/Level/tower.h"

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

		Element(RESOURCES::TEXTURE_TYPE texture,
				Language::TR_TEXT name,
				const EnemiesFactory::EnemyInfo& info);

		Element(RESOURCES::TEXTURE_TYPE texture,
				Language::TR_TEXT name,
				const String& description);

		Element(RESOURCES::TEXTURE_TYPE texture,
				Language::TR_TEXT name,
				TOWER_TYPES towerType);

		RESOURCES::TEXTURE_TYPE texture;
		Language::TR_TEXT name;
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

	void nextPage();
	void previousPage();
};

#endif // MANUALWINDOW_H
