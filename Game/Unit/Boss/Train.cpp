#include "stdafx.h"
#include "Train.h"

BossTrain* BossTrain::Create()
{
	BossTrain* pRet = new BossTrain();
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

bool BossTrain::Init()
{
	if(!MetalUnit::Init())
		return false;

	return true;
}

void BossTrain::Update()
{
	PhysicsObject::Update();
}

void BossTrain::DamageTaken(float damage)
{
}

void BossTrain::EnterPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
}

void BossTrain::OnPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
}

void BossTrain::ExitPhysicsCollision(PhysicsObject* b)
{
}

void BossTrain::Died()
{
}

DriveTrain* DriveTrain::Create()
{
	DriveTrain* pRet = new DriveTrain();
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

bool DriveTrain::Init()
{
	if (!MetalUnit::Init())
		return false;

	wstring textureFile = Textures + L"MetalSlug/Train.png";

	body = Animation::Create();

	Clip* clip;

	clip = Clip::Create(PlayMode::Loop);
	clip->AddFrame(Sprite::Create(textureFile, 8, 335, 139, 409), 5.0f);
	body->AddClip(clip);

	clip = Clip::Create(PlayMode::Loop);
	clip->AddFrame(Sprite::Create(textureFile, 154, 335, 286, 409), 5.0f);
	body->AddClip(clip);

	body->Play(0);

	AddChild(body, 2);

	frontLeg = Animation::Create();

	clip = Clip::Create(PlayMode::Loop);
	clip->AddFrame(Sprite::Create(textureFile, 3, 1310, 103, 1440), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 103, 1312, 207, 1440), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 209, 1311, 298, 1440), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 299, 1312, 393, 1440), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 396, 1311, 499, 1440), 0.1f);

	clip->AddFrame(Sprite::Create(textureFile, 11, 1440, 113, 1568), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 116, 1439, 230, 1568), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 231, 1440, 337, 1568), 0.1f);
	frontLeg->AddClip(clip);
	frontLeg->AnchorPosition(0, 1);
	frontLeg->Position(50, -75);
	frontLeg->Play(0);
	AddChild(frontLeg, 3);

	behindLeg = Animation::Create();

	clip = Clip::Create(PlayMode::Loop);
	clip->AddFrame(Sprite::Create(textureFile, 6, 1573, 105, 1680), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 118, 1574, 211, 1679), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 221, 1574, 309, 1679), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 315, 1574, 399, 1680), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 412, 1574, 489, 1682), 0.1f);

	clip->AddFrame(Sprite::Create(textureFile, 6, 1687, 89, 1793), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 107, 1687, 195, 1793), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 200, 1687, 293, 1792), 0.1f);
	behindLeg->AddClip(clip);
	behindLeg->AnchorPosition(1, 1);
	behindLeg->Position(-30, -75);
	behindLeg->Play(0);
	AddChild(behindLeg, 1);

	return true;
}

void DriveTrain::Update()
{
	PhysicsObject::Update();
}

void DriveTrain::DamageTaken(float damage)
{
}

void DriveTrain::EnterPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
}

void DriveTrain::OnPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
}

void DriveTrain::ExitPhysicsCollision(PhysicsObject* b)
{
}

void DriveTrain::Died()
{
}

