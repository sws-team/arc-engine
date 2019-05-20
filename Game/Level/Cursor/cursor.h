#ifndef CURSOR_H
#define CURSOR_H

#include "stdheader.h"
#include <vector>

class Cursor
{
public:
    Cursor();
    void update(RenderWindow *const window);

    bool moveLeft();
    bool moveRight();
    bool moveUp();
    bool moveDown();

    void setPos(const Vector2i &pos);
    void setCellSize(const Vector2i &cellSize);

    Vector2i pos() const;
    Vector2f pixelPos() const;

    void setMaxCell(const Vector2i &maxCell);

private:
    Texture cursorTexture;
    Sprite sprite;
    Vector2i m_pos;
    Vector2i m_maxCell;

    Clock clock;
    float timer;

    void nextFrame();
    constexpr static int FRAME_COUNT = 2;
    int currentFrame;    
};

#endif // CURSOR_H
