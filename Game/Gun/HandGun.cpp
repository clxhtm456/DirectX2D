#include "stdafx.h"
#include "HandGun.h"

HandGun* HandGun::Create()
{
	HandGun* pRet = new HandGun();
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

bool HandGun::Init()
{
	if (!__super::Init())
		return false;

	bulletSpeed = 2000.0f;
	SetAmmo(-1);//infinite

	return true;
}

Bullet * HandGun::CreateBullet()
{
	auto bullet = Weapon::CreateBullet();
	wstring handGunTexture = Textures + L"MetalSlug/shot.png";
	auto sprite = Sprite::Create(handGunTexture);
	sprite->Scale(1.5f, 1.5f);

	bullet->AddChild(sprite);
	bullet->SetDamage(1);
	bullet->GetCollider()->Size(10,10);
	return bullet;
}

void HandGun::Shot()
{
	Bullet* bullet = SearchBullet();
	bullet->Activate();
}
