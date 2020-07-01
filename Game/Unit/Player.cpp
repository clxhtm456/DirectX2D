#include "stdafx.h"

#include "Player.h"
#include "Game/FlagList.h"
static float BodyOffSetY = 8.0f;
static float BodyOffSetX = 11.0f;
static float shotSpeed = 1500.0f;
#define MOVEVIBRATION	3
#define MOVEVIBRATION_SPEED	10
#define JUMP_POWER		200.0f
#define MOVESPEED	400

Player * Player::Create()
{
	Player* pRet = new Player();
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

bool Player::Init()
{
	if (!PhysicsObject::Init())
		return false;
	Tag(TAG_PLAYER);
	AddFlag(FLAG_PLAYER);

	AnchorPosition(0.5f, 0.0f);
	collider->Position(0, 0);
	collider->Size(5,40);


	bodyAnim = Animation::Create();
	legAnim = Animation::Create();
	extraAnim = Animation::Create();

	AddChild(bodyAnim, 1);
	AddChild(legAnim, 0);
	AddChild(extraAnim, 1);

	landHeight = 10000.0;

	wstring spriteFile = Textures + L"MetalSlug/Marco_sprite.png";

	Clip* clip;

	//Body
	{
		//Idle0
		{
			clip = Clip::Create(PlayMode::Loop);

			clip->AddFrame(Sprite::Create(spriteFile, 12, 8, 41, 37), 0.2f);
			clip->AddFrame(Sprite::Create(spriteFile, 45, 8, 74, 37), 0.2f);
			clip->AddFrame(Sprite::Create(spriteFile, 78, 8, 108, 37), 0.2f);
			clip->AddFrame(Sprite::Create(spriteFile, 112, 8, 142, 37), 0.2f);
			clip->Position(4, 8);
			clip->AnchorPosition(0.5f, 0.0f);
			bodyAnim->AddClip(clip);
		}
		//Run1
		{
			clip = Clip::Create(PlayMode::Loop);

			clip->AddFrame(Sprite::Create(spriteFile, 47, 148, 77, 177), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 79, 148, 107, 177), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 109, 148, 136, 177), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 139, 148, 168, 177), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 171, 148, 201, 177), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 204, 148, 236, 177), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 239, 148, 271, 177), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 274, 148, 306, 177), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 308, 148, 339, 177), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 342, 148, 373, 177), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 376, 148, 407, 177), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 13, 148, 45, 177), 0.03f);
			clip->Position(4, 12);
			clip->AnchorPosition(0.5f, 0.0f);

			bodyAnim->AddClip(clip);
		}
		//Shot2
		{
			clip = Clip::Create(PlayMode::End);

			clip->AddFrame(Sprite::Create(spriteFile, 227, 6, 281, 31), 0.01f);
			clip->AddFrame(Sprite::Create(spriteFile, 281, 6, 336, 31), 0.01f);
			clip->AddFrame(CC_CALLBACK_0(Player::CreateShot, this));
			clip->AddFrame(Sprite::Create(spriteFile, 336, 6, 393, 31), 0.01f);
			clip->AddFrame(Sprite::Create(spriteFile, 393, 6, 436, 31), 0.02f);
			clip->AddFrame(Sprite::Create(spriteFile, 436, 6, 478, 31), 0.02f);
			clip->AddFrame(Sprite::Create(spriteFile, 478, 6, 519, 31), 0.02f);
			clip->AddFrame(Sprite::Create(spriteFile, 519, 6, 560, 31), 0.02f);
			clip->AddFrame([=]()->void
			{
				SetShootable(true);
			});
			clip->AddFrame(Sprite::Create(spriteFile, 560, 6, 598, 31), 0.02f);
			clip->AddFrame(Sprite::Create(spriteFile, 598, 6, 635, 31), 0.02f);
			clip->AddFrame(Sprite::Create(spriteFile, 635, 6, 677, 31), 0.02f);

			clip->AddFrame(Sprite::Create(spriteFile, 15, 105, 51, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 51, 105, 89, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 89, 105, 127, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 127, 105, 166, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 166, 105, 206, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 206, 104, 245, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 245, 104, 284, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 284, 104, 324, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 324, 104, 365, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 365, 104, 403, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 403, 103, 441, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 441, 103, 480, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 480, 102, 519, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 519, 101, 561, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 561, 101, 603, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 603, 101, 646, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 646, 101, 689, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 689, 100, 733, 135), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 733, 100, 779, 135), 0.06f);
			clip->SetStartCallBack(
				[=]()->void {
				//SetLookTop
				bPlayBodyAnim = true;
			}
			);
			clip->SetEndCallBack(
				[=]()->void {
				//SetIdle
				bPlayBodyAnim = false;
				state = AnimationState::IDLE;
			}
			);
			clip->Position(-10, 38);
			clip->AnchorPosition(0, 1);
			bodyAnim->AddClip(clip);
		}
		//LookTopStart3
		{
			clip = Clip::Create(PlayMode::End);

			clip->AddFrame(Sprite::Create(spriteFile, 1110, 0, 1143, 50), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 1146, 0, 1179, 50), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 1182, 0, 1214, 50), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 1216, 0, 1245, 50), 0.06f);
			clip->AddFrame([=]()->void {
				state = AnimationState::LOOKTOP;
				bPlayBodyAnim = false;
				});
			clip->SetStartCallBack(
				[=]()->void {
				//SetLookTop
				bPlayBodyAnim = true;
			}
			);
			clip->Position(4, -4);
			clip->AnchorPosition(0.5f, 0);

			bodyAnim->AddClip(clip);
		}
		//LookTop4
		{
			clip = Clip::Create(PlayMode::Loop);
			clip->AddFrame(Sprite::Create(spriteFile, 15, 57, 47, 83), 0.2f);
			clip->AddFrame(Sprite::Create(spriteFile, 47, 57, 81, 83), 0.2f);
			clip->AddFrame(Sprite::Create(spriteFile, 81, 57, 114, 83), 0.2f);
			clip->AddFrame(Sprite::Create(spriteFile, 114, 57, 147, 83), 0.2f);
			clip->Position(4, 12);
			clip->AnchorPosition(0.5f, 0);
			bodyAnim->AddClip(clip);
		}
		//TopShoot5
		{
			clip = Clip::Create(PlayMode::End);
			clip->AddFrame(Sprite::Create(spriteFile, 147, 16, 173, 83), 0.01f);
			clip->AddFrame(Sprite::Create(spriteFile, 173, 16, 199, 83), 0.01f);
			clip->AddFrame(CC_CALLBACK_0(Player::CreateShot, this));
			clip->AddFrame(Sprite::Create(spriteFile, 199, 16, 225, 83), 0.01f);
			clip->AddFrame(Sprite::Create(spriteFile, 225, 32, 251, 83), 0.04f);
			clip->AddFrame(Sprite::Create(spriteFile, 251, 32, 280, 83), 0.04f);
			clip->AddFrame([=]()->void
			{
				SetShootable(true);
			});
			clip->AddFrame(Sprite::Create(spriteFile, 280, 32, 308, 83), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 308, 32, 336, 83), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 336, 32, 365, 83), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 365, 32, 394, 83), 0.06f);
			clip->AddFrame(Sprite::Create(spriteFile, 394, 32, 423, 83), 0.06f);
			clip->SetStartCallBack(
				[=]()->void {
				bPlayBodyAnim = true;
			}
			);
			clip->SetEndCallBack(
				[=]()->void {
				bPlayBodyAnim = false;
				if (bLookTop)
					state = AnimationState::LOOKTOP;
				else
					state = AnimationState::IDLE;
			}
			);
			clip->Position(4, 14);
			clip->AnchorPosition(0.5f, 0);
			bodyAnim->AddClip(clip);
		}
		//JumpIdle6
		{
			clip = Clip::Create(PlayMode::Loop);
			clip->AddFrame(Sprite::Create(spriteFile, 841, 147, 871, 182), 0.2f);
			clip->Position(0, 4);
			clip->AnchorPosition(0.5f, 0);
			bodyAnim->AddClip(clip);
		}
		//JumpBehind7
		{
			clip = Clip::Create(PlayMode::Loop);
			clip->AddFrame(Sprite::Create(spriteFile, 432, 40, 455, 100), 0.2f);
			bodyAnim->AddClip(clip);
		}
		//JumpBehindShoot8
		{
			clip = Clip::Create(PlayMode::End);
			clip->AddFrame(Sprite::Create(spriteFile, 455, 40, 478, 100), 0.01f);
			clip->AddFrame(Sprite::Create(spriteFile, 478, 40, 501, 100), 0.01f);
			clip->AddFrame(CC_CALLBACK_0(Player::CreateShot, this));
			clip->AddFrame(Sprite::Create(spriteFile, 501, 40, 524, 100), 0.01f);
			clip->AddFrame(Sprite::Create(spriteFile, 524, 40, 550, 100), 0.04f);
			clip->AddFrame(Sprite::Create(spriteFile, 550, 40, 573, 100), 0.04f);
			clip->AddFrame([=]()->void
			{
				SetShootable(true);
			});
			clip->AddFrame(Sprite::Create(spriteFile, 573, 40, 597, 100), 0.04f);
			clip->SetStartCallBack(
				[=]()->void {
				bPlayBodyAnim = true;
			}
			);
			clip->SetEndCallBack(
				[=]()->void {
				bPlayBodyAnim = false;
			}
			);
			bodyAnim->AddClip(clip);
		}
	}

	//Leg
	{
		//Idle0
		{
			clip = Clip::Create(PlayMode::Loop);

			clip->AddFrame(Sprite::Create(spriteFile, 130, 1536, 151, 1553), 0.2f);
			clip->AnchorPosition(0.5f, 0.0f);
			legAnim->AddClip(clip);
		}
		//Run1
		{
			clip = Clip::Create(PlayMode::Loop);

			clip->AddFrame(Sprite::Create(spriteFile, 256, 1533, 280, 1553), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 280, 1534, 311, 1554), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 311, 1535, 345, 1555), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 345, 1534, 367, 1554), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 367, 1533, 385, 1553), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 385, 1534, 404, 1554), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 404, 1535, 428, 1555), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 428, 1534, 457, 1554), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 457, 1533, 491, 1553), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 491, 1534, 513, 1554), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 513, 1535, 530, 1555), 0.03f);
			clip->AddFrame(Sprite::Create(spriteFile, 530, 1534, 551, 1554), 0.03f);
			clip->AnchorPosition(0.5f, 0.0f);
			legAnim->AddClip(clip);
		}
		//Jump2
		{
			clip = Clip::Create(PlayMode::Loop);

			clip->AddFrame(Sprite::Create(spriteFile, 1016, 1530, 1049, 1552), 0.1f);
			clip->AnchorPosition(0.5f, 0.2f);
			legAnim->AddClip(clip);
		}
		//Normal_Jump3
		{
			clip = Clip::Create(PlayMode::Loop);

			clip->AddFrame(Sprite::Create(spriteFile, 781, 1529, 802, 1551), 0.1f);
			clip->AnchorPosition(0.5f, 0.2f);
			legAnim->AddClip(clip);
		}
	}
	legAnim->Play(0);
	bodyAnim->Play(0);

	
	EquipHandgun();

	/*for (int i = 0; i < 40; i++)
	{
	auto bullet = new Bullet();
	bulletList.push_back(bullet);
	}*/

	return true;
}

