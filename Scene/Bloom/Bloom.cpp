#include "stdafx.h"
#include "Bloom.h"

#include "Object/MetalSlug/Player.h"
#include "Viewer/FreeCamera.h"

Bloom::Bloom(SceneValues * values):
	Scene(values)
{
	player = new Player(D3DXVECTOR2(-100,170),D3DXVECTOR2(2,2));

}

Bloom::~Bloom()
{
	SAFE_DELETE(player);
}

void Bloom::Update()
{
	D3DXMATRIX V = values->m_MainCamera->View();
	D3DXMATRIX P = values->m_Projection;

	player->Update(V, P);
}

void Bloom::Render()
{
	player->Render();
}
