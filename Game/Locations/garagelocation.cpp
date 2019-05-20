#include "garagelocation.h"
#include "Game/gamecontroller.h"
#include "settings.h"
#include "Game/SpaceShip/spaceship.h"
#include "Game/SpaceShip/Weapons/abstractweapon.h"
#include "Game/Characters/character.h"
#include "globalvariables.h"
#include "Translations/language.h"
#include "Game/objectsfactory.h"
#include "ResourcesManager/resourcesmanager.h"

GarageLocation::GarageLocation()
	: StateWindow()
	,drawWeaponAmmo(false)
{
	setBackground(RESOURCES::GARAGE_BACKGROUND);
	//fill labels
	initLabels();
	//fill players
	const float playersOffset = 16 * Settings::Instance().getScaleFactor().x;
	float posX = playersLabel.getGlobalBounds().left;
	float posY = playersLabel.getGlobalBounds().top + playersLabel.getLocalBounds().height;
	for (unsigned int i = 0; i < GameController::Instance().players().size(); ++i)
	{
		GraphicImage *playerIcon = new GraphicImage;

		char buffer[64];
		sprintf (buffer, "images/ui/player%d.png", i + 1);
		const string playerIconFileName = string(buffer);

		playerIcon->texture.loadFromFile(playerIconFileName);
		playerIcon->sprite.setTexture(playerIcon->texture);
		playerIcon->sprite.setPosition(posX, posY);
		playerIcon->sprite.scale(Settings::Instance().getScaleFactor());
		posY += GlobalVariables::CELL_WIDTH + playersOffset;

		playerIcons.push_back(playerIcon);
	}
	currentPlayer = 0;
	player = GameController::Instance().players().at(currentPlayer);

	//fill spaceship
	updateSpaceShip();

	updateColor();

	ammo.text.setFillColor(Color::White);
	ammo.text.setFont(GlobalVariables::Instance().font());
	ammo.valueText.setFillColor(Color::Green);
	ammo.valueText.setFont(GlobalVariables::Instance().font());

	//fill weapons
	weaponItem.upSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::UI_UP));
	weaponItem.downSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::UI_DOWN));
	weaponItem.current = 0;

	posX = weaponsLabel.getGlobalBounds().left;
	posY = weaponsLabel.getGlobalBounds().top + weaponsLabel.getGlobalBounds().height;

	const float directionSize = GlobalVariables::CELL_WIDTH/2;
	const float cellSpace = 8;
	weaponItem.upSprite.setPosition(posX, posY);
	weaponItem.upSprite.scale(Settings::Instance().getScaleFactor());
	posY += directionSize + cellSpace;
	weaponItem.sprite.setPosition(posX, posY);


	const float ammoTextOffsetX = Settings::Instance().getScaleFactor().x * 6;
	const float ammoTextOffsetY = Settings::Instance().getScaleFactor().y * 6;
	ammo.text.setPosition(posX + ammoTextOffsetX, posY + ammoTextOffsetY);
	ammo.valueText.setPosition(posX + ammoTextOffsetX,
							   posY + Settings::Instance().getScaleFactor().y * 24 + ammoTextOffsetY * 2);

	posY += GlobalVariables::CELL_WIDTH + cellSpace;
	weaponItem.downSprite.setPosition(posX, posY);
	weaponItem.downSprite.scale(Settings::Instance().getScaleFactor());

	ShopItem::Item item;
	item.type = WEAPON_TYPES::MINIGUN;
	item.id = RESOURCES::MINIGUN_ICON;
	item.description = "mini gun";
	weaponItem.items.push_back(item);

	item.type = WEAPON_TYPES::ROCKET;
	item.id = RESOURCES::ROCKET_ICON;
	item.description = "rockcet gun";
	weaponItem.items.push_back(item);

	item.type = WEAPON_TYPES::LASER;
	item.id = RESOURCES::LASER_ICON;
	item.description = "laser gun";
	weaponItem.items.push_back(item);

	item.type = WEAPON_TYPES::FLAMETHOWER;
	item.id = RESOURCES::FLAMETHOWER_ICON;
	item.description = "FLAMETHOWER gun";
	weaponItem.items.push_back(item);

	updateCurrentWeapon();

	//fill artifacts
	artifactItem.upSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::UI_UP));
	artifactItem.downSprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::UI_DOWN));
	artifactItem.current = 0;

	posX = artifactsLabel.getGlobalBounds().left;
	posY = artifactsLabel.getGlobalBounds().top + artifactsLabel.getGlobalBounds().height;

	artifactItem.upSprite.setPosition(posX, posY);
	posY += directionSize + cellSpace;
	artifactItem.sprite.setPosition(posX, posY);
	artifactItem.upSprite.scale(Settings::Instance().getScaleFactor());
	posY += GlobalVariables::CELL_WIDTH + cellSpace;
	artifactItem.downSprite.setPosition(posX, posY);
	artifactItem.downSprite.scale(Settings::Instance().getScaleFactor());

	item.type = ARTIFACT_TYPES::DAMAGE_ARTIFACT;
	item.id = RESOURCES::ARTIFACT_1;
	item.description = "art 1";
	artifactItem.items.push_back(item);

	item.type = ARTIFACT_TYPES::ENERGY_ARTIFACT;
	item.id = RESOURCES::ARTIFACT_3;
	item.description = "art 2";
	artifactItem.items.push_back(item);

	item.type = ARTIFACT_TYPES::HEALTH_ARTIFACT;
	item.id = RESOURCES::ARTIFACT_2;
	item.description = "art 3";
	artifactItem.items.push_back(item);

	updateCurrentArtifact();


	//fill info block
	info.setFillColor(Color::Black);
	info.setFont(GlobalVariables::Instance().font());
	info.setPosition(infoLabel.getGlobalBounds().left, infoLabel.getGlobalBounds().top + infoLabel.getGlobalBounds().height);
	updateInfo();

	//fill stats


	//fill score

	updateScore();
	//fill characters
	updateCharacters();

	//fill controls
	choosePlayer.texture.loadFromFile("images/ui/frame0.png");
	choosePlayer.sprite.setTexture(choosePlayer.texture);
	choosePlayer.sprite.setPosition(playerIcons.at(currentPlayer)->sprite.getPosition());
	choosePlayer.sprite.scale(Settings::Instance().getScaleFactor());

	currentCharacteristic = 0;

	block.setOutlineColor(Color::Yellow);
	block.setFillColor(Color::Transparent);
	block.setOutlineThickness(5.f);
	updateCurrentBlock();
}

