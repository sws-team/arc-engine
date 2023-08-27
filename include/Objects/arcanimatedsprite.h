#ifndef ARCANIMATEDSPRITE_H
#define ARCANIMATEDSPRITE_H

#include "arcsprite.h"
#include <ArcTimer>

class ArcAnimatedSprite : public ArcSprite
{
public:
	ArcAnimatedSprite(const std::string& name);

	void setSpeed(int speed);
	void setFrameCount(int count);
	void setRowCount(int count);
	void setLooped(bool loop);
	void setCycled(bool cycled);
	void setRow(int row);
	void setFrameSize(const sf::Vector2f &frameSize);
	void setFrameSize(float width, float height);

	int speed() const;
	int frameCount() const;
	int rowCount() const;
	bool isLooped() const;
	bool isCycled() const;

	void setCallback(const std::function<void()>& func);

	void updateTextureRect();
	void setTexture(TextureType textureID) override;

protected:
	void nextFrame();
private:
	ArcTimer timer;
	int m_animationSpeed = 0;
	int m_frameCount = 0;
	int m_rowCount = 0;
	bool m_looped = true;
	bool m_cycled = false;
	int m_row = 0;
	sf::Vector2f m_frameSize;
	std::function<void()> callback = nullptr;

	int currentFrame = -1;
	int row = 0;
	bool finished = false;

	void finish();
};

#endif // ARCANIMATEDSPRITE_H
