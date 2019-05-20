#ifndef GARAGELOCATION_H
#define GARAGELOCATION_H

#include "statewindow.h"
#include "graphics.h"
#include "Game/SpaceShip/Weapons/weapontypes.h"
#include "Game/Artifacts/artifacttypes.h"
#include "timer.h"

class SpaceShip;
class Characteristics;

class GarageLocation : public StateWindow
{
public:
	GarageLocation();
	~GarageLocation() override;

protected:
	void paint(RenderWindow *window) override;
	void eventFilter(Event *event) override;
	void update() override;
	void back() override;

private:
	RectangleShape exitRect;

	vector<GraphicImage*> playerIcons;
	int currentPlayer;
	void updateColor();

	void updateCharacteristics(const Characteristics &stats);
	void updateInfo();

	class ShopItem
	{
	public:
		struct Item
		{
			int type;
//			Texture icon;
			RESOURCES::TEXTURE_TYPE id;
			String description;
		};

		vector<Item> items;

		Sprite sprite;
		Sprite upSprite;
		Sprite downSprite;
		int current;
		int currentType;
		bool isExists;

		void draw(RenderWindow *window);
	};

	ShopItem weaponItem;
	void updateCurrentWeapon();
	ShopItem artifactItem;
	void updateCurrentArtifact();

	vector<class Character*> characters;

	struct TextInfo
	{
		Text text;
		Text valueText;
	};
	vector<TextInfo> shopCharacteristics;

	Text info;

	SpaceShip *player;

	void updateCharacters();

	void updateSpaceShip();

	void initLabels();

	Text weaponsLabel;
	Text artifactsLabel;
	Text charactersLabel;
	Text playersLabel;
	Text spaceShipLabel;
	Text statsLabel;
	Text infoLabel;
	Text scoreLabel;
	Text pointsLabel;

	void updateScore();
	Text scoreValue;
	Text pointsValue;

	//controls

	constexpr static int CHARACTERISTICS_COST = 1;
	enum CONTROL_STATES
	{
		UNKNOWN_STATE = -1,

		CHOOSE_PLAYER,
		CHOOSE_CHARACTERISTICS,
		CHOOSE_WEAPON,
		CHOOSE_ARTIFACT,

		STATES_COUNT
	};
	CONTROL_STATES m_state;
	void setState(CONTROL_STATES state);

	GraphicImage choosePlayer;
	RectangleShape chooseCharacteristics;
	int currentCharacteristic;
	void updateCurrentCharacteristic();

	bool hasWeapon(const int type) const;

	RectangleShape block;
	void updateCurrentBlock();
	Timer blinkBlockTimer;
	bool blinkColor;

	TextInfo ammo;
	bool drawWeaponAmmo;
};

#endif // GARAGELOCATION_H