GarageLocation::~GarageLocation()
{
	for(GraphicImage *playerIcon : playerIcons)
		delete playerIcon;
}

void GarageLocation::paint(RenderWindow *window)
{
	drawBackground(window);
	for(GraphicImage *playerIcon : playerIcons)
		window->draw(playerIcon->sprite);

	window->draw(player->getIcon());

	weaponItem.draw(window);
	if (m_state == CHOOSE_WEAPON && drawWeaponAmmo)
	{
		window->draw(ammo.valueText);
		window->draw(ammo.text);
	}
	artifactItem.draw(window);

	for (unsigned int i = 0; i < characters.size(); ++i)
		window->draw(characters.at(i)->getIcon());

	for(const TextInfo& characteristic : shopCharacteristics)
	{
		window->draw(characteristic.text);
		window->draw(characteristic.valueText);
	}
	window->draw(scoreValue);
	window->draw(pointsValue);

	window->draw(info);
	window->draw(weaponsLabel);
	window->draw(artifactsLabel);
	window->draw(charactersLabel);
	window->draw(playersLabel);
	window->draw(spaceShipLabel);
	window->draw(statsLabel);
	window->draw(infoLabel);
	window->draw(pointsLabel);
	window->draw(scoreLabel);

	//draw controls
	switch (m_state)
	{
	case CHOOSE_PLAYER:
		window->draw(choosePlayer.sprite);
		break;
	case CHOOSE_CHARACTERISTICS:
		window->draw(chooseCharacteristics);
		break;
	case CHOOSE_WEAPON:
		break;
	default:
		break;
	}
	window->draw(block);
}

