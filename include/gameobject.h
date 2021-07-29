#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "stdheader.h"
#include "timer.h"
#include "enginedef.h"

class GameDrawable
{
public:
	GameDrawable()
	{

	}

	virtual ~GameDrawable()
	{

	}

	virtual void draw(sf::RenderTarget *const target) = 0;
	virtual void update() = 0;
	virtual void eventFilter(sf::Event *event)
	{

	}

protected:
	Timer timer;
};

class Animation : public GameDrawable
{
public:
	Animation();
	virtual ~Animation() override;

	void draw(sf::RenderTarget *const target) override;
	void update() override;

	void setTextureId(const TextureType& texture_id);
	int frameCount;
	int currentFrame;
	int animationSpeed;
	int row;
	sf::Vector2i size;
	std::function<void(Animation *)> callback;
	sf::Sprite sprite;
	void setShader(sf::Shader *shader);

	void pause();
	void resume();
	bool cycled;
	int rowCount;

	void updateTextureRect();

	bool loop;
protected:

	bool nextFrame();
	void finish();

private:
	sf::Int64 lastTimer;
	sf::Shader *shader;
	bool paused;
};

class GameObject : public Animation
{
public:
	GameObject(const TextureType& texture_id,
			   const sf::Vector2f& startPos,
			   const sf::Vector2i& frameSize,
			   const int fCount = 3);
	virtual ~GameObject() override;

	void draw(sf::RenderTarget *const target) override;

	sf::Sprite getSprite() const;
	void setSprite(const sf::Sprite &sprite);

	void setSpriteColor(const sf::Color& color);

	void move(float dx, float dy);
	void move(const sf::Vector2f& d);
	void setPos(const sf::Vector2f &pos);
	sf::Vector2f pos() const;

	void scale(float value);
	sf::Vector2f getSize() const;

	sf::FloatRect gameRect() const;

	sf::Sprite &getModifiableSprite();

	sf::Vector2f getCenter() const;

	void setRotation(float angle);

	void hide();
	void show();
	void setVisible(bool visible);

private:
	bool visible;
};

#endif // GAMEOBJECT_H
