#ifndef ARCANIMATEDSPRITE_H
#define ARCANIMATEDSPRITE_H

#include "stdheader.h"
#include "arcsprite.h"
#include "timer.h"

class ArcAnimatedSprite : public ArcSprite
{
public:
	ArcAnimatedSprite(const std::string& name);

	void update() override;

	void setSpeed(float speed);
	void setFrameCount(int count);
	void setRowCount(int count);
	void setLooped(bool loop);
	void setCycled(bool cycled);

	float speed() const;
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
	Timer timer;
	float m_animationSpeed = 0;
	int m_frameCount = 0;
	int m_rowCount = 0;
	bool m_looped = true;
	bool m_cycled = false;
	std::function<void()> callback = nullptr;

	int currentFrame = -1;
	int row = 0;
	bool finished = false;

	void finish();
};

#endif // ARCANIMATEDSPRITE_H