void GarageLocation::eventFilter(Event *event)
{
	StateWindow::eventFilter(event);

	if (event->type != Event::KeyPressed)
		return;

	const ControlSettings controls = GlobalVariables::Instance().controls().at(currentPlayer);

	if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.moveRight)))
	{
		CONTROL_STATES newState = static_cast<CONTROL_STATES>(static_cast<int>(m_state) + 1);
		if (m_state == STATES_COUNT)
			newState = CHOOSE_PLAYER;
		setState(newState);
	}
	if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.moveLeft)))
	{
		CONTROL_STATES newState = static_cast<CONTROL_STATES>(static_cast<int>(m_state) - 1);
		if (m_state == UNKNOWN_STATE)
			newState = CHOOSE_ARTIFACT;
		setState(newState);
	}

	switch (m_state)
	{
	case CHOOSE_PLAYER:
	{
		bool needUpdate = false;
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.moveUp)))
		{
			currentPlayer--;
			if (currentPlayer < 0)
				currentPlayer = GameController::Instance().players().size() - 1;
			needUpdate = true;
		}
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.moveDown)))
		{
			currentPlayer++;
			if (currentPlayer >= GameController::Instance().players().size())
				currentPlayer = 0;
			needUpdate = true;
		}
		if (needUpdate)
		{
			choosePlayer.sprite.setPosition(playerIcons.at(currentPlayer)->sprite.getPosition());
			player = GameController::Instance().players().at(currentPlayer);
			updateCurrentArtifact();
			updateCurrentWeapon();
			updateCharacters();
			updateScore();
			updateColor();
			updateInfo();
			updateSpaceShip();
			updateCharacteristics(player->getStats());
		}
	}
		break;
	case CHOOSE_CHARACTERISTICS:
	{
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.moveUp)))
		{
			currentCharacteristic--;
			if (currentPlayer < 0)
				currentPlayer = shopCharacteristics.size() - 1;
			updateCurrentCharacteristic();
		}
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.moveDown)))
		{
			currentCharacteristic++;
			if (currentPlayer >= shopCharacteristics.size())
				currentPlayer = 0;
			updateCurrentCharacteristic();
		}
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.shoot)))
		{
			if (player->getStats().unused_points <= 0)
				return;

			player->removePoints(CHARACTERISTICS_COST);
			Characteristics stats =	player->getPureStats();
			switch (currentCharacteristic)
			{
			case 0:
				stats.damage += 0.1f;
				break;
			case 1:
				stats.attack_speed += 0.1f;
				break;
			case 2:
				stats.crit_chance += 0.1f;
				break;
			case 3:
				stats.crit_damage += 0.1f;
				break;
			case 4:
				stats.reflection += 0.1f;
				break;
			case 5:
				stats.health += 0.1f;
				break;
			case 6:
				stats.energy += 0.1f;
				break;
			case 7:
				stats.shield += 0.1f;
				break;
			case 8:
				stats.ability_cost += 0.1f;
				break;
			default:
				break;
			}
			player->setStats(stats);
			updateCharacteristics(player->getStats());
			updateScore();
		}
	}

		break;
	case CHOOSE_WEAPON:
	{
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.moveUp)))
		{
			if(weaponItem.current != 0)
				weaponItem.current--;
			updateCurrentWeapon();
		}
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.moveDown)))
		{
			if (weaponItem.current != weaponItem.items.size() - 1)
				weaponItem.current++;
			updateCurrentWeapon();
		}
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.shoot)))
		{
			const bool exists = hasWeapon(weaponItem.currentType);
			if (exists)
			{
				//ammo
				SpaceShip *player = GameController::Instance().players().at(currentPlayer);
				vector<AbstractWeapon*> weapons = player->getWeapons();
				for(AbstractWeapon *weapon : weapons)
					if (weapon->type() == weaponItem.currentType)
					{
						if (player->getData().money > weapon->getStats().ammoCost)
						{
							player->addScore(-weapon->getStats().ammoCost);
							weapon->suply(weapon->getStats().ammoSuply);
						}
						break;
					}
			}
			else
			{
				const WeaponStats stats = ObjectsFactory::Instance().weaponStats(static_cast<WEAPON_TYPES>(weaponItem.currentType));
				if (player->getData().money	> stats.cost)
				{
					player->buyWeapon(stats.cost, static_cast<WEAPON_TYPES>(weaponItem.currentType));
					updateScore();
				}
			}
		}
	}
		break;
	case CHOOSE_ARTIFACT:
	{
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.moveUp)))
		{
			if(artifactItem.current != 0)
				artifactItem.current--;
			updateCurrentArtifact();
		}
		if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(controls.moveDown)))
		{
			if (artifactItem.current != artifactItem.items.size() - 1)
				artifactItem.current++;
			updateCurrentArtifact();
		}
	}
		break;
	default:
		break;
	}
}

