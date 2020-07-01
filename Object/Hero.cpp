#include "stdafx.h"
#include "Hero.h"

Hero::Hero(D3DXVECTOR2 position, D3DXVECTOR2 scale)
	:position(position), moveSpeed(200.0f)
{
	animation = new Animation();

	wstring spriteFile = Textures + L"./Astar/character.png";
	wstring shaderFile = Shaders + L"Effect.fx";

	Clip* clip;

	//Left 0
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 25, 54, 39, 73), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 25, 79, 39, 97), 0.3f);
		animation->AddClip(clip);
	}

	//Right 1
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 74, 54, 90, 73), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 74, 79, 90, 97), 0.3f);
		animation->AddClip(clip);
	}

	//Up
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 48, 54, 65, 73), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 48, 79, 66, 97), 0.3f);
		animation->AddClip(clip);
	}

	//Down
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 0, 54, 17, 73), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 0, 79, 17, 97), 0.3f);
		animation->AddClip(clip);
	}

	animation->Position(position);
	animation->Scale(scale);
	animation->Play(0);
}

Hero::~Hero()
{
	SAFE_DELETE(animation);
}

void Hero::Update(D3DXMATRIX& V, D3DXMATRIX& P)
{
	position = animation->Position();
	bool bMove = false;

	if (Key->Press('A'))
	{
		bMove = true;
		position.x -= moveSpeed * Time::Delta();
		animation->Play(0);
	}
	else if (Key->Press('D'))
	{
		bMove = true;
		position.x += moveSpeed * Time::Delta();
		animation->Play(1);
	}
	if (Key->Press('W'))
	{
		bMove = true;
		position.y += moveSpeed * Time::Delta();
		animation->Play(2);
	}
	else if (Key->Press('S'))
	{
		bMove = true;
		position.y -= moveSpeed * Time::Delta();
		animation->Play(3);
	}

	animation->Position(position);
	animation->Update(V, P);
}

void Hero::Render()
{
	animation->Render();
}

Sprite * Hero::GetSprite()
{
	return animation->GetSprite();
}
