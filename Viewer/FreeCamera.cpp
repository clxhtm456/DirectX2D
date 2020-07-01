#include "stdafx.h"
#include "FreeCamera.h"

FreeCamera::FreeCamera(float speed):
	speed(speed)
{
}

FreeCamera::~FreeCamera()
{
}

void FreeCamera::Position(float x, float y)
{
	Position(D3DXVECTOR2(x, y));
}

void FreeCamera::Position(D3DXVECTOR2 & vec)
{
	_position = vec;
}

void FreeCamera::Update()
{
	__super::Update();
}

void FreeCamera::Move(D3DXVECTOR2 destination)
{
	_position += destination * Time::deltaTime();
}

