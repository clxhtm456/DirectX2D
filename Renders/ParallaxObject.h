#ifndef __PARALLAX_OBJECT_H__
#define __PARALLAX_OBJECT_H__

#include "Sprite.h"
#include "Viewer/ParallaxCamera.h"

class ParallaxObject : public Node
{
public:
	static ParallaxObject* Create(Camera* camera, float parallaxFactorX = 0.0f,float parallaxFactorY = 0.0f);
	bool Init(Camera* camera, float parallaxFactorX, float parallaxFactorY);
	~ParallaxObject();
	void Move(float deltaX,float deltaY);
public:
	void SetParallaxFactorX(float val);
	void SetParallaxFactorY(float val);
private:
	float parallaxFactorX;
	float parallaxFactorY;
};

#endif