#include "arcanimatedsprite.h"

ArcAnimatedSprite::ArcAnimatedSprite(const std::string& name)
	: ArcSprite(name)
{
	setType(ArcEngine::ANIMATED_SPRITE);
}

void ArcAnimatedSprite::update()
{
	ArcSprite::update();
	if (timer.check(m_animationSpeed)) {
		nextFrame();
	}
}

void ArcAnimatedSprite::setSpeed(float speed)
{
	m_animationSpeed = speed;
}

void ArcAnimatedSprite::setFrameCount(int count)
{
	m_frameCount = count;
}

void ArcAnimatedSprite::setRowCount(int count)
{
	m_rowCount = count;
}

void ArcAnimatedSprite::setLooped(bool loop)
{
	m_looped = loop;
}

void ArcAnimatedSprite::setCycled(bool cycled)
{
	m_cycled = cycled;
}

void ArcAnimatedSprite::setRow(int row)
{
	m_row = row;
}

float ArcAnimatedSprite::speed() const
{
	return m_animationSpeed;
}

int ArcAnimatedSprite::frameCount() const
{
	return m_frameCount;
}

int ArcAnimatedSprite::rowCount() const
{
	return m_rowCount;
}

bool ArcAnimatedSprite::isLooped() const
{
	return m_looped;
}

bool ArcAnimatedSprite::isCycled() const
{
	return m_cycled;
}

void ArcAnimatedSprite::setCallback(const std::function<void ()> &func)
{
	callback = func;
}

void ArcAnimatedSprite::updateTextureRect()
{
	sprite.setTextureRect(sf::IntRect(currentFrame * m_frameSize.x, row * m_frameSize.y, m_frameSize.x, m_frameSize.y));
}

void ArcAnimatedSprite::setTexture(TextureType textureID)
{
	if (m_textureID == textureID)
		return;
	ArcSprite::setTexture(textureID);
	nextFrame();
}

void ArcAnimatedSprite::nextFrame()
{
	if (finished)
		return;
	currentFrame++;
	if (currentFrame >= m_frameCount) {
		if (m_cycled) {
			row++;
			if (row >= m_rowCount) {
				if (!m_looped) {
					finish();
					return;
				}
				row = m_row;
			}
		}
		else if (!m_looped) {
			finish();
			return;
		}
		currentFrame = 0;
	}
	updateTextureRect();
	return;
}

void ArcAnimatedSprite::setFrameSize(const sf::Vector2f &frameSize)
{
	m_frameSize = frameSize;
}

void ArcAnimatedSprite::setFrameSize(float width, float height)
{
	setFrameSize(sf::Vector2f(width, height));
}

void ArcAnimatedSprite::finish()
{
	finished = true;
	if (callback == nullptr)
		return;
	callback();
}
