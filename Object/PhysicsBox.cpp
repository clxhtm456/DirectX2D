#include "stdafx.h"
#include "PhysicsBox.h"

PhysicsBox::PhysicsBox()
{
	_position = D3DXVECTOR2(0, 0);
	_width = 1;
	_height = 1;
}

PhysicsBox::PhysicsBox(float posX, float posY, float sizeX, float sizeY)
{
	_position = D3DXVECTOR2(posX, posY);
	_width = sizeX;
	_height = sizeY;
}

PhysicsBox::PhysicsBox(D3DXVECTOR2 position,D3DXVECTOR2 size)
{
	_position = position;
	_width = size.x;
	_height = size.y;
}

PhysicsBox::~PhysicsBox()
{
}

void PhysicsBox::SetSizelikeTexture(Sprite * sprite)
{
	auto textureSize = sprite->TextureSize();
	_width = textureSize.x;
	_height = textureSize.y;
}