Player::Player():
	moveSpeed(MOVESPEED),
	focusOffSet(100,0),
	bDrawBound(false),
	bMove(false),
	extraHeight(0),
	bodyOffSet(BodyOffSetY),
	bShootable(true),
	bDrawExtraAnim(false),
	bLookTop(false),
	bPlayBodyAnim(false),
	state(AnimationState::IDLE)
{
}

Player::~Player()
{
	delete rect;
}

void Player::Update()
{
	D3DXVECTOR2 position = Position();

	if (Key->Press(VK_UP))
	{
		LookTop();
		getWeapon()->MoveAngle(90.0f);
		
	}else if (Key->Press(VK_DOWN))
	{
		if (!bGround)
		{
			getWeapon()->MoveAngle(270.0f);
			state = AnimationState::LOOK_BEHIND;

			legState = LegState::NORMAL_JUMP;
			legAnim->Play(3);
			bLookDown = true;
		}
		else
		{
			state = AnimationState::IDLE;
			if (!bLookTop)
			{
				getWeapon()->SetAngle(RotationDegree().y);
			}
			bLookDown = false;
		}
	}
	else
	{
		LookDown();
		if (!bGround)
		{
			if (!bLookTop)
			{
				getWeapon()->MoveAngle(RotationDegree().y);
			}
			state = AnimationState::IDLE;

			legState = LegState::RUN_JUMP;
			legAnim->Play(2);
			bLookDown = false;
		}
		else
		{
			getWeapon()->MoveAngle(RotationDegree().y);
		}
	}

	if (Key->Press(VK_LEFT))
	{
		bMove = true;
		RotationDegree(0, 180, 0);
		if (!bLookTop && !bLookDown)
		{
			getWeapon()->SetAngle(RotationDegree().y);
		}
		if (bGround == true)
			legState = LegState::RUN;
		position.x -= moveSpeed * Time::deltaTime();
	}
	else if (Key->Press(VK_RIGHT))
	{
		bMove = true;
		
		RotationDegree(0, 0, 0);
		if (!bLookTop && !bLookDown)
		{
			getWeapon()->SetAngle(RotationDegree().y);
		}
		if (bGround == true)
			legState = LegState::RUN;

		position.x += moveSpeed * Time::deltaTime();
	}
	else
	{
		bMove = false;
		if (bGround == true)
			legState = LegState::IDLE;
	}

	if (Key->Down(VK_SPACE))
	{
		Jump();
	}

	

	//if (Key->Press('W'))
	//{
	//	animation->Play(1);
	//}
	//else if (Key->Press('S'))
	//{
	//	animation->Play(1);
	//}
	//
	StartAnimation();
	switch (legState)
	{
	case LegState::IDLE:
		legAnim->Play(0);
		break;
	case LegState::RUN:
		legAnim->Play(1);
		break;
	case LegState::RUN_JUMP:
		//legAnim->Play(2);
		break;
	case LegState::NORMAL_JUMP:
		//legAnim->Play(3);
		break;
	default:
		break;
	}

	if(bMove && bGround)
	{
		extraHeight += MOVEVIBRATION_SPEED * Time::deltaTime();
		if (extraHeight >= MOVEVIBRATION)
			extraHeight = 0;
	}
	

	if (Key->Down('Z'))
	{
		Shoot();
	}
	
	
	Position(position);

	auto direction = RotationDegree().y > 0 ? 1 : -1;

	if (bLineBottonCollision)
		landHeight = Position().y;

	PhysicsObject::Update();
}

