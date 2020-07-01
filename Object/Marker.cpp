#include "stdafx.h"
#include "Marker.h"

Marker::Marker(wstring shaderFile, D3DXVECTOR2 start)
	:position(start), scale(1, 1), rotation(0, 0, 0)
{
	wstring textureFile = Textures + L"Bullet.png";

	clip = new Clip(PlayMode::Loop);
	clip->AddFrame(new Sprite(textureFile, shaderFile, 37, 2, 55, 18), 0.3f);
	clip->AddFrame(new Sprite(textureFile, shaderFile, 117, 7, 132, 22), 0.3f);
	clip->AddFrame(new Sprite(textureFile, shaderFile, 37, 116, 53, 133), 0.3f);
	clip->AddFrame(new Sprite(textureFile, shaderFile, 204, 109, 220, 126), 0.3f);

	clip->Position(position);
	clip->Play();

}

Marker::~Marker()
{
	SAFE_DELETE(clip);
}

void Marker::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{	
	clip->Position(position);
	clip->Update(V, P);
}

void Marker::Render()
{	
	clip->Render();
}
