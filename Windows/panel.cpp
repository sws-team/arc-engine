#include "panel.h"
#include "globalvariables.h"
#include "Level/level.h"
#include "GameObjects/HQMenu/hqmenu.h"
#include "Engine/CPU/cpu.h"
#include "settings.h"
#include "Game/soundcontroller.h"

Panel::Panel(tgui::Gui *gui, HQMenu *hqMenu, Level *level)
    :p_level(level)
    ,p_gui(gui)
    ,p_hqMenu(hqMenu)
    ,m_endTurnFocused(false)
{
    button_endTurn = tgui::Button::create();
    button_endTurn->setPosition(Settings::Instance().getResolution().x - 115.f, Settings::Instance().getResolution().y - 50.f);
    button_endTurn->setText("End turn");
    button_endTurn->setSize(100, 40);
    button_endTurn->connect("pressed", [&](){ this->endTurn(); });
    button_endTurn->connect("MouseEntered", [&](){ m_endTurnFocused = true; });
    button_endTurn->connect("MouseLeft", [&](){ m_endTurnFocused = false; });
    p_gui->add(button_endTurn);

    CPU::Instance().setEndTurnFunc(bind(&Panel::endTurn, this));

    updatePos();
}

void Panel::init(const View &fixed, const vector<Player> &players)
{
    this->players = players;
    p_hqMenu->setRace(players.at(0).race);

    const FloatRect gameRect = FloatRect(0, 0, Settings::Instance().getResolution().x, Settings::Instance().getResolution().y);
    const float size = 300.f;
    minimap = new View(FloatRect(fixed.getCenter().x, fixed.getCenter().y, size, gameRect.height*size/gameRect.width));

    minimap->setViewport(FloatRect(1.f-static_cast<float>(minimap->getSize().x)/gameRect.width-0.02f,
                                   1.f-static_cast<float>(minimap->getSize().y)/gameRect.height-0.02f,
                                   static_cast<float>(minimap->getSize().x)/gameRect.width,
                                   static_cast<float>(minimap->getSize().y)/gameRect.height));
    minimap->zoom(8.0f);

    const float offset = 20.f;
    const FloatRect miniMapGeometry = minimap->getViewport();

    miniback.setPosition(miniMapGeometry.left*gameRect.width - offset, miniMapGeometry.top*gameRect.height - offset);
    miniback.setSize(Vector2f(miniMapGeometry.width*gameRect.width + offset, miniMapGeometry.height*gameRect.height + offset));
    miniback.setFillColor(Color(160, 8, 8, 100));

    panel.setPosition(miniMapGeometry.left*gameRect.width-5, offset);
    panel.setSize(Vector2f(miniMapGeometry.width*gameRect.width+10, 400));
    panel.setFillColor(Color(8, 160, 8, 100));


    const Vector2f centerOfTopLeftPoint = Vector2f(gameRect.width * Settings::GAME_SCALE/2, gameRect.height * Settings::GAME_SCALE/2);
    minimap->setCenter(centerOfTopLeftPoint);
}

View *Panel::getMinimap() const
{
    return minimap;
}

void Panel::update(RenderWindow * const window)
{
#ifdef STEAM_API
    std::string userName = std::string(SteamFriends()->GetPersonaName());
#endif

    window->draw(panel);

    info.draw(window);
//    tileInfo.draw(window);

    window->draw(miniback);

    window->setView(*minimap); // Draw minimap
}

void Panel::endTurn()
{
    //end turn
    p_level->resetSelectedUnit();
    p_level->playerTurn();
    p_hqMenu->setRace(players.at(p_level->getCurrentPlayer() - 1).race);
    updateText();
    SoundController::Instance().playOnce(END_TURN_SOUND_FILE);

    const bool isAI = getCurrentPlayer().type == Player::CPU;
    if (isAI)
    {
        CPU::Instance().turn(p_level->getCurrentPlayer());
	}
}

vector<Player> Panel::getPlayers() const
{
    return players;
}

Player Panel::getCurrentPlayer() const
{
    return players.at(p_level->getCurrentPlayer() - 1);
}

void Panel::setInfo(const CellInfo &cellInfo)
{
    info.name.setString(cellInfo.name);
    info.description.setString(cellInfo.description);

    if (cellInfo.unitTexture == nullptr)
    {
        info.sprite = cellInfo.tileSprite;
        info.sprite.setScale(4, 4);
    }
    else
    {
        Sprite sprite;
        sprite.setTexture(*cellInfo.unitTexture);
        info.sprite = sprite;
        info.sprite.setScale(UNIT_ICON_SCALE, UNIT_ICON_SCALE);
    }
    updatePos();
}

void Panel::updateText()
{
	String drawText = String("Resources: ") + String(std::to_string(p_level->resources(p_level->getCurrentPlayer())));
	info.resources.setString(drawText);

	drawText = String("Player ") + String(std::to_string(p_level->getCurrentPlayer()))  + String(" turn.");
	info.player.setString(drawText);
}

bool Panel::getEndTurnButtonContains() const
{
    return m_endTurnFocused;
}

void Panel::updatePos()
{
    float margin = MARGIN * GlobalVariables::Instance().getScale().y;

    float y = panel.getGlobalBounds().top + margin;
    float xLeft = panel.getGlobalBounds().left + margin;

    info.player.setPosition(xLeft, y);

    y += margin + info.player.getGlobalBounds().height;
    info.resources.setPosition(xLeft, y);

    y += margin + info.resources.getGlobalBounds().height;
    info.name.setPosition(xLeft, y);

    y += margin + info.name.getGlobalBounds().height;
    info.sprite.setPosition(xLeft, y);

    y += margin + info.sprite.getGlobalBounds().height;
    info.description.setPosition(xLeft, y);
}

Panel::PanelCellInfo::PanelCellInfo()
{
    player.setFont(GlobalVariables::Instance().font());
    player.setFillColor(Color::Black);
    player.setCharacterSize(24);
    player.setStyle(Text::Bold);

    resources.setFont(GlobalVariables::Instance().font());
    resources.setFillColor(Color::Black);
    resources.setCharacterSize(24);
    resources.setStyle(Text::Bold);

    name.setFont(GlobalVariables::Instance().font());
    name.setFillColor(Color::Black);
    name.setCharacterSize(24);
    name.setStyle(Text::Bold);

    description.setFont(GlobalVariables::Instance().font());
    description.setFillColor(Color::Black);
    description.setCharacterSize(24);
    description.setStyle(Text::Bold);
}

void Panel::PanelCellInfo::draw(RenderWindow * const window)
{
    window->draw(player);
    window->draw(resources);
    window->draw(name);
    window->draw(sprite);
    window->draw(description);
}
