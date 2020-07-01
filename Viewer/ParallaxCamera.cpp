#include "stdafx.h"
#include "ParallaxCamera.h"

ParallaxCamera::ParallaxCamera(IFollow* focus):
	oldPosition(_position.x)
{
}

ParallaxCamera::~ParallaxCamera()
{
	moveCallbackList.clear();
}

void ParallaxCamera::Update()
{
	FollowCamera::Update();

	if (_position.x != oldPosition)
	{
		for (auto callback : moveCallbackList)
		{
			if (callback != NULL)
			{
				float delta = oldPosition - _position.x;

				callback(delta);
			}
		}
		oldPosition = _position.x;
	}
}

void ParallaxCamera::AddMoveCallback(const function<void(float)>& func)
{
	moveCallbackList.push_back(func);
}
