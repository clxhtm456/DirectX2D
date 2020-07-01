#include "stdafx.h"
#include "AstarScene.h"

#include "Viewer/FreeCamera.h"

#include "Hero.h"
#include "Chocobo.h"
#include "Astar.h"
#include "Object/TileMagma.h"

#define InTile \
position.x < magmas[i]->Position().x + magmas[i]->GetSprite()->TextureSize().x*0.5f&&\
position.x > magmas[i]->Position().x - magmas[i]->GetSprite()->TextureSize().x*0.5f&&\
position.y < magmas[i]->Position().y + magmas[i]->GetSprite()->TextureSize().y*0.5f&&\
position.y < magmas[i]->Position().y - magmas[i]->GetSprite()->TextureSize().y*0.5f

AstarScene::AstarScene(SceneValues* values) :
	Scene(values), mouse(0, 0), cameraPos(0, 0), position(0, 0),
	bButton(false)
{
	wstring textureFile = Textures + L"Astar/map.png";
	wstring shaderFile = Shaders + L"Effect.fx";

	backGroundImg = new Sprite(textureFile, shaderFile);

	player = new Hero(D3DXVECTOR2(-10, -150), D3DXVECTOR2(2, 2));
	enemy = new Chocobo(D3DXVECTOR2(227, 108), D3DXVECTOR2(3, 3));

	Sprite* enemySprite = enemy->GetSprite();
	auto tempX = enemy->Scale().x * enemySprite->TextureSize().x;
	auto tempY = enemy->Scale().y * enemySprite->TextureSize().y;
	D3DXVECTOR2 massSize = D3DXVECTOR2(tempX, tempY);

	pathFinder = new Astar(enemy->Position(), massSize);
	pathFinder->SetCallBack(CC_CALLBACK_1(Chocobo::AutoMove, enemy));


	values->m_MainCamera = new FreeCamera();

	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(Device, (Textures + L"Astar/tumb_magma.png").c_str(), NULL, NULL, &srv, NULL);
	assert(SUCCEEDED(hr));

}

AstarScene::~AstarScene()
{
	delete backGroundImg;
	delete player;

	for (auto magma : magmas)
		delete magma;

	delete enemy;
	delete pathFinder;
	//SAFE_DELETE(srv);
}

void AstarScene::RenderImGui()
{
	ImTextureID id = (ImTextureID)srv;
	if (ImGui::ImageButton(id, ImVec2(40, 40), ImVec2(0, 0), ImVec2(1, 1), 1) == true)
	{
		bButton = !bButton;
	}
	if (bButton == true)
	{
		EditTile();
	}
}

void AstarScene::RenderDebug()
{
}

void AstarScene::EditTile()
{
	if (Mouse->Down(0))
	{
		D3DXVECTOR2 temp = pathFinder->Position();

		magmas.push_back(new TileMagma(temp));
	}

	if (Key->Down(VK_DELETE))
	{
		for (UINT i = 0; i < magmas.size(); i++)
		{
			if (InTile)
			{
				magmas.erase(magmas.begin() + i);
			}
		}
	}
}

void AstarScene::Update()
{
	D3DXMATRIX V = values->m_MainCamera->View();
	D3DXMATRIX P = values->m_Projection;

	pathFinder->SetStartPos(enemy->Position());
	pathFinder->SetGoalPos(player->Position());

	pathFinder->Update(V, P);

	for (auto magma : magmas)
	{
		magma->Update(V, P);
	}
	backGroundImg->Update(V, P);
	player->Update(V, P);
	enemy->Update(V, P);
}

void AstarScene::Render()
{
	mouse = Mouse->Position();
	mouse.x = mouse.x - Width * 0.5f;
	mouse.y = (mouse.y - Height * 0.5f)*-1.0f;
	cameraPos = values->m_MainCamera->Position();
	position = mouse + cameraPos;

	pathFinder->SetMouseCursor(position);
	bMove = enemy->Move();

	pathFinder->ReadyToStart(bMove == false && Mouse->Down(1));

	RenderImGui();

	backGroundImg->Render();

	pathFinder->Render();
	player->Render();
	enemy->Render();

	for (auto magma : magmas)
	{
		magma->Render();
	}
}