void GarageLocation::update()
{
	if (blinkBlockTimer.check(1000))
	{
		blinkColor = !blinkColor;
		block.setOutlineColor(blinkColor?Color::Yellow:Color::Black);
	}
	StateWindow::update();
}

void GarageLocation::back()
{
	Engine::Instance().setState(Engine::MAIN_LOCATION);
}

void GarageLocation::updateColor()
{
	for (unsigned int i = 0; i < playerIcons.size(); ++i)
	{
		if (i == currentPlayer)
			playerIcons.at(i)->sprite.setColor(Color::White);
		else
			playerIcons.at(i)->sprite.setColor(GlobalVariables::GrayColor);
	}
}

void GarageLocation::updateCharacteristics(const Characteristics& stats)
{
	shopCharacteristics.clear();

	const float textOffsetY = 32;
	const float x = statsLabel.getGlobalBounds().left;
	float y = statsLabel.getGlobalBounds().top + statsLabel.getGlobalBounds().height;

	const wstring delimer = L" : ";

	TextInfo shopCharacteristic;
	shopCharacteristic.text.setFillColor(Color::Blue);
	shopCharacteristic.text.setFont(GlobalVariables::Instance().font());
	shopCharacteristic.valueText.setFillColor(Color::Black);
	shopCharacteristic.valueText.setFont(GlobalVariables::Instance().font());
//	shopCharacteristic.text.scale(m_scaleFactor);
	shopCharacteristic.text.setString(Language::Instance().translate(Language::DAMAGE) + delimer);

	shopCharacteristic.text.setPosition(x, y);	
	shopCharacteristic.valueText.setString(String(GlobalVariables::to_string_with_precision(stats.damage, 1)));
	shopCharacteristic.valueText.setPosition(x + shopCharacteristic.text.getGlobalBounds().width, y);
	shopCharacteristics.push_back(shopCharacteristic);

	y += textOffsetY;
	shopCharacteristic.text.setString(Language::Instance().translate(Language::ATTACK_SPEED) + delimer);
	shopCharacteristic.text.setPosition(x, y);
	shopCharacteristic.valueText.setString(String(GlobalVariables::to_string_with_precision(stats.attack_speed, 1)));
	shopCharacteristic.valueText.setPosition(x + shopCharacteristic.text.getGlobalBounds().width, y);
	shopCharacteristics.push_back(shopCharacteristic);

	y += textOffsetY;
	shopCharacteristic.text.setString(Language::Instance().translate(Language::CRIT_CHANCE) + delimer);
	shopCharacteristic.text.setPosition(x, y);
	shopCharacteristic.valueText.setString(String(GlobalVariables::to_string_with_precision(stats.crit_chance, 1)));
	shopCharacteristic.valueText.setPosition(x + shopCharacteristic.text.getGlobalBounds().width, y);
	shopCharacteristics.push_back(shopCharacteristic);

	y += textOffsetY;
	shopCharacteristic.text.setString(Language::Instance().translate(Language::CRIT_DAMAGE) + delimer);
	shopCharacteristic.text.setPosition(x, y);
	shopCharacteristic.valueText.setString(String(GlobalVariables::to_string_with_precision(stats.crit_damage, 1)));
	shopCharacteristic.valueText.setPosition(x + shopCharacteristic.text.getGlobalBounds().width, y);
	shopCharacteristics.push_back(shopCharacteristic);

	y += textOffsetY;
	shopCharacteristic.text.setString(Language::Instance().translate(Language::REFLECTION) + delimer);
	shopCharacteristic.text.setPosition(x, y);
	shopCharacteristic.valueText.setString(String(GlobalVariables::to_string_with_precision(stats.reflection, 1)));
	shopCharacteristic.valueText.setPosition(x + shopCharacteristic.text.getGlobalBounds().width, y);
	shopCharacteristics.push_back(shopCharacteristic);

	y += textOffsetY;
	shopCharacteristic.text.setString(Language::Instance().translate(Language::HEALTH) + delimer);
	shopCharacteristic.text.setPosition(x, y);
	shopCharacteristic.valueText.setString(String(GlobalVariables::to_string_with_precision(stats.health, 1)));
	shopCharacteristic.valueText.setPosition(x + shopCharacteristic.text.getGlobalBounds().width, y);
	shopCharacteristics.push_back(shopCharacteristic);

	y += textOffsetY;
	shopCharacteristic.text.setString(Language::Instance().translate(Language::ENERGY) + delimer);
	shopCharacteristic.text.setPosition(x, y);
	shopCharacteristic.valueText.setString(String(GlobalVariables::to_string_with_precision(stats.energy, 1)));
	shopCharacteristic.valueText.setPosition(x + shopCharacteristic.text.getGlobalBounds().width, y);
	shopCharacteristics.push_back(shopCharacteristic);

	y += textOffsetY;
	shopCharacteristic.text.setString(Language::Instance().translate(Language::SHIELD) + delimer);
	shopCharacteristic.text.setPosition(x, y);
	shopCharacteristic.valueText.setString(String(GlobalVariables::to_string_with_precision(stats.shield, 1)));
	shopCharacteristic.valueText.setPosition(x + shopCharacteristic.text.getGlobalBounds().width, y);
	shopCharacteristics.push_back(shopCharacteristic);

	y += textOffsetY;
	shopCharacteristic.text.setString(Language::Instance().translate(Language::ABILITY_COST) + delimer);
	shopCharacteristic.text.setPosition(x, y);
	shopCharacteristic.valueText.setString(String(GlobalVariables::to_string_with_precision(stats.ability_cost, 1)));
	shopCharacteristic.valueText.setPosition(x + shopCharacteristic.text.getGlobalBounds().width, y);
	shopCharacteristics.push_back(shopCharacteristic);

//	y += textOffsetY;
//	shopCharacteristic.text.setString("Reload time: ");
//	shopCharacteristic.text.setPosition(x, y);
//	shopCharacteristic.valueText.setString(String(std::to_string(stats.reload_time)));
//	shopCharacteristic.valueText.setPosition(x + shopCharacteristic.text.getGlobalBounds().width, y);
//	shopCharacteristics.push_back(shopCharacteristic);

//	y += textOffsetY;
//	shopCharacteristic.text.setString("Duration: ");
//	shopCharacteristic.text.setPosition(x, y);
//	shopCharacteristic.valueText.setString(String(std::to_string(stats.duration)));
//	shopCharacteristic.valueText.setPosition(x + shopCharacteristic.text.getGlobalBounds().width, y);
//	shopCharacteristics.push_back(shopCharacteristic);
}

