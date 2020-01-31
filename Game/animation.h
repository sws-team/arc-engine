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
	void setShader(Shader *shader);

protected:
	bool loop;

	bool nextFrame();
	void stop();

private:
	Int64 lastTimer;
	Shader *shader;
};

#endif // EFFECTS_H
