#include "stdafx.h"
#include "Slave.h"
#include "Player.h"
#include "Game/FlagList.h"

Slave * Slave::Create(Item* item)
{
	Slave* pRet = new Slave();
	if (pRet && pRet->Init(item))
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

bool Slave::Init(Item* item)
{
	if (!PhysicsObject::Init())
		return false;

	wstring slaveTexture = Textures + L"MetalSlug/Npc_sprite.png";
	
	AnchorPosition(0.5f, 0.0f);
	collider->Size(10, 25);
	phase = 0;

	moveSpeed = 50.0f;
	runSpeed = 500.0f;
	turnTimer = 3.0f;
	distance = 0.0f;

	AddFlag(FLAG_ENEMY);

	Tag(TAG_ENEMY);

	item->retain();
	this->item = item;

	slaveAnim = Animation::Create();

	Clip* clip;

	{
		//Idle0
		clip = Clip::Create(PlayMode::Loop);
		clip->AnchorPosition(0.5f, 0.0f);
		clip->AddFrame(Sprite::Create(slaveTexture,6,26,36,53),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 39, 26, 70, 53), 0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 73, 26, 109, 53), 0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 112, 26, 149, 53), 0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 152, 26, 189, 53), 0.1f);
		slaveAnim->AddClip(clip);
	}

	{
		//Attacked1
		clip = Clip::Create(PlayMode::End);
		clip->AnchorPosition(0.5f, 0.0f);
		clip->AddFrame(Sprite::Create(slaveTexture, 198, 24, 236, 53),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 239, 22, 279, 53),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 282, 19, 324, 53),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 327, 17, 370, 53),0.1f);
		clip->SetEndCallBack([=]()->void
		{
			slaveAnim->Play(2);
			SetFlagZero();
			AddFlag(FLAG_NPC);
			phase = 1;
		});
		slaveAnim->AddClip(clip);
	}

	{
		//Walk2
		clip = Clip::Create(PlayMode::Loop);
		clip->AnchorPosition(0.5f, 0.0f);
		clip->AddFrame(Sprite::Create(slaveTexture, 6, 248, 39, 286),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 42, 249, 76, 286),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 79, 248, 108, 286),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 111, 245, 132, 285),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 135, 246, 154, 284),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 157, 247, 180, 286),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 183, 249, 213, 286),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 216, 248, 241, 286),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 244, 247, 266, 286),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 269, 246, 291, 286),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 294, 247, 324, 285),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 327, 247, 360, 285),0.1f);
		slaveAnim->AddClip(clip);
	}

	{
		//GiveItem3
		clip = Clip::Create(PlayMode::End);
		clip->AnchorPosition(0.5f, 0.0f);
		clip->AddFrame(Sprite::Create(slaveTexture, 6, 400, 26, 436),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 29, 400, 51, 436),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 54, 399, 79, 436),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 82, 399, 109, 436),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 110, 401, 131, 436),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 134, 402, 155, 436),0.1f);
		clip->AddFrame([=]()->void
		{
			Node* parent = this->Parent();
			(this->item)->Position(this->NormalizedPosition().x, this->NormalizedPosition().y + 20.0f);
			(this->item)->autorelease();
			parent->AddChild((this->item),4);
			this->item = nullptr;
		});
		clip->AddFrame(Sprite::Create(slaveTexture, 158, 399, 191, 436),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 194, 398, 234, 436),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 237, 398, 281, 436),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 284, 397, 326, 436),0.1f);
		clip->AddFrame(Sprite::Create(slaveTexture, 329, 398, 371, 436),0.1f);
		clip->SetEndCallBack([=]()->void
			{
				slaveAnim->Play(4);
				slaveAnim->RotationDegree(0, 0, 0);
				phase = 3;
			});
		slaveAnim->AddClip(clip);
	}

	{
		//Run4
		clip = Clip::Create(PlayMode::Loop);
		clip->AnchorPosition(0.5f, 0.0f);
		clip->AddFrame(Sprite::Create(slaveTexture, 6, 484, 46, 523), 0.2f);
		clip->AddFrame(Sprite::Create(slaveTexture, 46, 482, 84, 523), 0.2f);
		clip->AddFrame(Sprite::Create(slaveTexture, 87, 482, 119, 523), 0.2f);
		clip->AddFrame(Sprite::Create(slaveTexture, 122, 482, 160, 521), 0.2f);
		clip->AddFrame(Sprite::Create(slaveTexture, 163, 483, 205, 523), 0.2f);
		clip->AddFrame(Sprite::Create(slaveTexture, 208, 482, 244, 523), 0.2f);
		clip->AddFrame(Sprite::Create(slaveTexture, 247, 482, 283, 523), 0.2f);
		slaveAnim->AddClip(clip);
	}
	slaveAnim->Play(0);
	AddChild(slaveAnim);

	bFree = false;

	return true;
}

Slave::~Slave()
{
	if (this->item != nullptr)
		this->item->release();
}

void Slave::Update()
{
	switch (phase)
	{
	case 1:
	{
		auto position = Position();
		if (turnTimer > 0)
		{
			turnTimer -= Time::deltaTime();
		}
		else
		{
			turnTimer = 3.0f;
			moveSpeed *= -1;
			if (moveSpeed < 0)
			{
				slaveAnim->RotationDegree(0, 180, 0);
			}
			else
			{
				slaveAnim->RotationDegree(0, 0, 0);
			}
		}
		position.x -= moveSpeed * Time::deltaTime();
		Position(position);

	}
		break;
	case 3:
	{
		auto position = Position();
		position.x -= runSpeed * Time::deltaTime();
		distance += runSpeed * Time::deltaTime();
		if (distance > 1000.0f)
			RemoveSelf();
		Position(position);
	}
		break;
	}
	PhysicsObject::Update();
}

void Slave::EnterPhysicsCollision(PhysicsObject * b, ColDirection direction)
{
	if (phase != 1)
		return;

	if (b->Tag() == TAG_PLAYER)
	{
		phase = 2;
		SetFlagZero();
		slaveAnim->Play(3);
	}
}

void Slave::ExitPhysicsCollision(PhysicsObject * b)
{
}

void Slave::OnPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
}

void Slave::DamageTaken(float damage)
{
	if (bFree == true)
		return;

	bFree = true;
	slaveAnim->Play(1);
}

void Slave::Died()
{
}