void GarageLocation::updateInfo()
{
	info.setString("dsgfsjkdgfkjshgkhsdf\nklghdsfkjlghkjds\nfhgkjdfshgkjdhfkgjhdsfjgkh\ndkjfghkldjhgkjdhgkhghd");
	switch (m_state)
	{
	case CHOOSE_PLAYER:
		info.setString("Choose player");
		break;
	case CHOOSE_CHARACTERISTICS:
		info.setString("Characteristics");
		break;
	case CHOOSE_WEAPON:
		break;
	case CHOOSE_ARTIFACT:
		break;
	default:
		break;
	}
}
#include "ResourcesManager/resourcesmanager.h"
void GarageLocation::updateCurrentWeapon()
{
	weaponItem.currentType = weaponItem.items.at(weaponItem.current).type;
	const bool isExists = hasWeapon(weaponItem.currentType);
	weaponItem.sprite.setTexture(ResourcesManager::Instance().getTexture(weaponItem.items.at(weaponItem.current).id));
	weaponItem.sprite.setColor(isExists?GlobalVariables::GrayColor:Color::White);
	weaponItem.sprite.setScale(Settings::Instance().getScaleFactor());

	weaponItem.upSprite.setColor(weaponItem.current == 0?GlobalVariables::GrayColor:Color::White);
	weaponItem.downSprite.setColor(weaponItem.current == weaponItem.items.size() - 1?GlobalVariables::GrayColor:Color::White);

	drawWeaponAmmo = hasWeapon(weaponItem.currentType);
	if (drawWeaponAmmo)
	{
		SpaceShip *player = GameController::Instance().players().at(currentPlayer);
		vector<AbstractWeapon*> weapons = player->getWeapons();
		for(AbstractWeapon *weapon : weapons)
			if (weapon->type() == weaponItem.currentType)
			{
				ammo.text.setString(to_string(weapon->getAmmo()));
				ammo.valueText.setString("+" + to_string(weapon->getStats().ammoSuply));
				break;
			}
	}
	updateCharacteristics(player->getStats());
}

