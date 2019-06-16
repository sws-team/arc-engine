﻿#include "level.h"
#include "Game/gameobject.h"
#include "settings.h"
#include "Game/effects.h"
#include "Game/gamepanel.h"
#include "Game/Audio/soundcontroller.h"
#include "globalvariables.h"
#include "Game/Collisions/collisions.h"
#include "savedgame.h"
#include "ResourcesManager/resourcesmanager.h"
#include "Game/Level/camera.h"
#include "Engine/engine.h"
#include "cursor.h"

constexpr static int TEXTURE_HEIGHT = 2048;
constexpr static int MAX_SHAKE_OFFSET = 5;
constexpr static int SHAKE_DURATION = 500;

Level::Level() :
	m_finished(false)
  ,m_isFailed(false)
  ,difficulty(1.f)
  ,gameMap(nullptr)
{
	Engine::Instance().camera()->init();
	panel = new GamePanel(this);
	const float k = static_cast<float>(Settings::Instance().getInscribedResolution().y)/Settings::Instance().getResolution().y;
	resolutionOffsetX = Settings::Instance().getResolution().x - k * Settings::Instance().getResolution().x;
	resolutionOffsetX /= 2;
}

Level::~Level()
{
	delete panel;
}

void Level::draw(RenderTarget *const target)
{	
	target->draw(m_background);
	target->setView(*Engine::Instance().camera()->getView());
	drawLevel(target);
	target->draw(deadZone);

	panel->draw(target);
}

void Level::update()
{
	if (timer.check(GlobalVariables::FRAME_TIME))
	{
		calculateCollisions();
		checkDeadZone();

		for(GameObject* enemy : enemies)
		{
			const int id = getTileByPos(enemy->pos()).id;
			Tile::TileProperties properties = gameMap->tileProperties.at(id);
			moveEnemy(enemy, properties.direction);
		}
	}
//	for(SpaceShip *spaceShip : GameController::Instance().players())
//	{
//		spaceShip->updateWithLevel(this);
//		spaceShip->update();
//	}



	Effects::Instance().update();

	panel->update();
}

Vector2f Level::getCenter() const
{
//	return view.getCenter();
}

Vector2f Level::getStartingPos() const
{
	return m_startPos;
}

void Level::startMission(const unsigned int n)
{
//	this->mission = Campaign::Instance().missions.at(n);
	SoundController::Instance().startBackgroundSound("sounds/map1.ogg");
//	difficulty = 1.f + static_cast<float>(SavedGameLoader::Instance().getSavedGame().completedLevels.size()) / 10;
//	fillLevel();

	gameMap = SavedGameLoader::Instance().maps.at(n);

	testTexture.loadFromFile("tiles.png", IntRect(0,0,32,32));
	testTexture.setRepeated(true);

	const Vector2i minPos = Vector2i(-Settings::Instance().getResolution().x,
									 -Settings::Instance().getResolution().y);

	deadZone.setPosition(minPos.x, minPos.y);
	deadZone.setOutlineThickness(3.f);
	deadZone.setSize(Vector2f(abs(minPos.x) * 3, abs(minPos.y) * 3));
	deadZone.setOutlineColor(Color::Red);
	deadZone.setFillColor(Color::Transparent);
	m_background.setTexture(testTexture);
	m_background.setPosition(minPos.x, minPos.y);
	m_background.setTextureRect(IntRect(minPos.x, minPos.y, abs(minPos.x) * 3, abs(minPos.y) * 3));
}

void Level::calculateCollisions()
{

}

void Level::checkDeadZone()
{

}

void Level::moveEnemy(GameObject *enemy, int direction)
{
	Vector2f d;
	d.x = 0;
	d.y = 0;
	switch (direction)
	{
	case Map::STAY:
		break;
	case Map::UP:
		d.y = -GlobalVariables::Instance().tileSize().y;
		break;
	case Map::DOWN:
		d.y = GlobalVariables::Instance().tileSize().y;
		break;
	case Map::LEFT:
		d.x = -GlobalVariables::Instance().tileSize().x;
		break;
	case Map::RIGHT:
		d.x = GlobalVariables::Instance().tileSize().x;
		break;
	}
	enemy->move(d.x, d.y);
}

//void Level::fillLevel()
//{
//	m_startPos.x = static_cast<float>(Settings::Instance().getResolution().x)/2;
//	m_startPos.y = static_cast<float>(Settings::Instance().getResolution().y)/2;

//	m_topBorder = view.getCenter().y - view.getSize().y/2;
//	m_bottomBorder = view.getCenter().y + view.getSize().y/2;

//	panel->setLeftBorder(leftBorder());

	//	SoundController::Instance().startBackgroundSound(mission.backgroundSound);
//}

void Level::drawLevel(RenderTarget * const target)
{
	for (size_t layer = 0; layer < gameMap->layers.size(); layer++)
		for (size_t tile = 0; tile < gameMap->layers[layer].tiles.size(); tile++)
			target->draw(gameMap->layers[layer].tiles[tile].sprite);

	for(GameObject* enemy : enemies)
	{
		enemy->draw(target);
	}

	Engine::Instance().cursor()->draw(target);
	Effects::Instance().draw(target);
}

void Level::spawn()
{
	GameObject *enemy = new GameObject(RESOURCES::ENEMY_TEXTURE, Engine::Instance().camera()->cellToPos(gameMap->spawnPos), Vector2i(32, 32), 4);
	moveEnemy(enemy, gameMap->spawnDirection);
	enemies.push_back(enemy);
}

Tile Level::getTileByPos(const Vector2f &pos)
{
	const Vector2i cell = Engine::Instance().camera()->posToCell(pos);
	return getTileByCell(cell);
}

Tile Level::getTileByCell(const Vector2i &cell)
{
	const int layer = 0;
	for (unsigned int i = 0; i < gameMap->layers[layer].tiles.size(); i++)
	{
		const Tile tile = gameMap->layers[layer].tiles[i];
		if (tile.cell == cell)
			return tile;

	}
	return Tile();
}

bool Level::isFinished() const
{
	return m_finished;
}

bool Level::isFailed() const
{
	return m_isFailed;
}

void Level::action()
{

}

void Level::change()
{

}
