#include "loadgamewindow.h"
#include "settings.h"
#include "globalvariables.h"
#include "Engine/engine.h"
#include "ResourcesManager/resourcesmanager.h"

LoadGameWindow::LoadGameWindow()
	: StateWindow()
	,currentPanel(0)
{
	setBackground(RESOURCES::LOAD_GAME_BACKGROUND);

	const float offset = Settings::Instance().getScaleFactor().y * 20;
	const float x = Settings::Instance().getResolution().x * 0.3f;
	float y = Settings::Instance().getResolution().y * 0.1f;

	for (unsigned int i = 0; i < SavedGameLoader::MAX_SAVE_SLOTS; ++i)
	{
		SavedGamePanel *panel = new SavedGamePanel;

		if (i < SavedGameLoader::Instance().savedGames().size())
		{
			const SavedGame save = SavedGameLoader::Instance().savedGames().at(i);
			panel->newGame = false;

			float iconPosX = x;
			for(const GameState& state : save.playersData)
			{
				iconPosX += offset;
				const int character = static_cast<int>(state.crew.at(0));
				Texture &texture = ResourcesManager::Instance().getTexture(static_cast<RESOURCES::TEXTURE_TYPE>(RESOURCES::RANDOM_CHARACTER_ICON + character));
				Sprite icon;
				icon.setTexture(texture);
				icon.setTextureRect(IntRect(0, 0, GlobalVariables::CELL_WIDTH, GlobalVariables::CELL_WIDTH));
				icon.scale(Settings::Instance().getScaleFactor());
				icon.setPosition(iconPosX, y + offset);
				iconPosX += GlobalVariables::CELL_WIDTH;

				panel->charactersIcons.push_back(icon);
			}
		}
		else
			panel->newGame = true;

		panel->image.texture.loadFromFile("images/Windows/panel.png");
		panel->image.sprite.setTexture(panel->image.texture);
		panel->image.sprite.setPosition(x, y);
		panel->image.sprite.scale(Settings::Instance().getScaleFactor());

		y += panel->image.texture.getSize().y * Settings::Instance().getScaleFactor().y + offset;

		panels.push_back(panel);
	}
}

LoadGameWindow::~LoadGameWindow()
{
	for(SavedGamePanel *panel : panels)
		delete panel;
}

unsigned int LoadGameWindow::saveNum() const
{
	return currentPanel;
}

bool LoadGameWindow::isNewGame() const
{
	return panels.at(currentPanel)->newGame;
}

void LoadGameWindow::back()
{
	Engine::Instance().setState(Engine::MAIN_MENU);
}

void LoadGameWindow::paint(RenderWindow *window)
{
	drawBackground(window);
	for(SavedGamePanel *panel : panels)
	{
		window->draw(panel->image.sprite);
		for(const Sprite& icon : panel->charactersIcons)
			window->draw(icon);
	}
}

void LoadGameWindow::eventFilter(Event *event)
{
	if (event->type == Event::MouseButtonPressed)
	{
//		cout <<event->mouseButton.x << " "  << event->mouseButton.y << endl
//			<< mission1Link.getGlobalBounds().top << " "
//			   << mission1Link.getGlobalBounds().left << " "
//				  << mission1Link.getGlobalBounds().width << " "
//				  << mission1Link.getGlobalBounds().height << " " << endl<<endl;

		for (unsigned int i = 0; i < panels.size(); ++i)
		{
			if (panels.at(i)->image.sprite.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
			{
				currentPanel = i;
				accept();
				return;
			}
		}
	}
	StateWindow::eventFilter(event);
}

void LoadGameWindow::accept()
{
	if (isNewGame())
		Engine::Instance().setState(Engine::NEW_GAME);
	else
	{
		SavedGameLoader::Instance().setCurrentSave(saveNum());
		Engine::Instance().setState(Engine::MAIN_LOCATION);
	}
}