void Player::Render()
{
	//ImGui::LabelText("shotAngle", "%0.2f", getWeapon()->GetAngle());
	ImGui::LabelText("WeaponPos", "x:%0.2f y:%0.2f", getWeapon()->NormalizedPosition().x,getWeapon()->NormalizedPosition().y);
	//ImGui::LabelText("test", "%d", Math::NumberOfFigure(51,3));
	PhysicsObject::Render();
}

void Player::LookTop()
{
	if (bLookTop)
		return;
	bLookTop = true;

	if (!bShootable)
		return;
	
	/*if (!bShootable)
	{
		state = AnimationState::TOPSHOOT;
	}
	else
	{*/
		state = AnimationState::STARTLOOKTOP;
		if (bodyAnim->GetCurrentClip() != 3 && bodyAnim->GetCurrentClip() != 4)
			BodyPlayAnimation(3);
	//}
	
	
}

void Player::LookDown()
{
	if (!bShootable)
		return;
	if(state == AnimationState::STARTLOOKTOP || state == AnimationState::LOOKTOP)
	{
		bPlayBodyAnim = false;
		state = AnimationState::IDLE;
	}
	
	//bPlayBodyAnim = false;
	bLookTop = false;
}

void Player::StartCrouch()
{
}

void Player::EndCrouch()
{
}

