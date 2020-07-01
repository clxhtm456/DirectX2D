#include "stdafx.h"
#include "Truck.h"
#include "Game/FlagList.h"

Truck* Truck::Create()
{
	Truck* pRet = new Truck();
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

bool Truck::Init()
{
	if (!PhysicsObject::Init())
		return false;

	Tag(TAG_ENEMY);
	AddFlag(FLAG_VEHICLE);

	SetHpPoint(50);

	wstring truckTexture = Textures + L"MetalSlug/Truck.png";

	truckClip = Clip::Create(PlayMode::Loop);
	truckClip->AddFrame(Sprite::Create(truckTexture,3,3,104,53), 0.1f);
	truckClip->AddFrame(Sprite::Create(truckTexture,107,4,208,53), 0.1f);
	truckClip->AddFrame(Sprite::Create(truckTexture,211,3,312,53), 0.1f);
	truckClip->AddFrame(Sprite::Create(truckTexture,315,4,416,53), 0.1f);

	truckClip->AnchorPosition(0.5f, 0.0f);
	truckClip->Play();
	AddChild(truckClip);

	AnchorPosition(0.5f, 0.0f);
	collider->Size(80, 25);

	rideObject = WallBlock::Create();
	rideObject->AnchorPosition(0.5f, 0.0f);
	rideObject->GetCollider()->Size(101, 25);
	rideObject->AddFlag(FLAG_RIDE);
	//rideObject->AddFlag(FLAG_02);
	rideObject->SetKinematic(true);
	AddChild(rideObject);

	return true;
}

Truck::~Truck()
{
}

void Truck::Update()
{
	auto position = Position();

	//position.x -= 100.0f * Time::deltaTime();

	Position(position);
	PhysicsObject::Update();
}

void Truck::DamageTaken(float damage)
{
	truckClip->DrawBloom(true,1.0f,0.5f,0.1f);
	AddEvent([=]()->void
	{
		truckClip->DrawBloom(false,0,0,0);
	},0.1f);

	SetHpPoint(GetHpPoint() - damage);
}

void Truck::EnterPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
}

void Truck::OnPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
}

void Truck::ExitPhysicsCollision(PhysicsObject* b)
{
}

void Truck::Died()
{
	wstring explosionTexture = Textures + L"MetalSlug/Explosion.png";
	auto explosion = Clip::Create(PlayMode::End);
	explosion->AddFrame(Sprite::Create(explosionTexture, 1, 234, 86, 321), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 86, 234, 172, 321), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 172, 234, 248, 321), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 248, 234, 330, 321), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 330, 234, 411, 321), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 411, 234, 493, 321), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 493, 234, 577, 321), 0.03f);
	
	explosion->AddFrame(Sprite::Create(explosionTexture, 5, 324, 85, 406), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 85, 324, 165, 406), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 165, 324, 243, 406), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 243, 324, 323, 406), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 323, 324, 401, 406), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 401, 324, 479, 406), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 479, 324, 559, 406), 0.03f);

	explosion->AddFrame(Sprite::Create(explosionTexture, 3, 409, 80, 492), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 80, 409, 154, 492), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 154, 409, 226, 492), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 226, 409, 297, 492), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 297, 409, 368, 492), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 368, 409, 436, 492), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 436, 409, 498, 492), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 498, 409, 560, 492), 0.03f);

	explosion->AddFrame(Sprite::Create(explosionTexture, 5, 496, 66, 573), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 66, 496, 126, 573), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 126, 496, 183, 573), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 183, 496, 236, 573), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 236, 496, 286, 573), 0.03f);
	explosion->AddFrame(Sprite::Create(explosionTexture, 286, 496, 336, 573), 0.03f);
	explosion->SetEndCallBack(CC_CALLBACK_0(Node::RemoveSelf, explosion));
	Director::getInstance()->GetRunningScene()->AddChild(explosion, 4);
	explosion->AnchorPosition(0.5f, 0.0f);
	explosion->Scale(2.0f, 2.0f);
	explosion->Position(NormalizedPosition());
	explosion->Play();

	RemoveSelf();
}
