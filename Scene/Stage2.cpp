#include "stdafx.h"
#include "Stage2.h"

#include "./Object/Player.h"
#include "./Viewer/FollowCamera.h"

Stage2::Stage2(SceneValues* values)
	:Scene(values)
{
	wstring shaderFile = Shaders + L"Effect.fx";
	wstring textureFile = Textures + L"Stage.png";

	background = new Sprite(textureFile, shaderFile);
	background->Position(0, 0);
	background->Scale(1.5f, 1.5f);

	player = new Player(D3DXVECTOR2(100, 500), D3DXVECTOR2(2.5f, 2.5f));
}

Stage2::~Stage2()
{
	SAFE_DELETE(values->MainCamera);
	SAFE_DELETE(background);
	SAFE_DELETE(player);
}

void Stage2::Update()
{
	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;

	background->Update(V, P);
	player->Update(V, P);
}

void Stage2::Render()
{
	RenderBackground();
	player->Render();
}

Player * Stage2::GetPlayer()
{
	return player;
}

void Stage2::RenderBackground()
{
	background->Render();
}
