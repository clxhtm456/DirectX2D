#ifndef __FOLLOW_CAMERA_H__
#define __FOLLOW_CAMERA_H__

#include "Camera.h"
#include "IFollow.h"

class FollowCamera : public Camera
{
private:
	float cameraSpeed;
	float maxSpeed;
	float accel;
	float startSpeed;

	float limitLeft;
	bool bLimitLeft;
	float limitRight;
	bool bLimitRight;

	bool FixX;
	bool FixY;
	bool lockLeft;
	bool lockRight;
	float yGoal;
	float xGoal;
	IFollow * focus;
public:
	static FollowCamera* Create(IFollow* focus = nullptr);
	bool Init(IFollow* focus);
	FollowCamera();
	virtual ~FollowCamera();

	void FixYPos(float val);
	void FixXPos(float val);
	void FixYPos(bool val);
	void FixXPos(bool val);

	void SetCameraSpeed(float val);
	void SetCameraMaxSpeed(float val);
	void SetCameraAccel(float val);

	void SetLockLeftPoint(bool val);
	void SetLockRightPoint(bool val);
	void SetLimitLeftPoint(float val);
	void SetLimitRightPoint(float val);
	void SetLimitLeftPoint(bool val);
	void SetLimitRightPoint(bool val);

	void ChangeTarget(IFollow* focus);

	void Update();
};

#endif