#ifndef __PARALLAX_CAMERA_H__
#define __PARALLAX_CAMERA_H__

#include "Viewer/FollowCamera.h"

class ParallaxCamera : public FollowCamera
{
public:
	ParallaxCamera(IFollow* focus = nullptr);
	virtual ~ParallaxCamera();

	void Update();
	void AddMoveCallback(const function<void(float)>& func);
private:
	float oldPosition;

	vector<function<void(float)>> moveCallbackList;

};

#endif