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

constexpr static int TEXTURE_HEIGHT = 2048;
constexpr static int MAX_SHAKE_OFFSET = 5;
constexpr static int SHAKE_DURATION = 500;

Level::Level() :
	m_finished(false)
  ,m_isFailed(false)
  ,difficulty(1.f)
{
	Engine::Instance().camera()->init();
	panel = new GamePanel(this);
	const float k = static_cast<float>(Settings::Instance().getInscribedResolution().y)/Settings::Instance().getResolution().y;
	resolutionOffsetX = Settings::Instance().getResolution().x - k * Settings::Instance().getResolution().x;
	resolutionOffsetX /= 2;

	minimapRect.setSize(Vector2f(Settings::Instance().getResolution().x * Settings::GAME_SCALE * 0.3f,
								 Settings::Instance().getResolution().y * Settings::GAME_SCALE * 0.3f));
	minimapRect.setPosition(-10, -10);
	minimapRect.setFillColor(Color::Red);
}

Level::~Level()
{
	delete panel;
}

void Level::draw(RenderTarget *const target)
{	
	target->draw(m_background);
	panel->draw(target);

	target->setView(*Engine::Instance().camera()->getView());

	drawLevel(target);

	rTexture.setView(*Engine::Instance().camera()->getMiniMapView());

	rTexture.create(Settings::Instance().getResolution().x, Settings::Instance().getResolution().y);
	rTexture.clear(Color::Blue);
	drawLevel(&rTexture);
	rTexture.display();

	Sprite sprite;
	sprite.setTexture(rTexture.getTexture());
	sprite.setPosition(0, 0);
	sprite.scale(Settings::GAME_SCALE * 0.3f, Settings::GAME_SCALE * 0.3f);

	target->draw(minimapRect);
	target->draw(sprite);
}

void Level::update()
{
	if (timer.check(GlobalVariables::FRAME_TIME))
	{
		calculateCollisions();
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
//	Map *gameMap = SavedGameLoader::Instance().maps.at(n);

	testTexture.loadFromFile("tiles.png", IntRect(0,0,32,32));
	testTexture.setRepeated(true);

	const int minPos = -99999;
	m_background.setTexture(testTexture);
	m_background.setPosition(minPos, minPos);
	m_background.setTextureRect(IntRect(minPos, minPos, abs(minPos) * 2, abs(minPos) * 2));
}

void Level::calculateCollisions()
{

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
	Map *map1 = SavedGameLoader::Instance().maps.at(0);
	for (size_t layer = 0; layer < map1->layers.size(); layer++)
		for (size_t tile = 0; tile < map1->layers[layer].tiles.size(); tile++)
			target->draw(map1->layers[layer].tiles[tile].sprite);

	Engine::Instance().cursor()->draw(target);
	Effects::Instance().draw(target);
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