void Player::Jump()
{
	if (bGround == true)
	{
		legState = LegState::RUN_JUMP;
		legAnim->Play(2);

		velocity = JUMP_POWER;
		bGround = false;
		bLineBottonCollision = false;
		objbottomCollision = false;

		_position.y += velocity * Time::deltaTime() * _scale.y;
	}
}

void Player::StartAnimation()
{
	if (state == AnimationState::TOPSHOOT || state == AnimationState::SHOOT || state == AnimationState::SHOOT_BEHIND)
		return;

	
	if(!bPlayBodyAnim)
	{
		if (state == AnimationState::LOOKTOP && (bodyAnim->GetCurrentClip() != 4 || bodyAnim->GetCurrentClip() != 3))
		{
			BodyPlayAnimation(4);
		}
		else if (state == AnimationState::LOOK_BEHIND)
		{
			BodyPlayAnimation(7);
		}
		else if (!bGround)
		{
			BodyPlayAnimation(6);
		}
		else if (!bMove)
		{
			BodyPlayAnimation(0);
		}
		else
		{
			BodyPlayAnimation(1);
		}
	}
		
}

void Player::Focus(D3DXVECTOR2 & position, D3DXVECTOR2 & size)
{
	auto direction = RotationDegree().y > 0 ? 1 : -1;
	position.x = Position().x - direction*focusOffSet.x;
	if(landHeight >= 10000.0f)
		position.y = Position().y + 150;
	else
		position.y = landHeight+150;
	size = Scale();
}

