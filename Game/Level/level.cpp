#include "level.h"
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
#include "enemy.h"
#include "tower.h"

constexpr static int TEXTURE_HEIGHT = 2048;
constexpr static int MAX_SHAKE_OFFSET = 5;
constexpr static int SHAKE_DURATION = 500;

Level::Level() :
	m_finished(false)
  ,m_isFailed(false)
  ,difficulty(1.f)
  ,gameMap(nullptr)
  ,m_state(READY)
  ,life(0.f)
{
	const float k = static_cast<float>(Settings::Instance().getInscribedResolution().y)/Settings::Instance().getResolution().y;
	resolutionOffsetX = Settings::Instance().getResolution().x - k * Settings::Instance().getResolution().x;
	resolutionOffsetX /= 2;
}

Level::~Level()
{

}

void Level::draw(RenderTarget *const target)
{
	target->draw(m_background);
	target->setView(*Engine::Instance().camera()->getView());
	drawLevel(target);
	target->draw(deadZone);

	Engine::Instance().panel()->draw(target);
}

void Level::update()
{
	if (timer.check(GlobalVariables::FRAME_TIME))
	{
		calculateCollisions();
		checkDeadZone();
		checkEnd();

		for(Enemy* enemy : enemies)
		{
			if (enemy->moveStep())
			{
				const Vector2i cell = Engine::Instance().camera()->posToCell(enemy->pos());
				if (cell == gameMap->endPos)
					continue;

				const int direction = getTileDirectionByCell(cell);
				enemy->moveNext(direction);
			}
		}
	}

	for(Enemy* enemy : enemies)
		enemy->update();

//	Effects::Instance().update();

	Engine::Instance().panel()->update();
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
	life = 100.f;
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
//	for (auto it = enemies.begin(); it != enemies.end();)
//	{
//		const Vector2f pos = it->spaceShip->pos();
//		if (pos.x > rightBorderVal||
//				pos.x < leftBorderVal ||
//				pos.y > bottomBorderVal ||
//				pos.y < topBorderVal)
//		{
//			delete it->spaceShip;
//			it = enemies.erase(it);
//		}
//		else
//			++it;
//	}
}

void Level::checkEnd()
{
	for (auto it = enemies.begin(); it != enemies.end();)
	{
		Enemy *enemy = *it;
		const Vector2i cell = Engine::Instance().camera()->posToCell(enemy->pos());
		if (cell == gameMap->endPos)
		{
			hitPlayer(enemy->getData().damage);
			delete enemy;
			it = enemies.erase(it);
		}
		else
			++it;
	}
}

Tower *Level::getTowerAtPos(const Vector2f &pos)
{
	for(Tower* tower : towers)
	{
		if (tower->pos() == pos)
			return tower;
	}
	return nullptr;
}

void Level::hitPlayer(float damage)
{
	cout << "HiT"<<endl;
	life -= damage;
	if (life <= 0.f)
	{
		//game over
	}
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

	for(Enemy* enemy : enemies)
		enemy->draw(target);

	for(Tower *tower : towers)
		tower->draw(target);

	Engine::Instance().cursor()->draw(target);
	//	Effects::Instance().draw(target);
}

void Level::spawn()
{
	Enemy *enemy = EnemiesFactory::createEnemy(EnemiesFactory::SMALL_SLOW, Engine::Instance().camera()->cellToPos(gameMap->spawnPos));
	enemy->moveNext(gameMap->spawnDirection);
	enemies.push_back(enemy);
}

void Level::test()
{

}

Tile Level::getTileByPos(const Vector2f &pos)
{
	const Vector2i cell = Engine::Instance().camera()->posToCell(pos);
	return getTileByCell(cell);
}

Tile Level::getTileByCell(const Vector2i &cell) const
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

void Level::chooseCurrent()
{
	choose(Engine::Instance().cursor()->cell(), Engine::Instance().cursor()->inPanel());
}

int Level::getTileDirectionByCell(const Vector2i& cell) const
{
	const int id = getTileByCell(cell).id;

	if(gameMap->tileProperties.find(id) != gameMap->tileProperties.end())
	{
		const Tile::TileProperties properties = gameMap->tileProperties.at(id);
		return properties.direction;
	}
	return 0;
}

void Level::chooseByPos(const Vector2f &pos)
{
	const Vector2i cell = Engine::Instance(). camera()->posToCell(pos);
	const int bottomCell = Engine::Instance().camera()->topCell() + Engine::Instance().camera()->currentViewCells().y - Engine::Instance().panel()->cellsCount();
	const bool inPanel = cell.y > bottomCell;
	choose(Engine::Instance().camera()->posToCell(pos), inPanel);
}

void Level::choose(const Vector2i &cell, bool inPanel)
{
	if (inPanel)
	{
		cout << "PANEL CLICK"<<endl;
//		m_state = ABILITY_FREEZE_BOMB;
//		m_state = ABILITY_BOMB;
		m_state = ADD_TOWER;

		switch (m_state)
		{
		case READY:
			break;
		case ADD_TOWER:
		{
			const TOWER_TYPES type = BASE;

			Engine::Instance().cursor()->activateTower(30, type);
		}
			break;
		case ABILITY_INCREASE_TOWER_ATTACK_SPEED:
		case ABILITY_INCREASE_TOWER_DAMAGE:
			Engine::Instance().cursor()->activateAbility(1, 1, 0, 0);
			break;
		case ABILITY_CARPET_BOMBING:
			Engine::Instance().cursor()->activateAbility(10, 3, 5, 2);
			break;
		case ABILITY_BOMB:
		case ABILITY_FREEZE_BOMB:
			Engine::Instance().cursor()->activateAbility(3, 3, 1, 1);
		break;
		}
	}
	else
	{
		switch (m_state)
		{
		case READY:

			break;
		case ADD_TOWER:
		{
			const int direction = Engine::Instance().level()->getTileDirectionByCell(cell);
			if (direction != 0)
				return;
			const Vector2f pos = Engine::Instance().camera()->cellToPos(cell);
			Tower *tower = new Tower(RESOURCES::TOWER_BASE, pos);
			towers.push_back(tower);
		}
			break;
		case ABILITY_CARPET_BOMBING:

			break;
		case ABILITY_BOMB:
			for (auto it = enemies.begin(); it != enemies.end();)
			{
				Enemy *enemy = *it;
				if (enemy->gameRect().intersects(Engine::Instance().cursor()->getAbilityRect()))
				{
					enemy->hit(15);
					if (!enemy->isAlive())
					{
						delete enemy;
						it = enemies.erase(it);
					}
					else
						++it;
				}
				else
					++it;
			}
			break;
		case ABILITY_FREEZE_BOMB:
			for(Enemy *enemy : enemies)
			{
				if (enemy->gameRect().intersects(Engine::Instance().cursor()->getAbilityRect()))
				{
					cout << "FREEZE" << endl;
					enemy->freeze(15.f, 3000);
				}
			}
			break;
		case ABILITY_INCREASE_TOWER_ATTACK_SPEED:
		{
			Tower* tower = getTowerAtPos(Engine::Instance().camera()->cellToPos(cell));
		}
			break;
		case ABILITY_INCREASE_TOWER_DAMAGE:
		{
			Tower* tower = getTowerAtPos(Engine::Instance().camera()->cellToPos(cell));
		}
			break;
		}
		Engine::Instance().cursor()->deactivate();
	}
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
