#include "stdafx.h"
#include "HMachineGun.h"

HMachineGun * HMachineGun::Create()
{
	HMachineGun* pRet = new HMachineGun();
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

bool HMachineGun::Init()
{
	if (!__super::Init())
		return false;

	bulletSpeed = 2000.0f;
	SetAmmo(200);
	active = false;
	step = 0;
	delay = 0;

	return true;
}

Bullet * HMachineGun::CreateBullet()
{
	auto bullet = Weapon::CreateBullet();
	wstring textureFile = Textures + L"MetalSlug/WeaponSFX.png";
	auto sprite = Sprite::Create(textureFile, 4, 18, 36, 27);
	sprite->Scale(2.0f, 2.0f);
	bullet->SetDamage(1);
	bullet->AddChild(sprite);
	bullet->GetCollider()->SetSizelikeTexture(sprite);
	return bullet;
}

void HMachineGun::Shot()
{
	active = true;
	step = 5;
	delay = 0.04f;
	angleCount = 1;
	trigger = false;
	
	CreateHBullet();
}

void HMachineGun::MoveAngle(float angle)
{
	goalAngle = angle;
}

float HMachineGun::GetAngle()
{
	return Weapon::GetAngle()+angleCount*2.0f;
}

void HMachineGun::Update()
{
	UpdateAngle();
	ShotMechanism();
	Weapon::Update();
	if (GetAmmo() == 0)
		zeroAmmoCallback();
}

void HMachineGun::UpdateAngle()
{
	/*if (seta < 0)
	seta += 360;*/
	int t = (int)goalAngle % 360;
	int t2 = t - 360;
	if (shotAngle == t)
		return;
	if (abs(t - shotAngle) < abs(shotAngle - t2))
	{
		if (shotAngle > t)
			shotAngle -= 270 * Time::deltaTime();
		else
			shotAngle += 270 * Time::deltaTime();
	}
	else
	{
		if (shotAngle > t2)
			shotAngle -= 270 * Time::deltaTime();
		else
			shotAngle += 270 * Time::deltaTime();
	}
}

void HMachineGun::ShotMechanism()
{
	if (step <= 0)
		return;

	if (delay > 0)
	{
		delay -= Time::deltaTime();
		return;
	}

	CreateHBullet();


	delay = 0.04f;
	step--;
}

void HMachineGun::CreateHBullet()
{
	if (GetAmmo() > 0)
	{
		Bullet* bullet = SearchBullet();
		bullet->Activate();
		bullet->RotationDegree(0, 0, shotAngle);

		if (trigger)
			angleCount++;
		else
			angleCount--;

		if (angleCount == 1 || angleCount == -1)
		{
			trigger = !trigger;
		}

		SetAmmo(GetAmmo() - 1);\
	}
}
