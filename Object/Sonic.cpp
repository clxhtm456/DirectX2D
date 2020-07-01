#include "stdafx.h"
#include "Sonic.h"

#define MAX_SPEED 800.0f
#define MAX_ROLL_SPEED 3000.0f
#define MOVE_SPEED 0.0f
#define ACCEL_SPEED 200.0f

Sonic::Sonic(D3DXVECTOR2 position, D3DXVECTOR2 scale)
	: position(position), moveSpeed(MOVE_SPEED), focusOffset(-500, -400), state(PlayerState::IDLE)
	, accel(ACCEL_SPEED), rollSpeed(0.0f), direction(true)
	, gravity(-10.0f), velocity(0.0f), bGround(true)
{
	player = new Animation();
	wstring spriteFile = Textures + L"./Sonic/PlayerSonic.png";
	wstring shaderFile = Shaders + L"Effect.fx";

	Clip* clip;
	//Idle(0 ~ 1)
	{
		clip = new Clip(PlayMode::End);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 2, 37, 30, 78), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 28, 37, 58, 78), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 55, 37, 86, 78), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 83, 37, 117, 78), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 115, 37, 147, 78), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 147, 37, 181, 78), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 178, 37, 213, 78), 0.3f);
		player->AddClip(clip);

		//idle2
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 93, 427, 119, 466), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 123, 426, 148, 465), 0.3f);
		player->AddClip(clip);
	}

	//Run(2 ~ 3)
	{
		//Normal Run
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 4, 80, 32, 119), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 36, 80, 62, 119), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 65, 80, 96, 119), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 96, 80, 135, 119), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 134, 80, 166, 119), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 170, 80, 202, 119), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 208, 80, 230, 119), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 236, 80, 263, 119), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 266, 80, 293, 119), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 293, 80, 331, 119), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 332, 80, 366, 119), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 368, 80, 400, 119), 0.1f);
		player->AddClip(clip);

		//Fast Run
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 72, 238, 102, 274), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 111, 238, 140, 274), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 149, 238, 178, 274), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 185, 238, 218, 274), 0.1f);
		player->AddClip(clip);
	}

	//Sit Down(4)
	{
		clip = new Clip(PlayMode::End);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 267, 43, 295, 76), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 299, 50, 331, 76), 0.3f);
		player->AddClip(clip);
	}

	//Roll(5)[Turbo]
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 71, 282, 111, 318), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 120, 282, 164, 318), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 171, 282, 211, 318), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 216, 282, 259, 318), 0.1f);

		player->AddClip(clip);
	}

	//Jump or Roll이 끝난후(6)
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 1, 169, 31, 200), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 34, 169, 62, 200), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 67, 169, 99, 200), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 130, 169, 161, 200), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 192, 169, 223, 200), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 228, 169, 260, 200), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 264, 169, 300, 200), 0.02f);

		clip->AddFrame(new Sprite(spriteFile, shaderFile, 0, 205, 31, 233), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 34, 205, 67, 233), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 69, 205, 100, 233), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 105, 205, 133, 233), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 139, 205, 167, 233), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 173, 205, 203, 233), 0.02f);

		player->AddClip(clip);
	}

	//Push(7)
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 8, 436, 37, 472), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 48, 435, 72, 472), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 80, 436, 108, 472), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 120, 435, 144, 472), 0.3f);
		player->AddClip(clip);
	}

	player->Scale(scale);
	player->Position(position);
	player->Play(static_cast<UINT>(state));

	player->DrawBound(true);
}

Sonic::~Sonic()
{
	SAFE_DELETE(player);
}


void Sonic::Focus(D3DXVECTOR2 * position, D3DXVECTOR2 * size)
{
	*position = player->Position() - focusOffset;
	(*size) = D3DXVECTOR2(1, 1);
}

Sprite * Sonic::GetSprite()
{
	return player->GetSprite();
}

void Sonic::Scale(D3DXVECTOR2 vec)
{
	player->Scale(vec);
}

