#include "level.h"
#include "Game/gameobject.h"
#include "settings.h"
#include "effects.h"
#include "gamepanel.h"
#include "Game/Audio/soundcontroller.h"
#include "globalvariables.h"
#include "Collisions/collisions.h"
#include "savedgame.h"
#include "ResourcesManager/resourcesmanager.h"
#include "Game/Level/camera.h"

constexpr static int TEXTURE_HEIGHT = 2048;
constexpr static int MAX_SHAKE_OFFSET = 5;
constexpr static int SHAKE_DURATION = 500;

Level::Level() :
	m_finished(false)
  ,m_isFailed(false)
  ,difficulty(1.f)
{
	panel = new GamePanel(this);
	const float k = static_cast<float>(Settings::Instance().getInscribedResolution().y)/Settings::Instance().getResolution().y;
	resolutionOffsetX = Settings::Instance().getResolution().x - k * Settings::Instance().getResolution().x;
	resolutionOffsetX /= 2;
}

Level::~Level()
{
	delete panel;
}

void Level::draw(RenderWindow *const window)
{
	p_camera->setView(window);
	drawLevel(window);
	p_camera->restore(window);
	panel->draw(window);
	drawLevel(window);
	p_camera->restore(window);
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

float Level::leftBorder() const
{
//	return view.getCenter().x - view.getSize().x/2;
}

float Level::rightBorder() const
{
//	return resolutionOffsetX + view.getCenter().x + view.getSize().x/2;
}

float Level::topBorder() const
{
	return m_topBorder + panel->height();
}

float Level::bottomBorder() const
{
	return m_bottomBorder + resolutionOffsetX/2;
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
	fillLevel();
}

void Level::calculateCollisions()
{

}

void Level::fillLevel()
{
	updateRatio();
//	m_startPos.x = static_cast<float>(Settings::Instance().getResolution().x)/2;
//	m_startPos.y = static_cast<float>(Settings::Instance().getResolution().y)/2;

//	m_topBorder = view.getCenter().y - view.getSize().y/2;
//	m_bottomBorder = view.getCenter().y + view.getSize().y/2;

//	panel->setLeftBorder(leftBorder());

	//	SoundController::Instance().startBackgroundSound(mission.backgroundSound);
}

void Level::updateRatio()
{
	//	view.setSize(Vector2f(Settings::Instance().getResolution().x* m_ratio, Settings::Instance().getResolution().y* m_ratio));
}

void Level::drawLevel(RenderWindow * const window)
{
	Map *map1 = SavedGameLoader::Instance().maps.at(0);
	for (size_t layer = 0; layer < map1->layers.size(); layer++)
		for (size_t tile = 0; tile < map1->layers[layer].tiles.size(); tile++)
			window->draw(map1->layers[layer].tiles[tile].sprite);

	Effects::Instance().draw(window);
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

void Level::setCamera(Camera *camera)
{
	p_camera = camera;
//	camera->init();
}
