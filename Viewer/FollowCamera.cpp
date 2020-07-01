#include "stdafx.h"
#include "FollowCamera.h"

#define CAMERA_SPEED	200.0f

FollowCamera * FollowCamera::Create(IFollow * focus)
{
	FollowCamera* pRet = new FollowCamera();
	if (pRet && pRet->Init(focus))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

bool FollowCamera::Init(IFollow * focus)
{
	if (!Camera::Init())
		return false;

	ChangeTarget(focus);
	FixX = 0;
	FixY = 0;
	lockLeft = false;
	lockRight = false;
	
	return true;
}

FollowCamera::FollowCamera():
	cameraSpeed(0), maxSpeed(CAMERA_SPEED),accel(CAMERA_SPEED), startSpeed(10),
	bLimitLeft(false), bLimitRight(false)
{
}

FollowCamera::~FollowCamera()
{
}

void FollowCamera::FixYPos(float val)
{
	FixY = true;
	yGoal = val;
}

void FollowCamera::FixXPos(float val)
{
	FixX = true;
	xGoal = val;
}

void FollowCamera::FixYPos(bool val)
{
	FixY = val;
}

void FollowCamera::FixXPos(bool val)
{
	FixX = val;
}

void FollowCamera::SetCameraSpeed(float val)
{
	startSpeed = val;
}

void FollowCamera::SetCameraMaxSpeed(float val)
{
	maxSpeed = val;
}

void FollowCamera::SetCameraAccel(float val)
{
	accel = val;
}

void FollowCamera::SetLockLeftPoint(bool val)
{
	lockLeft = val;
}

void FollowCamera::SetLockRightPoint(bool val)
{
	lockRight = val;
}

void FollowCamera::SetLimitLeftPoint(float val)
{
	limitLeft = val;
}

void FollowCamera::SetLimitRightPoint(float val)
{
	limitRight = val;
}

void FollowCamera::SetLimitLeftPoint(bool val)
{
	bLimitLeft = val;
}

void FollowCamera::SetLimitRightPoint(bool val)
{
	bLimitRight = val;
}

void FollowCamera::ChangeTarget(IFollow * focus)
{
	FollowCamera::focus = focus;

	if (this->focus != nullptr)
	{
		D3DXVECTOR2 pos;
		D3DXVECTOR2 scale;

		focus->Focus(pos, scale);

		_position = pos;
	}
}

void FollowCamera::Update()
{
	if (focus == nullptr)
		return;

	D3DXVECTOR2 pos;
	D3DXVECTOR2 scale;

	focus->Focus(pos, scale);

	D3DXVECTOR2 temp = pos;

	////포커스 대상위치를 화면 중심으로
	//temp.x -= Width * 0.5f;
	//temp.y -= Height * 0.5f;

	//temp.x += scale.x * 0.5f;
	//temp.y += scale.y * 0.5f;

	auto distance = Math::Distance(_position, temp);
	if (distance < 1.0f)
	{
		cameraSpeed = startSpeed;
		return;
	}
		

	auto goal = Math::MoveTowards(_position, temp, cameraSpeed * Time::deltaTime());

	if (FixX)
		goal.x = xGoal;

	if (FixY)
		goal.y = yGoal;

	/*if (goal.x - Width * 0.5f < limitLeft)
		goal.x = limitLeft + Width * 0.5f;

	if (goal.x + Width * 0.5f > limitRight)
		goal.x = limitRight - Width * 0.5f;*/

	if (lockLeft)
	{
		if ((goal.x - _position.x) < 0)
			goal = _position;
	}

	if (lockRight)
	{
		if ((goal.x - _position.x) > 0)
			goal = _position;
	}

	_position = goal;
	//_position = temp;

	if (bLimitLeft && (_position.x - (float)Width * (1 / mgnfc) * 0.5f < limitLeft))
		_position.x = limitLeft + (float)Width * (1 / mgnfc) * 0.5f;
	else if (bLimitRight && (_position.x + (float)Width * (1 / mgnfc) * 0.5f > limitRight))
		_position.x = limitRight - (float)Width * (1 / mgnfc) *0.5f;

	if (cameraSpeed < maxSpeed)
		cameraSpeed += accel * Time::deltaTime();

	__super::Update();
}