void Player::SetShootable(bool val)
{
	bShootable = val;
}

void Player::Shoot()
{
	if (!bShootable)
		return;

	bShootable = false;
	if (bLookTop || state == AnimationState::TOPSHOOT)
	{
		state = AnimationState::TOPSHOOT;
		BodyPlayAnimation(5);
	}
	else if (bGround == false && (state == AnimationState::LOOK_BEHIND || state == AnimationState::SHOOT_BEHIND))
	{
		state = AnimationState::SHOOT_BEHIND;
		BodyPlayAnimation(8);
	}
	else
	{
		state = AnimationState::SHOOT;
		BodyPlayAnimation(2);
	}
	
}

void Player::BehindShoot()
{
}

void Player::BodyPlayAnimation(int value)
{
	int extra = value;
	bodyAnim->Play(extra);
}

void Player::LegPlayAnimation(int value)
{
	int extra = value;
	legAnim->Play(extra);
}


void Player::SetWeapon(Weapon * weapon)
{
	if (_weapon != nullptr)
	{
		_weapon->ClearBullet();
		_weapon->removeFromParent();
		_weapon = nullptr;
	}
	_weapon = weapon;
	_weapon->SetZeroAmmoCallback(CC_CALLBACK_0(Player::EquipHandgun,this));
	_weapon->SetFlag(FLAG_PBULLET);
	_weapon->SetAmmoObserver(ammoObserver);
	_weapon->Position(0.0f, 20.0f);
	AddChild(_weapon);
}

void Player::EquipHandgun()
{
	if (_weapon != nullptr)
	{
		_weapon->ClearBullet();
		_weapon->removeFromParent();
		_weapon = nullptr;
	}
	_weapon = HandGun::Create();
	_weapon->SetFlag(FLAG_PBULLET);
	_weapon->SetAmmoObserver(ammoObserver);
	_weapon->Position(0.0f, 20.0f);
	AddChild(_weapon);
}

Weapon* Player::getWeapon()
{
	return _weapon;
}

void Player::SetAmmoObserver(IAmmoObserver* observer)
{
	ammoObserver = observer;
	_weapon->SetAmmoObserver(observer);
}

void Player::CreateShot()
{
	getWeapon()->StartShoot();
}

void Player::DrawBloom(bool val)
{
	//animation->DrawBloom(val);
}

void Player::DrawTwinkle(bool val)
{
	bodyAnim->DrawTwinkle(val);
	legAnim->DrawTwinkle(val);
}

void Player::ExitPhysicsCollision(PhysicsObject * b)
{
}

void Player::OnPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
}

void Player::DamageTaken(float damage)
{
}

void Player::Died()
{
}


void Player::EnterPhysicsCollision(PhysicsObject * b, ColDirection direction)
{

	//died
}