void GarageLocation::updateCurrentArtifact()
{
	bool isExists = false;
	SpaceShip *player = GameController::Instance().players().at(currentPlayer);
	Artifact* artifact = player->getAtrifact();
	artifactItem.currentType = artifactItem.items.at(artifactItem.current).type;

	if (artifact != nullptr && artifact->getType() == artifactItem.currentType)
		isExists = true;

	artifactItem.sprite.setTexture(ResourcesManager::Instance().getTexture(artifactItem.items.at(artifactItem.current).id));
	artifactItem.sprite.setColor(isExists?GlobalVariables::GrayColor:Color::White);
	artifactItem.sprite.setScale(Settings::Instance().getScaleFactor());

	artifactItem.upSprite.setColor(artifactItem.current == 0?GlobalVariables::GrayColor:Color::White);
	artifactItem.downSprite.setColor(artifactItem.current == artifactItem.items.size() - 1?GlobalVariables::GrayColor:Color::White);

	updateCharacteristics(player->getStats());
}

void GarageLocation::updateCharacters()
{
	characters = player->getCharacters();
	const float posX = charactersLabel.getGlobalBounds().left;
	float posY = charactersLabel.getGlobalBounds().top+ charactersLabel.getGlobalBounds().height;
	for (unsigned int i = 0; i < characters.size(); ++i)
	{
		characters.at(i)->getIcon().setPosition(posX, posY);
		posY += GlobalVariables::CELL_WIDTH;
		posY += 16;
	}
}

void GarageLocation::updateSpaceShip()
{
	player->getIcon().setPosition(spaceShipLabel.getGlobalBounds().left, spaceShipLabel.getGlobalBounds().top + spaceShipLabel.getGlobalBounds().height);
}

void GarageLocation::initLabels()
{
	weaponsLabel.setString(Language::Instance().translate(Language::WEAPONS));
	weaponsLabel.setFillColor(Color::Magenta);
	weaponsLabel.setFont(GlobalVariables::Instance().font());
	weaponsLabel.setPosition(Settings::Instance().getResolution().x * 0.5f, Settings::Instance().getResolution().y * 0.1f);

	artifactsLabel.setString(Language::Instance().translate(Language::ARTIFACTS));
	artifactsLabel.setFillColor(Color::Magenta);
	artifactsLabel.setFont(GlobalVariables::Instance().font());
	artifactsLabel.setPosition(Settings::Instance().getResolution().x * 0.6f, Settings::Instance().getResolution().y * 0.1f);

	playersLabel.setString(Language::Instance().translate(Language::PLAYERS));
	playersLabel.setFillColor(Color::Magenta);
	playersLabel.setFont(GlobalVariables::Instance().font());
	playersLabel.setPosition(Settings::Instance().getResolution().x * 0.05f, Settings::Instance().getResolution().y * 0.05f);

	scoreLabel.setString(Language::Instance().translate(Language::GEARS));
	scoreLabel.setFillColor(Color::Magenta);
	scoreLabel.setFont(GlobalVariables::Instance().font());
	scoreLabel.setPosition(Settings::Instance().getResolution().x * 0.2f, Settings::Instance().getResolution().y * 0.1f);

	pointsLabel.setString(Language::Instance().translate(Language::POINTS));
	pointsLabel.setFillColor(Color::Magenta);
	pointsLabel.setFont(GlobalVariables::Instance().font());
	pointsLabel.setPosition(Settings::Instance().getResolution().x * 0.2f, Settings::Instance().getResolution().y * 0.2f);

	statsLabel.setString(Language::Instance().translate(Language::STATS));
	statsLabel.setFillColor(Color::Magenta);
	statsLabel.setFont(GlobalVariables::Instance().font());
	statsLabel.setPosition(Settings::Instance().getResolution().x * 0.2f, Settings::Instance().getResolution().y * 0.3f);

	infoLabel.setString(Language::Instance().translate(Language::INFORMATION));
	infoLabel.setFillColor(Color::Magenta);
	infoLabel.setFont(GlobalVariables::Instance().font());
	infoLabel.setPosition(Settings::Instance().getResolution().x * 0.51f, Settings::Instance().getResolution().y * 0.6f);

	charactersLabel.setString(Language::Instance().translate(Language::CHARACTERS));
	charactersLabel.setFillColor(Color::Magenta);
	charactersLabel.setFont(GlobalVariables::Instance().font());
	charactersLabel.setPosition(Settings::Instance().getResolution().x * 0.86f, Settings::Instance().getResolution().y * 0.1f);

	spaceShipLabel.setString(Language::Instance().translate(Language::SPACESHIP));
	spaceShipLabel.setFillColor(Color::Magenta);
	spaceShipLabel.setFont(GlobalVariables::Instance().font());
	spaceShipLabel.setPosition(Settings::Instance().getResolution().x * 0.86f,
							   3 * (GlobalVariables::CELL_WIDTH + 10) + charactersLabel.getPosition().y + charactersLabel.getGlobalBounds().height);
}

