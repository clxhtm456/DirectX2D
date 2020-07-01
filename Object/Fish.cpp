#include "stdafx.h"
#include "Fish.h"

#define MAX_LIMIT_Y 200.0f
#define SPEED 200.0f

Fish::Fish(D3DXVECTOR2 position)
	:speed(SPEED), start(position), position(position)
{
	wstring textureFile = Textures + L"./Sonic/enemies.png";
	wstring ShaderFile = Shaders + L"Effect.fx";

	fish = new Animation();

	Clip* clip;

	//Up
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(textureFile, ShaderFile, 281, 315, 316, 350), 0.2f);
		clip->AddFrame(new Sprite(textureFile, ShaderFile, 322, 315, 357, 350), 0.2f);
		fish->AddClip(clip);
	}
	//Down
	{
		clip = new Clip(PlayMode::Reverse);
		clip->AddFrame(new Sprite(textureFile, ShaderFile, 281, 315, 316, 350), 0.2f);
		clip->AddFrame(new Sprite(textureFile, ShaderFile, 322, 315, 357, 350), 0.2f);
		fish->AddClip(clip);
	}

	fish->Position(position);
	fish->Play(0);
}

Fish::~Fish()
{
	SAFE_DELETE(fish);
}

void Fish::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	position.y += speed * Time::Delta();

	//up
	if (start.y + MAX_LIMIT_Y < position.y)
	{
		speed = -SPEED * 0.75f;
		fish->Play(0);
	}

	//dwon
	else if (start.y > position.y)
	{
		speed = SPEED;
		fish->Play(1);
	}

	fish->Position(position);
	fish->Update(V, P);

}

void Fish::Render()
{
	fish->Render();
}

Sprite * Fish::GetSprite()
{
	return fish->GetClip()->GetSprite();
}
