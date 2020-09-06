#ifndef CLOSINGWINDOW_H
#define CLOSINGWINDOW_H

#include "statewindow.h"
#include "gametimer.h"

class ClosingWindow : public StateWindow
{
public:
    ClosingWindow(swoosh::ActivityController& controller);

    void onStart() override;
    void onUpdate(double elapsed) override;
    void onDraw(sf::RenderTexture& surface) override;
    void back() override;

private:
    GameTimer timer;
    sf::Uint8 opacity;
};

#endif // CLOSINGWINDOW_H