void GarageLocation::updateScore()
{
	scoreValue.setFillColor(Color::Magenta);
	scoreValue.setFont(GlobalVariables::Instance().font());
	scoreValue.setPosition(scoreLabel.getGlobalBounds().left, scoreLabel.getGlobalBounds().top + scoreLabel.getGlobalBounds().height);

	pointsValue.setFillColor(Color::Magenta);
	pointsValue.setFont(GlobalVariables::Instance().font());
	pointsValue.setPosition(pointsLabel.getGlobalBounds().left, pointsLabel.getGlobalBounds().top + pointsLabel.getGlobalBounds().height);

	scoreValue.setString(String(GlobalVariables::to_string_with_precision(player->getData().money, 2)));
	pointsValue.setString(String(std::to_string(player->getStats().unused_points)));
}

void GarageLocation::setState(GarageLocation::CONTROL_STATES state)
{
	if (m_state == state)
		return;

	switch (state)
	{
	case CHOOSE_PLAYER:
		break;
	case CHOOSE_CHARACTERISTICS:
		updateCurrentCharacteristic();
		break;
	case CHOOSE_WEAPON:
		break;
	default:
		break;
	}
	m_state = state;
	updateCurrentBlock();
}

void GarageLocation::updateCurrentCharacteristic()
{
	const TextInfo characteristic = shopCharacteristics.at(currentCharacteristic);
	const float margin = 3.f;
	chooseCharacteristics.setPosition(characteristic.text.getPosition().x - margin, characteristic.text.getPosition().y - margin);
	chooseCharacteristics.setOutlineColor(Color::Red);
	chooseCharacteristics.setFillColor(Color::Transparent);
	chooseCharacteristics.setOutlineThickness(margin);
	chooseCharacteristics.setSize(Vector2f(
									  characteristic.text.getGlobalBounds().width + characteristic.valueText.getGlobalBounds().width + margin*2,
									  characteristic.text.getGlobalBounds().height + characteristic.valueText.getGlobalBounds().height + margin*2
									  ));
	updateInfo();
}

bool GarageLocation::hasWeapon(const int type) const
{
	bool isExists = false;
	SpaceShip *player = GameController::Instance().players().at(currentPlayer);
	vector<AbstractWeapon*> weapons = player->getWeapons();
	for(AbstractWeapon *weapon : weapons)
		if (weapon->type() == type)
		{
			isExists = true;
			break;
		}
	return isExists;
}

void GarageLocation::updateCurrentBlock()
{
	switch (m_state)
	{
	case CHOOSE_PLAYER:
	{
		block.setPosition(playersLabel.getPosition());
		block.setSize(Vector2f(150, 900));
	}
		break;
	case CHOOSE_CHARACTERISTICS:
	{
		block.setPosition(statsLabel.getPosition());
		block.setSize(Vector2f(500, 900));
	}
		break;
	case CHOOSE_WEAPON:
	{
		block.setPosition(weaponsLabel.getPosition());
		block.setSize(Vector2f(150, 200));
	}
		break;
	case CHOOSE_ARTIFACT:
	{
		block.setPosition(artifactsLabel.getPosition());
		block.setSize(Vector2f(150, 200));
	}
		break;
	default:
		break;
	}
}

void GarageLocation::ShopItem::draw(RenderWindow *window)
{
	window->draw(sprite);
	window->draw(upSprite);
	window->draw(downSprite);
}
