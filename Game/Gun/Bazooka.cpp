#include "stdafx.h"
#include "Bazooka.h"

Bazooka* Bazooka::Create()
{
	Bazooka* pRet = new Bazooka();
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

bool Bazooka::Init()
{
	if (!Weapon::Init())
		return false;

	bulletSpeed = 500.0f;
	SetAmmo(-1);//infinite

	return true;
}

Bullet* Bazooka::CreateBullet()
{
	auto bullet = Weapon::CreateBullet();


	wstring bazookaTexture = Textures + L"MetalSlug/Soldier.png";
	auto clip = Clip::Create(PlayMode::Loop);
	clip->AddFrame(Sprite::Create(bazookaTexture, 40, 806, 50, 816), 0.2f);
	clip->AddFrame(Sprite::Create(bazookaTexture, 53, 806, 63, 816), 0.2f);
	clip->AddFrame(Sprite::Create(bazookaTexture, 66, 806, 76, 816), 0.2f);
	clip->AddFrame(Sprite::Create(bazookaTexture, 79, 806, 89, 816), 0.2f);

	clip->Scale(2.0f, 2.0f);
	clip->Play();
	bullet->AddChild(clip);
	bullet->SetDamage(1);
	bullet->GetCollider()->SetSizelikeTexture(clip->GetSprite());

	return bullet;
}

void Bazooka::Shot()
{
	Bullet* bullet = SearchBullet();
	bullet->Activate();
}