float pressKeyTime = 0.0f;
void Sonic::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	textureSize.x = player->Scale().x * player->TextureSize().x;
	textureSize.y = player->Scale().y * player->TextureSize().y;

	//botomVector
	{
		bottomVector.x = position.x;
		bottomVector.y = position.y - textureSize.y * 0.5f - 40.0f;
	}

	if (moveSpeed == 0.0f)
	{
		//IDLE 조건
		if (state != PlayerState::IDLE_LIE_DOWN && state != PlayerState::TURBO && bGround)
		{
			state = PlayerState::IDLE;

			//IDLE1 -> IDLE2
			if (player->GetCurrentClip() == 0 && player->GetClip()->EndFrame())
				state = PlayerState::IDLE_LIE_DOWN;
		}
	}

	//이동
	position.x += moveSpeed * Time::Delta();

	//라인 위에 있을 때
	if (bGround && bLineBottomColision)
	{
		slope = (lineRightPoint.y - lineLeftPoint.y) / (lineRightPoint.x - lineLeftPoint.x);
		position.y = slope * (position.x - lineLeftPoint.x) + lineLeftPoint.y + (textureSize.y * 0.5f);
	}
	else
	{
		bGround = false;
	}

	//y축 가속도 업데이트

	velocity += gravity * Time::Delta();

	//이동키
	if (Key->Press('A'))
	{
		MoveLeft();

		if (bLineBottomColision)
			degree.z = Math::ToDegree(slope);
		else
			degree.z = 0.0f;
	}

	else if (Key->Press('D'))
	{
		MoveRight();

		if (bLineBottomColision)
			degree.z = Math::ToDegree(slope);
		else
			degree.z = 0.0f;
	}

	else if (Key->Press('S') && moveSpeed == 0.0f)
		SitDown();

	else
	{
		//감속
		if (moveSpeed > 0.0f)
			moveSpeed -= accel * 10 * Time::Delta();
		else if(moveSpeed < 0.0f)
			moveSpeed += accel * 10 * Time::Delta();

		//속도가 일정 수준 이하면 속도 0으로 고정
		if (moveSpeed < 10.0f && moveSpeed > -10.0f)
			moveSpeed = 0.0f;
	}

	//앉았다 일어날 때 터보인 경우 ROLL로 변경
	if (Key->Up('S'))
	{
		if (rollSpeed != 0.0f)
			state = PlayerState::ROLL;
		moveSpeed = rollSpeed;
		rollSpeed = 0.0;
	}

	//점프
	if (bGround == false)
	{
		position.y += velocity;
	}

	if (bLineBottomColision)
	{
		velocity = 0.0f;
		bGround = true;
	}

	if (Key->Press(VK_SPACE) && state != PlayerState::TURBO)
	{
		if (bGround == true)
		{
			velocity = 2.5f;
			bGround = false;
			state = PlayerState::ROLL;
		}
	}

	else if (Key->Up(VK_SPACE))
	{
		if (velocity > 0.25f)
			velocity = 0.25f;
	}

	//좌우 방향키 눌린 시간 초기화
	if (Key->Up('A'))
	{
		pressKeyTime = 0.0f;
	}
	if (Key->Up('D'))
	{
		pressKeyTime = 0.0f;
	}

	player->Play(static_cast<UINT>(state));
	player->Position(position);
	player->RotationDegree(degree);
	player->Update(V, P);

}

void Sonic::Render()
{
	ImGui::Separator();
	ImGui::LabelText("Move Speed", "%f", moveSpeed);
	ImGui::LabelText("Roll Speed", "%f", rollSpeed);
	ImGui::LabelText("Sonic State", "%d", state);
	ImGui::LabelText("PlayerSonic Position", "%f, %f", position.x, position.y);
	ImGui::LabelText("On Ground", "%d", bGround ? 1 : 0);

	player->Render();
}


void Sonic::MoveLeft()
{
	direction = false;
	pressKeyTime += Time::Delta();

	if (moveSpeed > -MAX_SPEED)
		moveSpeed -= accel * Time::Delta();
	else
		moveSpeed = -MAX_SPEED;

	// test
	if (moveSpeed > 0.0f)
		moveSpeed -= accel * 5.0f * Time::Delta();

	//누르고 있다가 -300 을 돌파하면
	if (moveSpeed < -300.0 && ((state != PlayerState::ROLL && state != PlayerState::PUSH) || bGround))
		state = PlayerState::FAST_RUN;

	else if ((state != PlayerState::ROLL && state != PlayerState::PUSH) || bGround)
		state = PlayerState::NORMAL_RUN;

	degree.y = 180.0f;

}

void Sonic::MoveRight()
{
	direction = true;
	pressKeyTime += Time::Delta();

	if (moveSpeed < MAX_SPEED)
		moveSpeed += accel * Time::Delta();
	else
		moveSpeed = MAX_SPEED;

	// test
	if (moveSpeed < 0.0f)
		moveSpeed += accel * 5.0f * Time::Delta();

	//누르고 있다가 +300 을 돌파하면
	if (moveSpeed > 300.0 && ((state != PlayerState::ROLL && state != PlayerState::PUSH) || bGround))
		state = PlayerState::FAST_RUN;

	else if ((state != PlayerState::ROLL && state != PlayerState::PUSH) || bGround)
		state = PlayerState::NORMAL_RUN;

	degree.y = 0.0f;
}

void Sonic::SitDown()
{
	if (state != PlayerState::TURBO)
		state = PlayerState::SIT_DOWN;

	if (Key->Press(VK_SPACE))
	{
		state = PlayerState::TURBO;

		if (direction == true && rollSpeed < MAX_ROLL_SPEED)
			rollSpeed += accel * 20 * Time::Delta();
		else if (direction == false && rollSpeed > -MAX_ROLL_SPEED)
			rollSpeed -= accel * 20 * Time::Delta();
	}
}





