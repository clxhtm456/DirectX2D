#include "stdafx.h"
#include "PlaySonic.h"

#include "./Viewer/FollowCamera.h"
#include "./Viewer/FreeCamera.h"
#include "./Renders/Line.h"
#include "./Scene/Sonic/FileManager.h"

#include "./Object/Sonic.h"
#include "./Object/Ring.h"
#include "./Object/Fish.h"
#include "./Object/Mole.h"

PlaySonic::PlaySonic(SceneValues * values)
	:Scene(values), lineVisible(false), slope(0)
{
	wstring textureFile = Textures + L"./Sonic/sonicStage.png";
	wstring shaderFile = Shaders + L"Effect.fx";;

	background = new Sprite(textureFile, shaderFile);

	//Line
	FileManager::LoadLine();
	linePoints = FileManager::GetMarker2();
	for (size_t i = 0; i < linePoints.size(); i++)
	{
		lines.push_back(new Line(linePoints[i].first, linePoints[i].second));
	}

	//Ring
	FileManager::Load(L"ring.bin");
	ringPoints = FileManager::GetMarker();
	for (size_t i = 0; i < ringPoints.size(); i++)
	{
		rings.push_back(new Ring(ringPoints[i]));
	}

	//Fish
	FileManager::Load(L"fish.bin");
	fishPoints = FileManager::GetMarker();
	for (size_t i = 0; i < fishPoints.size(); i++)
	{
		fishes.push_back(new Fish(fishPoints[i]));
	}

	//Mole
	FileManager::Load(L"mole.bin");
	molePoints = FileManager::GetMarker();
	for (size_t i = 0; i < molePoints.size(); i++)
	{
		moles.push_back(new Mole(molePoints[i]));
	}

	player = new Sonic(D3DXVECTOR2(-5500, -150), D3DXVECTOR2(1, 1));

	SAFE_DELETE(values->MainCamera);
	/*values->MainCamera = new FreeCamera();
	((FreeCamera*)values->MainCamera)->Speed(1000.0f);*/
	values->MainCamera = new FollowCamera(player);
}

PlaySonic::~PlaySonic()
{
	SAFE_DELETE(values->MainCamera);
	SAFE_DELETE(background);

	for (Line* line : lines)
		SAFE_DELETE(line);

	for (Ring* ring : rings)
		SAFE_DELETE(ring);

	for (Fish* fish : fishes)
		SAFE_DELETE(fish);

	for (Mole* mole : moles)
		SAFE_DELETE(mole);
}

void PlaySonic::Update()
{
	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;

	D3DXVECTOR2 playerPos = player->Position();

	//¸µ
	{
		for (Ring* ring : rings)
		{
			if ((playerPos.x - Width < ring->Position().x &&
				playerPos.x + Width > ring->Position().x) &&
				(playerPos.y - Height < ring->Position().y &&
					playerPos.y + Height >ring->Position().y))
			{
				ring->Visible(true);
			}
			else
				ring->Visible(false);
		}
	}

	for (Line* line : lines)
	{
		if (Math::SementIntersect(player->Position(), player->BottomVector(), line->LeftPoint(), line->RightPoint()))
		{
			line->DrawCollision(true);
			player->LineBottomCollision(true);
			player->LineLeftPoint(line->LeftPoint());
			player->LineRightPoint(line->RightPoint());
			break;
		}
		else
		{
			line->DrawCollision(false);
			player->LineBottomCollision(false);
			slope = -99.9f;
		}
	}

	background->Update(V, P);

	if (lineVisible == true)
	{
		for (Line* line : lines)
			line->Update(V, P);
	}

	for (Ring* ring : rings)
		ring->Update(V, P);

	for (Fish* fish : fishes)
		fish->Update(V, P);

	for (Mole* mole : moles)
		mole->Update(V, P);

	player->Update(V, P);

}

void PlaySonic::Render()
{
	ImGui::Checkbox("Line Visible", &lineVisible);
	ImGui::LabelText("Slope", "%0.1f", slope);

	background->Render();

	for (Ring* ring : rings)
		ring->Render();

	for (Fish* fish : fishes)
		fish->Render();

	for (Mole* mole : moles)
		mole->Render();

	player->Render();

	if (lineVisible == true)
	{
		for (Line* line : lines)
			line->Render();
	}
}