WoodenTrain* WoodenTrain::Create()
{
	WoodenTrain* pRet = new WoodenTrain();
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

bool WoodenTrain::Init()
{
	if (!MetalUnit::Init())
		return false;

	wstring textureFile = Textures + L"MetalSlug/Train.png";

	body = Animation::Create();

	Clip* clip;

	clip = Clip::Create(PlayMode::Loop);
	clip->AddFrame(Sprite::Create(textureFile,8,335,139,409),5.0f);
	body->AddClip(clip);

	clip = Clip::Create(PlayMode::Loop);
	clip->AddFrame(Sprite::Create(textureFile, 154, 335, 286, 409), 5.0f);
	body->AddClip(clip);

	body->Play(0);

	AddChild(body,2);
	
	GetCollider()->Size(100, 250);

	frontLeg = Animation::Create();

	clip = Clip::Create(PlayMode::Loop);
	clip->AddFrame(Sprite::Create(textureFile, 3, 1310, 103, 1440), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 103, 1312, 207, 1440), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 209, 1311, 298, 1440), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 299, 1312, 393, 1440), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 396, 1311, 499, 1440), 0.1f);

	clip->AddFrame(Sprite::Create(textureFile, 11, 1440, 113, 1568), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 116, 1439, 230, 1568), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 231, 1440, 337, 1568), 0.1f);
	frontLeg->AddClip(clip);
	frontLeg->AnchorPosition(0, 1);
	frontLeg->Position(50, -75);
	frontLeg->Play(0);
	AddChild(frontLeg,3);

	behindLeg = Animation::Create();

	clip = Clip::Create(PlayMode::Loop);
	clip->AddFrame(Sprite::Create(textureFile, 6, 1573, 105, 1680), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 118, 1574, 211, 1679), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 221, 1574, 309, 1679), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 315, 1574, 399, 1680), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 412, 1574, 489, 1682), 0.1f);

	clip->AddFrame(Sprite::Create(textureFile, 6, 1687, 89, 1793), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 107, 1687, 195, 1793), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 200, 1687, 293, 1792), 0.1f);
	behindLeg->AddClip(clip);
	behindLeg->AnchorPosition(1, 1);
	behindLeg->Position(-30, -75);
	behindLeg->Play(0);
	AddChild(behindLeg,1);

	return true;
}

void WoodenTrain::Update()
{
	PhysicsObject::Update();
}

void WoodenTrain::Render()
{
	MetalUnit::Render();
}

void WoodenTrain::DamageTaken(float damage)
{
}

void WoodenTrain::EnterPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
}

void WoodenTrain::OnPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
}

void WoodenTrain::ExitPhysicsCollision(PhysicsObject* b)
{
}

void WoodenTrain::Died()
{
}

MetalTrain* MetalTrain::Create()
{
	MetalTrain* pRet = new MetalTrain();
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

bool MetalTrain::Init()
{
	if (!MetalUnit::Init())
		return false;

	wstring textureFile = Textures + L"MetalSlug/Train.png";

	body = Animation::Create();

	Clip* clip;

	clip = Clip::Create(PlayMode::Loop);
	clip->AddFrame(Sprite::Create(textureFile, 8, 335, 139, 409), 5.0f);
	body->AddClip(clip);

	clip = Clip::Create(PlayMode::Loop);
	clip->AddFrame(Sprite::Create(textureFile, 154, 335, 286, 409), 5.0f);
	body->AddClip(clip);

	body->Play(0);

	AddChild(body, 2);

	frontLeg = Animation::Create();

	clip = Clip::Create(PlayMode::Loop);
	clip->AddFrame(Sprite::Create(textureFile, 3, 1310, 103, 1440), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 103, 1312, 207, 1440), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 209, 1311, 298, 1440), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 299, 1312, 393, 1440), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 396, 1311, 499, 1440), 0.1f);

	clip->AddFrame(Sprite::Create(textureFile, 11, 1440, 113, 1568), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 116, 1439, 230, 1568), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 231, 1440, 337, 1568), 0.1f);
	frontLeg->AddClip(clip);
	frontLeg->AnchorPosition(0, 1);
	frontLeg->Position(50, -75);
	frontLeg->Play(0);
	AddChild(frontLeg, 3);

	behindLeg = Animation::Create();

	clip = Clip::Create(PlayMode::Loop);
	clip->AddFrame(Sprite::Create(textureFile, 6, 1573, 105, 1680), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 118, 1574, 211, 1679), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 221, 1574, 309, 1679), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 315, 1574, 399, 1680), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 412, 1574, 489, 1682), 0.1f);

	clip->AddFrame(Sprite::Create(textureFile, 6, 1687, 89, 1793), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 107, 1687, 195, 1793), 0.1f);
	clip->AddFrame(Sprite::Create(textureFile, 200, 1687, 293, 1792), 0.1f);
	behindLeg->AddClip(clip);
	behindLeg->AnchorPosition(1, 1);
	behindLeg->Position(-30, -75);
	behindLeg->Play(0);
	AddChild(behindLeg, 1);

	return true;
}

void MetalTrain::Update()
{
	PhysicsObject::Update();
}

void MetalTrain::DamageTaken(float damage)
{
}

void MetalTrain::EnterPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
}

void MetalTrain::OnPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
}

void MetalTrain::ExitPhysicsCollision(PhysicsObject* b)
{
}

void MetalTrain::Died()
{
}
