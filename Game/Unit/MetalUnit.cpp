#include "stdafx.h"
#include "MetalUnit.h"


void MetalUnit::Twinkle(Animation* animation)
{

	if (twinkleCount <= 0)
		return;

	animation->DrawTwinkle(bTwinkle);
	bTwinkle = !bTwinkle;
	twinkleCount--;
	AddEvent([=]()->void
		{
			Twinkle(animation);
		}, 0.05f);
}
