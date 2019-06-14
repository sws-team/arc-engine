#include "gamepanel.h"
#include "globalvariables.h"
#include "settings.h"
#include "Level/level.h"
#include "ResourcesManager/resourcesmanager.h"

#include <stdlib.h>

const int CHARACTERS_SIZE = 22;

GamePanel::GamePanel(Level *level) :
	GameDrawable()
  ,p_level(level)
{
	const Vector2f scaleFactor = Settings::Instance().getScaleFactor();
	m_cellSize.x = CELL_SIZE * scaleFactor.x;
	m_cellSize.y = CELL_SIZE * scaleFactor.y;
	m_panelHorizontalOffset = HORIZONTAL_OFFSET * scaleFactor.x;
	m_panelVerticalOffset = VERTICAL_OFFSET * scaleFactor.y;

	m_height = 64 * scaleFactor.y;
}

GamePanel::~GamePanel()
{

}

void GamePanel::draw(RenderTarget * const target)
{
/*
		const float startPosX = i % 2 == 0 ? 0 : Settings::Instance().getResolution().x/2;
		const float startPoxY = i > 1 ? m_height : 0;

		SpaceShip *player = GameController::Instance().players().at(i);

		float posX = startPosX;
		float posY = startPoxY;

		posY += m_top;

		panels.at(i).sprite.setPosition(posX, posY);
		window->draw(panels.at(i).sprite);

		const int texturePosX = odd ? 0 : 480;
		posX += texturePosX * Settings::Instance().getScaleFactor().x;

		posX += m_panelHorizontalOffset;
		posY += m_panelVerticalOffset;

		player->getScaledIcon().setPosition(posX, posY);
		window->draw(player->getScaledIcon());

		posX += m_cellSize.x + m_panelHorizontalOffset;

		AbstractWeapon *weapon = player->getCurrentWeapon();
		if (weapon != nullptr)
		{
			weapon->getScaledIcon().setPosition(posX, posY);
			posX += m_cellSize.x + m_panelHorizontalOffset/2;
			window->draw(weapon->getScaledIcon());

			panels.at(i).ammoText.setPosition(posX, posY + m_panelHorizontalOffset/2);
			panels.at(i).ammoText.setString(std::to_string(weapon->getAmmo()));
			window->draw(panels.at(i).ammoText);
		}
		posX += AMMO_SIZE * Settings::Instance().getScaleFactor().x + m_panelHorizontalOffset;

		const float scaledLineWidth = Settings::Instance().getScaleFactor().x * LINE_WIDTH;

		{//shield
			const float shieldRate = player->getData().shield / (player->getDefaultData().shield + player->getDefaultData().shield * player->getStats().shield);
			panels.at(i).shield.setPos(Vector2f(posX, posY));
			panels.at(i).shield.setValue(shieldRate);
			panels.at(i).shield.draw(window);
		}

		posX += scaledLineWidth + m_panelHorizontalOffset;
		{//health
			const float healthRate = player->getData().health / (player->getDefaultData().health + player->getDefaultData().health * player->getStats().health);
			panels.at(i).health.setPos(Vector2f(posX, posY));
			panels.at(i).health.setValue(healthRate);
			panels.at(i).health.draw(window);
		}

		posX += scaledLineWidth + m_panelHorizontalOffset;

		{//energy
			const float energyRate = player->getData().energy / (player->getDefaultData().energy + player->getDefaultData().energy * player->getStats().energy);
			panels.at(i).energy.setPos(Vector2f(posX, posY));
			panels.at(i).energy.setValue(energyRate);
			panels.at(i).energy.draw(window);
		}
		posX += scaledLineWidth + m_panelHorizontalOffset;

		Character *character = player->getCurrentCharacter();
		if (character != nullptr)
		{
			character->getScaledIcon().setPosition(posX, posY);
			window->draw(character->getScaledIcon());
		}
		posX += m_cellSize.x + m_panelHorizontalOffset;

		//ability
		if (character != nullptr)
		{
			character->getAbilityIcon().setPosition(posX, posY);
			window->draw(character->getAbilityIcon());
		}
		posX += m_cellSize.x + m_panelHorizontalOffset;

		//artifact
		Artifact *artifact = player->getAtrifact();
		if (artifact != nullptr)
		{
			artifact->getScaledIcon().setPosition(posX, posY);
			window->draw(artifact->getScaledIcon());
		}
		posX += m_cellSize.x + m_panelHorizontalOffset;

		//dollar
		posX += 16;
		//score
		panels.at(i).scoreText.setPosition(posX, posY + m_panelHorizontalOffset/2);
		const float score = p_level->getLevelBonuses().at(player);
		panels.at(i).scoreText.setString(GlobalVariables::to_string_with_precision(score, 1));
		window->draw(panels.at(i).scoreText);
	*/
}

void GamePanel::update()
{

}

void GamePanel::setLeftBorder(float left)
{
	m_left = left;
}

float GamePanel::height() const
{
	return m_height * panels.size();
}

void GamePanel::setTop(float y)
{
	m_top = y;
}

