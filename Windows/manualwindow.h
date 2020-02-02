#ifndef MANUALWINDOW_H
#define MANUALWINDOW_H

#include "statewindow.h"
#include "ResourcesManager/textures_types.h"
#include "Translations/language.h"


class ManualWindow : public StateWindow
{
public:
	ManualWindow();

	void back() override;

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
		Element()
		{

		}
		Element(
				RESOURCES::TEXTURE_TYPE texture,
				Language::TR_TEXT name,
				const Vector2i& size,
				Element::ElementType type,
				Language::TR_TEXT description,
				RESOURCES::TEXTURE_TYPE animation,

				Language::TR_TEXT info,
				RESOURCES::TEXTURE_TYPE ability_animation)
			: 	texture(texture)
			,name(name)
			,size(size)
			,type(type)
			,description(description)
			,animation(animation)
			,info(info)
			,ability_animation(ability_animation)
		{

		}
		RESOURCES::TEXTURE_TYPE texture;
		Language::TR_TEXT name;
		Vector2i size;
		ElementType type;

		Language::TR_TEXT description;
		RESOURCES::TEXTURE_TYPE animation;

		Language::TR_TEXT info;
		RESOURCES::TEXTURE_TYPE ability_animation;

		RectangleShape rect;
		Sprite icon;
		Text nameText;
		Text descriptionText;
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
};

#endif // MANUALWINDOW_H
