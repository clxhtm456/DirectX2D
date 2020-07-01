#include "stdafx.h"
#include "SlugTank.h"

SlugTank * SlugTank::Create()
{
	SlugTank* pRet = new SlugTank();
	if (pRet && pRet->Init())
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

bool SlugTank::Init()
{
	if (!Vehicle::Init())
		return false;
	return true;
}

float SlugTank::GetAngle()
{
	return 0.0f;
}

void SlugTank::ActiveBullet()
{
}

void SlugTank::DeactiveBullet()
{
}
