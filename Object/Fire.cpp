#include "stdafx.h"
#include "Fire.h"

Fire::Fire(wstring shaderFile, D3DXVECTOR2 position)
	:position(position), scale(1, 1), rotation(0, 0, 0)
{
	wstring textureFile = Textures + L"Fire.png";

	clip = new Clip(PlayMode::Loop);
	
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			float startX = x * 43;
			float startY = y * 67;
			float endX = (x + 1) * 43;
			float endY = (y + 1) * 67;

			clip->AddFrame(new Sprite(textureFile, shaderFile, startX, startY, endX, endY), 0.1f);
		}
	}

	clip->Position(position);
	clip->Scale(1.5f, 1.5f);
	clip->Play();

	clip->DrawBound(true);

}

Fire::~Fire()
{
	SAFE_DELETE(clip);
}

void Fire::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	clip->Position(position);
	clip->Update(V, P);
}

void Fire::Render()
{
	ImGui::Separator();
	ImGui::SliderFloat2("Fire Scale", (float*)&scale, 1.5, 5);
	ImGui::SliderFloat3("Fire Rotation", (float*)&rotation, -90 + 1e-6f, 90 - 1e-6f);

	clip->Scale(scale);
	clip->RotationDegree(rotation);

	clip->Render();
}
