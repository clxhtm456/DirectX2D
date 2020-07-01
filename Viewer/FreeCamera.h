#ifndef __FREE_CAMERA_H__
#define __FREE_CAMERA_H__

#include "Camera.h"

class FreeCamera : public Camera
{
private:
	float speed;
public:
	void Move(D3DXVECTOR2 destination);
public:
	FreeCamera(float speed = 200.0f);
	~FreeCamera();

	void Position(float x, float y);
	void Position(D3DXVECTOR2& vec);

	void SetSpeed(float val) { speed = val; }

	void Update();
};

#endif