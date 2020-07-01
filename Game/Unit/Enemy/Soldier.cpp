#include "stdafx.h"
#include "Soldier.h"
#include "Game/Unit/Player.h"
#include "Game/Gun/Bazooka.h"

Soldier * Soldier::Create()
{
	Soldier* pRet = new Soldier();
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

bool Soldier::Init()
{
	if(!MetalUnit::Init())
		return false;

	AddFlag(FLAG_ENEMY);
	Tag(TAG_ENEMY);

	SetHpPoint(1);
	_target = nullptr;
	attackCoolTime = 0.2f;

	moveSpeed = 50.0f;
	turnTimer = 3.0f;

	soldierAnimation = Animation::Create();

	wstring soldierTexture = Textures + L"MetalSlug/Soldier.png";
	wstring soldierTexture2 = Textures + L"MetalSlug/Soldier2.png";

	Clip* clip;

	{
		//idle0
		clip = Clip::Create(PlayMode::Loop);
		clip->AnchorPosition(0.5f, 0.0f);
		clip->AddFrame(Sprite::Create(soldierTexture, 2, 2, 41, 48), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 41, 2, 82, 48), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 82, 2, 123, 48), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 123, 2, 162, 48), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 162, 2, 203, 48), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 203, 2, 244, 48), 0.1f);
		soldierAnimation->AddClip(clip);
	}

	{
		//Run1
		clip = Clip::Create(PlayMode::Loop);
		clip->AnchorPosition(0.5f, 0.0f);
		clip->AddFrame(Sprite::Create(soldierTexture, 2, 53, 45, 94), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 45, 53, 90, 94), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 90, 53, 134, 94), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 134, 53, 177, 94), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 177, 53, 220, 94), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 220, 53, 263, 94), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 263, 53, 307, 94), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 307, 53, 349, 94), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 349, 53, 388, 94), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 388, 53, 428, 94), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 428, 53, 470, 94), 0.1f);
		soldierAnimation->AddClip(clip);
	}

	{
		//Attack2
		clip = Clip::Create(PlayMode::End);
		clip->AnchorPosition(0.5f, 0.0f);
		clip->AddFrame(Sprite::Create(soldierTexture, 2, 240, 44, 286), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture, 44, 240, 88, 286), 0.5f);
		clip->AddFrame(Sprite::Create(soldierTexture, 88, 240, 133, 286), 0.3f);
		clip->AddFrame(CC_CALLBACK_0(Soldier::Attack, this));
		clip->SetEndCallBack([=]()->void
			{
				soldierAnimation->Play(0);
			});
		soldierAnimation->AddClip(clip);
	}

	{
		//Died3
		clip = Clip::Create(PlayMode::End);
		clip->AnchorPosition(0.5f, 0.0f);
		clip->AddFrame(Sprite::Create(soldierTexture2, 239, 1096, 277, 1145), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture2, 277, 1096, 314, 1145), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture2, 314, 1096, 350, 1145), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture2, 350, 1096, 386, 1145), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture2, 386, 1096, 422, 1145), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture2, 422, 1096, 457, 1145), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture2, 457, 1096, 494, 1145), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture2, 494, 1096, 531, 1145), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture2, 531, 1096, 566, 1145), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture2, 566, 1096, 603, 1145), 0.1f);
		clip->AddFrame(Sprite::Create(soldierTexture2, 603, 1096, 642, 1145), 0.1f);
		clip->SetEndCallBack([=]()->void
			{
				StartTwinkle(soldierAnimation);
				AddEvent(CC_CALLBACK_0(Node::RemoveSelf, this), 0.5f);
			});
		soldierAnimation->AddClip(clip);
	}
	soldierAnimation->Play(1);
	AddChild(soldierAnimation);

	AnchorPosition(0.5f, 0.0f);
	collider->Size(20, 40);

	_weapon = Bazooka::Create();
	_weapon->Position(0.0f, 30.0f);
	_weapon->SetFlag(FLAG_EBULLET);
	AddChild(_weapon);

	//인식범위 radius
	detectRadius = EventBox::Create(CC_CALLBACK_2(Soldier::DetectPlayer,this),320,200);
	detectRadius->AnchorPosition(0.5f, 0.5f);
	detectRadius->AddFlag(FLAG_CAMERAOB);
	AddChild(detectRadius);

	direction = 1;
	phase = 0;


	return true;
}

void Soldier::Update()
{
	if (phase == 0)
	{
		auto position = Position();
		if (turnTimer > 0)
		{
			turnTimer -= Time::deltaTime();
		}
		else
		{
			turnTimer = 3.0f;
			direction *= -1;
			if (direction < 0)
			{
				RotationDegree(0, 180, 0);
			}
			else
			{
				RotationDegree(0, 0, 0);
			}
		}
		position.x -= direction*moveSpeed * Time::deltaTime();
		Position(position);
	}
	else if(phase == 1)
	{
		if (Math::Distance(_target->NormalizedPosition(), this->NormalizedPosition()) > 500)
		{
			phase = 2;
			soldierAnimation->Play(1);
		}
		else
		{
			if (attackCoolTime <= 0)
			{
				soldierAnimation->Play(2);
				attackCoolTime = 2.0f;
			}
			else
				attackCoolTime -= Time::deltaTime();
		}
	}
	else if(phase == 2)
	{
		if (Math::Distance(_target->NormalizedPosition(), this->NormalizedPosition()) <= 500)
		{
			phase = 1;
			soldierAnimation->Play(0);
		}
		else
		{
			auto position = Position();
			turnTimer = 3.0f;
			if (_target->NormalizedPosition().x > this->NormalizedPosition().x)
			{
				direction = -1;
				RotationDegree(0, 180, 0);
			}
			else
			{
				direction = 1;
				RotationDegree(0, 0, 0);
			}
			position.x -= direction * moveSpeed * Time::deltaTime();
			Position(position);
		}
	}
	PhysicsObject::Update();
}

void Soldier::DetectPlayer(PhysicsObject* object, ColDirection direction)
{
	if (phase == 3)
		return;
	if (object->Tag() == TAG_PLAYER)
	{
		phase = 1;
		soldierAnimation->Play(0);
		_target = object;
		detectRadius->RemoveSelf();
	}
}

void Soldier::Attack()
{
	if (_target == nullptr)
		return;

	if (_target->NormalizedPosition().x > this->NormalizedPosition().x)
	{
		RotationDegree(0, 180, 0);
	}
	else
	{
		RotationDegree(0, 0, 0);
	}

	float slope = (_target->NormalizedPosition().y+40.0f - _weapon->NormalizedPosition().y) / (_target->NormalizedPosition().x - _weapon->NormalizedPosition().x);
	auto direction = RotationDegree().y > 0 ? 0 : 180;
	float degree = direction + Math::ToDegree(slope);

	_weapon->SetAngle(degree);
	_weapon->StartShoot();
}

void Soldier::DamageTaken(float damage)
{
	SetHpPoint(GetHpPoint() - damage);
}

void Soldier::EnterPhysicsCollision(PhysicsObject * b, ColDirection direction)
{
}

void Soldier::OnPhysicsCollision(PhysicsObject * b, ColDirection direction)
{
}

void Soldier::ExitPhysicsCollision(PhysicsObject * b)
{
}

void Soldier::Died()
{
	soldierAnimation->Play(3);
	DecFlag(FLAG_ENEMY);
	phase = 3;
}
