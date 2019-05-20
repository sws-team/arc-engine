#include "cursor.h"
#include "globalvariables.h"
#include "Game/Audio/soundcontroller.h"

Cursor::Cursor()
{
    cursorTexture.loadFromFile("cursor.png");
    cursorTexture.setSmooth(true);
    sprite.setTexture(cursorTexture);
    sprite.setTextureRect(IntRect(0, 0, GlobalVariables::Instance().tileSize().x, GlobalVariables::Instance().tileSize().y));
    sprite.setPosition(0, 0);
    m_pos = Vector2i(1, 1);

    currentFrame = 0;
    timer = 0.f;
    clock.restart();
}

void Cursor::update(RenderWindow *const window)
{
    timer += clock.getElapsedTime().asMicroseconds()/100;
    if (timer > 4000)
    {
        nextFrame();
        timer = 0;
    }
    window->draw(sprite);
    clock.restart();
}

bool Cursor::moveLeft()
{
    if (m_pos.x > 0)
    {
        m_pos.x--;
        sprite.setPosition(m_pos.x * GlobalVariables::Instance().tileSize().x, m_pos.y * GlobalVariables::Instance().tileSize().y);
        SoundController::Instance().playOnce(CURSOR_MOVE_SOUND_FILE);
        return true;
    }
    return false;
}

bool Cursor::moveRight()
{
    if (m_pos.x < m_maxCell.x - 1)
    {
        m_pos.x++;
        sprite.setPosition(m_pos.x * GlobalVariables::Instance().tileSize().x, m_pos.y * GlobalVariables::Instance().tileSize().y);
        SoundController::Instance().playOnce(CURSOR_MOVE_SOUND_FILE);
        return true;
    }
    return false;
}

bool Cursor::moveUp()
{
    if (m_pos.y > 0)
    {
        m_pos.y--;
        sprite.setPosition(m_pos.x * GlobalVariables::Instance().tileSize().x, m_pos.y * GlobalVariables::Instance().tileSize().y);
        SoundController::Instance().playOnce(CURSOR_MOVE_SOUND_FILE);
        return true;
    }
    return false;
}

bool Cursor::moveDown()
{
    if (m_pos.y < m_maxCell.y - 1)
    {
        m_pos.y++;
        sprite.setPosition(m_pos.x * GlobalVariables::Instance().tileSize().x, m_pos.y * GlobalVariables::Instance().tileSize().y);
        SoundController::Instance().playOnce(CURSOR_MOVE_SOUND_FILE);
        return true;
    }
    return false;
}

void Cursor::setPos(const Vector2i &pos)
{
    m_pos = pos;
    sprite.setPosition(pos.x * GlobalVariables::Instance().tileSize().x, pos.y * GlobalVariables::Instance().tileSize().y);
}

void Cursor::setCellSize(const Vector2i &cellSize)
{
    GlobalVariables::Instance().tileSize() = cellSize;
}

Vector2i Cursor::pos() const
{
    return m_pos;
}

Vector2f Cursor::pixelPos() const
{
    return Vector2f(m_pos.x * GlobalVariables::Instance().tileSize().x, m_pos.y * GlobalVariables::Instance().tileSize().y);
}

void Cursor::setMaxCell(const Vector2i &maxCell)
{
    m_maxCell = maxCell;
}

void Cursor::nextFrame()
{
    currentFrame++;
    if (currentFrame >= FRAME_COUNT)
        currentFrame = 0;
    sprite.setTextureRect(IntRect(currentFrame * 32, 0, 32, 32));
}
