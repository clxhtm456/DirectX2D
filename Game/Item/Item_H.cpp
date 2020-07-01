#include "stdafx.h"
#include "Item_H.h"
#include "Game/Unit/Player.h"
#include "Game/Gun/HMachineGun.h"

Item_H * Item_H::Create()
{
	Item_H* pRet = new Item_H();
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

bool Item_H::Init()
{
	if(!Item::Init())
		return false;

	wstring textureFile = Textures + L"MetalSlug/ItemSprite.png";
	

	auto itemSprite = Sprite::Create(textureFile, 470.0f, 233.0f, 492.0f, 253.0f);
	itemSprite->Scale(2.0f, 2.0f);
	AddChild(itemSprite);

	collider->SetSizelikeTexture(itemSprite);

	return true;
}

Item_H::~Item_H()
{
}

void Item_H::ItemEffect(PhysicsObject * b)
{
	if (b->Tag() == TAG_PLAYER)
	{
		dynamic_cast<Player*>(b)->SetWeapon(HMachineGun::Create());
		RemoveSelf();
	}
		
}
