#include "gamepanel.h"
#include "globalvariables.h"
#include "settings.h"
#include "Level/level.h"
#include "ResourcesManager/resourcesmanager.h"
#include "Engine/engine.h"
#include "Level/camera.h"

#include <stdlib.h>

const float GamePanel::PANEL_HEIGHT = GlobalVariables::CELL_SIZE * GamePanel::PANEL_CELLS_COUNT;

GamePanel::GamePanel(Level *level) :
	GameDrawable()
  ,p_level(level)
{
	m_sprite.setTexture(ResourcesManager::Instance().getTexture(RESOURCES::PANEL_TEXTURE));
}

GamePanel::~GamePanel()
{

}

void GamePanel::draw(RenderTarget * const target)
{
	m_sprite.setScale(target->getView().getSize().x / PANEL_WIDTH,
					  (target->getView().getSize().y / PANEL_HEIGHT) * PANEL_SCALE);

	const float left = target->getView().getCenter().x - target->getView().getSize().x/2;
	const float top = target->getView().getCenter().y + target->getView().getSize().y/2 - m_sprite.getGlobalBounds().height;

	m_sprite.setPosition(left, top);

	target->draw(m_sprite);


	//draw minimap
	rTexture.setView(*Engine::Instance().camera()->getMiniMapView());
	rTexture.create(static_cast<unsigned int>(Settings::Instance().getResolution().x),
					static_cast<unsigned int>(Settings::Instance().getResolution().y));
	rTexture.clear(Color::Transparent);
	p_level->drawLevel(&rTexture);
	rTexture.display();

	Sprite sprite;
	sprite.setTexture(rTexture.getTexture());
	sprite.setPosition(0, 0);
	sprite.scale(Settings::GAME_SCALE * 0.25f, Settings::GAME_SCALE * 0.25f);

	const float minimapOffset = m_sprite.getGlobalBounds().height - sprite.getGlobalBounds().height;


	sprite.setPosition(m_sprite.getPosition().x + m_sprite.getGlobalBounds().width - sprite.getGlobalBounds().width + minimapOffset,
					   m_sprite.getPosition().y);

	target->draw(sprite);
}

void GamePanel::update()
{

}

