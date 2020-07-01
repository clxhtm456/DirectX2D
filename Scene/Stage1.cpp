#include "stdafx.h"
#include "Stage1.h"

#include "./Object/Player.h"
#include "./Object/Bullet.h"
#include "./Object/Fire.h"

#include "./Viewer/FollowCamera.h"

Stage1::Stage1(SceneValues* values)
	:Scene(values)
{
	wstring shaderFile = Shaders + L"Effect.fx";
	wstring textureFile = Textures + L"CastleStage.png";

	background = new Sprite(textureFile, shaderFile);
	background->Position(640, 320);
	background->Scale(1.5f, 1.5f);

	player = new Player(D3DXVECTOR2(100, 170), D3DXVECTOR2(2.5f, 2.5f));
	bullet = new Bullet(shaderFile, D3DXVECTOR2(300, 300), 0, 0);
	fire = new Fire(shaderFile, D3DXVECTOR2(300, 170));

	SAFE_DELETE(values->MainCamera);
	values->MainCamera = new FollowCamera(player);
}

Stage1::~Stage1()
{
	SAFE_DELETE(background);
	SAFE_DELETE(player);
	SAFE_DELETE(bullet);
	SAFE_DELETE(fire);
}

void Stage1::Update()
{
	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;

	background->Update(V, P);
	player->Update(V, P);
	bullet->Update(V, P);
	fire->Update(V, P);
}

void Stage1::Render()
{
	static bool bCheck = false;

	Sprite* playerRect = player->GetSprite();
	Sprite* fireRect = fire->GetClip()->GetSprite();
	Sprite* bulletRect = bullet->GetClip()->GetSprite();
	
	bCheck = Sprite::Aabb(playerRect, bullet->Position());
	//bCheck = playerRect->Aabb(bullet->Position());
	ImGui::LabelText("Aabb", "%d", bCheck ? 1 : 0);

	bool bCheck2 = false;
	bCheck2 = Sprite::Aabb(playerRect, fireRect);
	//bCheck2 = playerRect->Aabb(fireRect);
	ImGui::LabelText("Aabb2", "%d", bCheck2 ? 1 : 0);

	

	bool bCheck3 = false;
	bCheck3 = Sprite::Obb(bulletRect, fireRect);
	ImGui::LabelText("Obb", "%d", bCheck3 ? 1 : 0);

	//Todo:
	bulletRect->DrawCollision(bCheck3);
	fireRect->DrawCollision(bCheck3);

	//RenderBackground();
	//player->Render();
	bullet->Render();
	fire->Render();
}

Player * Stage1::GetPlayer()
{
	return player;
}

void Stage1::RenderBackground()
{
	
	background->Render();
}
