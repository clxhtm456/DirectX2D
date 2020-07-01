#pragma once
#include "./Viewer/IFollow.h"

class Player : public IFollow
{
private:
	float moveSpeed;
	Animation* animation;

	D3DXVECTOR2 focusOffset;

public:
	Player(D3DXVECTOR2 position, D3DXVECTOR2 scale);
	~Player();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	void Focus(D3DXVECTOR2* position, D3DXVECTOR2* size) override;

	Sprite* GetSprite();
};