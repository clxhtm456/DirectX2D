#include "stdafx.h"
#include "Chocobo.h"

Chocobo::Chocobo(D3DXVECTOR2 position, D3DXVECTOR2 scale):
	bMove(false),
	startMove(false)
{
	animation = new Animation();

	wstring textureFile = Textures + L"Astar/character.png";
	wstring shaderFile = Shaders + L"Effect.fx";

	Clip* clip;

	//---------------------------------------------------------------------
	//Animation
	//---------------------------------------------------------------------
	//Left 0
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(textureFile, shaderFile, 123, 1430, 142, 1447), 0.3f);
		clip->AddFrame(new Sprite(textureFile, shaderFile, 123, 1460, 142, 1477), 0.3f);
		animation->AddClip(clip);
	}

	//Right 1
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(textureFile, shaderFile, 172, 1430, 191, 1447), 0.3f);
		clip->AddFrame(new Sprite(textureFile, shaderFile, 172, 1460, 191, 1477), 0.3f);
		animation->AddClip(clip);
	}

	//Up
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(textureFile, shaderFile, 147, 1430, 166, 1447), 0.3f);
		clip->AddFrame(new Sprite(textureFile, shaderFile, 147, 1460, 166, 1477), 0.3f);
		animation->AddClip(clip);
	}

	//Down
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(textureFile, shaderFile, 98, 1430, 117, 1447), 0.3f);
		clip->AddFrame(new Sprite(textureFile, shaderFile, 98, 1460, 117, 1477), 0.3f);
		animation->AddClip(clip);
	}

	animation->Position(position);
	animation->Scale(scale);
	animation->Play(0);

	animation->DrawBound(true);
}

Chocobo::~Chocobo()
{
	SAFE_DELETE(animation);
}

void Chocobo::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	auto position = Position();
	//bool bMove = false;
	if (!startMove)
	{
		if (Key->Down('J'))
		{
			bMove = true;
			position.x -= animation->TextureSize().x * Scale().x;
			animation->Play(0);
		}
		else if (Key->Down('L'))
		{
			bMove = true;
			position.x += animation->TextureSize().x * Scale().x;
			animation->Play(1);
		}

		if (Key->Down('I'))
		{
			bMove = true;
			position.y += animation->TextureSize().y * Scale().y;
			animation->Play(2);
		}
		else if (Key->Down('K'))
		{
			bMove = true;
			position.y -= animation->TextureSize().y * Scale().y;
			animation->Play(3);
		}
	}
	else
	{
		float positionX = wayPoint[recentStage].x * animation->TextureSize().x * Scale().x - Width * 0.5f;
		float positionY = wayPoint[recentStage].y * animation->TextureSize().y * Scale().y - Height * 0.5f;
		D3DXVECTOR2 goalPosition = D3DXVECTOR2(positionX, positionY);
		//거리계산
		float distance = sqrt(pow((goalPosition.x - position.x), 2) + pow((goalPosition.y - position.y), 2));

		if (distance < 0.1f)
		{
			recentStage++;
			if (recentStage == lastStage)
			{
				startMove = false;
				return;
			}
		}
		else
		{
			auto moveSpeed = 200.0f;
			position = Math::MoveTowards(position, goalPosition, moveSpeed * Time::deltaTime());
		}
	}
	

	animation->Position(position);
	animation->Update(V, P);
}

void Chocobo::Render()
{
	animation->Render();
}

void Chocobo::AutoMove(vector<POINT> waypoint)
{
	startMove = true;
	wayPoint = waypoint;
	recentStage = 0;
	lastStage = wayPoint.size();
}
