#include "stdafx.h"
#include "Bullet.h"
#include "Game/Scene/EffectManager.h"
#include "Game/Unit/MetalUnit.h"
#include "Game/Unit/Player.h"

#define MAX_DISTANCE	2000.0f
#define BULLET_TAG		500

Bullet* Bullet::Create(IBulletManager* manager)
{
	Bullet* pRet = new Bullet(manager);
	if(pRet && pRet->Init(manager))
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


bool Bullet::Init(IBulletManager* manager)
{
	if (!PhysicsObject::Init())
		return false;

	bulletManager = manager;
	_running = false;
	gravity = 0.0f;
	_tag = BULLET_TAG;

	return true;
}

void Bullet::RemoveManager()
{
	bulletManager = nullptr;
}

Bullet::Bullet(IBulletManager* manager)
{
	
}

Bullet::~Bullet()
{
}

void Bullet::SetRunning(bool val)
{
	_running = val;
	_visible = val;
	if (_running)
	{
		bulletManager->ActiveBullet();
	}
	else
	{
		if (bulletManager != nullptr)
			bulletManager->DeactiveBullet();
		else
			RemoveSelf();
	}
}

void Bullet::ResetPosition(float angle)
{
}

void Bullet::Update()
{
	auto position = Position();

	if (Math::Distance(startPosition, _position) > MAX_DISTANCE)
	{
		DeActive();
		return;
	}
		

	if (LeftCollision() == true || RightCollision() == true || bLineBottonCollision == true)
	{
		DeActive();
		return;
	}
	PhysicsObject::Update();
}

void Bullet::SetRotation(float angle)
{
}

void Bullet::Activate()
{
	SetRunning(true);
	Reset();
	startPosition = _position;


	auto angle = bulletManager->GetAngle();

	auto radian = Math::ToRadian(angle);

	moveAccel.x = cosf(radian);
	moveAccel.y = sinf(radian);
	moveAccel *= moveSpeed;
}

void Bullet::CreateEffect()
{
	auto effect = EffectManager::GetInstance()->SearchEffect("BulletEffect");
	if (effect == nullptr)
	{
		wstring textureFile = Textures + L"MetalSlug/WeaponSFX.png";
		effect = Clip::Create(PlayMode::End);
		effect->Scale(2.0f, 2.0f);
		effect->AddFrame(Sprite::Create(textureFile, 4, 6134, 20, 6152), 0.01f);
		effect->AddFrame(Sprite::Create(textureFile, 20, 6134, 36, 6152), 0.01f);
		effect->AddFrame(Sprite::Create(textureFile, 36, 6134, 42, 6152), 0.01f);
		effect->AddFrame(Sprite::Create(textureFile, 42, 6134, 58, 6152), 0.01f);
		effect->AddFrame(Sprite::Create(textureFile, 58, 6134, 74, 6152), 0.01f);
		effect->AddFrame(Sprite::Create(textureFile, 74, 6134, 90, 6152), 0.01f);
		effect->AddFrame(Sprite::Create(textureFile, 90, 6134, 106, 6152), 0.01f);
		effect->AddFrame(Sprite::Create(textureFile, 106, 6134, 122, 6152), 0.01f);
		effect->AddFrame(Sprite::Create(textureFile, 122, 6134, 138, 6152), 0.01f);
		effect->AddFrame(Sprite::Create(textureFile, 138, 6134, 154, 6152), 0.01f);
		effect->AddFrame(Sprite::Create(textureFile, 138, 6134, 170, 6152), 0.01f);
		EffectManager::GetInstance()->CreateEffect("BulletEffect", effect);
	}
	effect->Scale(2.0f, 2.0f);
	effect->Position(this->NormalizedPosition());
	effect->Play();
	Director::getInstance()->GetRunningScene()->AddChild(effect, 4);
}

void Bullet::DeActive()
{
	SetRunning(false);
	ExitCollisionAll();
}

void Bullet::EnterPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
	if (b->Tag() == BULLET_TAG)
		return;

	CreateEffect();

	if (b->Tag() == TAG_ENEMY || b->Tag() == TAG_PLAYER)
	{
		((MetalUnit*)b)->DamageTaken(damage);
	}

	DeActive();
}

void Bullet::ExitPhysicsCollision(PhysicsObject * b)
{
}

void Bullet::OnPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
}
