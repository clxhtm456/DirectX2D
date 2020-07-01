#include "stdafx.h"
#include "Mole.h"

#define MAX_LIMIT_X 50.0f
#define SPEED 20.0f

Mole::Mole(D3DXVECTOR2 position)
	:speed(SPEED), start(position), position(position)
{
	wstring textureFile = Textures + L"./Sonic/enemies.png";
	wstring shaderFile = Shaders + L"Effect.fx";

	mole = new Animation();

	Clip* clip;

	//Move
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(textureFile, shaderFile, 0, 605, 18, 624), 0.2f);
		clip->AddFrame(new Sprite(textureFile, shaderFile, 30, 605, 50, 624), 0.2f);

		mole->AddClip(clip);
	}

	mole->Scale(2.5f, 2.5f);
	mole->Play(0);
}

Mole::~Mole()
{
	SAFE_DELETE(mole);
}

void Mole::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	position.x += speed * Time::Delta();

	if (start.x + MAX_LIMIT_X < position.x)
	{
		speed = -SPEED;
		mole->Rotation(0, 0, 0);
	}

	if (start.x > position.x)
	{
		speed = SPEED;
		mole->RotationDegree(0, 180, 0);
	}

	mole->Position(position);
	mole->Play(0);
	mole->Update(V, P);
}

void Mole::Render()
{
	mole->Render();
}
