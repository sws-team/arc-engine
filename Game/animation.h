#ifndef EFFECTS_H
#define EFFECTS_H

#include "stdheader.h"
#include "gamedrawable.h"
#include "ResourcesManager/textures_types.h"

struct Animation : public GameDrawable
{
	Animation();
public:
	virtual ~Animation() override;
	virtual void draw(RenderTarget *const target) override;
	void update() override;

	void setTextureId(const RESOURCES::TEXTURE_TYPE& texture_id);
	int frameCount;
	int currentFrame;
	int animationSpeed;
	int row;
	Vector2i size;
	function<void(Animation *)> callback;
	Sprite sprite;
protected:
	bool loop;


	Transform transform;

	bool nextFrame();
	void stop();
	void setShader(Shader *shader);

private:
	Int64 lastTimer;
	Shader *shader;
};

#endif // EFFECTS_H
