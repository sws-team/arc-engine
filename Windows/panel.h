#ifndef PANEL_H
#define PANEL_H

#include "stdheader.h"
#include "player.h"

class Level;
class HQMenu;

class Panel
{
public:
    Panel(tgui::Gui *gui, HQMenu *hqMenu, Level *level);
    void init(const View &fixed, const vector<Player> &players);

    View *getMinimap() const;

    void update(RenderWindow *const window);

    void endTurn();

    vector<Player> getPlayers() const;

    Player getCurrentPlayer() const;

    void setInfo(const CellInfo& cellInfo);

    void updateText();

    bool getEndTurnButtonContains() const;

private:
    View *minimap;
    RectangleShape miniback;
    RectangleShape panel;

    Level* p_level;

    tgui::Gui *p_gui;
    shared_ptr<tgui::Button> button_endTurn;
    HQMenu *p_hqMenu;

    vector<Player> players;

    struct PanelCellInfo
    {
        Text player;
        Text resources;

        PanelCellInfo();
        Text name;
        Text description;
        Sprite sprite;
        void draw(RenderWindow *const window);
    };
    PanelCellInfo info;

    constexpr static double MARGIN = 10;
    constexpr static float UNIT_ICON_SCALE = 0.5;

    bool m_endTurnFocused;
    void updatePos();
};

#endif // PANEL_H
