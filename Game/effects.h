#ifndef EFFECTS_H
#define EFFECTS_H

#include "stdheader.h"
#include "gamedrawable.h"
#include "graphics.h"
#include "ResourcesManager/textures_types.h"

struct Animation : public GameDrawable
{
	Animation();
	virtual ~Animation() override;
	virtual void draw(RenderTarget *const target) override;
	void update() override;

	void setTextureId(const RESOURCES::TEXTURE_TYPE& texture_id);

	bool loop;
	int frameCount;
	int currentFrame;
	int animationSpeed;
	int row;
	function<void(Animation *)> callback;

	Sprite sprite;
	Vector2i size;

	Transform transform;

	bool nextFrame();
	void stop();
	void setShader(Shader *shader);

private:
	Int64 lastTimer;
	Shader *shader;
};


class Effects : public GameDrawable
{
public:
	Effects();
	static Effects &Instance();

	void showAnimation(const RESOURCES::TEXTURE_TYPE &texture_id,
					   const Vector2f& pos,
					   const Vector2i& size,
					   int duration,
					   int frameCount,
					   int row = 0);

	void draw(RenderTarget *const target) override;
	void update() override;

	void removeAnimation(Animation *animation);

	void moveAnimnations(float dy);

private:
	vector<Animation*> animations;
//TODO singleton
	Int64 lastTime;
};

#endif // EFFECTS_H
